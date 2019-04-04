#include "system_sound.h"
#include <unordered_map>

static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> music;
static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> effects;

void Sound::initialise()
{
	musicOn = true;
	effectsOn = true;
}

void Sound::addMusic(const std::string& musicName, std::shared_ptr<sf::Sound> musicSound)
{
	music[musicName] = musicSound;
}

void Sound::playMusic(const std::string& musicName) noexcept
{
	auto found = music.find(musicName);
	if (musicOn && found != music.end())
	{
		found->second->play();
	}
}
void Sound::stopMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (musicOn && found != music.end())
	{
		found->second->stop();
	}
}

void Sound::addEffect(const std::string& effectName, std::shared_ptr<sf::Sound> effectSound)
{
	effects[effectName] = effectSound;
}

void Sound::playEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (effectsOn && found != effects.end())
	{
		found->second->play();
	}
}

void Sound::stopEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (effectsOn && found != effects.end())
	{
		found->second->stop();
	}
}

void Sound::clearAllSounds()
{
	music.clear();
	effects.clear();
}