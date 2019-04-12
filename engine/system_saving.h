#pragma once
#include <string>
#include <vector>

namespace Saving 
{
	void initialise();

	void saveLevel(std::string);
	std::string* getLevelSaved();

	std::string* getMusicSettingSaved();

	std::string* getEffectSettingSaved();

	std::string* getResolutionSettingSaved();

	std::string* getFullscreenSettingSaved();

	std::string* getBorderlessSettingSaved();

	std::vector<std::string>* getKeysButtonsSaved();

	std::vector<std::string>* getControllerButtonsSaved();

	void saveSettingsInFile();
};