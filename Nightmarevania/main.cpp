#include "engine.h"
#include "game.h"

// Scenes
TestingScene testing;
MainMenuScene main_menu;
SettingsScene settings;

int main() 
{
	Engine::Start(GAMEX, GAMEY, "Nightmarevania",&settings);
	return 0;
}