#pragma once

#include <SFML/Audio.hpp>
#include <memory>

namespace Audio 
{
	void initialise(std::string&, std::string&);

	void playMusic(const std::string&);
	void stopMusic(const std::string&);
	void pauseMusic(const std::string&);
	void turnMusicOn();
	void turnMusicOff();
	bool isMusicOn();

	void playEffect(const std::string&);
	void stopEffect(const std::string&);
	void turnEffectsOn();
	void turnEffectsOff();
	bool areEffectsOn();

	void clearAllSounds();
};