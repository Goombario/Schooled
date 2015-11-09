#ifndef ACTOR_H
#define ACTOR_H

#include "Schooled.h"

class Item;
typedef Item* ItemPtr;

class Actor
{
public:
	// Create empty Actor
	Actor();

	// Create actor containing Tile and Stats
	Actor(Tile, Stats);

	// Create actor containing Tile, Stats, and a held Item
	Actor(Tile, Stats, ItemPtr);

	// Create actor containing Tile, Stats, held item and custom messages
	Actor(Tile, Stats, ItemPtr, std::string, std::string);

	// Create actor from file
	Actor(std::string);

	// Attack another Actor
	void attack(Actor&);

	COORD getLocation() { return location; }
	int getX() { return location.X; }
	int getY() { return location.Y; }
	int getMinX(){ return minX; }
	int getMaxX(){ return maxX; }
	int getMinY(){ return minY; }
	int getMaxY(){ return maxY; }
	Stats getStats() { return stats; }
	Tile getTile() { return tile; }
	std::string getMAttack() { return m_attack; }
	std::string getMDefend() { return m_defend; }

	void setLocation(COORD c) { location = c; }
	void setMinX(int x){ minX = x; }
	void setMaxX(int x){ maxX = x; }
	void setMinY(int y){ minY = y; }
	void setMaxY(int y){ maxY = y; }

	int dropItem();

private:
	COORD location;
	Stats stats;
	Tile tile;
	ItemPtr item;
	std::string m_attack = "";
	std::string m_defend = "";
	int minX, maxX, minY, maxY;
};



#endif