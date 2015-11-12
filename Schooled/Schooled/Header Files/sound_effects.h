#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H
#include <windows.h>

class Sound
{
private:
	LPCTSTR fileLocation;
	bool loop;
public:
	Sound();
	Sound(LPCTSTR, bool);
	void play();
	void stop();
};


#endif