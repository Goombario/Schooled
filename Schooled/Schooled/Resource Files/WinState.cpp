#include "../Header Files/WinState.h"
#include "../Header Files/Console_color.h"
#include <string>
using std::string;

namespace con = JadedHoboConsole;

WinState WinState::m_WinState;

void WinState::Init()
{

}

void WinState::Cleanup()
{

}

void WinState::Pause()
{

}

void WinState::Resume()
{

}

void WinState::HandleEvents(GameEngine* game)
{
	KEYPRESS sKeyPress = console.WaitForKeypress();
	game->PopState();
}

void WinState::Update(GameEngine* game)
{

}

void WinState::Draw(GameEngine* game)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Open the buffer for writing
	buffer.open(hConsole);

	buffer.clear();

	buffer.draw("Congraturations! A winner is you!", con::fgHiMagenta, 4, 20);
	buffer.draw("Press any key to return to menu", con::fgHiWhite, 3, 20);

	// Close the buffer
	buffer.close(hConsole);
}

