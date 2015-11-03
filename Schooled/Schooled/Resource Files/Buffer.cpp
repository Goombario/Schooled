#include "../Header Files/Buffer.h"

Buffer::Buffer()
{
	for (int a = 0; a < schooled::SCREEN_HEIGHT; a++){
		for (int b = 0; b < schooled::SCREEN_WIDTH; b++){
			buffer[a][b].Char.AsciiChar = ' ';
			buffer[a][b].Attributes = con::fgBlack | con::bgBlack;
		}
	}
	dwBufferCoord = { 0, 0 };
	rcRegion = { 0, 0, schooled::SCREEN_WIDTH - 1, 
		schooled::SCREEN_HEIGHT - 1 };
	dwBufferSize = { schooled::SCREEN_WIDTH, schooled::SCREEN_HEIGHT };
}

void Buffer::draw(std::string s, WORD w, int row, int col)
{
	int startCol = col;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		buffer[row][col].Char.AsciiChar = s[i];
		buffer[row][col].Attributes = w;
		if (col < schooled::SCREEN_WIDTH - 1)
		{
			col++;
		}
		else
		{
			col = startCol;
			row--;
		}
	}
}

void Buffer::draw(char c, WORD w, int row, int col)
{
	buffer[row][col].Char.AsciiChar = c;
	buffer[row][col].Attributes = w;
}

void Buffer::draw(WORD w, int row, int col)
{
	buffer[row][col].Attributes = w;
}

void Buffer::open(HANDLE h)
{
	ReadConsoleOutput(h, (CHAR_INFO *)buffer,
		dwBufferSize, dwBufferCoord, &rcRegion);
}

void Buffer::close(HANDLE h)
{
	WriteConsoleOutput(h, (CHAR_INFO *)buffer,
		dwBufferSize, dwBufferCoord, &rcRegion);
}

