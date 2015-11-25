#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include "../Console Library/Console.h"
using std::vector;

class GameState;

class GameEngine
{
public:
	// Initialize the engine
	void Init();

	// Cleanup the engine before exiting
	void Cleanup();

	// Manipulate the states
	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	// Tell the state to call its own functions
	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }


private:
	// The stack of states
	vector <GameState*> states;
	bool m_running;
};


#endif