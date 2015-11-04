#ifndef ITEM_H
#define ITEM_H

#include "Schooled.h"

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

	// Sets the Tile object to the passed Tile variable
	void setTile(Tile);

private:
	Tile tile;
	Stats stats;

};

#endif