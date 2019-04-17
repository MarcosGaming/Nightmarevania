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
EndingScene endingScene;

int main() 
{
	Engine::Start(GAMEX, GAMEY, "Nightmarevania",&main_menu);
	return 0;
}