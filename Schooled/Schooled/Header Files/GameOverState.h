#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"
#include <string>
using std::string;



class GameOverState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static GameOverState* Instance() {
		return &m_GameOverState;
	}

protected:
	GameOverState() { }

private:
	static GameOverState m_GameOverState;
};

#endif