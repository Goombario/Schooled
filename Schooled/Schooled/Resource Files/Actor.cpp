#include "../Header Files/Actor.h"

Actor::Actor(Tile t, Stats s) : tile(t), stats(s) {}

Actor::Actor() {}

void Actor::attack(Actor& defender){
	defender.stats.HP -= stats.STR;
}