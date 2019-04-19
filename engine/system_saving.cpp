#include "system_saving.h"
#include "system_controller.h"
#include "system_sound.h"
#include "system_resolution.h"
#include "../lib_conversor/conversor.h"
#include <fstream>

// Settings saved
static std::string levelSaved;
static std::string musicSettingSaved;
static std::string effectsSettingSaved;
static std::string resolutionSettingSaved;
static std::string fullscreenSettingSaved;
static std::string borderlessSettingSaved;
static std::vector<std::string> keysButtonsSaved;
static std::vector<std::string> controllerButtonsSaved;
// Save file
static std::fstream saveFile;
// Functions that is going to get which settings are set on the system before storing them in the file
static void getSettingsFromSystem();

void Saving::initialise()
{
	// Read file where the level and the settings is stored
	std::string userpath = getenv("USERPROFILE");
	saveFile.open(userpath + "/Documents/NightmarevaniaSave.txt", std::fstream::in);
	// If the file is not empty 
	if (saveFile.peek() != EOF)
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
			case 6:
				keysButtonsSaved.push_back(line);
				break;
			case 7:
				keysButtonsSaved.push_back(line);
				break;
			case 8:
				keysButtonsSaved.push_back(line);
				break;
			case 9:
				keysButtonsSaved.push_back(line);
				break;
			case 10:
				keysButtonsSaved.push_back(line);
				break;
			case 11:
				keysButtonsSaved.push_back(line);
				break;
			case 12:
				keysButtonsSaved.push_back(line);
				break;
			case 13:
				keysButtonsSaved.push_back(line);
				break;
			case 14:
				controllerButtonsSaved.push_back(line);
				break;
			case 15:
				controllerButtonsSaved.push_back(line);
				break;
			case 16:
				controllerButtonsSaved.push_back(line);
				break;
			case 17:
				controllerButtonsSaved.push_back(line);
				break;
			}
			counter++;
		}
	}
	saveFile.close();
}

void Saving::saveLevel(std::string level) { levelSaved = level; }
std::string* Saving::getLevelSaved() { return &levelSaved; }

std::string* Saving::getMusicSettingSaved() { return &musicSettingSaved; }

std::string* Saving::getEffectSettingSaved() { return &effectsSettingSaved; }

std::string* Saving::getResolutionSettingSaved() { return &resolutionSettingSaved; }

std::string* Saving::getFullscreenSettingSaved() { return &fullscreenSettingSaved; }

std::string* Saving::getBorderlessSettingSaved() { return &borderlessSettingSaved; }

std::vector<std::string>* Saving::getKeysButtonsSaved() { return &keysButtonsSaved; }

std::vector<std::string>* Saving::getControllerButtonsSaved() { return &controllerButtonsSaved; }

void Saving::saveSettingsInFile()
{
	// Clear vectors
	keysButtonsSaved.clear();
	controllerButtonsSaved.clear();
	// Get settings from the differnet elements in the system
	getSettingsFromSystem();
	// Open the file and remove all the contents from it
	std::string userpath = getenv("USERPROFILE");
	saveFile.open(userpath + "/Documents/NightmarevaniaSave.txt", std::fstream::out | std::fstream::trunc);
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
	// Add moveright action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[0] << std::endl;
	// Add moveleft action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[1] << std::endl;
	// Add jump action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[2] << std::endl;
	// Add attack action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[3] << std::endl;
	// Add defend action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[4] << std::endl;
	// Add up attack action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[5] << std::endl;
	// Add down attack action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[6] << std::endl;
	// Add pause action corresponding to keyboard/mouse
	saveFile << keysButtonsSaved[7] << std::endl;
	// Add jump action corresponding to controller
	saveFile << controllerButtonsSaved[0] << std::endl;
	// Add attack action corresponding to controller
	saveFile << controllerButtonsSaved[1] << std::endl;
	// Add defend action corresponding to controller
	saveFile << controllerButtonsSaved[2] << std::endl;
	// Add pause action corresponding to controller
	saveFile << controllerButtonsSaved[3] << std::endl;
	// Close file
	saveFile.close();
}

static void getSettingsFromSystem()
{
	// Set level safe to 0 if it is empty or if does not corresponds to a level
	if (levelSaved.empty() || (levelSaved != "1" && levelSaved != "2" && levelSaved != "3" && levelSaved != "4"))
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
	else if (Resolution::isResolution1024x576On())
	{
		resolutionSettingSaved = "1024x576";
	}
	else
	{
		resolutionSettingSaved = "1280x720";
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
	// Moveright keyboard/mouse key/button to save
	std::string string = Controller::MoveRightButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Moveleft keyboard/mouse key/button to save
	string = Controller::MoveLeftButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Jump keyboard/mouse key/button to save
	string = Controller::JumpButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Attack keyboard/mouse key/button to save
	string = Controller::AttackButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Defend keyboard/mouse key/button to save
	string = Controller::DefendButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Up attack keyboard/mouse key/button to save
	string = Controller::UpAttackButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Down attack keyboard/mouse key/button to save
	string = Controller::DownAttackButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Pause keyboard/mouse key/button to save
	string = Controller::PauseButton;
	if (Controller::getActionKey(string) != NULL)
	{
		keysButtonsSaved.push_back(Conversor::KeyboardKeyToString(*Controller::getActionKey(string)));
	}
	else
	{
		keysButtonsSaved.push_back(Conversor::MouseButtonToString(*Controller::getActionMouseButton(string)));
	}
	// Jump controller button to save
	string = Controller::JumpButton;
	controllerButtonsSaved.push_back(Conversor::ControllerButtonToString(*Controller::getActionControllerButton(string)));
	// Attack controller button to save
	string = Controller::AttackButton;
	controllerButtonsSaved.push_back(Conversor::ControllerButtonToString(*Controller::getActionControllerButton(string)));
	// Defend controller button to save
	string = Controller::DefendButton;
	controllerButtonsSaved.push_back(Conversor::ControllerButtonToString(*Controller::getActionControllerButton(string)));
	// Pause controller button to save
	string = Controller::PauseButton;
	controllerButtonsSaved.push_back(Conversor::ControllerButtonToString(*Controller::getActionControllerButton(string)));
}