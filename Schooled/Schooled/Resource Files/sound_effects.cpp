#include "../Header Files/sound_effects.h"
#include <string>


Sound::Sound()
{
	fileLocation = NULL;
}

Sound::Sound(LPCTSTR file, bool l)
{
	fileLocation = file;
	loop = l;
}

void Sound::play()
{
	if (loop)
	{
		PlaySound(fileLocation, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else
	{
		PlaySound(fileLocation, NULL, SND_FILENAME | SND_ASYNC);
	}
}

void Sound::stop()
{
	PlaySound(NULL, 0, SND_ASYNC);
}

