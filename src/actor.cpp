
#include "../include/libtcod.hpp"
#include "actor.hpp"

Actor::Actor(int x, int y, int ch, const TCODColor &color): 
	x(x), y(y), ch(ch), color(color){ }

void Actor::render() const {
	TCODConsole::root->setChar(x,y,ch);
	TCODConsole::root->setCharForeground(x,y,color);
}

