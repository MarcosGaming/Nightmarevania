#pragma once
#include <string>

namespace Resolution 
{
	void initialise(std::string&, std::string&, std::string&);

	bool isFullScreenOn();
	bool isBorderlessOn();
	bool isResolution1920x1080On();
	bool isResolution1600x900On();
	bool isResolution1280x720On();
	bool isResolution1024x576On();
	
	void changeTo1920x1080();
	void changeTo1600x900();
	void changeTo1280x720();
	void changeTo1024x576();

	void turnFullScreenOn();
	void turnFullScreenOff();

	void turnBorderlessOn();
	void turnBorderlessOff();
};