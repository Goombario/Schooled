#ifndef WINSTATE_H
#define WINSTATE_H

#include "GameState.h"

class WinState : public GameState
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
	static WinState* Instance() {
		return &m_WinState;
	}

protected:
	// Create an empty object
	WinState() { }

private:
	static WinState m_WinState;
	bool firstTime = true;	// Only draw the text slowly the first time.
	string winText;

	//Gets a line with newline characters
	string getTextBlock(string);

};

#endif