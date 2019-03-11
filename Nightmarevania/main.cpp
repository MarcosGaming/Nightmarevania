#include "engine.h"
#include "game.h"

using namespace std;

// Scenes
TestingScene testing;

int main() 
{
	// Previous 1280x720
	Engine::Start(1920, 1080, "Nightmarevania",&testing);
	return 0;
}