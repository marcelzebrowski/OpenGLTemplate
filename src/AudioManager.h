#pragma once
#include <string>
#include <map>
#include <fmod/fmod.hpp>
#include <time.h>
#include <stdlib.h>

class AudioManager
{
public:
	AudioManager();

	~AudioManager();

	void Update(float elapsed);

	void LoadSFX(const std::string& path);
	void LoadSong(const std::string& path);

	void PlaySFX(const std::string& path, float minVolume, float maxVolume, float minPitch, float maxPitch);
	void PlaySong(const std::string& path);

	void StopSFXs();

	void StopSongs();

	void SetMasterVolume(float volume);

	void SetSFXVolume(float volume);

	void SetSongsVolume(float volume);


private:
	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	enum Category { CATEGORY_SFX, CATEGORY_SONG, CATEGORY_COUNT };

	void Load(Category type, const std::string& path);

	float RandomBetween(float min, float max);

	float ChangeOctave(const float frequency, const float variation);
	float ChangeSemitone(const float frequncy, const float variation);

	FMOD::System* system;
	FMOD::ChannelGroup* master;
	FMOD::ChannelGroup* groups[CATEGORY_COUNT];
	SoundMap sounds[CATEGORY_COUNT];
	FMOD_MODE modes[CATEGORY_COUNT];

	// Fmod does not support fading between two songs, we have to implement this feature manually
	FMOD::Channel* currentSong;
	std::string currentSongPath;
	std::string nextSongPath;
	enum FadeState { FADE_NONE, FADE_IN, FADE_OUT };
	FadeState fade;
};

