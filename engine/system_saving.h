#pragma once
#include <string>

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

	void saveSettingsInFile();
};