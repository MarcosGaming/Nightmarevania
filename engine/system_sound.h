#pragma once

#include <SFML/Audio.hpp>
#include <memory>

namespace SoundSystem 
{
	void initialise();

	void addMusic(const std::string&, std::shared_ptr<sf::Sound>);
	void playMusic(const std::string&);
	void stopMusic(const std::string&);
	void turnMusicOn();
	void turnMusicOff();

	void addEffect(const std::string&, std::shared_ptr<sf::Sound>);
	void playEffect(const std::string&);
	void stopEffect(const std::string&);
	void turnEffectsOn();
	void turnEffectsOff();

	void clearAllSounds();
};