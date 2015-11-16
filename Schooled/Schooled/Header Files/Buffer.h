#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include "Schooled.h"

class Buffer
{
public:
	// initialize an empty buffer
	Buffer();	

	// Draw to the buffer
	int draw(std::string, WORD, int, int);	// Returns the row it ends on
	void draw(char, WORD, int, int);
	void draw(WORD, int, int);

	// Simulate typing to the buffer
	void type(std::string, WORD, int, int, HANDLE);

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