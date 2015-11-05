#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string getFileContents(ifstream&);            //Gets file contents

int main(int argc, char *argv[])
{
	
	
	ifstream Reader("title.txt");             //Open file

	string Art = getFileContents(Reader);       //Get file

	cout << Art << endl;               //Print it to the screen

	Reader.close();                           //Close file

	return 0;
}

string getFileContents(ifstream& File)
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