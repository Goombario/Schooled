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

	COORD enemy1{ 18, 16 };
        int enemy1X = 18;
	int enemy1Y = 16;

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
}

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

class Room
{
public:
	const char *getMessage();
	friend istream& operator >>(ifstream& stream, Room& r);
	friend ostream& operator <<(ofstream& stream, const Room& r);

	int roomArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH];
	COORD location;
private:
	string message;
};

struct Tile
{
	char character;
	int colorCode;
	bool isPassable;
};

struct Stats{
	int HP;
	int EN;
	int STR;
};

Stats people[] = {
	{10, 2, 2}, // our player
	{10, 2, 1}  // Weak bully? Cyber bully maybe?
};

// Global variables
const Tile tileIndex[] = {
	{ ' ', con::fgBlack, true },	// (0) MAP_FLOOR
	{ '=', con::fgHiGreen, false },	// (1) MAP_WALL_TOP
	{ 'D', con::fgHiBlue, true },	// (2) MAP_DOOR
	{ '|', con::fgHiGreen, false },	// (3) MAP_WALL_SIDE
	{ 'X', con::fgHiWhite, false }, // (4) ENEMY
	{ '~', con::fgHiWhite, true },  // (5) KEY
	{ 'D', con::fgHiRed, false },   // (6) MAP_DOOR_LOCKED
	{ 'D', con::fgLoBlue, false }   // (7) DOOR_TO_NEW_ROOM
};

map<string, char *> messages =
{
	{ "Q_RANDOM",		"Random: What do you want ?" },
	{ "GET_KEY",		"It's a key! You picked it up." },
	{ "DOOR_LOCKED",	"The door is locked, you need a key." },
	{ "USE_KEY",		"You used a key!" },
	{ "Q_USE_KEY",		"The door is locked, use a key?"},
	{ "Q_NEXT_ROOM", "Go to next room?" },
	{ "NEW_ROOM", "New room." },
	{ "ENEMY_DEATH", "It died..." },
	{ "UNATTACKABLE", "You can't kill that, you silly!" },
	{ "ATTACKABLE", "You hit that thing with " /*+ people[0].STR + " damage! Wow!"*/ }
};

string message = "Q_RANDOM";
int keyCount = 0;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output

///////////////////////////////////////////////////////////////////////////////
// Function declarations

// Draws the map to the screen
void displayMap(Room);

// Changes the current room to the number of the one given.
void changeRoom(Room&, COORD);

// Returns a room from the given file (without the COORD)
Room loadRoom(string);

// Saves a room to the given file (without the COORD)
void saveRoom(string, Room);

void drawTile(int x, int y);
void enemy1();
void flavourText();

// Checks if a tile is passable
bool isPassable(int mapX, int mapY, Room);

// Checks if a tile is interactable
int isInteractable(int mapX, int mapY, Room);
int object = 0;
int attackable = 0;
bool useKey = false;
bool goToRoom = false;
Room roomArray[schooled::FLOOR_HEIGHT][schooled::FLOOR_WIDTH];

void enemy1();
void flavourText();
int attack(int mapX, int mapY, Room currentRoom);

int main()
{
	///////////////////////////////////////////////////////////////////////////
	// Initialization
	console.SetTitle("Schooled V0.1");

	// Initialize the player's on-screen location
	COORD player{ 2, 19 };
	COORD highlight{ 2, 18 };
	COORD delta{ 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, schooled::SCREEN_WIDTH - 1,
		schooled::SCREEN_HEIGHT - 1 };

	/*Room tempRoom;
	for (int i = 1; i <= 3; i++)
	{
		tempRoom = loadRoom("Rooms/Room" + to_string(i) + ".txt");
		roomArray[tempRoom.location.X][tempRoom.location.Y] = tempRoom;
	}

	Room currentRoom = roomArray[1][1];*/

	// Load the rooms from the file
	Room roomOne = loadRoom("Rooms/Room1.txt");
	roomOne.location = { 1, 1 };

	Room roomTwo = loadRoom("Rooms/Room2.txt");
	roomTwo.location = { 1, 2 };

	Room roomThree = loadRoom("Rooms/Room3.txt");
	roomThree.location = { 1, 0 };

	// Puts the rooms into the floor array
	roomArray[roomOne.location.X][roomOne.location.Y] = roomOne;
	roomArray[roomTwo.location.X][roomTwo.location.Y] = roomTwo;
	roomArray[roomThree.location.X][roomThree.location.Y] = roomThree;

	Room currentRoom = roomOne;

	vector<const char *> log;
	string attack_c;
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
		schooled::buffer[player.Y][player.X].Attributes = con::fgHiWhite;
		schooled::buffer[player.Y][player.X].Char.AsciiChar = '8';

		// Display the highlight
		schooled::buffer[highlight.Y][highlight.X].Attributes = con::bgHiWhite;
		
		// Writes the buffer to the screen
		WriteConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
			schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);

		// Display keycount
		console.Position(5, 21);
		console << keyCount;
		
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
			attackable = attack(highlight.X, highlight.Y, currentRoom);
			if (attackable == 1){
				log.push_back(messages["ATTACKABLE"]);
				if (people[1].HP == 0){
					currentRoom.roomArray[highlight.Y][highlight.X] = 0;
					log.push_back(messages["ENEMY_DEATH"]);
				}
			}
			else{
				log.push_back(messages["UNATTACKABLE"]);
			}
			break;

			//checks interactable
		case CONSOLE_KEY_SPACE:
			object = isInteractable(highlight.X, highlight.Y, currentRoom);
			switch (object)
			//basically if the object is interactable
			{
				// ENEMY
			case 4:
				log.push_back(messages["Q_RANDOM"]);
				break;

				// KEY
			case 5:
				log.push_back(messages["GET_KEY"]);
				keyCount++;
				currentRoom.roomArray[highlight.Y][highlight.X] = 0;
				break;

				// MAP_DOOR_LOCKED
			case 6:
				if (useKey == true && keyCount > 0)
				{
					log.push_back(messages["USE_KEY"]);
					keyCount--;
					currentRoom.roomArray[highlight.Y][highlight.X] = 2;
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

			case 7:
					//room transition
				if (goToRoom == true)
				{
					if (player.Y < 10)
					{
						changeRoom(currentRoom, { 0, 1 });
						player.Y = 18;
						highlight.Y = 17;
					}
					else if (player.Y > 10)
					{
						changeRoom(currentRoom, { 0, -1 });
						player.Y = 1;
						highlight.Y = 2;
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
			delta.X = (highlight.X - player.X);
			delta.Y = (highlight.Y - player.Y);

			// Check if the player can move in specified direction
			if (isPassable(highlight.X, highlight.Y, currentRoom))
			{
				// If allowed, move in specified direction
				player.X = highlight.X;
				player.Y = highlight.Y;
			}
			break;

			// quit
		case CONSOLE_KEY_ESCAPE:
			return 0;

			// Ignore any other key
		default:
			break;
		}
		
		
		// Check if a move action has been performed, and adjusts highlight
		if (delta.X != 0 || delta.Y != 0)
		{
			highlight.X = player.X + delta.X;
			highlight.Y = player.Y + delta.Y;
		}
	}
	return 0;
}
void displayMap(Room currentRoom){
	int tile;

	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			tile = currentRoom.roomArray[a][b];
			schooled::buffer[a][b].Char.AsciiChar = tileIndex[tile].character;
			schooled::buffer[a][b].Attributes = tileIndex[tile].colorCode;
		}
	}
	

}

bool isPassable(int mapX, int mapY, Room currentRoom){
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)
		return false;

	int tileValue = 0;
	tileValue = currentRoom.roomArray[mapY][mapX];

	if (tileIndex[tileValue].isPassable)
		return true;

	else
		return false;
}
int isInteractable(int mapX, int mapY, Room currentRoom){
	int tileValue = 0;
	tileValue = currentRoom.roomArray[mapY][mapX];
	
	if (tileValue == 4)
		return 4;
	else if (tileValue == 5)
		return 5;
	else if (tileValue == 6)
		return 6;
	else if (tileValue == 7)
		return 7;
	else
		return 0;
}
int attack(int mapX, int mapY, Room currentRoom){
	int tileValue = 0;
	tileValue = currentRoom.roomArray[mapY][mapX];
	if (tileValue == 4){
		people[1].HP = people[1].HP - people[0].STR;
		return 1;
	}
	else{
		return 0;
	}
}

void changeRoom(Room& currentRoom, COORD change)
{
	roomArray[currentRoom.location.X][currentRoom.location.Y] = currentRoom;
	currentRoom = roomArray
		[currentRoom.location.X + change.X]
		[currentRoom.location.Y + change.Y];
}

const char *Room::getMessage() { return message.c_str(); }

istream& operator >>(ifstream& stream, Room& r)
{
	string m;
	getline(stream, m);
	r.message = m;
	
	//stream >> r.location.X;
	//stream >> r.location.Y;


	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> r.roomArray[a][b];
		}
	}
	return stream;
}

ostream& operator <<(ofstream& stream, const Room& r)
{
	stream << r.message << endl;
	//stream << r.location.X << " " << r.location.Y << endl;

	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << r.roomArray[a][b] << " ";
		}
		stream << endl;
	}
	return stream;
}

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
