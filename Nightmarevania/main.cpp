#include "engine.h"
#include "game.h"

using namespace std;

// Scenes
TestingScene testing;
LevelOne levelOne;
LevelTwo levelTwo;
LevelThree levelThree;

int main() 
{
	// Previous 1280x720
	//Engine::Start(1920, 1080, "Nightmarevania", &testing);

	//for debugging...
	//Engine::Start(1920, 1080, "Nightmarevania",&levelOne);
	Engine::Start(1920, 1080, "Nightmarevania", &levelTwo);
	//Engine::Start(1920, 1080, "Nightmarevania", &levelThree);

	return 0;
}