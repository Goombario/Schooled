#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "GameState.h"
#include "Room.h"
#include "Log.h"
#include "Actor.h"
#include "Schooled.h"
#include <Windows.h>
#include <map>

class PlayingState : public GameState
{
public:
	// Initialize the state
	void Init();

	// Cleanup the state before exiting
	void Cleanup();

	// Pause or resume the state before/after changing
	void Pause();
	void Resume();

	// Operations done by the state, can call to the game engine
	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	// Return an instance of the object
	static PlayingState* Instance() {
		return &m_PlayingState;
	}

protected:
	// Create an empty object
	PlayingState() { }

private:
	static PlayingState m_PlayingState;

	// Variables
	Room roomArray[schooled::FLOOR_HEIGHT][schooled::FLOOR_WIDTH];
	int tCount, keyCount, bossCount, enemyIndex, enemyEN;
	bool pTurn, increment, running, winGame, masterKey, enemiesMoved;
	bool attack_animation, defend_animation;
	map<string, bool> pickupFlags;
	WORD highlightColor;
	string scheme;
	Actor player;
	COORD highlight, delta;
	Log log;
	Room currentRoom;
	HANDLE hConsole;


	// Handle the attack key
	void attack();

	// Changes the current room to the number of the one given.
	void changeRoom(Room&, COORD);

	// Draw the unchanging base of the buffer
	void drawBase();

	// Draw temporary effects upon the buffer
	void drawVFX(HANDLE);

	// Drop the master key
	void dropMasterKey(ActorPtr);

	// The enemies take their turn
	void enemyTurn(Actor&);

	// Increment the turn counter. If out of turns, switch pTurn;
	void incrementTurn();

	// Handle the interact key
	void interact();

	// Get the starting location
	void getStartLocation();

	// Load the rooms, and set up the layout
	void loadRooms();

	// Move the highlight
	void moveHighlight(KEYCODE);

	// Transition to the next room
	void transitionRoom();

};

#endif