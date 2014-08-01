#include "main.hpp"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP), fovRadius(200), 
	screenWidth(screenWidth), screenHeight(screenHeight) 
{
	TCODConsole::initRoot(80, 58, "asdklei", false);
	player = new Actor(40,25, '@', "player", TCODColor::white);
	player->destructible= new PlayerDestructible(30, 2, "your cadaver");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26);
	actors.push(player);
	map = new Map(80, 50);
	gui = new Gui();
	gui->message(TCODColor::red, "Hello, strainger.");
}

Engine::~Engine() {
	actors.clearAndDelete();
	delete map;
	delete gui;
}

void Engine::render() {
	TCODConsole::root->clear();
	map->render();
	gui->render();
	for ( Actor **iterator=actors.begin(); iterator != actors.end(); iterator++) {
		Actor *actor = *iterator;
		if ( map->isInFov(actor->x, actor->y) ) {
			actor->render();
		}
	}
	player->render();
}

void Engine::update() {
	if ( gameStatus == STARTUP ) map->computeFov();
	gameStatus = IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);
	player->update();
	if (gameStatus == NEW_TURN ) {
		for ( Actor **iterator=engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
		{
			Actor *actor = *iterator;
			if (actor != player ) {
				actor->update();
			}
		}
	}
}

void Engine::sendToBackground(Actor *actor) {
	actors.remove(actor);
	actors.insertBefore(actor,0);
}

