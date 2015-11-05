#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <Windows.h>
#include "Buffer.h"
#include "Actor.h"
#include "Item.h"
#include "Schooled.h"


using std::string;
using std::vector;

class Room
{
public:
	Room();
	Room(string);
	string getMessage();
	COORD getNorth() { return entrances[0]; }
	COORD getSouth() { return entrances[1]; }
	COORD getEast() { return entrances[2]; }
	COORD getWest() { return entrances[3]; }

	bool isPassable(COORD);	// Checks if a tile is passable
	void display(Buffer&);	// Display the room to the screen
	void save(string); // Save room to a file

	int findActor(COORD); // Find Actor in actorList
	Actor& getActor(COORD);	// Get Actor in actorList
	void removeActor(COORD); // Remove actor from actorList
	void moveActors(COORD);	// Move all actors in actorList
	vector<Actor> getActorList(); // Getting the entire actor list
	void moveEnemy(COORD, Actor&);	// Move an enemy towards player
	bool isAdjacent(COORD, Actor&); //checks if enemy is adjacent to the player
	bool lineOfSight(COORD, Actor&);	// Determines if actor can see coordinate

	int getTileInt(COORD c) { return tileArray[c.Y][c.X]; }
	int getItemInt(COORD c) { return itemArray[c.Y][c.X]; }
	int getActorInt(COORD c) { return actorArray[c.Y][c.X]; }
	COORD getLocation() { return location; }
	int getX() { return location.X; }
	int getY() { return location.Y; }

	void setTileInt(COORD c, int i) { tileArray[c.Y][c.X] = i; }
	void setItemInt(COORD c, int i) { itemArray[c.Y][c.X] = i; }
	void setActorInt(COORD c, int i) { actorArray[c.Y][c.X] = i; }
	void setLocation(COORD c) { location = c; }

private:
	vector<Actor> actorList;
	COORD location;
	string message;
	COORD entrances[4];
	int tileArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int itemArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int actorArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
};

static const Tile tileIndex[] = {	// symbol, colour, isPassable
	{ ' ', con::fgBlack, true, 0 },	// (0) MAP_FLOOR
	{ '=', con::fgHiGreen, false, 1 },	// (1) MAP_WALL_TOP
	{ 'D', con::fgHiBlue, true, 2 },	// (2) MAP_DOOR
	{ '|', con::fgHiGreen, false, 3 },	// (3) MAP_WALL_SIDE

};
static const Item itemIndex[] = {
	Item({ ' ', con::fgBlack, true, 0 }, { 1, 1, 1 }),		// (0) NULL
	Item({ '~', con::fgHiWhite, true, 1 }, { 1, 1, 1 }),	// (1) KEY
	Item({ 'D', con::fgLoBlue, false, 2 }, { 1, 1, 1 }),	// (2) DOOR_TO_NEW_ROOM
	Item({ 'D', con::fgHiRed, false, 3 }, { 1, 1, 1 })		// (3) MAP_DOOR_LOCKED

};

static const Actor actorIndex[] = {
	Actor(),												// (0) NULL
	Actor({ 'X', con::fgHiWhite, false, 1 }, { 10, 2, 1 }),	// (1) BULLY_WEAK
};

#endif