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
Healer::Healer(float amount): amount(amount) {
}

bool Healer::use(Actor *item, Actor *target)
{
	if (target->destructible ) {
		float amountHealed = target->destructible->heal(amount);
		if ( amountHealed > 0 ) {
			return Pickable::use(item, target);
		}
	}
	return false;
}

