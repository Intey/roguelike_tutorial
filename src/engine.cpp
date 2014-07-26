#include "../include/libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"

Engine::Engine() : fovRadius(200), computeFov(true) {
	TCODConsole::initRoot(80, 50, "asdklei", false);
	//add player
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	actors.push(player);
	map = new Map(80, 45);
}

Engine::~Engine() {
	actors.clearAndDelete();
	delete map;
}

void Engine::render() {
	TCODConsole::root->clear();
	map->render();
	for ( Actor **iterator=actors.begin(); iterator != actors.end(); iterator++) 
	{
		Actor *actor = *iterator;
		if ( map->isInFov(actor->x, actor->y) ) {
			actor->render();
		}
	}
}

void Engine::update() {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
	if ( gameStatus == STARTUP ) map->computeFov();
	int dx = 0, dy = 0;
	switch(key.vk) {
		case TCODK_UP	 : dy = -1; break;
		case TCODK_DOWN  : dy =  1; break;
		case TCODK_LEFT  : dx = -1; break;
		case TCODK_RIGHT : dx =  1; break;
		default:break;
	}
	if (dx != 0 || dy != 0 ) {
		gameStatus = NEW_TURN;
		if ( player->moveOrAttack( player->x + dx, player->y + dy ) ){
			map->computeFov();
		}
	}
	if (gameStatus == NEW_TURN ) {
		for ( Actor **iterator=engine.actors.begin();
				iterator != engine.actors.end(); iterator++) {
			Actor *actor=*iterator;
			if (actor != player ) {
				actor->update();
			}
		}
	}
	if ( computeFov ) { 
		map->computeFov();
		computeFov = false;
	}
}

