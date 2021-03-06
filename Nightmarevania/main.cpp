#include "engine.h"
#include "game.h"

// Scenes
MainMenuScene main_menu;
SettingsScene settings;
CreditsScene credits;
LevelOne levelOne;
LevelTwo levelTwo;
LevelThree levelThree;
EndingScene endingScene;
LevelSword levelSword;
LevelOutside levelOutside;

// Game starts at main menu
int main() 
{
	Engine::Start(GAMEX, GAMEY, "Nightmarevania", &main_menu);
	return 0;
} 