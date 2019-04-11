#pragma once
#include <string>

namespace Saving 
{
	void initialise();

	void saveLevel(std::string);
	std::string* getLevelSaved();

	void saveMusicSetting(std::string);
	std::string* getMusicSettingSaved();

	void saveEffectSetting(std::string);
	std::string* getEffectSettingSaved();

	void saveResolutionSetting(std::string);
	std::string* getResolutionSettingSaved();

	void saveFullscreenSetting(std::string);
	std::string* getFullscreenSettingSaved();

	void saveBorderlessSetting(std::string);
	std::string* getBorderlessSettingSaved();

	void saveSettingsInFile();
};