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

	// Draw congratulations text
	string temp = "Congraturations! A winner is you!";
	int tempCol = 30 - temp.length() / 2;
	buffer.draw(temp, con::fgHiMagenta, 8, tempCol);

	// Close the buffer
	buffer.close(hConsole);
	Sleep(2000);

	// Draw return text
	buffer.open(hConsole);
	
	temp = "Press any key to return to menu";
	tempCol = 30 - temp.length() / 2;
	buffer.draw(temp, con::fgHiWhite, 9, tempCol);

	buffer.close(hConsole);
	
}

