#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include "Console Library/Console.h"
#include "Console Color/Console_color.h"

using namespace std;
namespace con = JadedHoboConsole;	// Used for the color
namespace schooled{
	int const MAP_WIDTH = 60;
	int const MAP_HEIGHT = 20;
	int const SCREEN_WIDTH = 80;
	int const SCREEN_HEIGHT = 25;
	int const FLOOR_WIDTH = 3;
	int const FLOOR_HEIGHT = 3;

	int const MAP_FLOOR = 0;
	int const MAP_WALL_TOP = 1;
	int const MAP_DOOR = 2;
	int const MAP_WALL_SIDE = 3;
	int const ENEMY = 4;
    int const KEY = 5;
	int const MAP_DOOR_LOCKED = 6;
	int const DOOR_TO_NEW_ROOM = 7;

	const int ITEM_INDEX_SIZE = 2;


	COORD enemy1{ 18, 16 };
        int enemy1X = 18;
	int enemy1Y = 16;

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
}

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

///////////////////////////////////////////////////////////////////////////////
// Classes and structures

struct Tile
{
	char character;
	int colorCode;
	bool isPassable;
	bool isInteractable;
};

struct Stats{
	int HP;
	int EN;
	int STR;
};

class Actor
{
public:
	Actor();
	Actor(Tile, Stats);
	Stats stats;
	Tile tile;
	COORD location;
private:

};

class Room
{
public:
	const char *getMessage();
	friend istream& operator >>(ifstream& stream, Room& r);
	friend ostream& operator <<(ofstream& stream, const Room& r);
	COORD getNorth() { return entrances[0]; }
	COORD getSouth() { return entrances[1]; }
	COORD getEast() { return entrances[2]; }
	COORD getWest() { return entrances[3]; }
	vector<Actor> getActor() { return actorList; }

	int tileArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int itemArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int actorArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	vector<Actor> actorList;

	COORD location;
private:
	string message;
	COORD entrances[4];
};

class Item
{
public:
	Item();
	Item(Tile, Stats);
	Tile getTile() { return tile; }
	Stats getStats() { return stats; }
	Tile tile;
	Stats stats;
private:

};



ostream& operator <<(ofstream& stream, const Room& r)
{
	stream << r.message << endl;
	//stream << r.location.X << " " << r.location.Y << endl;

	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << r.tileArray[a][b] << " ";
		}
		stream << endl;
	}
	return stream;
}

///////////////////////////////////////////////////////////////////////////////
// Global variables

const Tile tileIndex[] = {	// symbol, colour, isPassable
	{ ' ', con::fgBlack, true, false },		// (0) MAP_FLOOR
	{ '=', con::fgHiGreen, false, false },	// (1) MAP_WALL_TOP
	{ 'D', con::fgHiBlue, true, false },	// (2) MAP_DOOR
	{ '|', con::fgHiGreen, false, false },	// (3) MAP_WALL_SIDE
	{ 'X', con::fgHiWhite, false, true },	// (4) ENEMY (UNUSED)
	{ '~', con::fgHiWhite, true, true },	// (5) KEY (UNUSED)
	{ 'D', con::fgHiRed, false, true },		// (6) MAP_DOOR_LOCKED
	{ 'D', con::fgLoBlue, false, true }		// (7) DOOR_TO_NEW_ROOM
};
const Item itemIndex[] = {
	Item(),		    										// (0) NULL
	Item({ '~', con::fgHiWhite, true, true }, { 1, 1, 1 }),	// (1) KEY
	Item({ 'D', con::fgLoBlue, false, true }, { 1, 1, 1 })	// (2) DOOR_TO_NEW_ROOM
};

const Actor actorIndex[] = {
	Actor(),													// (0) NULL
	Actor({ 'X', con::fgHiWhite, false, true }, { 10, 2, 1 })	// (1) BULLY_WEAK
};

map<string, char *> messages =
{
	{ "Q_RANDOM",		"Random: What do you want ?" },
	{ "GET_KEY",		"It's a key! You picked it up." },
	{ "DOOR_LOCKED",	"The door is locked, you need a key." },
	{ "USE_KEY",		"You used a key!" },
	{ "Q_USE_KEY",		"The door is locked, use a key?"},
	{ "Q_NEXT_ROOM",	"Go to next room?" },
	{ "NEW_ROOM",		"New room." },
	{ "ENEMY_DEATH",	"It died..." },
	{ "UNATTACKABLE",	"You can't kill that, you silly!" },
	{ "ATTACKABLE",		"You hit that thing with " /*+ people[0].STR + " damage! Wow!"*/ }
};

Stats people[] = {
	{ 10, 2, 2 }, // our player
	{ 10, 2, 1 }  // Weak bully? Cyber bully maybe?
};

Room roomArray[schooled::FLOOR_HEIGHT][schooled::FLOOR_WIDTH];

///////////////////////////////////////////////////////////////////////////////
// Function declarations

// Draws the map to the screen
void displayMap(Room);

// Changes the current room to the number of the one given.
void changeRoom(Room&, COORD, vector<Actor>);

// Returns a room from the given file (without the COORD)
Room loadRoom(string);

// Saves a room to the given file (without the COORD)
void saveRoom(string, Room);

// Deletes all dynamic variables
void exitGame();

// Finds the actor in the current room
int findActor(vector<Actor>, COORD);

void drawTile(int x, int y);
void enemy1();
void flavourText();

// Checks if a tile is passable
bool isPassable(int mapX, int mapY, Room);

// Checks if a tile is interactable
bool isInteractable(int mapX, int mapY, Room);

// attacks an actor
void attack(Room&, Actor, Actor&, vector<const char *>&);

int main()
{
	///////////////////////////////////////////////////////////////////////////
	// Initialization
	console.SetTitle("Schooled V0.1");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output

	// Initialize the player's on-screen location
	Actor player({ '8', con::fgHiWhite }, { 10, 2, 2 });
	player.location = { 2, 18 };
	COORD highlight{ 2, 17 };
	COORD delta{ 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, schooled::SCREEN_WIDTH - 1,
		schooled::SCREEN_HEIGHT - 1 };

	int object = 0;
	int keyCount = 0;
	int attackable = 0;
	int temp = 0;
	bool useKey = false;
	bool goToRoom = false;

	/*Room tempRoom;
	for (int i = 1; i <= 3; i++)
	{
		tempRoom = loadRoom("Rooms/Room" + to_string(i) + ".txt");
		roomArray[tempRoom.location.X][tempRoom.location.Y] = tempRoom;
	}

	Room currentRoom = roomArray[1][1];*/

	// Load the rooms from the file
	Room roomOne = loadRoom("Rooms/Room1_1.txt");
	roomOne.location = { 1, 1 };

	Room roomTwo = loadRoom("Rooms/Room2_1.txt");
	roomTwo.location = { 1, 2 };

	Room roomThree = loadRoom("Rooms/Room3_1.txt");
	roomThree.location = { 1, 0 };

	// Puts the rooms into the floor array
	roomArray[roomOne.location.X][roomOne.location.Y] = roomOne;
	roomArray[roomTwo.location.X][roomTwo.location.Y] = roomTwo;
	roomArray[roomThree.location.X][roomThree.location.Y] = roomThree;

	Room currentRoom = roomOne;
	vector<Actor> actorList = currentRoom.actorList;

	// Initialize the log
	vector<const char *> log;
	log.push_back("");

	// Main program loop
	while (true)
	{
		// Wipe console clear
		console.Clear();

		///////////////////////////////////////////////////////////////////////
		// Output phase

		// Begins reading to the buffer.
		ReadConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
			schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);

		// Draw the map
		displayMap(currentRoom);

		// Display the character
		schooled::buffer[player.location.Y][player.location.X].Attributes = con::fgHiWhite;
		schooled::buffer[player.location.Y][player.location.X].Char.AsciiChar = '8';

		// Display the highlight
		schooled::buffer[highlight.Y][highlight.X].Attributes = con::bgHiWhite;
		
		// Writes the buffer to the screen
		WriteConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
			schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);

		// Display keycount
		console.Position(5, 21);
		console << keyCount;

		console.Position(5, 22);
		console << player.location.X << "," << player.location.Y;
		
		console.Position(5, 23);
		//console << actorList[0].stats.HP;
		
		// Display the messages
		console.Position(21, 23);
		SetConsoleTextAttribute(hConsole, con::fgHiWhite);
		if (log.size() > 2)
		{
			console << log[log.size() - 1];
			console.Position(21, 22);
			console << log[log.size() - 2];
			console.Position(21, 21);
			console << log[log.size() - 3];
		}
		else if (log.size() > 1)
		{
			console << log[log.size() - 1];
			console.Position(21, 22);
			console << log[log.size() - 2];
		}
		else
			console << log[log.size() - 1];


		///////////////////////////////////////////////////////////////////////
		// Input phase

		KEYPRESS sKeyPress = console.WaitForKeypress();

		///////////////////////////////////////////////////////////////////////
		// Processing phase

		delta.X = 0;
		delta.Y = 0;

		switch (sKeyPress.eCode)
		{
			// down selected
		case CONSOLE_KEY_DOWN:
			delta.X = 0;
			delta.Y = 1;
			break;

			// left selected
		case CONSOLE_KEY_LEFT:
			delta.X = -1;
			delta.Y = 0;
			break;

			// right selected
		case CONSOLE_KEY_RIGHT:
			delta.X = 1;
			delta.Y = 0;
			break;

			// up selected
		case CONSOLE_KEY_UP:
			delta.X = 0;
			delta.Y = -1;
			break;

			//attack things B)
		case CONSOLE_KEY_N:
			if (currentRoom.actorArray[highlight.Y][highlight.X] > 0){
				attack(currentRoom, player, actorList[findActor(actorList, highlight)], log);

				// If the actor died
				if (actorList[findActor(actorList, highlight)].stats.HP <= 0){
					Actor tempActor = actorList[findActor(actorList, highlight)];
					currentRoom.actorArray[tempActor.location.Y][tempActor.location.X] = 0;
					log.push_back(messages["ENEMY_DEATH"]);
					actorList.erase(actorList.begin() + findActor(actorList, highlight));
					
				}
			}
			else{
				log.push_back(messages["UNATTACKABLE"]);
			}
			break;

			//checks interactable
		case CONSOLE_KEY_SPACE:
			switch (currentRoom.itemArray[highlight.Y][highlight.X])
			//basically if the object is interactable
			{
				// KEY
			case 1:
				log.push_back(messages["GET_KEY"]);
				keyCount++;
				currentRoom.itemArray[highlight.Y][highlight.X] = 0;
				break;

				// ENEMY
			case 4:
				log.push_back(messages["Q_RANDOM"]);
				break;

				// MAP_DOOR_LOCKED
			case 3:
				if (useKey == true && keyCount > 0)
				{
					log.push_back(messages["USE_KEY"]);
					keyCount--;
					currentRoom.itemArray[highlight.Y][highlight.X] = 2;
					useKey = false;
				}
				else if (useKey == false && keyCount > 0)
				{
					log.push_back(messages["Q_USE_KEY"]);
					useKey = true;
				}
				else 
					log.push_back(messages["DOOR_LOCKED"]);
				break;

			case 2:
					//room transition
				if (goToRoom == true)
				{
					if (player.location.Y < 10)  // going up
					{
						changeRoom(currentRoom, { 0, 1 }, actorList);
						player.location = currentRoom.getSouth();
						highlight.Y = currentRoom.getSouth().Y - 1;
						highlight.X = currentRoom.getSouth().X;
					}
					else if (player.location.Y > 10)	// going down
					{
						changeRoom(currentRoom, { 0, -1 }, actorList);
						player.location = currentRoom.getNorth();
						highlight.Y = currentRoom.getNorth().Y + 1;
						highlight.X = currentRoom.getNorth().X;
					}

					log.clear();
					log.push_back(currentRoom.getMessage());
					goToRoom = false;
				}
				else if (goToRoom == false)
				{
					log.push_back(messages["Q_NEXT_ROOM"]);
					goToRoom = true;
				}
				break;

			default:
				break;
				
			}
			break;

			// move key pressed
		case CONSOLE_KEY_M:
			delta.X = (highlight.X - player.location.X);
			delta.Y = (highlight.Y - player.location.Y);

			// Check if the player can move in specified direction
			if (isPassable(highlight.X, highlight.Y, currentRoom))
			{
				// If allowed, move in specified direction
				player.location.X = highlight.X;
				player.location.Y = highlight.Y;
			}
			break;

			// quit
		case CONSOLE_KEY_ESCAPE:
			exitGame();
			return 0;

			// Ignore any other key
		default:
			break;
		}
		
		
		// Check if a move action has been performed, and adjusts highlight
		if (delta.X != 0 || delta.Y != 0)
		{
			highlight.X = player.location.X + delta.X;
			highlight.Y = player.location.Y + delta.Y;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Operators

bool operator ==(COORD a, COORD b)
{
	return (a.X == b.X && a.Y == b.Y);
}

istream& operator >>(ifstream& stream, Room& r)
{
	string m;
	getline(stream, m);
	r.message = m;
	getline(stream, m); // blank

	// Get the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> r.tileArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the entrance coordinates
	for (COORD &c : r.entrances)
	{
		stream >> c.X >> c.Y;
	}
	getline(stream, m); // blank

	// Get the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> r.itemArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> r.actorArray[a][b];
			if (r.actorArray[a][b] > 0)
			{
				r.actorList.push_back(actorIndex[r.actorArray[a][b]]);
				r.actorList[r.actorList.size() - 1].location = { b, a };
			}
		}
	}

	return stream;
}

Item::Item(Tile t, Stats s) : tile(t), stats(s) {}

Item::Item() {}

Actor::Actor(Tile t, Stats s) : tile(t), stats(s) {}

Actor::Actor() {}

int findActor(vector<Actor> a, COORD c)
{
	for (unsigned int i = 0; i < a.size(); i++)
	{
		if (a[i].location == c)
			return i;
	}
	return -1;
}

void displayMap(Room currentRoom){
	int tile;

	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			if (currentRoom.actorArray[a][b] > 0)
			{
				tile = currentRoom.actorArray[a][b];
				schooled::buffer[a][b].Char.AsciiChar = actorIndex[tile].tile.character;
				schooled::buffer[a][b].Attributes = actorIndex[tile].tile.colorCode;
			}
			else if (currentRoom.itemArray[a][b] > 0)
			{
				tile = currentRoom.itemArray[a][b];
				schooled::buffer[a][b].Char.AsciiChar = itemIndex[tile].tile.character;
				schooled::buffer[a][b].Attributes = itemIndex[tile].tile.colorCode;
			}
			else
			{
				tile = currentRoom.tileArray[a][b];
				schooled::buffer[a][b].Char.AsciiChar = tileIndex[tile].character;
				schooled::buffer[a][b].Attributes = tileIndex[tile].colorCode;
			}
		}
	}
}

bool isPassable(int mapX, int mapY, Room currentRoom){
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)
		return false;

	int tileValue = currentRoom.tileArray[mapY][mapX];
	int actorValue = currentRoom.actorArray[mapY][mapX];
	if (tileIndex[tileValue].isPassable && actorValue == 0)
		return true;
	return false;
}

bool isInteractable(int mapX, int mapY, Room currentRoom){
	int tileValue;
	tileValue = currentRoom.tileArray[mapY][mapX];
	
	if (tileIndex[tileValue].isInteractable)
		return true;
	return false;
}

void attack(Room& currentRoom, Actor attacker, Actor& defender, vector<const char *>& log){
	string message;
	defender.stats.HP -= attacker.stats.STR;
	message = messages["ATTACKABLE"] + to_string(attacker.stats.STR) + " damage! Wow!";
	log.push_back(messages["ATTACKABLE"]);
}

void changeRoom(Room& currentRoom, COORD change, vector<Actor> actorList)
{
	currentRoom.actorList = actorList;
	roomArray[currentRoom.location.X][currentRoom.location.Y] = currentRoom;
	currentRoom = roomArray[currentRoom.location.X + change.X][currentRoom.location.Y + change.Y];

}

const char *Room::getMessage() { return message.c_str(); }

Room loadRoom(string fileName)
{
	ifstream stream;
	stream.open(fileName);

	if (stream.fail())
	{
		cout << "File open failed.\n";	
		exit(1); 
	}

	Room newRoom;
	stream >> newRoom;
	stream.close();
	return (newRoom);
}

void saveRoom(string fileName, Room room)
{
	ofstream stream;
	stream.open(fileName);

	if (stream.fail())
	{
		cout << "File open failed.\n";
		exit(1);
	}

	stream << room;
	stream.close();
	return;
}

void exitGame()
{
	
	return;
}