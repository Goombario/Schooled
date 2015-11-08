#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "GameState.h"
#include "Room.h"
#include "Log.h"

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
	Actor player;
	COORD highlight, delta;
	Log log;
	Room currentRoom;
	HANDLE hConsole;
	KEYPRESS sKeyPress;

	// Changes the current room to the number of the one given.
	void changeRoom(Room&, COORD);

	// Draw the current log
	void displayLog(vector<string>, Buffer&);

	// If turn is over return false
	bool playerTurn(Actor);

	// Load the rooms, and set up the layout
	void loadRooms();

	// The enemies take their turn
	void enemyTurn();
};


#endif