#include "../Header Files/PlayingState.h"
#include "../Header Files/GameEngine.h"
#include "../Header Files/GameState.h"
#include "../Header Files/Schooled.h"

#include "../Header Files\Item.h"
#include "../Header Files\Buffer.h"
#include "../Header Files\Console_color.h"
#include "../Header Files\Actor.h"
#include "../Header Files\Room.h"
#include "../Header Files\Log.h"

using std::string;
using std::to_string;

PlayingState PlayingState::m_PlayingState;

void PlayingState::Init()
{
	tCount = 0;
	keyCount = 0;
	pTurn = true;

	loadRooms();
	currentRoom = roomArray[1][1];

	player = Actor({ '8', con::fgHiWhite }, { 10, 2, 2 });
	player.setLocation({ 2, 3 });
	highlight = { player.getX(), player.getY() + 1 };
	delta = { 0, 0 };
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void PlayingState::Cleanup()
{

}

void PlayingState::Pause()
{

}

void PlayingState::Resume()
{

}

void PlayingState::HandleEvents(GameEngine* game)
{
	if (pTurn)
	{
		KEYPRESS sKeyPress = console.WaitForKeypress();

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
			incrementTurn();
			if (currentRoom.getActorInt(highlight) > 0){
				Actor *a = &currentRoom.getActor(highlight);
				player.attack(currentRoom.getActor(highlight));
				log.push_back(a->getMDefend() + " Deal " + to_string(player.getStats().STR) + " damage! Wow!");

				// If the actor died
				if (a->getStats().HP <= 0)
				{
					currentRoom.setActorInt(a->getLocation(), 0);
					log.push_back(messages["ENEMY_DEATH"]);
					currentRoom.setItemInt(a->getLocation(), a->dropItem());
					currentRoom.removeActor(highlight);
				}
			}
			else{
				log.push_back(messages["UNATTACKABLE"]);
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
				break;

			default:
				break;

			}
			break;

			// move key pressed
		case CONSOLE_KEY_M:
			incrementTurn();
			delta.X = (highlight.X - player.getX());
			delta.Y = (highlight.Y - player.getY());

			// Check if the player can move in specified direction
			if (currentRoom.isPassable(highlight))
			{
				// If allowed, move in specified direction
				player.setLocation(highlight);
			}
			break;

			// quit
		case CONSOLE_KEY_ESCAPE:
			currentRoom.save("Rooms/Room1.sav");
			game->Quit();

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
	else
	{
		enemyTurn();
		incrementTurn();
	}
}

void PlayingState::Update(GameEngine* game)
{
	delta.X = 0;
	delta.Y = 0;

}

void PlayingState::Draw(GameEngine* game)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Open the buffer for writing
	buffer.open(hConsole);

	// Clear the buffer
	buffer.clear();

	// Draw the map
	currentRoom.display(buffer);

	// Display the character
	buffer.draw('8', con::fgHiWhite, player.getY(), player.getX());

	// Display the highlight
	buffer.draw(con::bgHiWhite, highlight.Y, highlight.X);

	// Display stats
	buffer.draw("Keys: " + to_string(keyCount), con::fgHiWhite, 22, 5);	// Key count
	buffer.draw((to_string(player.getLocation().X) + ","		// Player coordinates
		+ to_string(player.getLocation().Y)), con::fgHiWhite, 24, 5);
	string tempTurn = (pTurn) ? "Player" : "Enemy";
	buffer.draw("Turn: " + tempTurn + " " + to_string(tCount), con::fgHiWhite, 23, 5);
	buffer.draw(("HP: " + to_string(player.getStats().HP)), con::fgHiWhite, 21, 5);	// Player hitpoints

	// Display the messages
	log.display(buffer);

	// Close the buffer
	buffer.close(hConsole);
}

void PlayingState::loadRooms()
{
	// Load the rooms from the file
	Room roomOne("Rooms/Room4_1.txt");
	roomOne.setLocation({ 1, 1 });

	Room roomTwo("Rooms/Room2_1.txt");
	roomTwo.setLocation({ 1, 2 });

	Room roomThree("Rooms/Room3_1.txt");
	roomThree.setLocation({ 1, 0 });

	// Puts the rooms into the floor array
	roomArray[roomOne.getX()][roomOne.getY()] = roomOne;
	roomArray[roomTwo.getX()][roomTwo.getY()] = roomTwo;
	roomArray[roomThree.getX()][roomThree.getY()] = roomThree;

	currentRoom = roomOne;
}

void PlayingState::changeRoom(Room& currentRoom, COORD change)
{
	roomArray[currentRoom.getX()][currentRoom.getY()] = currentRoom;
	currentRoom = roomArray[currentRoom.getX() + change.X][currentRoom.getY() + change.Y];

}

void PlayingState::incrementTurn()
{
	tCount++;

	// If it is the player's turn, check if they are out of turns
	if (pTurn && tCount == player.getStats().EN)
	{
		tCount = 0;
		pTurn = false;
	}
	// If it is the enemies' turn
	else if (!pTurn)
	{
		// Find the highest EN stat
		int highestEN = 0;
		for (Actor a : currentRoom.getActorList())
		{
			if (a.getStats().EN > highestEN)
			{
				highestEN = a.getStats().EN;
			}
		}
		// Check if turn count is exceeding highest EN
		if (tCount >= highestEN)
		{
			tCount = 0;
			pTurn = true;
		}
	}
}

void PlayingState::enemyTurn()
{
	
	for (Actor& a : currentRoom.getActorList())
	{
		if (a.getStats().EN+1 >= tCount)
		{
			if (currentRoom.isAdjacent(player.getLocation(), a))
			{
				a.attack(player);
				log.push_back(a.getMAttack() + " Take " + to_string(a.getStats().STR) + " damage! Ouch!");
				Sleep(200);
				if (player.getStats().HP <= 0)
				{
					log.push_back(messages["PLAYER_DEATH"]);
					KEYPRESS aKeyPress = console.WaitForKeypress();
				}
			}
			else
			{
				//currentRoom.moveActors(player.getLocation());
				currentRoom.moveEnemy(player.getLocation(), a);
			}
		}
		
	}
	
}