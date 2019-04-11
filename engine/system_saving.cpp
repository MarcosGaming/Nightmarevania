#include "system_saving.h"
#include <vector>
#include <fstream>

static std::string levelSaved;
static std::string musicSettingSaved;
static std::string effectsSettingSaved;
static std::string resolutionSettingSaved;
static std::string fullscreenSettingSaved;
static std::string borderlessSettingSaved;

static std::fstream saveFile;

void Saving::initialise()
{
	// Read file where the level and the settings is stored
	std::string userpath = getenv("USERPROFILE");
	saveFile.open(userpath + "/Documents/NightmarevaniaSave.txt", std::fstream::out);
	// If the file is not empty 
	if (saveFile.peek() != saveFile.eof())
	{
		int counter = 0;
		std::string line;
		// The file is organized in lines, each one representing a different setting
		while (std::getline(saveFile, line))
		{
			switch (counter)
			{
			case 0:
				levelSaved = line;
				break;
			case 1:
				musicSettingSaved = line;
				break;
			case 2:
				effectsSettingSaved = line;
				break;
			case 3:
				resolutionSettingSaved = line;
				break;
			case 4:
				fullscreenSettingSaved = line;
				break;
			case 5:
				borderlessSettingSaved = line;
				break;
			}
			counter++;
		}
	}
}

void Saving::saveLevel(std::string level) { levelSaved = level; }
std::string* Saving::getLevelSaved() { return &levelSaved; }

void Saving::saveMusicSetting(std::string music) { musicSettingSaved = music; }
std::string* Saving::getMusicSettingSaved() { return &musicSettingSaved; }

void Saving::saveEffectSetting(std::string effect) { effectsSettingSaved = effect; }
std::string* Saving::getEffectSettingSaved() { return &effectsSettingSaved; }

void Saving::saveResolutionSetting(std::string resolution) { resolutionSettingSaved = resolution; }
std::string* Saving::getResolutionSettingSaved() { return &resolutionSettingSaved; }

void Saving::saveFullscreenSetting(std::string fullscreen) { fullscreenSettingSaved = fullscreen; }
std::string* Saving::getFullscreenSettingSaved() { return &fullscreenSettingSaved; }

void Saving::saveBorderlessSetting(std::string borderless) { borderlessSettingSaved = borderless; }
std::string* Saving::getBorderlessSettingSaved() { return &borderlessSettingSaved; }

void Saving::saveSettingsInFile();