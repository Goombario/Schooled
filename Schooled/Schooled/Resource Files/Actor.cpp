#include <fstream>
#include <Windows.h>
#include <string>
#include "../Header Files/Schooled.h"
#include "../Header Files/Actor.h"
#include "../Header Files/Item.h"
using std::string;

Actor::Actor(Tile t, Stats s, ItemPtr i, string atk, string def) : tile(t), stats(s), item(i) 
{
	m_attack = atk;
	m_defend = def;
}

Actor::Actor(Tile t, Stats s, ItemPtr i) : tile(t), stats(s), item(i) {}

Actor::Actor(Tile t, Stats s) : tile(t), stats(s) {}

Actor::Actor() {}

Actor::Actor(std::string filename)
{
	std::ifstream stream("Enemies/" + filename);
	std::string line;

	std::getline(stream, line);

	stream.close();

}

void Actor::attack(Actor& defender){
	defender.stats.HP -= stats.STR;
}

int Actor::dropItem()
{
	int itemInt = item->getTile().tileInt;
	item = nullptr;
	return itemInt;
}

void Actor::pickUp(ItemPtr pickUp){
	stats.HP += pickUp->getStats().HP;
	stats.EN += pickUp->getStats().EN;
	stats.STR += pickUp->getStats().STR;
}