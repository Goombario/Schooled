#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"

class GameOverState : public GameState
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
	static GameOverState* Instance() {
		return &m_GameOverState;
	}

protected:
	// Create an empty object
	GameOverState() { }

private:
	static GameOverState m_GameOverState;
	bool firstTime = true;	// Only draw the text slowly the first time.
};

#endif