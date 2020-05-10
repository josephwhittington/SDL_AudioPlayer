#include <iostream>

#include "SDL.h"

#define MUSIC_PATH "files/music/"

// Data
struct AudioData
{
	Uint8* position;
	Uint32 length;
};

void AudioCallback(void* data, Uint8* stream, int streamLength)
{
	AudioData* audio = (AudioData*)data;

	if (audio->length == 0) return;

	Uint32 length = (Uint32)streamLength;

	length = (length > audio->length) ? audio->length : length;

	SDL_memcpy(stream, audio->position, length);

	audio->position += length;
	audio->length -= length;
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "Audio Initialization failed\n";
	}
	else std::cout << "SDL audio library initialization successful\n";

	SDL_AudioSpec wavSpec;
	Uint8* wavStart;
	Uint32 wavLength;

	if (SDL_LoadWAV(std::string(MUSIC_PATH).append("ambiance.wav").c_str(), &wavSpec, &wavStart, &wavLength) == NULL)
	{
		std::cout << "Error loading audio file\n";
	}
	else std::cout << "File loaded successfully\n";

	// Audio data
	AudioData audio;
	audio.position = wavStart;
	audio.length = wavLength;

	wavSpec.callback = AudioCallback;
	wavSpec.userdata = &audio;

	//SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
	SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	if (device == 0)
	{
		// No compatible audio device found
		std::cout << "No compatible audio device found\n";
	}

	// Play audio
	SDL_PauseAudioDevice(device, 0);

	while (audio.length > 0)
	{
		SDL_Delay(100);
	}

	// Free memory
	SDL_CloseAudioDevice(device);
	SDL_FreeWAV(wavStart);

	// Shutdown SDL
	SDL_Quit();

	return EXIT_SUCCESS;
}