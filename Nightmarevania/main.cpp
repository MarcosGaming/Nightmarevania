#include "engine.h"
#include "game.h"

using namespace std;

// Scenes


int main() 
{
	// Previous 1280x720
	Engine::Start(1920, 1080, "Nightmarevania",&menu);
	return 0;
}