#include "../Header Files/PlayingState.h"
#include "../Header Files/GameOverState.h"
#include "../Header Files/WinState.h"
#include "../Header Files/MenuState.h"
#include "../Header Files/ShareState.h"
#include "../Header Files/Item.h"
#include "../Header Files/Console_color.h"
#include "../Header Files/sound_effects.h"

#include <fstream>

using std::string;
using std::to_string;

// Colour namespace
namespace con = JadedHoboConsole;

PlayingState PlayingState::m_PlayingState;

///////////////////////////////////////////////////////////////////////////////
// State Handling
void PlayingState::Init()
{
	// Setting variables
	tCount = 0;
	keyCount = 0;
	bossCount = 0;
	enemyIndex = 0;
	enemyEN = 0;
	pTurn = true;
	running = true;
	winGame = false;
	masterKey = false;
	enemiesMoved = false;
	attack_animation = false;
	defend_animation = false;
	highlightColor = con::bgHiWhite;

	// Initialize the pickup flags
	pickupFlags = map<string, bool>
	{
		{ "KEY", false },
		{ "HP", false },
		{ "EN", false },
		{ "STR", false },
	};

	// Loading all the rooms
	loadRooms();

	// Clear the log
	log.clear();

	// Setting the player
	player = Actor({ '8', con::fgHiWhite }, { 12, 1, 3 });
	getStartLocation();
	delta = { 0, 0 };
	
	// Get handle to output
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Set the control scheme
	scheme = schooled::getSetting("ControlScheme");
	//snd::dungeonMusic->play();
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
	if (!pTurn) return;	// If it isn't the player's turn, end
	
	// Get the keypress
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

		// Attack key pressed
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
		snd::playerDeath->play();
		Sleep(700);
		Pause();
		game->ChangeState(GameOverState::Instance());
		running = false;
		return;
	}

	// If the principal is beaten, show the "Congrats" screen
	if (winGame)
	{
		game->ChangeState(WinState::Instance());
		running = false;
		return;
	}

	// Move the enemies
	if (!pTurn)
	{
		Actor *a = &currentRoom.getActor(enemyIndex);
		enemyTurn(*a);
		if (enemyEN < a->getStats().EN - 1)
		{
			enemyEN++;
			//a->setActive(true);
		}
		else
		{
			// Go to next enemy
			if (enemyIndex < currentRoom.getActorList().size() - 1)
			{
				enemyIndex++;
				//a->setActive(false);
			}
			else	// All enemies have moved
			{
				enemyIndex = 0;
				pTurn = true;
				/*tCount++;
				increment = true;*/
			}
			enemyEN = 0;
		}

		/*// First enemy moved shouldn't pause the game (OPTIONAL, KINDA BROKEN)
		if (a->hasActed() && !enemiesMoved)
		{
			enemiesMoved = true;
		}
		else*/ if (a->hasActed())
		{
			// If it acted, pause the game
			Sleep(500);
			a->setActed(false);	
		}
		a = nullptr;
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

	// Set the highlight colour
	if (currentRoom.getActorInt(highlight) != 0)
	{
		highlightColor = (currentRoom.getActor(highlight).getStats().EN == 0)	// If NPC, choose cyan, else red
			? con::bgHiCyan : con::bgHiRed;
	}
	else if (currentRoom.getItemInt(highlight) != 0)
	{
		highlightColor = (currentRoom.getItemInt(highlight) == 3 && keyCount == 0)	// If over locked door with no key
			? con::bgHiRed : con::bgHiGreen;
	}
	else
	{
		highlightColor = con::bgHiWhite;
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
	// Open the buffer for writing
	buffer.open(hConsole);

	// Clear the buffer
	buffer.clear();

	drawBase();

	// Close the buffer
	buffer.close(hConsole);

	// Draw special effects
	drawVFX(hConsole);

	// Reset the flags
	pickupFlags["KEY"] = false;
	pickupFlags["HP"] = false;
	pickupFlags["EN"] = false;
	pickupFlags["STR"] = false;
}

///////////////////////////////////////////////////////////////////////////////
// Private Functions

void PlayingState::attack()
{
	if (currentRoom.getActorInt(highlight) > 0) // If there's an actor
	{
		Actor *a = &currentRoom.getActor(highlight);	// Create pointer to actor
		player.attack(currentRoom.getActor(highlight));	//Attack it
		
		// If NPC, play their respective text and hit sound
		if (currentRoom.getActor(highlight).getTile().tileInt >= 13)	
		{
			snd::attack1->play();
			log.push_back(a->getMDefend(), con::fgLoCyan);
		}
		else	// Play battle sound and text
		{
			snd::attack1->play();
			log.push_back(a->getMDefend() + " Deal " + 
				to_string(player.getStats().STR) + " damage! Wow!", con::fgLoCyan);
			attack_animation = true;
		}

		// If the actor died
		if (a->getStats().HP <= 0)
		{
			// Remove actor, play death message, drop item if they have one
			currentRoom.setActorInt(a->getLocation(), 0);
			log.push_back(messages["ENEMY_DEATH"]);
			currentRoom.setItemInt(a->getLocation(), a->dropItem());

			// If it's a boss enemy
			if (a->getTile().tileInt == 3 || a->getTile().tileInt == 4 || 
				a->getTile().tileInt == 11 || a->getTile().tileInt == 47)
			{
				// Increase the count of sub-bosses defeated
				bossCount++;
				if (bossCount == 4)
				{
					// If all sub-bosses are defeated, drop masterkey
					dropMasterKey(a);
				}
			}
			// If the principal died, set win condition to true
			else if (a->getTile().tileInt == 9)
			{
				winGame = true;	
			}
			currentRoom.removeActor(highlight); // Remove dead body
		}
		a = nullptr;	// Set pointer to null. They never existed.
	}
	else
	{
		// Outputs a random idle attack message
		log.push_back(messagesLog[currentRoom.randomLog()]);
	}
}

void PlayingState::changeRoom(Room& cRoom, COORD change)
{
	// Change selected room and reset counters
	roomArray[cRoom.getX()][cRoom.getY()] = cRoom;
	cRoom = roomArray[cRoom.getX() + change.X][cRoom.getY() + change.Y];
	tCount = 0;
	enemyIndex = 0;
	highlightColor = con::bgHiWhite;
}

void PlayingState::drawBase()
{
	// Draw the map
	currentRoom.display(buffer);

	// Display the character
	buffer.draw('8', con::fgHiWhite, player.getY() + schooled::OFFSET, player.getX());

	// Display the highlight
	buffer.draw(highlightColor, highlight.Y + schooled::OFFSET, highlight.X);

	// Display keys
	if (masterKey == true)
	{
		// Masterkey in effect
		buffer.draw("JAN. KEY", con::fgHiRed, 24, 5);
	}
	else
	{
		WORD keyCol = con::fgHiWhite;
		if (pickupFlags["KEY"])
		{
			keyCol = con::fgHiGreen;
			pickupFlags["KEY"] = false;
		}
		buffer.draw("Keys: " + to_string(keyCount), keyCol, 24, 5);	// Key count
	}

	// Draw the current turn counter
	string tempTurn = (pTurn) ? "Player" : "Enemy";
	WORD turnColor;
	turnColor = (tempTurn == "Player") ? con::fgHiCyan : con::fgHiRed;
	buffer.draw("Turn: ", con::fgHiWhite, 0, 3);
	buffer.draw(tempTurn, turnColor, 0, 9);

	// Draw the current message
	int tempCol = 30 - currentRoom.getMessage().length() / 2;
	buffer.draw(currentRoom.getMessage(), con::fgHiWhite, 0, tempCol);

	// Draw the HP
	WORD HPCol;
	if (pickupFlags["HP"])
	{
		HPCol = con::fgHiGreen;
	}
	else if (player.getStats().HP < 5)
	{
		HPCol = con::fgHiRed;
	}
	else if (player.getStats().HP < 10)
	{
		HPCol = con::fgHiYellow;
	}
	else
	{
		HPCol = con::fgHiWhite;
	}
	buffer.draw(("HP: " + to_string(player.getStats().HP)), HPCol, 21, 5);	// Player hitpoints

	// Draw the EN stat
	tempCol = (pickupFlags["EN"]) ? con::fgHiGreen : con::fgHiWhite;
	buffer.draw(("EN: " + to_string(player.getStats().EN)), tempCol, 22, 5); // Player endurance

	// Draw the STR stat
	tempCol = (pickupFlags["STR"]) ? con::fgHiGreen : con::fgHiWhite;
	buffer.draw(("STR: " + to_string(player.getStats().STR)), tempCol, 23, 5); //Player strength

	// Display the messages
	log.display(buffer);
}

void PlayingState::drawVFX(HANDLE hConsole)
{
	if (attack_animation)	// Open and close buffer to simulate flashing
	{
		buffer.open(hConsole);
		buffer.draw(con::bgHiYellow, highlight.Y + schooled::OFFSET, highlight.X);
		buffer.close(hConsole);

		Sleep(100);

		buffer.open(hConsole);
		buffer.draw(highlightColor, highlight.Y + schooled::OFFSET, highlight.X);
		buffer.close(hConsole);

		attack_animation = false;
	}
	else if (defend_animation)	// Open and close buffer to simulate flashing
	{
		buffer.open(hConsole);
		buffer.draw('8', con::bgHiYellow, player.getY() + schooled::OFFSET, player.getX());
		buffer.close(hConsole);

		Sleep(100);

		buffer.open(hConsole);
		buffer.draw('8', con::bgBlack | con::fgHiWhite, player.getY() + schooled::OFFSET, player.getX());
		buffer.close(hConsole);

		defend_animation = false;
	}
}

void PlayingState::enemyTurn(Actor& a)
{
	// If the enemy has Endurance, move it
	if (a.getStats().EN > tCount)
	{
		// If the actor is an enemy and is in range, they will attack
		if (currentRoom.isAdjacent(player.getLocation(), a) && a.getTile().tileInt != 13 && a.getTile().tileInt != 20)
		{
			// Play sound effect, attack, output message, play animation, and complete turn
			snd::attack2->play();
			a.attack(player);
			log.push_back(a.getMAttack() + " Take " + to_string(a.getStats().STR) + " damage! Ouch!", con::fgLoRed);
			defend_animation = true;
			a.setActed(true);
		}
		else // If not in range, move enemy
		{
			currentRoom.moveActors(player.getLocation(), a);
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
		enemiesMoved = false;
		// Check if there are any enemies to move
		bool areEnemies = false;
		for (Actor a : currentRoom.getActorList())
		{
			if (a.getStats().EN > 0)
			{
				if (currentRoom.lineOfSight(player.getLocation(), a)) areEnemies = true;
			}
		}

		// If there are no enemies, return to player turn
		if (!areEnemies)
		{
			pTurn = true;
		}
	}
	// If it is the enemies' turn	// UNUSED
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
	if (currentRoom.getActorInt(highlight) != 0) // If the player is highlighting an actor
	{
		if (currentRoom.getActor(highlight).getStats().STR == 0) // If the actor is an NPC
		{
			// Talk to NPC
			log.push_back(currentRoom.getActor(highlight).getMDefend(), con::fgLoCyan);

			// Check if they're holding an item
			if (currentRoom.getActor(highlight).holdItem())
			{
				// Get their item type
				int temp = currentRoom.getActor(highlight).dropItem();
				if (temp == 1) // If the item is a key
				{
					log.push_back(messages["RECIEVE_KEY"]);
					keyCount++;
					pickupFlags["KEY"] = true;
				}
				else // If the item is not a key
				{
					log.push_back(currentRoom.itemIndex[temp].getMPickup());
					player.pickUp(currentRoom.getItemStats(temp));
				}
			}
		}
		else 
		{
			// The enemy just wants to fight, but you can try to talk to them
			// This isn't Undertale
			log.push_back(messages["ENEMY_INTERACT"]);
		}
		
	}
	else if (currentRoom.getItemInt(highlight) != 0) // If it's an item
	{
		int tempInt = currentRoom.getItemInt(highlight);
		switch (tempInt)
		{
			// KEY
		case 1:
			snd::key->play();
			log.push_back(messages["GET_KEY"]);
			keyCount++;
			pickupFlags["KEY"] = true;
			currentRoom.setItemInt(highlight, 0);
			break;

			// ROOM_TRANSITION
		case 2:
			snd::nextRoom->play();
			transitionRoom();
			break;

			// MAP_DOOR_LOCKED
		case 3:
			if (masterKey == true) // If you have the masterkey, you can open all the things
			{
				snd::nextRoom->play();
				log.push_back(messages["USE_MASTERKEY"]);
				currentRoom.setItemInt(highlight, 0);
			}
			else if (keyCount > 0) // You know what this does, right? (Use a key)
			{
				snd::nextRoom->play();
				log.push_back(messages["USE_KEY"]);
				keyCount--;
				currentRoom.setItemInt(highlight, 0);
			}
			else // The door is locked and you have no keys. You are in a dark room. Suddenly, Shia Labeouf.
			{
				snd::lockedDoor->play();
				log.push_back(messages["DOOR_LOCKED"]);
			}
			break;

			//PRINCIPAL_LOCKED_DOOR
		case 12:
			if (masterKey == true) // Can only be opened by masterkey
			{
				log.push_back(messages["USE_MASTERKEY"]);
				currentRoom.setItemInt(highlight, 0);
			}
			else
			{
				snd::lockedDoor->play();
				log.push_back(messages["P_DOOR_LOCKED"]);
			}
			break;

			// MASTERKEY
		case 13:
			log.push_back(currentRoom.itemIndex[13].getMPickup());
			masterKey = true;
			currentRoom.setItemInt(highlight, 0);
			break;

			// ANY OTHER ITEM
		default:	
			log.push_back(Room::itemIndex[tempInt].getMPickup());
			ItemPtr tempPtr = currentRoom.getItemStats(tempInt);
			player.pickUp(tempPtr);
			currentRoom.setItemInt(highlight, 0);

			// Set pickup flags
			if (tempPtr->getStats().HP > 0)
			{
				pickupFlags["HP"] = true;
			}
			if (tempPtr->getStats().EN > 0)
			{
				pickupFlags["EN"] = true;
			}
			if (tempPtr->getStats().STR > 0)
			{
				pickupFlags["STR"] = true;
			}
			break;
		}
	}
	
}

void PlayingState::getStartLocation()
{
	// Get the coordinates of the current room entrances and exits
	COORD north = currentRoom.getNorth();
	COORD south = currentRoom.getSouth();
	COORD east = currentRoom.getEast();
	COORD west = currentRoom.getWest();
	COORD empty = { 0, 0 };

	// Check which doors the player can spawn at.
	if (MenuState::levelSelected() == 0)
	{
		player.setLocation({ 29 , 12 });
		highlight.Y = player.getY() - 1;
		highlight.X = player.getX();
	}
	else if (south != empty)
	{
		// There is a door here
		player.setLocation({ south.X, south.Y - 1 });
		highlight.Y = player.getY() - 1;
		highlight.X = player.getX();
	}
	else if (north != empty)
	{
		// There is a door here
		player.setLocation({ north.X, north.Y + 1 });
		highlight.Y = player.getY() + 1;
		highlight.X = player.getX();
	}
	else if (east != empty)
	{
		// There is a door here
		player.setLocation({ east.X - 1, east.Y });
		highlight.Y = player.getY();
		highlight.X = player.getX() - 1;
	}
	else if (west != empty)
	{
		// There is a door here
		player.setLocation({ west.X + 1, west.Y });
		highlight.Y = player.getY();
		highlight.X = player.getX() + 1;
	}
}

void PlayingState::loadRooms()
{
	Room temp;

	// Get the list of room files
	vector<string> roomFileList = shared::getRoomNames();
	
	// If the level selector has chosen a level
	if (MenuState::levelSelected() != 0)
	{
		temp = Room(roomFileList[MenuState::levelSelected() - 1]);
		temp.setLocation({ 1, 1 });
		roomArray[1][1] = temp;
		currentRoom = roomArray[1][1];
	}
	else
	{	
		// Load floor from the file
		std::ifstream stream("Rooms/Floor_1.txt");
		if (!stream)
		{
			std::cout << "File open failed.\n";
			exit(1);
		}
		string line;
		COORD tempCoord;

		// Read the rooms from the floor file
		while (stream.good())
		{
			stream >> line;
			temp = Room("Rooms/" + line + ".txt");
			stream >> tempCoord.X >> tempCoord.Y;
			temp.setLocation(tempCoord);
			roomArray[tempCoord.X][tempCoord.Y] = temp;
		}
		stream.close();
		// Set first room
		currentRoom = roomArray[3][7];
	}


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

	// If the scheme is Double-Tap
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
	// If the player presses the directional keys of the wrong scheme, don't move
	else if ((scheme == "Classic" || scheme == "Double-Tap") && eCode < 88 ||
		(scheme == "Classic Lefty" || scheme == "Double-Tap Lefty") && eCode > 49663)
	{
		delta = { 0, 0 };
	}
}

void PlayingState::transitionRoom()
{
	// If you were playing a level from the level select, return to menu
	if (MenuState::levelSelected() != 0)
	{
		running = false;
		return;
	}

	// Get the current room entrances and exits
	COORD north = currentRoom.getNorth();
	COORD south = currentRoom.getSouth();
	COORD east = currentRoom.getEast();
	COORD west = currentRoom.getWest();

	// Change the room in the proper direction
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
	
	// Clear the log
	log.clear();
}

void PlayingState::dropMasterKey(ActorPtr a)
{
	// Checking which tile is available for a dropped item
	if (currentRoom.isPassable({ a->getX() - 1, a->getY() }))
	{
		currentRoom.setItemInt({ a->getX() - 1, a->getY() }, 13);
	}
	else if (currentRoom.isPassable({ a->getX() + 1, a->getY() }))
	{
		currentRoom.setItemInt({ a->getX() + 1, a->getY() }, 13);
	}
	else if (currentRoom.isPassable({ a->getX(), a->getY() + 1 }))
	{
		currentRoom.setItemInt({ a->getX(), a->getY() + 1 }, 13);
	}
	else if (currentRoom.isPassable({ a->getX(), a->getY() - 1 }))
	{
		currentRoom.setItemInt({ a->getX(), a->getY() - 1 }, 13);
	}
	else
	{
		currentRoom.setItemInt(a->getLocation(), 13);
	}
}