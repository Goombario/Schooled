#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include <string>
#include <vector>
using std::vector;
using std::string;



class MenuState : public GameState
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
	static MenuState* Instance() {
		return &m_MenuState;
	}

	static int levelSelected() { return lSelect; }

protected:
	// Create an empty object
	MenuState() { }

private:
	static MenuState m_MenuState;
	string art;
	int menuSelect, levelSelect, selectedControl;
	vector<string> menuSelections, levelSelections, 
		cScheme, dScheme, clScheme, dlScheme;
	bool selectingControl, selectingLevel, selectingCredits, changedSettings, startingGame, showObjective;
	static int lSelect;

	void handleMenu(GameEngine* game);
	void saveSetting(string, string);
	void initSettings();

	//Gets file contents (NEEDS REMOVAL)
	string getFileContents(std::ifstream&);

	// Sets up the scheme displays
	void setSchemes();
};


#endif