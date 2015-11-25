#include "../Header Files/sound_effects.h"
#include <string>


Sound::Sound()
{
	fileLocation = NULL;
}

Sound::Sound(LPCTSTR file, bool l, bool a)
{
	fileLocation = file;
	loop = l;
	async = a;
}

void Sound::play()
{
	if (loop && async)	// Looping and playing asynchronously 
	{
		PlaySound(fileLocation, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (async)		// Playing asynchronously 
	{
		PlaySound(fileLocation, NULL, SND_FILENAME | SND_ASYNC);
	}
	else
	{
		PlaySound(fileLocation, NULL, SND_FILENAME);
	}
}

void Sound::stop()
{
	// Play a null sound
	PlaySound(NULL, 0, SND_ASYNC);
}

