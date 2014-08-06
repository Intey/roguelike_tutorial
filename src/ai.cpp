#include "main.hpp"

//how long, monster track player, after losing his sight
static const int TRACKING_TURNS = 3;

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety) {
	if ( !engine.map->isWalkable(targetx, targety) ) 
		return false;

	for ( Actor **iterator=engine.actors.begin(); iterator != engine.actors.end(); iterator++ ) 
	{
		Actor *actor = *iterator;
		bool corpseOrItem = (actor->destructible && actor->destructible->isDead() ) || actor->pickable;
		if ( corpseOrItem && actor->x == targetx && actor->y == targety ) {
			engine.gui->message(TCODColor::lightGrey, "There's a %s here.", actor->name);
		}
		if ( actor->destructible && !actor->destructible->isDead() && 
				actor->x == targetx && actor->y == targety ) 
		{
			owner->attacker->attack(owner, actor);
			return false;
		}
	}
	owner->x=targetx;
	owner->y=targety;
	return true;
}

void PlayerAi::update(Actor *owner) {
	if ( owner->destructible && owner->destructible->isDead() ) {
		return;
	}
	int dx = 0, 
		dy = 0;
	switch ( engine.lastKey.vk ) {
		case TCODK_UP	 : dy -= 1; break;
		case TCODK_DOWN  : dy  = 1; break;
		case TCODK_LEFT  : dx -= 1; break;
		case TCODK_RIGHT : dx  = 1; break;
		case TCODK_CHAR	 : handleActionKey(owner, engine.lastKey.c); break;
		default:break;
	}
	if ( dx != 0 || dy != 0 ) {
		engine.gameStatus = Engine::NEW_TURN;
		if ( moveOrAttack(owner, owner->x+dx, owner->y+dy)){
			engine.map->computeFov();
		}
	}
}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety) {
	int dx = targetx - owner->x;
	int dy = targety - owner->y;
	int stepdx = (dx > 0 ? 1 : -1);
	int stepdy = (dy > 0 ? 1 : -1);
	float distance = sqrtf(dx*dx+dy*dy);
	if ( distance >= 2 ) {
		dx = (int)(round(dx/distance));
		dy = (int)(round(dy/distance));
		if ( engine.map->isWalkable(owner->x+dx, owner->y+dy) ) {
			owner->x += dx;
			owner->y += dy;
		} else if (engine.map->isWalkable(owner->x+stepdx, owner->y)){
			owner->x += stepdx;
		} else if (engine.map->isWalkable(owner->x, owner->y + stepdy)){
			owner->y += stepdy;
		}
	}else if ( owner->attacker ) {
		owner->attacker->attack(owner, engine.player);
	}
}


void MonsterAi::update(Actor *owner) {
	if ( owner->destructible && owner->destructible->isDead() ) {
		return;
	}
	if ( engine.map->isInFov(owner->x, owner->y) ) {
		moveCount = TRACKING_TURNS;
	} else {
		moveCount--;
	}
	if (moveCount > 0) {
		moveOrAttack(owner, engine.player->x, engine.player->y);
	}
}
void PlayerAi::handleActionKey(Actor *player, int ascii) {
	switch(ascii) {
		case 'g': {
			bool found = false;

			for ( Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++)
			{

				Actor *item = *iterator;

				if (	
						item->pickable			&& 
						item->x == player->x 	&& 
						item->y == player->y 	) 
				{

					if ( item->pickable->pick(item, player) ) 
					{
						found = true;
						engine.gui->message(TCODColor::lightGrey, "You pick the %s.", item->name);
						break;

					} else if ( !found) {
						found = true;
						engine.gui->message(TCODColor::red, "Your inventory is full.");
					}
				}
			}
			if (!found) {
				engine.gui->message(TCODColor::lightGrey, "There's nothing here that you can pick.");
			}
			engine.gameStatus = Engine::NEW_TURN;
			}
			break;
		case 'i':
			{

				Actor *item = chooseFromInventory(player);
				if ( item ) {
					item->pickable->use(item, player);
					engine.gameStatus = Engine::NEW_TURN;
				}
			}
			break;
		case 'd':
			{
				Actor *actor = chooseFromInventory(player);
				if ( actor ) {
					actor->pickable->drop(actor, player);
					engine.gameStatus = Engine::NEW_TURN;
				}
			}
			break;
	}
}
Actor *PlayerAi::chooseFromInventory(Actor *owner) {
	static const int INVENTORY_WIDTH = 50;
	static const int INVENTORY_HEIGHT = 28;
	static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	con.setDefaultForeground(TCODColor(200,180,50));
	con.printFrame(0,0,INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");
	con.setDefaultForeground(TCODColor::white);
	int shortcut = 'a';
	int y = 1;
	for (Actor **it = owner->container->inventory.begin(); it != owner->container->inventory.end(); it++ ) {
		Actor *actor = *it;
		con.print(2, y, "(%c) %s", shortcut, actor->name);
		y++;
		shortcut++;
	}
	TCODConsole::blit(&con, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, 
			TCODConsole::root, engine.screenWidth / 2 - INVENTORY_WIDTH / 2,
			engine.screenHeight / 2  - INVENTORY_HEIGHT / 2);
	TCODConsole::flush();
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	if ( key.vk == TCODK_CHAR ) {
		int actorIndex = key.c - 'a';
		if ( actorIndex >= 0 && actorIndex < owner->container->inventory.size() ) {
			return owner->container->inventory.get(actorIndex);
		}
	}
	return NULL;
}

ConfusedMonsterAi::ConfusedMonsterAi(int nbTurns, Ai *oldAi) : 
	nbTurns(nbTurns), oldAi(oldAi) { }

void ConfusedMonsterAi::update(Actor *owner) {
	TCODRandom *rng=TCODRandom::getInstance();
	int dx = rng->getInt(-1,1);
	int dy = rng->getInt(-1,1);
	//random move or attack
	if ( dx != 0 || dy != 0 ) {
		int destx = owner->x + dx;
		int desty = owner->y + dy;
		if ( engine.map->isWalkable(destx, desty) ) {
			owner->x = destx;
			owner->y = desty;
		} else {
			Actor *actor = engine.getActor(destx, desty);
			if ( actor ) {
				owner->attacker->attack(owner, actor);
			}
		}
	}

	if ( --nbTurns == 0 ) { //duration of confuse is over
		owner->ai = oldAi;
		delete this;
	}
}

