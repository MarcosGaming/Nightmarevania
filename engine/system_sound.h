#pragma once

#include <SFML/Audio.hpp>

namespace Sound 
{
	bool musicOn;

	bool effectsOn;

	void initialise();

	void addMusic(const std::string&, std::shared_ptr<sf::Sound>);
	void playMusic(const std::string&);
	void stopMusic(const std::string&);

	void addEffect(const std::string&, std::shared_ptr<sf::Sound>);
	void playEffect(const std::string&);
	void stopEffect(const std::string&);

	void clearAllSounds();
};