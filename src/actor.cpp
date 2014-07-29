#include <stdio.h>
#include "main.hpp"

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &color): 
	x(x), y(y), ch(ch), name(name), color(color), blocks(true), attacker(NULL), destructible(NULL), ai(NULL) {
	}

void Actor::render() const {
	TCODConsole::root->setChar(x,y,ch);
	TCODConsole::root->setCharForeground(x,y,color);
}

void Actor::update() {
	if ( ai ) ai->update(this);
}

