#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void MenuScene::Load() 
{
	// Menu scene elements
	auto txt = makeEntity();
	setLoaded(true);
}

void MenuScene::Update(const double& dt) 
{
	Scene::Update(dt);
}
