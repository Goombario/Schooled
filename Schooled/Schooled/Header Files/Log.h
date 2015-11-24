#ifndef LOG_H
#define LOG_H

#include <string>
#include <vector>
#include <Windows.h>
#include <map>

using std::map;
using std::string;
using std::vector;

class Buffer;

class Log
{
public:
	// Create an empty Log object
	Log();

	// Add a string to the log (colour optional)
	void push_back(string);
	void push_back(string, WORD);

	// Output the log
	void display(HANDLE);
	void display(Buffer&, int row=0);

	// Clear the log
	void clear();

private:
	vector<string> log;		// The log containing the phrases
	vector<WORD> colours;	// Contains the colours
	vector<bool> newLines;	// If the line is new or not
	const int TEXT_START=15;
	bool newLine;
};

//generic global messages to be reused
static map<string, string> messages =
{
	{ "Q_RANDOM", "Random: What do you want ?" },
	{ "GET_KEY", "It's a key! You picked it up." },
	{ "DOOR_LOCKED", "The door is locked, you need a key." },
	{ "P_DOOR_LOCKED", "You need the Janitor's key to get through here." },
	{ "USE_KEY", "You used a key!" },
	{ "USE_MASTERKEY", "You used the Janitor's Keys!" },
	{ "RECIEVE_KEY", "You got a key!" },
	{ "Q_NEXT_ROOM", "Go to next room?" },
	{ "NEW_ROOM", "New room." },
	{ "ENEMY_DEATH", "It died..." },
	{ "UNATTACKABLE", "You can't kill that, you silly!" },
	{ "ATTACKABLE", "You hit that thing with " },
	{ "ENEMY_ATTACK", "You got hit with " },
	{ "PLAYER_DEATH", "You died..." },
	{"ENEMY_INTERACT", "What d'you think this is? The Talk? Fight me!"}
};




#endif