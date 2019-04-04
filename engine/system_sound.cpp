#include "system_sound.h"
#include <unordered_map>

static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> music;
static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> effects;

void SoundSystem::initialise()
{
	musicOn = true;
	effectsOn = true;
}

void SoundSystem::addMusic(const std::string& musicName, std::shared_ptr<sf::Sound> musicSound)
{
	music[musicName] = musicSound;
}

void SoundSystem::playMusic(const std::string& musicName) noexcept
{
	auto found = music.find(musicName);
	if (musicOn && found != music.end())
	{
		found->second->play();
	}
}
void SoundSystem::stopMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (musicOn && found != music.end())
	{
		found->second->stop();
	}
}

void SoundSystem::addEffect(const std::string& effectName, std::shared_ptr<sf::Sound> effectSound)
{
	effects[effectName] = effectSound;
}

void SoundSystem::playEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (effectsOn && found != effects.end())
	{
		found->second->play();
	}
}

void SoundSystem::stopEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (effectsOn && found != effects.end())
	{
		found->second->stop();
	}
}

void SoundSystem::clearAllSounds()
{
	music.clear();
	effects.clear();
}