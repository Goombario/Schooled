#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include <string>
#include <fstream>
using std::string;

class MenuState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static MenuState* Instance() {
		return &m_MenuState;
	}

protected:
	MenuState() { }

private:
	//Gets file contents (NEEDS REMOVAL)
	string getFileContents(std::ifstream&);
	static MenuState m_MenuState;
	string art;
};


#endif