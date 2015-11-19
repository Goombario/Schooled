#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "GameState.h"
#include "Room.h"
#include "Log.h"
#include "Actor.h"
#include "Schooled.h"
#include <Windows.h>

class PlayingState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static PlayingState* Instance() {
		return &m_PlayingState;
	}

protected:
	PlayingState() { }

private:
	static PlayingState m_PlayingState;

	// Variables
	Room roomArray[schooled::FLOOR_HEIGHT][schooled::FLOOR_WIDTH];
	int tCount, keyCount;
	string scheme;
	bool pTurn, increment, running;
	Actor player;
	COORD highlight, delta;
	Log log;
	Room currentRoom;
	HANDLE hConsole;


	// Handle the attack key
	void attack();

	// Changes the current room to the number of the one given.
	void changeRoom(Room&, COORD);

	// The enemies take their turn
	void enemyTurn();

	// Increment the turn counter. If out of turns, switch pTurn;
	void incrementTurn();

	// Handle the interact key
	void interact();

	// Load the rooms, and set up the layout
	void loadRooms();

	// Move the highlight
	void moveHighlight(KEYCODE);

	// Transition to the next room
	void transitionRoom();

	
};

#endif