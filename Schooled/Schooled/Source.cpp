#include <iostream>
#include <cassert>
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

	int const MAP_FLOOR = 0;
	int const MAP_WALL_TOP = 1;
	int const MAP_DOOR = 2;
	int const MAP_WALL_SIDE = 3;
	int const ENEMY = 4;

	COORD enemy1{ 18, 16 };

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
}

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

//declaring the map and it's dimensions
int roomOneArray[schooled::MAP_HEIGHT][schooled::MAP_WIDTH]{
	{ 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 },
	{ 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 2, 1, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3 },
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
const Tile tileIndex[] = {
	{ ' ', con::fgBlack, true },	// (0) MAP_FLOOR
	{ '=', con::fgHiGreen, false },	// (1) MAP_WALL_TOP
	{ 'D', con::fgHiBlue, true },	// (2) MAP_DOOR
	{ '|', con::fgHiGreen, false },	// (3) MAP_WALL_SIDE
	{ 'X', con::fgHiWhite, false }  // (4) ENEMY
};

void displayMap();	// Display the map
int message = 0;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output

///////////////////////////////////////////////////////////////////////////////
// Function declarations

// Draws the map to the screen
void displayMap();

// Checks if a tile is passable
bool isPassable(int mapX, int mapY);

// Checks if a tile is interactable
bool isInteractable(int mapX, int mapY);

void enemy1();
void flavourText();


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
		displayMap();

		// Display the character
		schooled::buffer[player.Y][player.X].Attributes = con::fgHiWhite;
		schooled::buffer[player.Y][player.X].Char.AsciiChar = '8';

		// Display the highlight
		schooled::buffer[highlight.Y][highlight.X].Attributes = con::bgHiWhite;
		
		// Writes the buffer to the screen
		WriteConsoleOutput(hConsole, (CHAR_INFO *)schooled::buffer,
			schooled::dwBufferSize, schooled::dwBufferCoord, &rcRegion);

		// Writes a message to the screen
		if (message == 1){
			console.Position(21, 21);
			SetConsoleTextAttribute(hConsole, con::fgHiWhite);
			console << "Random: What do you want?";
		}

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

			// move key pressed
		case CONSOLE_KEY_M:
			delta.X = (highlight.X - player.X);
			delta.Y = (highlight.Y - player.Y);

			// Check if the player can move in specified direction
			if (isPassable(highlight.X, highlight.Y))
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
		//checks to see if anything is interactable
		if (isInteractable(player.X + delta.X, player.Y + delta.Y))
		{
			message = 1;
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
void displayMap(){
	int tile;

	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			tile = roomOneArray[a][b];
			schooled::buffer[a][b].Char.AsciiChar = tileIndex[tile].character;
			schooled::buffer[a][b].Attributes = tileIndex[tile].colorCode;
		}
	}
}

bool isPassable(int mapX, int mapY){
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)
		return false;

	int tileValue = roomOneArray[mapY][mapX];
	if (tileIndex[tileValue].isPassable)
		return true;
	else
		return false;
}
bool isInteractable(int mapX, int mapY){
	int tileValue = roomOneArray[mapY][mapX];
	if (tileValue == 4)
		return true;
	else
		return false;
}
