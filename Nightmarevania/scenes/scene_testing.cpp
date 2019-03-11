#include "scene_testing.h"
#include "../game.h"
#include <iostream>
#include <LevelSystem.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void TestingScene::Load()
{
	// Level file
	ls::loadLevelFile("res/levels/level_test.txt", 60.0f);
	// Tiles offset
	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));

	setLoaded(true);
}

void TestingScene::UnLoad()
{
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void TestingScene::Update(const double& dt)
{
	Scene::Update(dt);
}

void TestingScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}