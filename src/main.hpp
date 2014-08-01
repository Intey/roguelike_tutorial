#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "../include/libtcod.hpp"
//solve for circular dependence (destructible/attacker/ai <-> actor)
class Actor;
#include "destructible.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "pickable.hpp"
#include "container.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "engine.hpp"
