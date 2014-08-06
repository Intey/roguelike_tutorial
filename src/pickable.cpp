#include "main.hpp"

bool Pickable::pick(Actor *item, Actor *owner) {
	if ( owner->container && owner->container->add(item) ) {
		engine.actors.remove(item);
		return true;
	}
	return false;
}

bool Pickable::use(Actor *item, Actor *target) {
	if ( target->container) {
		target->container->remove(item);
		delete item;
		return true;
	}
	return false;
}

void Pickable::drop(Actor *item, Actor *owner) {
	if (owner->container ) {
		owner->container->remove(item);
		engine.actors.push(item);
		item->x = owner->x;
		item->y = owner->y;
		engine.gui->message(TCODColor::lightGrey, "%s drops a %s", owner->name, item->name);
	}
}

Healer::Healer(float amount): amount(amount) {
}

bool Healer::use(Actor *item, Actor *target) {
	if (target->destructible ) {
		float amountHealed = target->destructible->heal(amount);
		if ( amountHealed > 0 ) {
			return Pickable::use(item, target);
		}
	}
	return false;
}

LightingBolt::LightingBolt(float range, float damage) : range(range), damage(damage) { }

bool LightingBolt::use(Actor *item, Actor *owner) {
	Actor *closestMonster = engine.getClosestMonster(owner->x, owner->y, range);
	if ( ! closestMonster ) { // no target to attack
		engine.gui->message(TCODColor::lightGrey, "No enemy is close enough to strike.");	
		return false;
	}
	engine.gui->message(TCODColor::lightBlue, "A lighting bolt strikes the %s with a loud thunder!\n"
			"The damage is %g hit points.",
			closestMonster->name, damage);
	closestMonster->destructible->takeDamage(closestMonster,damage);
	return Pickable::use(item, owner);
}

Fireball::Fireball(float range, float damage) : 
	LightingBolt(range, damage) { }

bool Fireball::use(Actor *item, Actor *owner) {
	engine.gui->message(TCODColor::cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
	int x, y;
	if ( ! engine.pickATile(&x, &y) ) {
		return false;
	}
	engine.gui->message(TCODColor::orange, "The fireball explodes, burning everything within %g tiles!", range);
	for ( Actor ** iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
	{
		Actor *actor = *iterator;

		if ( actor->destructible && !actor->destructible->isDead() && actor->getDistance(x, y) <= range ) 
		{
			engine.gui->message(TCODColor::orange, "The %s gets burned for %g hit points.", 
					actor->name, damage);
			actor->destructible->takeDamage(actor, damage);
		}
	}
	return Pickable::use(item, owner);
}

Confuser::Confuser(int nbTurns, float range) :
	nbTurns(nbTurns), range(range) { }

bool Confuser::use(Actor *item, Actor *owner) {
	engine.gui->message(TCODColor::cyan, "Left-click an enemy to confuse it,\nor right-click to cancel.");
	int x, y;
	if ( ! engine.pickATile(&x, &y, range) ) {
		return false;
	}
	Actor *actor = engine.getActor(x, y);
	if ( ! actor ) {
		return false;
	}
	Ai *confusedAi = new ConfusedMonsterAi ( nbTurns, actor->ai );
	actor->ai = confusedAi;
	
	engine.gui->message(TCODColor::lightGreen, "The eyes if the %s look vacant,\nas he starts to stumble around!",
		actor->name);
	return Pickable::use(item, owner);
}

