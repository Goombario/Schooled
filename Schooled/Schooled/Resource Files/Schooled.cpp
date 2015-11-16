#include "../Header Files/Schooled.h"
#include <fstream>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Operators

bool operator ==(COORD a, COORD b)
{
	return (a.X == b.X && a.Y == b.Y);
}

COORD operator +(COORD a, COORD b)
{
	return{ a.X + b.X, a.Y + b.Y };
}

COORD operator -(COORD a, COORD b)
{
	return{ a.X - b.X, a.Y - b.Y };
}

string schooled::getSetting(string a_key)
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