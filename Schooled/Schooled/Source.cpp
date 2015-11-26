//(C) Fissure Studios 2015

#include "Header Files\GameEngine.h"
#include "Header Files\MenuState.h"
#include "Header Files\Room.h"
#include <ctime>

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

int main()
{
	GameEngine game;

	// Initialize the game
	game.Init();

	// Set the seed
	srand((unsigned int)time(0));

	// Load the indices
	Room::loadTileIndex("tileIndex.txt");
	Room::loadItemIndex("itemIndex.txt");
	Room::loadActorIndex("actorIndex.txt");

	// Load the main menu
	game.ChangeState(MenuState::Instance());

	// Main game loop
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