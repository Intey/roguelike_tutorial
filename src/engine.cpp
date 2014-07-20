#include "../include/libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"

Engine::Engine() {
	TCODConsole::initRoot(80,50, "asdklei", false);
	//add NPC
	actors.push(new Actor(60,13,'@',TCODColor::yellow));
	map = new Map(80,50);
	//add player
	player = new Actor(40,25,'@',TCODColor::white);
	actors.push(player);
}

Engine::~Engine() {
	actors.clearAndDelete();
	delete map;
}

void Engine::render() {
	TCODConsole::root->clear();
	map->render();
	for ( Actor **iterator=actors.begin(); iterator != actors.end(); iterator++)
		(*iterator)->render();
}

void Engine::update() {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
	switch(key.vk) {
		case TCODK_UP : 
			if ( map->isWalkable(player->x,player->y-1)) {
				player->y--;   
			}
			break;
		case TCODK_DOWN : 
			if ( map->isWalkable(player->x,player->y+1)) {
				player->y++;
			}
			break;
		case TCODK_LEFT : 
			if ( map->isWalkable(player->x-1,player->y)) {
				player->x--;
			}
			break;
		case TCODK_RIGHT : 
			if ( map->isWalkable(player->x+1,player->y)) {
				player->x++;
			}
			break;
		default:break;
	}
}

