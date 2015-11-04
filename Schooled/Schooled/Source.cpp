#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <conio.h>
#include <Windows.h>

#include "Header Files\Schooled.h"
#include "Header Files\Item.h"
#include "Header Files\Buffer.h"
#include "Header Files\Console_color.h"
#include "Header Files\Actor.h"
#include "Header Files\Room.h"

#include "Console Library/Console.h"

using namespace std;
namespace con = JadedHoboConsole;	// Used for the color

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

///////////////////////////////////////////////////////////////////////////////
// Global variables

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
// Function declarations

// Changes the current room to the number of the one given.
void changeRoom(Room&, COORD);

// Deletes all dynamic variables
void exitGame();

// Draws the log to the screen
void displayLog(vector<string>, Buffer&);
void displayLog(vector<string>);


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

	// Initialize the log
	vector<string> log;
	log.push_back("");

	// Initialize the buffer
	Buffer buffer;

	// Main program loop
	while (true)
	{
		// Wipe console clear
		console.Clear();

		///////////////////////////////////////////////////////////////////////
		// Output phase

		// Begins reading to the buffer.
		buffer.open(hConsole);

		// Draw the map
		currentRoom.display(buffer);

		// Display the character
		buffer.draw('8', con::fgHiWhite, player.getY(), player.getX());

		// Display the highlight
		buffer.draw(con::bgHiWhite, highlight.Y, highlight.X);

		// Display stats
		buffer.draw("Keys: " + to_string(keyCount), con::fgHiWhite, 21, 5);	// Key count
		buffer.draw((to_string(player.getLocation().X) + ","		// Player coordinates
			+ to_string(player.getLocation().Y)), con::fgHiWhite, 22, 5);

		buffer.draw(("HP: " + to_string(player.getStats().HP)), con::fgHiWhite, 24, 5);	// Player hitpoints
		
		// Writes the buffer to the screen
		buffer.close(hConsole);

		// Display the messages
		displayLog(log);

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
				Actor *a = &currentRoom.getActor(highlight);
				player.attack(currentRoom.getActor(highlight));
				log.push_back(messages["ATTACKABLE"] +  to_string(player.getStats().STR) + " damage! Wow!");

				// If the actor died
				if (a->getStats().HP <= 0)
				{
					currentRoom.setActorInt(a->getLocation(), 0);
					log.push_back(messages["ENEMY_DEATH"]);
					currentRoom.removeActor(highlight);
					
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
			currentRoom.moveActors(player.getLocation());
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
				if (keyCount > 0)
				{
					log.push_back(messages["USE_KEY"]);
					keyCount--;
					currentRoom.setItemInt(highlight, 0);
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
						changeRoom(currentRoom, { 0, 1 });
						player.setLocation(currentRoom.getSouth());
						highlight.Y = currentRoom.getSouth().Y - 1;
						highlight.X = currentRoom.getSouth().X;
					}
					else if (player.getY() > 10)	// going down
					{
						changeRoom(currentRoom, { 0, -1 });
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
			currentRoom.moveActors(player.getLocation());
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
				currentRoom.moveActors(player.getLocation());
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




void changeRoom(Room& currentRoom, COORD change)
{
	roomArray[currentRoom.getX()][currentRoom.getY()] = currentRoom;
	currentRoom = roomArray[currentRoom.getX() + change.X][currentRoom.getY() + change.Y];

}

void displayLog(vector<string> log, Buffer& buffer)
{
	int max = (log.size() >= 3) ? (log.size() - 3) : 0;	// determines the number of lines to display
	int row, col;
	int grayscale = 0;
	row = schooled::SCREEN_HEIGHT-1;
	col = 23;
	for (int i = log.size() - 1; i >= max; i--)
	{
		switch (grayscale)
		{
		case 0:
			buffer.draw(log[i], con::fgHiWhite, row, col);
			break;

		case 1:
			buffer.draw(log[i], con::fgLoWhite, row, col);
			break;

		case 2:
			buffer.draw(log[i], con::fgGray, row, col);
			break;
		}
		grayscale++;
		row--;
		col = 23;
	}
}

void displayLog(vector<string> log)
{
	int max = (log.size() >= 3) ? (log.size() - 3) : 0;	// determines the number of lines to display
	int row, col;
	int grayscale = 0;
	row = schooled::SCREEN_HEIGHT - 1;
	col = 23;
	for (int i = log.size() - 1; i >= max; i--)
	{
		switch (grayscale)
		{
		case 0:
			for (int e = 0; e < log[i].size(); e++)
			{
				consol
				console << log[i][e];
			}

			buffer.draw(log[i], con::fgHiWhite, row, col);
			break;

		case 1:
			buffer.draw(log[i], con::fgLoWhite, row, col);
			break;

		case 2:
			buffer.draw(log[i], con::fgGray, row, col);
			break;
		}
		grayscale++;
		row--;
		col = 23;
	}
}



void exitGame()
{

	return;
}

