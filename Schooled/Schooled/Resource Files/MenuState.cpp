#include "../Header Files/MenuState.h"
#include "../Header Files/GameState.h"
#include "../Header Files/PlayingState.h"
#include "../Header Files/Console_color.h"
#include "../Console Library/Console.h"
#include "../Header Files/sound_effects.h"
#include "../Header Files/ShareState.h"

#include <fstream>
#include <stdio.h>
using schooled::getSetting;
using schooled::controlOptions;
	
namespace con = JadedHoboConsole;

MenuState MenuState::m_MenuState;
int MenuState::lSelect = 0;

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

	// Check if settings file exists.
	if (!std::ifstream("Settings.txt"))
	{
		initSettings();
	}

	// Play the music
	snd::title->play();

	// Set the default location of the selection to "Start Game"
	menuSelect = 0;

	// Set the menu options
	menuSelections.push_back("Start Game");
	menuSelections.push_back("Control Options");
	menuSelections.push_back("Level Select");
	menuSelections.push_back("Credits");
	menuSelections.push_back("Quit");

	// Set flags
	selectingControl = false;
	selectingLevel = false;
	changedSettings = false;

	// Get the level list
	levelSelections = shared::getRoomNames();

	// Get the control from the settings file
	selectedControl = 0;
	string sControl = schooled::getSetting("ControlScheme");

	for (unsigned int i = 0; i < controlOptions.size(); i++)
	{
		if (controlOptions[i] == sControl)
		{
			selectedControl = i;
			break;
		}
	}
}

void MenuState::Cleanup()
{
	snd::title->stop();
}

void MenuState::Pause()
{
	snd::title->stop();
	if (changedSettings)
	{
		saveSetting("ControlScheme", controlOptions[selectedControl]);
	}
}

void MenuState::Resume()
{
	// Play the music
	snd::title->play();
}

void MenuState::HandleEvents(GameEngine* game)
{
	KEYPRESS sKeyPress = console.WaitForKeypress();
	switch (sKeyPress.eCode)
	{
	case CONSOLE_KEY_ESCAPE:
		if (!selectingControl && !selectingLevel)
		{
			game->Quit();
		}
		else if (selectingControl)
		{
			selectingControl = false;
			changedSettings = false;
		}
		else if (selectingLevel)
		{
			selectingLevel = false;
			lSelect = 0;
			levelSelect = 0;
		}
		break;
		
	case CONSOLE_KEY_UP:
		if (selectingControl) break; // Don't move if changing controls

		if (selectingLevel)	// Choosing a level to play
		{
			if (levelSelect > 0)
			{
				levelSelect--;
			}
			else
			{
				levelSelect = levelSelections.size() - 1;
			}
			break;
		}

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
		if (selectingControl) break; // Don't move if changing controls
		if (selectingLevel)	// Choosing a level to play
		{
			if (levelSelect < levelSelections.size() - 1)
			{
				levelSelect++;
			}
			else
			{
				levelSelect = 0;
			}
			break;
		}
		if (menuSelect < menuSelections.size() - 1)
		{
			menuSelect++;
		}
		else
		{
			menuSelect = 0;
		}
		break;

	case CONSOLE_KEY_LEFT:
		if (!selectingControl) break;
		if (selectedControl > 0)
		{
			selectedControl--;
		}
		else
		{
			selectedControl = controlOptions.size() - 1;
		}
		break;

	case CONSOLE_KEY_RIGHT:
		if (!selectingControl) break;
		if (selectedControl < controlOptions.size() - 1)
		{
			selectedControl++;
		}
		else
		{
			selectedControl = 0;
		}
		break;

	case CONSOLE_KEY_RETURN:
	case CONSOLE_KEY_Z:
		handleMenu(game);
		break;

	default:
		break;
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

	// Level select screen
	if (selectingLevel)
	{
		buffer.draw("Choose a level (ESC to go back)", con::fgHiWhite, 1, 20);
		int row = 3;
		WORD colour;
		int col;
		for (unsigned int i = 0; i < levelSelections.size(); i++)
		{
			col = 30 - levelSelections[i].size() / 2;	// Centering the text

			// Determine the colour of the text
			if (levelSelect == i)
			{
				colour = con::fgHiWhite;
			}
			else
			{
				colour = con::fgLoWhite;
			}
			buffer.draw(levelSelections[i], colour, row, col);
			row ++;
		}
	}
	else
	{
		buffer.draw(art, con::fgHiWhite, 1, 3);

		// Draw the menu options to the screen
		int row = 16;
		WORD colour;
		int col;
		for (unsigned int i = 0; i < menuSelections.size(); i++)
		{
			col = 30 - menuSelections[i].size() / 2; // Centering the text

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

		if (selectingControl)
		{
			string temp = "<- " + controlOptions[selectedControl] + " ->";
			buffer.draw(temp, con::fgHiWhite, 21, (30 - temp.size() / 2));
		}
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

void MenuState::handleMenu(GameEngine* game)
{
	switch (menuSelect)
	{
	case 0:
		snd::title->stop();
		snd::pewpew->play();
		game->PushState(PlayingState::Instance());
		break;

	case 1:
		if (!selectingControl)
		{
			selectingControl = true;
		}
		else
		{
			selectingControl = false;
			changedSettings = true;
		}
		break;

	case 2:
		if (!selectingLevel)
		{
			selectingLevel = true;
		}
		else
		{
			lSelect = levelSelect;
			snd::title->stop();
			snd::pewpew->play();
			game->PushState(PlayingState::Instance());
		}
		break;

	case 3:
		break;

	case 4:
		game->Quit();
		break;

	default:
		break;
	}
}

void MenuState::saveSetting(string a_key, string change)
{
	std::ifstream inStream("Settings.txt");
	std::ofstream outStream("temp.txt");
	string line;

	if (!inStream || !outStream)
	{
		std::cout << "File open failed.\n";
		exit(1);
	}
	while (std::getline(inStream, line))
	{
		if (line.substr(0, line.find(':')) == a_key)
		{
			line = a_key + ": " + change;
		}
		outStream << line << std::endl;
	}
	outStream.flush();

	inStream.close();
	outStream.close();

	
	if (remove("Settings.txt") != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");

	int result = rename("temp.txt", "Settings.txt");
	if (result == 0)
		puts("File successfully renamed");
	else
		perror("Error renaming file");
}

void MenuState::initSettings()
{
	std::ofstream stream("Settings.txt");
	if (!stream)
	{
		std::cout << "File open failed.\n";
		exit(1);
	}
	stream << "ControlScheme: Classic" << std::endl;
	stream.close();
}