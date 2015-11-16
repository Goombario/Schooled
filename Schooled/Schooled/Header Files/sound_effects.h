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

typedef Sound* SndPtr;

namespace snd
{
	const SndPtr title = new Sound(L"Sounds/schooled64.wav", true);
}
#endif