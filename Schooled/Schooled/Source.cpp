#include <iostream>
#include <conio.h>
#include "Console Library/Console.h"
using namespace std;
namespace {
	unsigned const MAP_WIDTH = 20;
	unsigned const MAP_HEIGHT = 15;
}

class Tile
{
public:
	char getChar() { return character; };
	int getColorCode() { return colorCode; }
private:
	char character;
	int colorCode;
};

int main()
{
	console.SetTitle("Schooled V0.1");

	// Initialization
	// Initialize the player's on-screen location
	int nPlayerX = 40, nPlayerY = 12;
	//int nMapArray[MAP_WIDTH][MAP_HEIGHT];

	// Main program loop
	while (true)
	{
		// Wipe console clear
		console.Clear();

		// Output phase
		console.Position(nPlayerX, nPlayerY);
		console << '8';

		// Input phase
		KEYPRESS sKeyPress = console.WaitForKeypress();


		// Processing phase
		switch (sKeyPress.eCode)
		{
			// move down
		case CONSOLE_KEY_DOWN:
			nPlayerY++;
			break;

			// move left
		case CONSOLE_KEY_LEFT:
			nPlayerX--;
			break;

			// move right
		case CONSOLE_KEY_RIGHT:
			nPlayerX++;
			break;

			// move up
		case CONSOLE_KEY_UP:
			nPlayerY--;
			break;

			// quit
		case CONSOLE_KEY_ESCAPE:
			return 0;

			// Ignore any other key
		default:
			break;
		}

	}
	return 0;
}