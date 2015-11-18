#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H
#include <windows.h>

class Sound
{
private:
	LPCTSTR fileLocation;
	bool loop;
	bool async;
public:
	Sound();
	Sound(LPCTSTR, bool, bool=true);
	void play();
	void stop();
};

typedef Sound* SndPtr;

namespace snd
{
	const SndPtr title = new Sound(L"Sounds/schooled64.wav", true);
	const SndPtr basement = new Sound(L"Sounds/Basement_Smaller.wav", true);
	const SndPtr pewpew = new Sound(L"Sounds/pewpew.wav", false, false);
}
#endif