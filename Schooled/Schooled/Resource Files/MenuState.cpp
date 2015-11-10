#include "../Header Files/MenuState.h"
#include "../Header Files/GameState.h"
#include "../Header Files/PlayingState.h"
#include "../Header Files/Console_color.h"
#include "../Console Library/Console.h"
#include <fstream>

namespace con = JadedHoboConsole;

MenuState MenuState::m_MenuState;

void MenuState::Init()
{
	// Set and clear the buffer
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	buffer.open(hConsole);
	buffer.clear();
	buffer.close(hConsole);

	// Get the artwork
	std::ifstream Reader("title.txt");	//Open file
	art = getFileContents(Reader);			//Get file
	Reader.close();							//Close file
}

void MenuState::Cleanup()
{
	
}

void MenuState::Pause()
{

}

void MenuState::Resume()
{

}

void MenuState::HandleEvents(GameEngine* game)
{
	KEYPRESS sKeyPress = console.WaitForKeypress();
	switch (sKeyPress.eCode)
	{
	case CONSOLE_KEY_ESCAPE:
		game->Quit();
	default:
		game->PushState(PlayingState::Instance());
	}
}

void MenuState::Update(GameEngine* game)
{

}

void MenuState::Draw(GameEngine* game)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Open the buffer for writing
	buffer.open(hConsole);

	buffer.clear();

	buffer.draw(art, con::fgHiWhite, 5, 3);

	// Close the buffer
	buffer.close(hConsole);
}

string MenuState::getFileContents(std::ifstream& File)
{
	string Lines = "";        //All lines

	if (File)                      //Check if everything is good
	{
		while (File.good())
		{
			string TempLine;                  //Temp line
			getline(File, TempLine);        //Get temp line
			TempLine += "\n";                      //Add newline character

			Lines += TempLine;                     //Add newline
		}
		return Lines;
	}
	else                           //Return error
	{
		return "ERROR File does not exist.";
	}
}



