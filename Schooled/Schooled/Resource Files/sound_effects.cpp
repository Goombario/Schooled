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
	if (loop && async)
	{
		PlaySound(fileLocation, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else if (async)
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
	PlaySound(NULL, 0, SND_ASYNC);
}

