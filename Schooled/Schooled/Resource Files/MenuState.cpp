#include "../Header Files/MenuState.h"
#include "../Header Files/GameState.h"
#include "../Header Files/PlayingState.h"
#include "../Header Files/Console_color.h"
#include "../Console Library/Console.h"
#include "../Header Files/sound_effects.h"

#include <fstream>

namespace snd
{
	Sound title(L"Sounds/schooled64.wav", true);
}

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

	// Play the music
	//snd::title.play();

	// Set the default location of the selection to "Start Game"
	menuSelect = 0;

	// Set the menu options
	menuSelections.push_back("Start Game");
	menuSelections.push_back("Control Options");

	start = false;
}

void MenuState::Cleanup()
{
	
}

void MenuState::Pause()
{
	snd::title.stop();
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
		break;
		
	case CONSOLE_KEY_UP:
		if (menuSelect > 0)
		{
			menuSelect--;
		}
		else
		{
			menuSelect = menuSelections.size() - 1;
		}
		break;

	case CONSOLE_KEY_DOWN:
		if (menuSelect < menuSelections.size() - 1)
		{
			menuSelect++;
		}
		else
		{
			menuSelect = 0;
		}
		break;

	case CONSOLE_KEY_RETURN:
	case CONSOLE_KEY_Z:
		handleMenu();
		break;

	default:
		break;
	}
}

void MenuState::Update(GameEngine* game)
{
	if (start)
	{
		Pause();
		game->PushState(PlayingState::Instance());
	}

}

void MenuState::Draw(GameEngine* game)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Open the buffer for writing
	buffer.open(hConsole);

	buffer.clear();

	buffer.draw(art, con::fgHiWhite, 1, 3);

	// Draw the menu options to the screen
	int row = 18;
	for (int i = 0; i < menuSelections.size(); i++)
	{
		WORD colour;
		int col;
		col = 30 - menuSelections[i].size() / 2;

		// Determine the colour of the text
		if (menuSelect == i)
		{
			colour = con::fgHiWhite;
		}
		else
		{
			colour = con::fgLoWhite;
		}
		buffer.draw(menuSelections[i], colour, row, col);
		row += 2;
	}

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

void MenuState::handleMenu()
{
	switch (menuSelect)
	{
	case 0:
		start = true;
		break;

	case 1:
		break;

	default:
		break;
	}
}


