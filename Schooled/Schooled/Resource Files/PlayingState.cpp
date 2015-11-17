#include "../Header Files/PlayingState.h"
#include "../Header Files/GameOverState.h"
#include "../Header Files/MenuState.h"
#include "../Header Files\Item.h"
#include "../Header Files\Console_color.h"

using std::string;
using std::to_string;

namespace con = JadedHoboConsole;

PlayingState PlayingState::m_PlayingState;

///////////////////////////////////////////////////////////////////////////////
// State Handling
void PlayingState::Init()
{
	tCount = 0;
	keyCount = 0;
	pTurn = true;
	running = true;
	loadRooms();
	currentRoom = roomArray[1][1];

	player = Actor({ '8', con::fgHiWhite }, { 10, 2, 2 });
	player.setLocation({ 4, 4 });
	highlight = { player.getX(), player.getY() + 1 };
	delta = { 0, 0 };
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	scheme = schooled::getSetting("ControlScheme");
}

void PlayingState::Cleanup()
{
	log.clear();
	for (int i = 0; i < schooled::FLOOR_HEIGHT; i++)
	{
		for (int e = 0; e < schooled::FLOOR_WIDTH; e++)
		{
			roomArray[i][e] = Room();
		}
	}
}

void PlayingState::Pause()
{

}

void PlayingState::Resume()
{

}

void PlayingState::HandleEvents(GameEngine* game)
{
	if (!pTurn) return;
	
	KEYPRESS sKeyPress = console.WaitForKeypress();
	switch (sKeyPress.eCode)
	{
		// Move the highlight
	case CONSOLE_KEY_DOWN:
	case CONSOLE_KEY_LEFT:
	case CONSOLE_KEY_RIGHT:
	case CONSOLE_KEY_UP:
	case CONSOLE_KEY_W:
	case CONSOLE_KEY_A:
	case CONSOLE_KEY_S:
	case CONSOLE_KEY_D:
		moveHighlight(sKeyPress.eCode);
		break;

		//attack things B)
	case CONSOLE_KEY_X:
	case CONSOLE_KEY_N:
		// Check control schemes
		if ((sKeyPress.eCode == CONSOLE_KEY_X &&
			(scheme == "Classic" || scheme == "Double-Tap")) ||
			sKeyPress.eCode == CONSOLE_KEY_N &&
			(scheme == "Classic Lefty" || scheme == "Double-Tap Lefty"))
		{
			tCount++;
			increment = true;
			attack();
		}

		break;

		//checks interactable
	case CONSOLE_KEY_SPACE:
		interact();
		break;

		// move key pressed
	case CONSOLE_KEY_Z:
	case CONSOLE_KEY_M:
		// Check control schemes
		if (sKeyPress.eCode == CONSOLE_KEY_Z && scheme == "Classic" ||
			sKeyPress.eCode == CONSOLE_KEY_M && scheme == "Classic Lefty")
		{
			delta.X = (highlight.X - player.getX());
			delta.Y = (highlight.Y - player.getY());

			// Check if the player can move in specified direction
			if (currentRoom.isPassable(highlight))
			{
				// If allowed, move in specified direction
				player.setLocation(highlight);
				tCount++;
				increment = true;
			}
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
}

void PlayingState::Update(GameEngine* game)
{
	// If the game over screen has occured, return to menu
	if (!running)
	{
		game->PopState();
		return;
	}

	// If the player is dead, quit the game
	if (player.getStats().HP <= 0 && running)
	{
		game->ChangeState(GameOverState::Instance());
		running = false;
		return;
	}

	// Check if a move action has been performed, and adjusts highlight
	if (delta.X != 0 || delta.Y != 0)
	{
		highlight.X = player.getX() + delta.X;
		highlight.Y = player.getY() + delta.Y;

	}
	// Reset the delta
	delta.X = 0;
	delta.Y = 0;

	// Move the enemies
	if (!pTurn)
	{
		enemyTurn();
		tCount++;
		increment = true;
	}

	// If the turn counter was incremented
	if (increment)
	{
		incrementTurn();
	}
	increment = false;
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
	buffer.draw("Keys: " + to_string(keyCount), con::fgHiWhite, 24, 5);	// Key count
	//buffer.draw((to_string(player.getLocation().X) + ","		// Player coordinates
	//	+ to_string(player.getLocation().Y)), con::fgHiWhite, 24, 5);
	string tempTurn = (pTurn) ? "Player" : "Enemy";
	buffer.draw(("HP: " + to_string(player.getStats().HP)), con::fgHiWhite, 21, 5);	// Player hitpoints
	buffer.draw(("EN: " + to_string(player.getStats().EN)), con::fgHiWhite, 22, 5); // Player endurance
	buffer.draw(("STR: " + to_string(player.getStats().STR)), con::fgHiWhite, 23, 5); //Player strength

	// Display the messages
	log.display(buffer);

	// Close the buffer
	buffer.close(hConsole);
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions

void PlayingState::attack()
{
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
}

void PlayingState::changeRoom(Room& cRoom, COORD change)
{
	roomArray[cRoom.getX()][cRoom.getY()] = cRoom;
	cRoom = roomArray[cRoom.getX() + change.X][cRoom.getY() + change.Y];

}

void PlayingState::enemyTurn()
{

	for (Actor& a : currentRoom.getActorList())
	{
		if (a.getStats().EN > tCount)
		{
			if (currentRoom.isAdjacent(player.getLocation(), a))
			{
				a.attack(player);
				log.push_back(a.getMAttack() + " Take " + to_string(a.getStats().STR) + " damage! Ouch!");
				Sleep(200);
			}
			else
			{
				currentRoom.moveEnemy(player.getLocation(), a);
			}
		}

	}

}

void PlayingState::incrementTurn()
{

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

void PlayingState::interact()
{
	switch (currentRoom.getItemInt(highlight))
		//basically if the object is interactable
	{
		// KEY
	case 1:
		log.push_back(messages["GET_KEY"]);
		keyCount++;
		currentRoom.setItemInt(highlight, 0);
		break;

	case 2:
		//room transition
		transitionRoom();
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
		{
			log.push_back(messages["DOOR_LOCKED"]);
		}
		break;
	// POTION
	case 4:
		log.push_back(messages["POTION"]);
		player.pickUp(currentRoom.getItemStats(currentRoom.randomItem()));
		currentRoom.setItemInt(highlight, 0);
		break;

	default:
		break;
	}
}

void PlayingState::loadRooms()
{
	// Load the rooms from the file
	Room roomOne("Rooms/Room4_1.txt");
	roomOne.setLocation({ 1, 1 });

	Room roomTwo("Rooms/Room2_1.txt");
	roomTwo.setLocation({ 1, 0 });

	Room roomThree("Rooms/Room3_1.txt");
	roomThree.setLocation({ 1, 2 });

	Room roomFour("Rooms/Level_4.txt");
	roomFour.setLocation({ 0, 1 });

	// Puts the rooms into the floor array
	roomArray[roomOne.getX()][roomOne.getY()] = roomOne;
	roomArray[roomTwo.getX()][roomTwo.getY()] = roomTwo;
	roomArray[roomThree.getX()][roomThree.getY()] = roomThree;
	roomArray[roomFour.getX()][roomFour.getY()] = roomFour;

	currentRoom = roomOne;
}

void PlayingState::moveHighlight(KEYCODE eCode)
{
	switch (eCode)
	{
		// down selected
	case CONSOLE_KEY_S:
	case CONSOLE_KEY_DOWN:
		delta.X = 0;
		delta.Y = 1;
		break;

		// left selected
	case CONSOLE_KEY_A:
	case CONSOLE_KEY_LEFT:
		delta.X = -1;
		delta.Y = 0;
		break;

		// right selected
	case CONSOLE_KEY_D:
	case CONSOLE_KEY_RIGHT:
		delta.X = 1;
		delta.Y = 0;
		break;

		// up selected
	case CONSOLE_KEY_W:
	case CONSOLE_KEY_UP:
		delta.X = 0;
		delta.Y = -1;
		break;
	}

	if (eCode < 88 && scheme == "Double-Tap Lefty" ||
		eCode > 49663 && scheme == "Double-Tap")
	{
		// Check if the player can move in specified direction
		if (currentRoom.isPassable(highlight) && 
			highlight - player.getLocation() == delta)	// and delta is highlight space
		{
			// If allowed, move in specified direction
			player.setLocation(highlight);
			tCount++;
			increment = true;
		}
	}
	else if ((scheme == "Classic" || scheme == "Double-Tap") && eCode < 88 ||
		(scheme == "Classic Lefty" || scheme == "Double-Tap Lefty") && eCode > 49663)
	{
		delta = { 0, 0 };
	}
}

void PlayingState::transitionRoom()
{
	COORD north = currentRoom.getNorth();
	COORD south = currentRoom.getSouth();
	COORD east = currentRoom.getEast();
	COORD west = currentRoom.getWest();

	if (highlight == north)			// Going up
	{
		changeRoom(currentRoom, { 0, -1 });
		north = currentRoom.getNorth();
		player.setLocation({ north.X, north.X - 1 });
		highlight.Y = player.getY() - 1;
		highlight.X = player.getX();
	}
	else if (highlight == south)	// Going down
	{
		changeRoom(currentRoom, { 0, 1 });
		south = currentRoom.getSouth();
		player.setLocation({ south.X, south.Y + 1 });
		highlight.Y = player.getY() + 1;
		highlight.X = player.getX();
	}
	else if (highlight == east)	// Going right
	{
		changeRoom(currentRoom, { 1, 0 });
		west = currentRoom.getWest();
		player.setLocation({ west.X + 1, west.Y });
		highlight.Y = player.getY();
		highlight.X = player.getX() + 1;
	}
	else if (highlight == west)		// Going left
	{
		changeRoom(currentRoom, { -1, 0 });
		east = currentRoom.getEast();
		player.setLocation({ east.X - 1, east.Y });
		highlight.Y = player.getY();
		highlight.X = player.getX() -1;
	}
	

	log.clear();
	log.push_back(currentRoom.getMessage());
}