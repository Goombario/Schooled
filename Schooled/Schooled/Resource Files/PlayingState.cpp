#include "../Header Files/PlayingState.h"
#include "../Header Files/GameOverState.h"
#include "../Header Files/MenuState.h"
#include "../Header Files/ShareState.h"
#include "../Header Files\Item.h"
#include "../Header Files\Console_color.h"
#include "../Header Files/sound_effects.h"

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

	snd::dungeonMusic->play();

	log.clear();
	log.push_back(currentRoom.getMessage());

	player = Actor({ '8', con::fgHiWhite }, { 10, 2, 2 });
	getStartLocation();
	delta = { 0, 0 };
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	scheme = schooled::getSetting("ControlScheme");
	snd::dungeonMusic->play();
}

void PlayingState::Cleanup()
{
	snd::menuHighlight->stop();
	log.clear();
}

void PlayingState::Pause()
{
	snd::menuHighlight->stop();
	snd::dungeonMusic->stop();
}

void PlayingState::Resume()
{
	snd::menuHighlight->play();
	snd::dungeonMusic->play();
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
		Pause();
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
		
		if (currentRoom.getActor(highlight).getTile().tileInt >= 13)
		{
		snd::attack1->play();

			log.push_back(a->getMDefend());
		}
		else
		{
			log.push_back(a->getMDefend() + " Deal " + to_string(player.getStats().STR) + " damage! Wow!");
		}

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
			if (currentRoom.isAdjacent(player.getLocation(), a) && a.getTile().tileInt != 13 && a.getTile().tileInt != 20)
			{
				snd::attack2->play();
				a.attack(player);
				log.push_back(a.getMAttack() + " Take " + to_string(a.getStats().STR) + " damage! Ouch!");
				Sleep(200);
			}
			else
			{
				currentRoom.moveActors(player.getLocation(), a);
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
	if (currentRoom.getActorInt(highlight) != 0)
	{
		if (currentRoom.getActor(highlight).getStats().STR == 0)
		{
			log.push_back(currentRoom.getActor(highlight).getMDefend());
			if (currentRoom.getActor(highlight).holdItem())
			{
				int temp = currentRoom.getActor(highlight).dropItem();
				if (temp == 1)
				{
					log.push_back(messages["RECIEVE_KEY"]);
					keyCount++;
				}
				else
				{
					log.push_back(currentRoom.itemIndex[temp].getMPickup());
					player.pickUp(currentRoom.getItemStats(temp));
				}
			}
		}
		else
		{
			log.push_back(messages["ENEMY_INTERACT"]);
		}
		
	}
	else
	{
		switch (currentRoom.getItemInt(highlight))
			//basically if the object is interactable
		{
			// KEY
		case 1:
		snd::key->play();
			log.push_back(messages["GET_KEY"]);
			keyCount++;
			currentRoom.setItemInt(highlight, 0);
			break;

		case 2:
			//room transition
		snd::nextRoom->play();
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
			snd::lockedDoor->play();
				log.push_back(messages["DOOR_LOCKED"]);
			}
			break;
			// FACE_PAINT
		case 4:
			log.push_back(currentRoom.itemIndex[4].getMPickup());
			player.pickUp(currentRoom.getItemStats(4));
			currentRoom.setItemInt(highlight, 0);
			break;

			// GOLDFISH
		case 5:
			log.push_back(currentRoom.itemIndex[5].getMPickup());
			player.pickUp(currentRoom.getItemStats(5));
			currentRoom.setItemInt(highlight, 0);
			break;

			// GLASSES
		case 6:
			log.push_back(currentRoom.itemIndex[6].getMPickup());
			player.pickUp(currentRoom.getItemStats(6));
			currentRoom.setItemInt(highlight, 0);
			break;

			// BACKPACK
		case 7:
			log.push_back(currentRoom.itemIndex[7].getMPickup());
			player.pickUp(currentRoom.getItemStats(7));
			currentRoom.setItemInt(highlight, 0);
			break;

			// TEARS
		case 8:
			log.push_back(currentRoom.itemIndex[8].getMPickup());
			player.pickUp(currentRoom.getItemStats(8));
			currentRoom.setItemInt(highlight, 0);
			break;

			// BANDAID
		case 9:
			log.push_back(currentRoom.itemIndex[9].getMPickup());
			player.pickUp(currentRoom.getItemStats(9));
			currentRoom.setItemInt(highlight, 0);
			break;

		default:
			break;
		}
	}
	
}

void PlayingState::getStartLocation()
{
	COORD north = currentRoom.getNorth();
	COORD south = currentRoom.getSouth();
	COORD east = currentRoom.getEast();
	COORD west = currentRoom.getWest();
	COORD empty = { 0, 0 };

	// Check which doors the player can spawn at.
	if (north != empty)
	{
		player.setLocation({ north.X, north.Y + 1 });
		highlight.Y = player.getY() + 1;
		highlight.X = player.getX();
	}
	else if (south != empty)
	{
		player.setLocation({ south.X, south.Y - 1 });
		highlight.Y = player.getY() - 1;
		highlight.X = player.getX();
	}
	else if (east != empty)
	{
		player.setLocation({ east.X + 1, east.Y });
		highlight.Y = player.getY();
		highlight.X = player.getX() + 1;
	}
	else if (west != empty)
	{
		player.setLocation({ west.X - 1, west.Y });
		highlight.Y = player.getY();
		highlight.X = player.getX() - 1;
	}
}

void PlayingState::loadRooms()
{
	Room temp;
	vector<string> roomFileList = shared::getRoomNames();
	vector<COORD> locationList = { { 1, 1 }, { 1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 2 } };
	
	// If the level selector has chosen a level
	if (MenuState::levelSelected() != 0)
	{
		temp = Room(roomFileList[MenuState::levelSelected() - 1]);
		temp.setLocation({ 1, 1 });
		roomArray[1][1] = temp;
	}
	else
	{	// Load the rooms from the file
		for (unsigned int i = 0; i < locationList.size(); i++)
		{
			temp = Room(roomFileList[i]);
			temp.setLocation(locationList[i]);
			if (temp.getX() >= 0 && temp.getY() >= 0)
			{
				roomArray[temp.getX()][temp.getY()] = temp;
			}
		}
	}
	currentRoom = roomArray[1][1];
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
	if (MenuState::levelSelected() != 0)
	{
		running = false;
		return;
	}
	COORD north = currentRoom.getNorth();
	COORD south = currentRoom.getSouth();
	COORD east = currentRoom.getEast();
	COORD west = currentRoom.getWest();

	if (highlight == north)			// Going up
	{
		changeRoom(currentRoom, { 0, -1 });
		south = currentRoom.getSouth();
		player.setLocation({ south.X, south.Y - 1 });
		highlight.Y = player.getY() - 1;
		highlight.X = player.getX();
	}
	else if (highlight == south)	// Going down
	{
		changeRoom(currentRoom, { 0, 1 });
		north = currentRoom.getNorth();
		player.setLocation({ north.X, north.Y + 1 });
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