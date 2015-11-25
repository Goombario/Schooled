#ifndef WINSTATE_H
#define WINSTATE_H

#include "GameState.h"

class WinState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static WinState* Instance() {
		return &m_WinState;
	}

protected:
	WinState() { }

private:
	static WinState m_WinState;
	bool firstTime = true;	// Only draw the text slowly the first time.
};

#endif