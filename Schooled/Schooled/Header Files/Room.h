#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include "Schooled.h"

class Actor;
class Room;
class Buffer;
class Item;
typedef Item* ItemPtr;

using std::string;
using std::vector;


class Room
{
public:

	// Create an empty Room object
	Room();

	// Create a Room object from a file
	Room(string);

	// Get the title of the room
	string getMessage();

	// Get the entrance locations
	COORD getNorth() { return entrances[0]; }
	COORD getSouth() { return entrances[1]; }
	COORD getEast() { return entrances[2]; }
	COORD getWest() { return entrances[3]; }

	bool isPassable(COORD);	// Checks if a tile is passable
	void display(Buffer&);	// Display the room to the screen
	void save(string); // Save room to a file

	int findActor(COORD); // Find Actor in actorList
	Actor& getActor(COORD);	// Get Actor in actorList
	Actor& getActor(int);
	void removeActor(COORD); // Remove actor from actorList
	void moveActors(COORD, Actor&);	// Move all actors in actorList
	vector<Actor>& getActorList(); // Getting the entire actor list
	void moveEnemy1(COORD, Actor&);	// Move an enemy towards player
	void moveCat(COORD, Actor&);	// Move a cat towards player
	void moveMinion(Actor&); //Moves the minion npc
	bool isAdjacent(COORD, Actor&); //checks if enemy is adjacent to the player
	bool lineOfSight(COORD, Actor&);	// Determines if actor can see coordinate

	// Get functions
	static ItemPtr getItemStats(int);
	int randomItem();					// Random function option
	string randomLog();		// Generates random number between 1 and 10
	static ItemPtr getItemStats(string);
	int getTileInt(COORD c) { return tileArray[c.Y][c.X]; }
	int getItemInt(COORD c) { return itemArray[c.Y][c.X]; }
	int getActorInt(COORD c) { return actorArray[c.Y][c.X]; }
	COORD getLocation() { return location; }
	int getX() { return location.X; }
	int getY() { return location.Y; }

	//set functions
	void setTileInt(COORD c, int i) { tileArray[c.Y][c.X] = i; }
	void setItemInt(COORD c, int i) { itemArray[c.Y][c.X] = i; }
	void setActorInt(COORD c, int i) { actorArray[c.Y][c.X] = i; }
	void setLocation(COORD c) { location = c; }

	//static indicies
	static vector<const Tile> tileIndex;
	static vector<const Item> itemIndex;
	static vector<const Actor> actorIndex;

	//load static indicies from file
	static void loadTileIndex(string);
	static void loadItemIndex(string);
	static void loadActorIndex(string);
	
private:
	//member variables
	vector<Actor> actorList;
	COORD location, startPoint;
	string message;
	COORD entrances[4];
	int tileArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int itemArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int actorArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
};

#endif