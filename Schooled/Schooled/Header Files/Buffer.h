#ifndef BUFFER_H
#define BUFFER_H

#include "Schooled.h"
#include "Console_color.h"
#include <string>
#include <Windows.h>
namespace con = JadedHoboConsole;	// Used for the color


class Buffer
{
public:
	// initialize an empty buffer
	Buffer();	

	// Draw to the buffer
	void draw(std::string, WORD, int, int);
	void draw(char, WORD, int, int);
	void draw(WORD, int, int);

	// Clear the buffer
	void clear();

	// Open the buffer for writing
	void open(HANDLE);

	// Close the buffer, and write it to the screen
	void close(HANDLE);

private:
	CHAR_INFO buffer[schooled::SCREEN_HEIGHT][schooled::SCREEN_WIDTH];
	COORD dwBufferSize;
	COORD dwBufferCoord;
	SMALL_RECT rcRegion;
};


#endif