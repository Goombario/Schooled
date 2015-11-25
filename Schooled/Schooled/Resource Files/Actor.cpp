#include <fstream>
#include <Windows.h>
#include <string>
#include "../Header Files/Schooled.h"
#include "../Header Files/Actor.h"
#include "../Header Files/Item.h"
#include "../Header Files/Console_color.h"
#include "../Header Files/Room.h"
using std::string;

// Colours
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
	// Load enemies from file
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
	if (item == 0)	// If item equals 0, no held item
	{
		return 0;
	}
	int itemInt = item->getTile().tileInt;	// Otherwise, return Item from Actor
	item = nullptr;		// set Held Item to null pointer
	return itemInt;
}

bool Actor::holdItem()
{
	if (item == 0)	// Return false if no held item
	{
		return false;
	}
	return true;	// Return true if holds item
	
}

void Actor::pickUp(ItemPtr pickUp)
{
	// Have pickup stats alter player stats
	stats.HP += pickUp->getStats().HP;	
	stats.EN += pickUp->getStats().EN;
	stats.STR += pickUp->getStats().STR;
}

void Actor::setAggro(bool b)
{
	if (b == true)	// Setting aggro to true
	{
		tile.colorCode = con::fgHiRed;
		aggro = true;
	}
	else	// Setting aggro to false
	{
		tile.colorCode = Room::actorIndex[tile.tileInt].tile.colorCode;
		aggro = false;
	}
}

void Actor::setActive(bool isActive)
{
	if (isActive)	// If actor is moving
	{
		tile.colorCode = con::fgHiMagenta;
	}
	else if (aggro)	// If aggro
	{
		tile.colorCode = con::fgHiRed;
	}
	else			// No aggro
	{
		tile.colorCode = Room::actorIndex[tile.tileInt].tile.colorCode;
	}
}