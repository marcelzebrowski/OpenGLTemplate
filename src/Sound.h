#pragma once

#include <fmod/fmod.hpp>
#include <map>

typedef std::map<std::string, FMOD::Sound*> SoundMap;

class Sound {
public:

	Sound() {
		init();
	}

	~Sound() {
		release();
	}

	void streamLoop(const std::string& path) {
		loadOrStream(path, true, FMOD_LOOP_NORMAL);
	}

	void stream(const std::string& path) {
		loadOrStream(path, true, FMOD_DEFAULT);
	}

	void load(const std::string& path, FMOD_MODE mode) {
		loadOrStream(path, false, mode);
	}

	void play(const std::string& path) {
		FMOD::Channel* channel = nullptr;
		auto sound = sounds.find(path);
		if (sound == sounds.end()) {
			return;
		}

		system->playSound(sound->second, false, 0, &channel);
	}

	void update(float elapsed) {
		if (system != nullptr) {
			system->update();
		}
	}

	void togglePaused(FMOD::Channel* channel) {
		bool paused;
		channel->getPaused(&paused);
		channel->setPaused(!paused);
	}

	float changeOctave(const float frequency, const float variation) {
		static float octave_ratio = 2.0f;
		return frequency * pow(octave_ratio, variation);
	}

	float changeSemitone(const float frequncy, const float variation) {
		static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);
		return frequncy * pow(semitone_ratio, variation);
	}

private:
	void loadOrStream(const std::string& path, bool stream, FMOD_MODE mode) {
		FMOD::Sound* sound;
		if (stream) {
			system->createStream(path.c_str(), mode, 0, &sound);
		}
		else {
			system->createSound(path.c_str(), mode, 0, &sound);
		}

		sounds.insert(std::make_pair(path, sound));
	}

	void init() {
		FMOD::System_Create(&system);
		system->init(100, FMOD_INIT_NORMAL, 0);

	}

	void release() {

		for (auto sound : sounds) {
			sound.second->release();
		}
		sounds.clear();

		if (system != nullptr) {
			system->release();
			system = nullptr;
		}
	}

private:
	FMOD::System* system = nullptr;



	SoundMap sounds;
};