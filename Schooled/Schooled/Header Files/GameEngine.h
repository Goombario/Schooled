#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include "../Console Library/Console.h"
using std::vector;

class GameState;

class GameEngine
{
public:
	void Init();
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

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