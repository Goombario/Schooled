#include "../Header Files/GameOverState.h"
#include "../Header Files/Console_color.h"
#include <string>
using std::string;

namespace con = JadedHoboConsole;

GameOverState GameOverState::m_GameOverState;

void GameOverState::Init()
{

}

void GameOverState::Cleanup()
{

}

void GameOverState::Pause()
{

}

void GameOverState::Resume()
{

}

void GameOverState::HandleEvents(GameEngine* game)
{
	KEYPRESS sKeyPress = console.WaitForKeypress();
	game->PopState();
}

void GameOverState::Update(GameEngine* game)
{

}

void GameOverState::Draw(GameEngine* game)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Open the buffer for writing
	buffer.open(hConsole);

	// Clear the buffer
	buffer.clear();

	buffer.draw("Game Over", con::fgHiRed, 4, 20);
	buffer.draw("Press any key to return to menu", con::fgHiWhite, 3, 20);

	// Close the buffer
	buffer.close(hConsole);
}

