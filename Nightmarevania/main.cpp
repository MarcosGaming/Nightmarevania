#include "engine.h"
#include "game.h"

// Scenes
TestingScene testing;
MainMenuScene main_menu;
SettingsScene settings;
CreditsScene credits;

int main() 
{
	Engine::Start(GAMEX, GAMEY, "Nightmarevania",&main_menu);

	return 0;
}