#include <iostream>
#include <cassert>
#include <conio.h>
#include <Windows.h>
#include "Console Library/Console.h"
#include "Console Color/Console_color.h"
using namespace std;
namespace con = JadedHoboConsole;

//declaring the map and it's dimensions
namespace schooled{
	int const MAP_WIDTH = 60;
	int const MAP_HEIGHT = 20;
	int const MAP_FLOOR = 0;
	int const MAP_WALL_TOP = 1;
	int const MAP_DOOR = 2;
	int const MAP_WALL_SIDE = 3;
	int const ENEMY = 4;
	int const KEY = 5;
	int const MAP_DOOR_LOCKED = 6;
	int enemy1X = 18;
	int enemy1Y = 16;
}
#define NDEBUG

int roomOneArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH]{
	{ 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 },
	{ 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 6, 1, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 5, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3 },
	{ 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 }
};

struct Tile
{
	char character;
	int colorCode;
	bool isPassable;
};

// Global variables
Tile tileIndex[] = {
	{ ' ', con::fgBlack, true },	// (0) MAP_FLOOR
	{ '_', con::fgHiGreen, false },	// (1) MAP_WALL_TOP
	{ 'D', con::fgHiBlue, true },	// (2) MAP_DOOR
	{ '|', con::fgHiGreen, false },	// (3) MAP_WALL_SIDE
	{ 'X', con::fgHiWhite, false }, // (4) ENEMY
	{ '~', con::fgHiWhite, true },  // (5) KEY
	{ 'D', con::fgHiRed, false }  // (6) MAP_DOOR_LOCKED
};


void displayMap();	// Display the map
int message = 0;
int keyCount = 0;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output


void displayMap();
void drawTile(int x, int y);
void enemy1();
void flavourText();
bool isPassable(int mapX, int mapY);
int isInteractable(int mapX, int mapY);
int object = 0;
bool useKey = false;

int main()
{
	console.SetTitle("Schooled V0.1");

	// Initialization
	// Initialize the player's on-screen location
	int nPlayerX = 2, nPlayerY = 19;
	int nHighlightX = 2, nHighlightY = 18;
	int nDeltaX = 0, nDeltaY = 0;

	// Main program loop
	while (true)
	{
		// Wipe console clear
		console.Clear();

		///////////////////////////////////////////////////////////////////////
		// Output phase

		displayMap();

		// Display the character
		console.Position(nPlayerX, nPlayerY);
		SetConsoleTextAttribute(hConsole, con::fgHiWhite);
		console << '8';

		// Display the highlight
		console.Position(nHighlightX, nHighlightY);
		SetConsoleTextAttribute(hConsole, con::bgHiWhite);
		console << tileIndex[roomOneArray[nHighlightY][nHighlightX]].character;
		
		// Display keycount
		console.Position(5, 21);
		console << keyCount;

		// Display the messages
		if (message == 1){
			console.Position(21, 21);
			console << "Random: What do you want?";
		}
		else if (message == 2){
			console.Position(21, 21);
			console << "It's a key! You picked it up.";
		}
		else if (message == 3){
			console.Position(21, 21);
			console << "The door is locked, you need a key.";
		}
		else if (message == 4){
			console.Position(21, 21);
			console << "You used a key!";
		}
		else if (message == 5){
			console.Position(21, 21);
			console << "The door is locked, use a key?";
		}

		///////////////////////////////////////////////////////////////////////
		// Input phase

		KEYPRESS sKeyPress = console.WaitForKeypress();

		///////////////////////////////////////////////////////////////////////
		// Processing phase

		nDeltaX = 0;
		nDeltaY = 0;

		switch (sKeyPress.eCode)
		{
			// down selected
		case CONSOLE_KEY_DOWN:
			nDeltaX = 0;
			nDeltaY = 1;
			break;

			// left selected
		case CONSOLE_KEY_LEFT:
			nDeltaX = -1;
			nDeltaY = 0;
			break;

			// right selected
		case CONSOLE_KEY_RIGHT:
			nDeltaX = 1;
			nDeltaY = 0;
			break;

			// up selected
		case CONSOLE_KEY_UP:
			nDeltaX = 0;
			nDeltaY = -1;
			break;
			//checks interactable
		case CONSOLE_KEY_N:
			object = isInteractable(nHighlightX, nHighlightY);
			if (object != 0)
			{
				if (object == 4){
					message = 1;
				}
				else if (object == 5){
					message = 2;
					keyCount++;
					roomOneArray[nHighlightY][nHighlightX] = 0;
				}
				else if (object == 6){
					if (useKey == true && keyCount > 0){
						message = 4;
						keyCount--;
						roomOneArray[nHighlightY][nHighlightX] = 2;
						useKey = false;
					}
					else if (useKey == false && keyCount > 0){
						message = 5;
						useKey = true;
					}
					else{
						message = 3;
					}
				}
			}
			break;

			// move key pressed
		case CONSOLE_KEY_M:
			nDeltaX = (nHighlightX - nPlayerX);
			nDeltaY = (nHighlightY - nPlayerY);

			// Check if the player can move in specified direction
			if (isPassable(nHighlightX, nHighlightY))
			{
				// If allowed, move in specified direction
				nPlayerX = nHighlightX;
				nPlayerY = nHighlightY;
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
		if (nDeltaX != 0 || nDeltaY != 0)
		{
			nHighlightX = nPlayerX + nDeltaX;
			nHighlightY = nPlayerY + nDeltaY;
		}


	}
	return 0;
}
void displayMap(){
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		console.Position(0,a);
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			drawTile(b, a);
		}
	}
}
void drawTile(int x, int y)
{
	console.Position(x, y);
	int tile = roomOneArray[y][x];
	SetConsoleTextAttribute(hConsole, tileIndex[tile].colorCode);
	console << tileIndex[tile].character;
}

bool isPassable(int mapX, int mapY){
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)
		return false;

	int tileValue = roomOneArray[mapY][mapX];
	if (tileValue == 0)
		return true;
	else if (tileValue == 2)
		return true;
	else if (tileValue == 5)
		return true;
	else
		return false;
}
int isInteractable(int mapX, int mapY){
	int tileValue = roomOneArray[mapY][mapX];
	if (tileValue == 4)
		return 4;
	else if (tileValue == 5)
		return 5;
	else if (tileValue == 6)
		return 6;
	else
		return 0;
}
