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
	Log();
	void push_back(string);
	void display(HANDLE);
	void display(Buffer&);
	void clear();

private:
	vector<string> log;
	bool newLine;
};

static map<string, string> messages =
{
	{ "Q_RANDOM", "Random: What do you want ?" },
	{ "GET_KEY", "It's a key! You picked it up." },
	{ "DOOR_LOCKED", "The door is locked, you need a key." },
	{ "USE_KEY", "You used a key!" },
	{ "Q_USE_KEY", "The door is locked, use a key?" },
	{ "Q_NEXT_ROOM", "Go to next room?" },
	{ "NEW_ROOM", "New room." },
	{ "ENEMY_DEATH", "It died..." },
	{ "UNATTACKABLE", "You can't kill that, you silly!" },
	{ "ATTACKABLE", "You hit that thing with " },
	{ "ENEMY_ATTACK", "You got hit with " },
	{ "PLAYER_DEATH", "You died..." },
	{"POTION", "Milk? Sweet! I feel better already!"}
};




#endif