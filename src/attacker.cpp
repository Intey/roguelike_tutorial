#include <stdio.h>
#include "main.hpp"

Attacker::Attacker(float power): power(power) {
}

void Attacker::attack(Actor *owner, Actor *target) {
	float dmg = power - target->destructible->defence;
	if ( target->destructible && !target->destructible->isDead() ) {
		if ( dmg > 0 ) {
			printf("%s attacks %s for %g hit points.\n", owner->name, target->name, dmg);
		} else {
			printf("%s attacks %s but it has no effect!\n", owner->name, target->name);
		}
		target->destructible->takeDamage(target, power);
	} else {
		printf("%s attacks %s in vain.\n", owner->name, target->name); 
	}
}

