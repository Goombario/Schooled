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
	const SndPtr startGame = new Sound(L"Sounds/pewpew.wav", false);
	const SndPtr dungeonMusic = new Sound(L"Sounds/background_1.wav", true);
	const SndPtr pause = new Sound(L"Sounds/pause.wav", false);
	const SndPtr unpause = new Sound(L"Sounds/unpause.wav", false);
	const SndPtr key = new Sound(L"Sounds/key.wav", false);
	const SndPtr menuSelect = new Sound(L"Sounds/select.wav", false);
	const SndPtr menuDeselect = new Sound(L"Sounds/back.wav", false);
	const SndPtr lockedDoor = new Sound(L"Sounds/locked_jiggle.wav", false);
	const SndPtr menuHighlight = new Sound(L"Sounds/boop.wav", false);
	const SndPtr nextRoom = new Sound(L"Sounds/door_sounds.wav", false);
	const SndPtr attack1 = new Sound(L"Sounds/hit_1.wav", false);
	const SndPtr attack2 = new Sound(L"Sounds/hit_2.wav", false);
	const SndPtr playerDeath = new Sound(L"Sounds/death.wav", false);
}
#endif