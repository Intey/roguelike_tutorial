#include <stdio.h>
#include "../include/libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"


Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &color): 
	x(x), y(y), ch(ch), name(name), color(color) { }

void Actor::render() const {
	TCODConsole::root->setChar(x,y,ch);
	TCODConsole::root->setCharForeground(x,y,color);
}

bool Actor::moveOrAttack(int x, int y) {
	if ( !engine.map->isWalkable(x,y) ) return false;
	for ( Actor **iterator=engine.actors.begin();
			iterator != engine.actors.end(); iterator++) {
		Actor *actor=*iterator;
		if ( actor->x == x && actor->y == y) {
			printf("The %s laughs at your puny efforts to attack him!\n",
					actor->name);
			return false;
		}
	}
	this->x=x;
	this->y=y;
	return true;
}

void Actor::update() {
	printf("the %s growls!\n",name);
}

