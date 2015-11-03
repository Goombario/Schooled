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


	//COORD enemy1{ 18, 16 };
    //int enemy1X = 18;
	//int enemy1Y = 16;

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
	int tileInt;
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
	vector<Actor> getActor() { return actorList; }
	bool isPassable(COORD);	// Checks if a tile is passable
	void display();	// Display the room to the screen
	void save(string); // Save room to a file

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

	vector<Actor> actorList;
private:
	COORD location;
	string message;
	COORD entrances[4];
	int tileArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int itemArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	int actorArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
};

class Item
{
public:
	Item();
	Item(Tile, Stats);
	Tile getTile() { return tile; }
	Stats getStats() { return stats; }

	void setTile(Tile t) { tile = t; }
private:
	Tile tile;
	Stats stats;

};

///////////////////////////////////////////////////////////////////////////////
// Global variables

const Tile tileIndex[] = {	// symbol, colour, isPassable
	{ ' ', con::fgBlack, true, 0 },	// (0) MAP_FLOOR
	{ '=', con::fgHiGreen, false, 1 },	// (1) MAP_WALL_TOP
	{ 'D', con::fgHiBlue, true, 2 },	// (2) MAP_DOOR
	{ '|', con::fgHiGreen, false, 3 },	// (3) MAP_WALL_SIDE

};
const Item itemIndex[] = {
	Item({ ' ', con::fgBlack, true, 0 }, { 1, 1, 1 }),		// (0) NULL
	Item({ '~', con::fgHiWhite, true, 1 }, { 1, 1, 1 }),	// (1) KEY
	Item({ 'D', con::fgLoBlue, false, 2 }, { 1, 1, 1 }),	// (2) DOOR_TO_NEW_ROOM
	Item({ 'D', con::fgHiRed, false, 3 }, { 1, 1, 1 })		// (3) MAP_DOOR_LOCKED

};

const Actor actorIndex[] = {
	Actor(),												// (0) NULL
	Actor({ 'X', con::fgHiWhite, false, 1 }, { 10, 2, 1 }),	// (1) BULLY_WEAK
};

map<string, string> messages =
{
	{ "Q_RANDOM",		"Random: What do you want ?" },
	{ "GET_KEY",		"It's a key! You picked it up." },
	{ "DOOR_LOCKED",	"The door is locked, you need a key." },
	{ "USE_KEY",		"You used a key!" },
	{ "Q_USE_KEY",		"The door is locked, use a key?"},
	{ "Q_NEXT_ROOM",	"Go to next room?" },
	{ "NEW_ROOM",		"New room." },
	{ "ENEMY_DEATH",	"It died..." },
	{ "UNATTACKABLE", "You can't kill that, you silly!" },
	{ "ATTACKABLE",		"You hit that thing with " },
	{ "ENEMY_ATTACK", "You got hit with " },
	{ "PLAYER_DEATH", "You died..." }
};

Room roomArray[schooled::FLOOR_HEIGHT][schooled::FLOOR_WIDTH];

///////////////////////////////////////////////////////////////////////////////
// Operators

bool operator ==(COORD a, COORD b)
{
	return (a.X == b.X && a.Y == b.Y);
}

COORD operator +(COORD a, COORD b)
{
	return { a.X + b.X, a.Y + b.Y };
}

///////////////////////////////////////////////////////////////////////////////
// Function declarations

// Changes the current room to the number of the one given.
void changeRoom(Room&, COORD, vector<Actor>);

// Deletes all dynamic variables
void exitGame();

// initializes the buffer
void setBuffer();

// Moves the enemy
void moveEnemy(COORD, Actor&, Room&);

bool lineOfSight(COORD, Actor&, Room&);

// Draws the log to the screen
void displayLog(vector<string>);

// draws a string to the buffer
void drawToBuffer(string, WORD, int, int);

// Finds the actor in the current room
int findActor(vector<Actor>, COORD);
int eAttack = 0;

void drawTile(int x, int y);
void enemy1();
void flavourText();



int main()
{
	///////////////////////////////////////////////////////////////////////////
	// Initialization
	console.SetTitle("Schooled V0.1");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output

	// Initialize the player's on-screen location
	Actor player({ '8', con::fgHiWhite}, { 10, 2, 2 });
	player.setLocation({ 2, 18 });
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
	Room roomOne("Rooms/Room1_1.txt");
	roomOne.setLocation({ 1, 1 });

	Room roomTwo("Rooms/Room2_1.txt");
	roomTwo.setLocation({ 1, 2 });

	Room roomThree("Rooms/Room3_1.txt");
	roomThree.setLocation({ 1, 0 });

	// Puts the rooms into the floor array
	roomArray[roomOne.getX()][roomOne.getY()] = roomOne;
	roomArray[roomTwo.getX()][roomTwo.getY()] = roomTwo;
	roomArray[roomThree.getX()][roomThree.getY()] = roomThree;

	Room currentRoom = roomOne;
	vector<Actor> actorList = currentRoom.actorList;

	// Initialize the log
	vector<string> log;
	log.push_back("");

	// Initialize the buffer
	ReadConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
		schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);

	setBuffer();

	WriteConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
		schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);

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
		currentRoom.display();

		// Display the character
		schooled::buffer[player.getY()][player.getX()].Attributes = con::fgHiWhite;
		schooled::buffer[player.getY()][player.getX()].Char.AsciiChar = '8';

		// Display the highlight
		schooled::buffer[highlight.Y][highlight.X].Attributes = con::bgHiWhite;

		// Display the messages
		displayLog(log);

		// Display stats
		drawToBuffer(to_string(keyCount), con::fgHiWhite, 21, 5);	// Key count
		drawToBuffer((to_string(player.getLocation().X) + ","		// Player coordinates
			+ to_string(player.getLocation().Y)), con::fgHiWhite, 22, 5);

		drawToBuffer(("HP: " + to_string(player.getStats().HP)), con::fgHiWhite, 24, 5);	// Player hitpoints
		
		// Writes the buffer to the screen
		WriteConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
			schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);		

		if (player.getStats().HP <= 0)
			return 0;


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
			if (currentRoom.getActorInt(highlight) > 0){
				Actor *a = &actorList[findActor(actorList, highlight)];
				player.attack(*a);
				log.push_back(messages["ATTACKABLE"] +  to_string(player.getStats().STR) + " damage! Wow!");

				// If the actor died
				if (a->getStats().HP <= 0)
				{
					currentRoom.setActorInt(a->getLocation(), 0);
					log.push_back(messages["ENEMY_DEATH"]);
					actorList.erase(actorList.begin() + findActor(actorList, highlight));
					
				}
				else
				{
					a->attack(player);
					log.push_back(messages["ENEMY_ATTACK"] + to_string(a->getStats().STR) + " damage! Ouch!");
				}
				if (player.getStats().HP <= 0){
					log.push_back(messages["PLAYER_DEATH"]);
					KEYPRESS aKeyPress = console.WaitForKeypress();
				}
			}
			else{
				log.push_back(messages["UNATTACKABLE"]);
			}
			for (Actor& a : actorList)
			{
				moveEnemy(player.getLocation(), a, currentRoom);
			}
			break;

			//checks interactable
		case CONSOLE_KEY_SPACE:
			switch (currentRoom.getItemInt(highlight))
			//basically if the object is interactable
			{
				// KEY
			case 1:
				log.push_back(messages["GET_KEY"]);
				keyCount++;
				currentRoom.setItemInt(highlight, 0);
				break;

				// MAP_DOOR_LOCKED
			case 3:
				if (useKey == true && keyCount > 0)
				{
					log.push_back(messages["USE_KEY"]);
					keyCount--;
					currentRoom.setItemInt(highlight, 0);
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
					if (player.getY() < 10)  // going up
					{
						changeRoom(currentRoom, { 0, 1 }, actorList);
						player.setLocation(currentRoom.getSouth());
						highlight.Y = currentRoom.getSouth().Y - 1;
						highlight.X = currentRoom.getSouth().X;
					}
					else if (player.getY() > 10)	// going down
					{
						changeRoom(currentRoom, { 0, -1 }, actorList);
						player.setLocation(currentRoom.getNorth());
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
			for (Actor& a : actorList)
			{
				moveEnemy(player.getLocation(), a, currentRoom);
			}
			break;
			
			// move key pressed
		case CONSOLE_KEY_M:
			delta.X = (highlight.X - player.getX());
			delta.Y = (highlight.Y - player.getY());

			// Check if the player can move in specified direction
			if (currentRoom.isPassable(highlight))
			{
				// If allowed, move in specified direction
				player.setLocation(highlight);
				for (Actor& a : actorList)
				{
						moveEnemy(player.getLocation(), a, currentRoom);
				}
			}
			break;

			// quit
		case CONSOLE_KEY_ESCAPE:
			currentRoom.save("Rooms/Room1.sav");
			exitGame();
			return 0;

			// Ignore any other key
		default:
			break;
		}
		
		
		// Check if a move action has been performed, and adjusts highlight
		if (delta.X != 0 || delta.Y != 0)
		{
			highlight.X = player.getX() + delta.X;
			highlight.Y = player.getY() + delta.Y;
			
		}
		
	}
	return 0;
}

Item::Item(Tile t, Stats s) : tile(t), stats(s) {}

Item::Item() {}

Actor::Actor(Tile t, Stats s) : tile(t), stats(s) {}

Actor::Actor() {}

int findActor(vector<Actor> a, COORD c)
{
	for (unsigned int i = 0; i < a.size(); i++)
	{
		if (a[i].getLocation() == c)
			return i;
	}
	return -1;
}

void Room::display(){
	int tile;
	Actor tempA;
	Item tempI;
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			if (actorArray[a][b] > 0)
			{
				tile = actorArray[a][b];
				tempA = actorIndex[tile];
				schooled::buffer[a][b].Char.AsciiChar = tempA.getTile().character;
				schooled::buffer[a][b].Attributes = tempA.getTile().colorCode;
			}
			else if (itemArray[a][b] > 0)
			{
				tile = itemArray[a][b];
				tempI = itemIndex[tile];
				schooled::buffer[a][b].Char.AsciiChar = tempI.getTile().character;
				schooled::buffer[a][b].Attributes = tempI.getTile().colorCode;
			}
			else
			{
				tile = tileArray[a][b];
				schooled::buffer[a][b].Char.AsciiChar = tileIndex[tile].character;
				schooled::buffer[a][b].Attributes = tileIndex[tile].colorCode;
			}
		}
	}
}

bool Room::isPassable(COORD tile){
	int mapX = tile.X;
	int mapY = tile.Y;
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)
		return false;

	int tileValue = tileArray[mapY][mapX];
	int itemValue = itemArray[mapY][mapX];
	int actorValue = actorArray[mapY][mapX];
	Item tempI = itemIndex[itemValue];
	if (tileIndex[tileValue].isPassable && actorValue == 0 && tempI.getTile().isPassable)
		return true;
	return false;
}

void Actor::attack(Actor& defender){
	defender.stats.HP -= stats.STR;
}

void changeRoom(Room& currentRoom, COORD change, vector<Actor> actorList)
{
	currentRoom.actorList = actorList;
	roomArray[currentRoom.getX()][currentRoom.getY()] = currentRoom;
	currentRoom = roomArray[currentRoom.getX() + change.X][currentRoom.getY() + change.Y];

}

void displayLog(vector<string> log)
{
	int max = (log.size() >= 3) ? (log.size() - 3) : 0;
	int row, col;
	row = schooled::SCREEN_HEIGHT-1;
	col = 23;
	for (int i = log.size() - 1; i >= max; i--)
	{
		drawToBuffer(log[i], con::fgHiWhite, row, col);
		row--;
		col = 23;
	}
}

void setBuffer()
{
	for (int a = 0; a < schooled::SCREEN_HEIGHT; a++){
		for (int b = 0; b < schooled::SCREEN_WIDTH; b++){
			schooled::buffer[a][b].Char.AsciiChar = ' ';
			schooled::buffer[a][b].Attributes = con::fgBlack | con::bgBlack;
		}
	}
}

void drawToBuffer(string s, WORD w, int row, int col)
{
	int startCol = col;
	for (int i = 0; i < s.size(); i++)
	{
		schooled::buffer[row][col].Char.AsciiChar = s[i];
		schooled::buffer[row][col].Attributes = w;
		if (col < schooled::SCREEN_WIDTH - 1)
		{
			col++;
		}
		else
		{
			col = startCol;
			row--;
		}
	}
}

Room::Room() {}

Room::Room(string fileName)
{
	ifstream stream;
	stream.open(fileName);

	if (stream.fail())
	{
		cout << "File open failed.\n";	
		exit(1); 
	}

	string m;
	getline(stream, m);
	message = m;
	getline(stream, m); // blank

	// Get the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> tileArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the entrance coordinates
	for (COORD &c : entrances)
	{
		stream >> c.X >> c.Y;
	}
	getline(stream, m); // blank

	// Get the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> itemArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> actorArray[a][b];
			if (actorArray[a][b] > 0)
			{
				actorList.push_back(actorIndex[actorArray[a][b]]);
				actorList[actorList.size() - 1].setLocation({ b, a });
			}
		}
	}
	stream.close();
}

string Room::getMessage() { return message; }

void Room::save(string fileName)
{
	ofstream stream;
	stream.open(fileName);

	if (stream.fail())
	{
		cout << "File open failed.\n";
		exit(1);
	}
	
	stream << message << endl << endl;

	// Save the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << tileArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << endl;

	// Save the entrance coordinates
	for (COORD &c : entrances)
	{
		stream << c.X << " " << c.Y << endl;
	}
	stream << endl;

	// Save the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << itemArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << endl;

	// Save the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << actorArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << flush;
	stream.close();
}

void exitGame()
{

	return;
}

//Actor a : actorList;
//int enemyX = a.getX();
//int enemyY = a.getY();

void moveEnemy(COORD playerPos, Actor& enemy, Room& currentRoom) 
{
	// If enemy line of sight then move
	int differenceX, differenceY, deltaX, deltaY;
	int enemyX = enemy.getX();
	int enemyY = enemy.getY();
	int playerX = playerPos.X;
	int playerY = playerPos.Y;
	deltaX = 0;
	deltaY = 0;
	differenceX = enemyX - playerX;
	differenceY = enemyY - playerY;

	if (lineOfSight(playerPos, enemy, currentRoom) == true){
		if (differenceX > -2 && differenceX < 2 && differenceY > -2 && differenceY < 2){
			deltaX = 0;
			deltaY = 0;
		}
		else{
			if (abs(differenceX) > abs(differenceY))
			{
				deltaX = (enemyX > playerX) ? -1 : 1;
				if (!currentRoom.isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaX = 0;
					deltaY = (enemyY > playerY) ? -1 : 1;
				}
			}
			else
			{
				deltaY = (enemyY > playerY) ? -1 : 1;
				if (!currentRoom.isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaY = 0;
					deltaX = (enemyX > playerX) ? -1 : 1;
				}
			}
		}
	}
	else{
		deltaX = 0;
		deltaY = 0;
	}

	if (currentRoom.isPassable({ enemyX + deltaX, enemyY + deltaY })) 
	{
		currentRoom.setActorInt(enemy.getLocation(), 0);
		enemy.setLocation({ enemyX + deltaX, enemyY + deltaY });
		currentRoom.setActorInt(enemy.getLocation(), enemy.getTile().tileInt);
	}
}
bool lineOfSight(COORD playerPos, Actor& enemy, Room& currentRoom)
{
	bool isFound = false;

	for (int a = 1; isFound == false; a++)
	{
		if (! currentRoom.isPassable({ enemy.getX() - a, enemy.getY() }))
		{
			enemy.setMinX(enemy.getX() - a - 1);
			isFound = true;
		}
	}
	isFound = false;
	for (int a = 1; isFound == false; a++)
	{
		if (! currentRoom.isPassable({ enemy.getX() + a, enemy.getY() }))
		{
			enemy.setMaxX(enemy.getX() + a + 1);
			isFound = true;
		}
	}
	isFound = false;
	for (int a = 1; isFound == false; a++)
	{
		if (! currentRoom.isPassable({ enemy.getX(), enemy.getY() - a }))
		{
			enemy.setMinY(enemy.getY() - a - 1);
			isFound = true;
		}

	}
	isFound = false;
	for (int a = 1; isFound == false; a++)
	{
		if (! currentRoom.isPassable({ enemy.getX(), enemy.getY() + a }))
		{
			enemy.setMaxY(enemy.getY() + a + 1);
			isFound = true;
		}

	}
	if (playerPos.X >= enemy.getMinX() && playerPos.Y >= enemy.getMinY() && playerPos.X <= enemy.getMaxX() && playerPos.Y <= enemy.getMaxY())
		return true;
	else
		return false;
}