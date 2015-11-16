#include "Header Files\GameEngine.h"
#include "Header Files\MenuState.h"

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

int main()
{
	GameEngine game;

	// Initialize the game
	game.Init();

	// Load the main menu
	game.ChangeState(MenuState::Instance());

	while (game.Running())
	{
		game.Update();
		game.Draw();
		game.HandleEvents();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}