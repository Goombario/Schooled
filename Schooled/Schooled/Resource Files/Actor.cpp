#include <fstream>
#include <Windows.h>
#include <string>
#include "../Header Files/Schooled.h"
#include "../Header Files/Actor.h"
#include "../Header Files/Item.h"
#include "../Header Files/Console_color.h"
#include "../Header Files/Room.h"
using std::string;

namespace con = JadedHoboConsole;

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
	if (item == 0)
	{
		return 0;
	}
	int itemInt = item->getTile().tileInt;
	item = nullptr;
	return itemInt;
}

bool Actor::holdItem()
{
	if (item == 0)
	{
		return false;
	}
	int itemInt = item->getTile().tileInt;
	if (itemInt > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Actor::pickUp(ItemPtr pickUp){
	stats.HP += pickUp->getStats().HP;
	stats.EN += pickUp->getStats().EN;
	stats.STR += pickUp->getStats().STR;
}

void Actor::setAggro(bool b)
{
	if (b == true)
	{
		tile.colorCode = con::fgHiRed;
	}
	else
	{
		tile.colorCode = Room::actorIndex[tile.tileInt].tile.colorCode;
	}
}