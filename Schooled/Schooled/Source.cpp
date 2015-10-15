#include <iostream>
#include <conio.h>
#include "Console Library/Console.h"
using namespace std;

//declaring the map and it's dimensions
int const MAP_WIDTH = 20;
int const MAP_HEIGHT = 15;
int const MAP_FLOOR = 0;
int const MAP_WALL_TOP = 1;
int const MAP_DOOR = 2;
int const MAP_WALL_SIDE = 3;
int roomOneArray[MAP_HEIGHT][MAP_WIDTH]{
	{ 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 1, 1, 1, 3 },
	{ 3, 1, 2, 1, 1, 1, 1, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 3, 1, 2, 1, 3, 0, 0, 3, 1, 1, 1, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3 },
	{ 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3 },
	{ 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3 }
};

class Tile
{
public:
	char getChar() { return character; };
	int getColorCode() { return colorCode; }
private:
	char character;
	int colorCode;
};
void displayMap();

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
		displayMap();
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
void displayMap(){
	for (int a = 0; a < MAP_HEIGHT; a++){
		console.Position(0,a);
		for (int b = 0; b < MAP_WIDTH; b++){
			if (roomOneArray[a][b] == 0){
				console << ' ';
			}
			else if (roomOneArray[a][b] == 1){
				console << '_';
			}
			else if (roomOneArray[a][b] == 2){
				console << 'D';
			}
			else if (roomOneArray[a][b] == 3){
				console << '|';
			}
		}
	}
}

