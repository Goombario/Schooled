#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H
#include <windows.h>

// Contains a sound file and methods for manipulating it
class Sound
{
private:
	LPCTSTR fileLocation;
	bool loop;
	bool async;
public:
	// Create an empty Sound object
	Sound();

	// Create a Sound object containing a .wav file
	Sound(LPCTSTR, bool, bool=true);

	// Play the sound
	void play();

	// Stop the sound
	void stop();
};

// Pointer to Sound object
typedef Sound* SndPtr;

// Sound effects and music
namespace snd
{
	const SndPtr title = new Sound(L"Sounds/schooled64.wav", true);
	const SndPtr startGame = new Sound(L"Sounds/pewpew.wav", false, false);
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