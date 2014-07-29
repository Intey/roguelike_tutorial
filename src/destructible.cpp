
#include <stdio.h>
#include "main.hpp"

Destructible::Destructible(float maxHp, float defence, const char *corpseName):
maxHp(maxHp), hp(maxHp), defence(defence), corpseName(corpseName) { }

float Destructible::takeDamage(Actor *owner, float damage)
{
	damage -= defence;
	if ( damage > 0 ) {
		hp -= damage;
		if ( hp <= 0)
			die(owner);
	} else {
		damage = 0;
	}
	return damage;
}

void Destructible::die(Actor *owner) {
	owner->ch = '%';
	owner->color=TCODColor::darkBlue;
	owner->name = corpseName;
	owner->blocks=false;
	engine.sendToBackground(owner);
}

MonsterDestructible::MonsterDestructible(float maxHp, float defence, const char *corpseName):
	Destructible(maxHp, defence, corpseName) {
}

PlayerDestructible::PlayerDestructible(float maxHp, float defence, const char *corpseName):
	Destructible(maxHp, defence, corpseName) {
}

void MonsterDestructible::die(Actor *owner) {
	printf("%s is dead\n", owner->name);
	Destructible::die(owner);
}

void PlayerDestructible::die(Actor *owner) {
	printf("You died!\n");
	Destructible::die(owner);
	engine.gameStatus = Engine::DEFEAT;
}

