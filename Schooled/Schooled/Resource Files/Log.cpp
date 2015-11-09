#include "../Header Files/Log.h"

#include <Windows.h>
#include <cassert>

#include "../Console Library/Console.h"
#include "../Header Files/Console_color.h"
#include "../Header Files/Buffer.h"
#include "../Header Files/Schooled.h"

namespace con = JadedHoboConsole;	// Used for the color

using std::map;
using std::string;
using std::vector;

Log::Log()
{
	push_back("");
	push_back("");
}

void Log::push_back(string s)
{
	log.push_back(s);
	newLine = true;
}

void Log::clear()
{
	log.clear();
}

void Log::display(HANDLE hConsole)
{
	int max = (log.size() >= 3) ? (log.size() - 3) : 0;	// determines the number of lines to display
	int row, col;
	int grayscale = 2;
	row = schooled::SCREEN_HEIGHT - 3;
	col = 23;
	for (unsigned int i = max; i < log.size(); i++)
	{
		switch (grayscale)
		{
		case 0:
			SetConsoleTextAttribute(hConsole, con::fgHiWhite);
			for (unsigned int e = 0; e < log[i].size(); e++)
			{
				assert(col + e < schooled::SCREEN_WIDTH);
				console.Position(col + e, row);
				console << log[i][e];
				if (newLine)
				{
					Sleep(10);
				}
			}
			newLine = false;
			break;

		case 1:
			SetConsoleTextAttribute(hConsole, con::fgLoWhite);
			for (unsigned int e = 0; e < log[i].size(); e++)
			{
				assert(col + e < schooled::SCREEN_WIDTH);
				console.Position(col + e, row);
				console << log[i][e];
			}
			break;

		case 2:
			SetConsoleTextAttribute(hConsole, con::fgGray);
			for (unsigned int e = 0; e < log[i].size(); e++)
			{
				assert(col + e < schooled::SCREEN_WIDTH);
				console.Position(col + e, row);
				console << log[i][e];
			}
			break;
		}
		grayscale--;
		row++;
		col = 23;
	}

}

void Log::display(Buffer& buffer)
{
	const int SIZE = 4;
	int max = (log.size() >= SIZE) ? (log.size() - SIZE) : 0;	// determines the number of lines to display
	int row, col;
	int grayscale = 0;
	row = schooled::SCREEN_HEIGHT - SIZE;
	col = 23;
	for (int i = log.size() - 1; i >= max; i--)
	{
		switch (grayscale)
		{
		case 0:
			buffer.draw(log[i], con::fgHiWhite, row, col);
			break;

		case 1:
			buffer.draw(log[i], con::fgLoWhite, row, col);
			break;

		default:
			buffer.draw(log[i], con::fgGray, row, col);
			break;
		}
		grayscale++;
		row++;
		col = 23;
	}
}