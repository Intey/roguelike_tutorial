#include "../include/libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"

Engine::Engine() : fovRadius(200), computeFov(true) {
	TCODConsole::initRoot(80,50, "asdklei", false);
	//add player
	player = new Actor(40,25,'P',TCODColor::white);
	actors.push(player);
	map = new Map(80,45);
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
	switch(key.vk) {
		case TCODK_UP : 
			if ( map->isWalkable(player->x,player->y-1)) {
				player->y--;   
				computeFov = true;
			}
			break;
		case TCODK_DOWN : 
			if ( map->isWalkable(player->x,player->y+1)) {
				player->y++;
				computeFov = true;
			}
			break;
		case TCODK_LEFT : 
			if ( map->isWalkable(player->x-1,player->y)) {
				player->x--;
				computeFov = true;
			}
			break;
		case TCODK_RIGHT : 
			if ( map->isWalkable(player->x+1,player->y)) {
				player->x++;
				computeFov = true;
			}
			break;
		default:break;
	}
	if ( computeFov ) { 
		map->computeFov();
		computeFov = false;
	}
}

