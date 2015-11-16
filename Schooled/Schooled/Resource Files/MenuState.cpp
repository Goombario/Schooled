#include "../Header Files/MenuState.h"
#include "../Header Files/GameState.h"
#include "../Header Files/PlayingState.h"
#include "../Header Files/Console_color.h"
#include "../Console Library/Console.h"
#include "../Header Files/sound_effects.h"

#include <fstream>
#include <stdio.h>

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

	// Check if settings file exists.
	if (!std::ifstream("Settings.txt"))
	{
		initSettings();
	}

	// Play the music
	//snd::title->play();

	// Set the default location of the selection to "Start Game"
	menuSelect = 0;

	// Set the menu options
	menuSelections.push_back("Start Game");
	menuSelections.push_back("Control Options");
	menuSelections.push_back("Quit");

	// Set the control options
	controlOptions.push_back("Classic");
	controlOptions.push_back("Double-Tap");

	selectControl = false;
	changedSettings = false;

	// Get the control from the settings file
	selectedControl = 0;
	string sControl = getSetting("ControlScheme");

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
		game->Quit();
		break;
		
	case CONSOLE_KEY_UP:
		if (selectControl) break; // Don't move if changing controls
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
		if (selectControl) break; // Don't move if changing controls
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
		if (!selectControl) break;
		if (selectedControl < controlOptions.size() - 1)
		{
			selectedControl++;
		}
		else
		{
			selectedControl = 0;
		}
		changedSettings = true;
		break;

	case CONSOLE_KEY_RIGHT:
		if (!selectControl) break;
		if (selectedControl > 0)
		{
			selectedControl--;
		}
		else
		{
			selectedControl = controlOptions.size() - 1;
		}
		changedSettings = true;
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

	buffer.draw(art, con::fgHiWhite, 1, 3);

	// Draw the menu options to the screen
	int row = 18;
	for (unsigned int i = 0; i < menuSelections.size(); i++)
	{
		WORD colour;
		int col;
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

	if (selectControl)
	{
		string temp = "<- " + controlOptions[selectedControl] + " ->";
		buffer.draw(temp, con::fgHiWhite, 21, (30 - temp.size() / 2));
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
		game->PushState(PlayingState::Instance());
		break;

	case 1:
		if (!selectControl)
		{
			selectControl = true;
		}
		else
		{
			selectControl = false;
		}
		break;

	case 2:
		game->Quit();

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


string MenuState::getSetting(string a_key)
{
	std::ifstream stream("Settings.txt");
	string line;

	if (!stream)
	{
		std::cout << "File open failed.\n";
		exit(1);
	}

	while (std::getline(stream, line))
	{
		if (line.substr(0, line.find(':')) == a_key)
		{
			return line.substr(line.find(':') + 2);
		}
	}

	return "";
}
