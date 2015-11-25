#include "../Header Files/WinState.h"
#include "../Header Files/Console_color.h"
#include <string>
#include <fstream>
using std::string;

namespace con = JadedHoboConsole;

WinState WinState::m_WinState;

void WinState::Init()
{
	winText = getTextBlock("outro.txt");
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
	buffer.draw(winText, con::fgHiWhite, 0, 0);

	// Close the buffer
	buffer.close(hConsole);
	Sleep(2000);

	// Draw return text
	buffer.open(hConsole);
	
	string temp = "Press any key to return to menu";
	int tempCol = 30 - temp.length() / 2;
	buffer.draw(temp, con::fgHiCyan, 15, tempCol);

	buffer.close(hConsole);
	
}

string WinState::getTextBlock(string filename)
{
	string tempLine, fullLine;
	std::ifstream stream(filename);
	if (!stream)
	{
		perror("File failed to load");
		exit(1);
	}

	while (getline(stream, tempLine))
	{
		fullLine += tempLine + "#";
	}

	stream.close();
	return fullLine;
}

