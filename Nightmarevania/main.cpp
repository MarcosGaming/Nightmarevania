#include "engine.h"
#include "game.h"

// Scenes
TestingScene testing;
LevelOne levelOne;
LevelTwo levelTwo;
LevelThree levelThree;
MainMenuScene main_menu;
SettingsScene settings;
CreditsScene credits;

int main() 
{
	//Engine::Start(GAMEX, GAMEY, "Nightmarevania",&main_menu);
	//debugging:
	Engine::Start(GAMEX, GAMEY, "Nightmarevania",&levelOne);
	//Engine::Start(GAMEX, GAMEY, "Nightmarevania",&levelTwo);
	//Engine::Start(GAMEX, GAMEY, "Nightmarevania",&levelThree);
	
	
	return 0;
}