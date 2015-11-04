#ifndef ACTOR_H
#define ACTOR_H

#include <Windows.h>
#include "Schooled.h"

class Actor
{
public:
	// Create empty Actor
	Actor();

	// Create actor containing Tile and Stats
	Actor(Tile, Stats);

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

	void setLocation(COORD c) { location = c; }
	void setMinX(int x){ minX = x; }
	void setMaxX(int x){ maxX = x; }
	void setMinY(int y){ minY = y; }
	void setMaxY(int y){ maxY = y; }

private:
	COORD location;
	Stats stats;
	Tile tile;
	int minX, maxX, minY, maxY;
};



#endif