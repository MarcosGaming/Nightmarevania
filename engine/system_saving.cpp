#include "system_saving.h"
#include "system_controller.h"
#include "system_sound.h"
#include "system_resolution.h"
#include <vector>
#include <fstream>

static std::string levelSaved;
static std::string musicSettingSaved;
static std::string effectsSettingSaved;
static std::string resolutionSettingSaved;
static std::string fullscreenSettingSaved;
static std::string borderlessSettingSaved;

static std::fstream saveFile;

static void getSettingsFromSystem();

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
	saveFile.close();
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

void Saving::saveSettingsInFile()
{
	getSettingsFromSystem();
	// Open the file and remove all the contents from it
	std::string userpath = getenv("USERPROFILE");
	saveFile.open(userpath + "/Documents/NightmarevaniaSave.txt", std::fstream::out | std::fstream::app);
	// Add level to file
	saveFile << levelSaved << std::endl;
	// Add music setting to file 
	saveFile << musicSettingSaved << std::endl;
	// Add effects setting to file
	saveFile << effectsSettingSaved << std::endl;
	// Add resolution setting to file
	saveFile << resolutionSettingSaved << std::endl;
	// Add music setting to file
	saveFile << fullscreenSettingSaved << std::endl;
	// Add borderless setting to file
	saveFile << borderlessSettingSaved << std::endl;
	// Close file
	saveFile.close();
}

static void getSettingsFromSystem()
{
	// Set level safe to 0 if it is empty
	if (levelSaved.empty())
	{
		levelSaved = "0";
	}
	// Set music setting to save
	if (Audio::isMusicOn())
	{
		musicSettingSaved = "On";
	}
	else
	{
		musicSettingSaved = "Off";
	}
	// Set effect setting to save
	if (Audio::areEffectsOn())
	{
		effectsSettingSaved = "On";
	}
	else
	{
		effectsSettingSaved = "Off";
	}
	// Set resolution setting to save
	if (Resolution::isResolution1920x1080On())
	{
		resolutionSettingSaved = "1920x1080";
	}
	else if (Resolution::isResolution1600x900On())
	{
		resolutionSettingSaved = "1600x900";
	}
	else if (Resolution::isResolution1280x720On())
	{
		resolutionSettingSaved = "1280x720";
	}
	else
	{
		resolutionSettingSaved = "1024x576";
	}
	// Set fullscreen setting to save
	if (Resolution::isFullScreenOn())
	{
		fullscreenSettingSaved = "On";
	}
	else
	{
		fullscreenSettingSaved = "Off";
	}
	// Set borderless setting to save
	if (Resolution::isBorderlessOn())
	{
		borderlessSettingSaved = "On";
	}
	else
	{
		borderlessSettingSaved = "Off";
	}
}