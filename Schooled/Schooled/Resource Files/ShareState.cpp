#include "../Header Files/ShareState.h"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::to_string;

namespace shared
{
	vector<string> getRoomNames()
	{
		vector<string> roomNameList;
		int counter = 1;
		ifstream stream;
		string filename;
		bool good = true;

		while (good)
		{
			filename = "Rooms/Level_" + to_string(counter) + ".txt";
			stream.open(filename);
			if (stream.good())
			{
				roomNameList.push_back(filename);
				counter++;
			}
			else
				good = false;
			stream.close();
		}
		return roomNameList;
	}
}