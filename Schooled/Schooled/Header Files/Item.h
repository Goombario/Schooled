#ifndef ITEM_H
#define ITEM_H

#include "Schooled.h"
#include <string>
using std::string;

class Item
{
public:
	// Creates empty Item
	Item();

	// Creates Item with Tile and Stats
	Item(Tile, Stats);

	// Returns the Tile variable
	Tile getTile();

	// Returns the Stats variable
	Stats getStats();

	// Returns the pickup message
	string getMPickup();

	// Returns the name of the item
	string getName();

	// Sets the Tile object to the passed Tile variable
	void setTile(Tile);

	// Sets the Stats struct
	void setStats(Stats);

	// Sets the pickup message
	void setMPickup(string);

	// Set the name of the item
	void setName(string);
private:
	Tile tile;
	Stats stats;
	string mPickup;
	string name;
};

typedef Item* ItemPtr;

#endif