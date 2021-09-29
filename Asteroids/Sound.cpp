
#include "Sound.h"

waveFileIn* Sound::thrusterSound;
waveFileIn* Sound::explosionSound;
waveFileIn* Sound::bulletSound;

bool Sound::started = false;

Thrust Sound::thrusterEnv;
Expo Sound::explosionEnv;
Bul Sound::bulletEnv;




