#pragma once

namespace Resolution 
{
	void initialise();

	bool isFullScreenOn();
	bool isBorderlessOn();
	bool isResolution1920x1080On();
	bool isResolution1600x900On();
	bool isResolution1280x720On();
	bool isResolution1024x576On();
	
	void changeTo1920x1080(bool borderless);
	void changeTo1600x900(bool borderless);
	void changeTo1280x720(bool borderless);
	void changeTo1024x576(bool borderless);

	void turnFullScreenOn();
	void turnFullScreenOff();

	void turnBorderlessOn();
	void turnBorderlessOff();
};