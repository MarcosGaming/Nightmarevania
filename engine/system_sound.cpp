#include "system_sound.h"
#include <unordered_map>

static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> music;
static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> effects;

static bool audioMusicOn;
static bool audioEffectsOn;

void Audio::initialise()
{
	audioMusicOn = true;
	audioEffectsOn = true;
}

// Methods that handle the game music
void Audio::addMusic(const std::string& musicName, std::shared_ptr<sf::Sound> musicSound)
{
	music[musicName] = musicSound;
}
void Audio::playMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (audioMusicOn && found != music.end())
	{
		found->second->play();
	}
}
void Audio::stopMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (audioMusicOn && found != music.end())
	{
		found->second->stop();
	}
}
void Audio::turnMusicOn() { audioMusicOn = true; }
void Audio::turnMusicOff() { audioMusicOn = false; }
bool Audio::isMusicOn() { return audioMusicOn; }

// Methods that handle the game effects
void Audio::addEffect(const std::string& effectName, std::shared_ptr<sf::Sound> effectSound)
{
	effects[effectName] = effectSound;
}
void Audio::playEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (audioEffectsOn && found != effects.end())
	{
		found->second->play();
	}
}
void Audio::stopEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (audioEffectsOn && found != effects.end())
	{
		found->second->stop();
	}
}
void Audio::turnEffectsOn() { audioEffectsOn = true; }
void Audio::turnEffectsOff() { audioEffectsOn = false; }
bool Audio::areEffectsOn() { return audioEffectsOn; }

void Audio::clearAllSounds()
{
	music.clear();
	effects.clear();
}