#include "scene_credits.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>
#include <system_resolution.h>
#include <iostream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> background;
// Return button
static shared_ptr<Entity> goBack_btn;

static shared_ptr<Texture> background_tex;
// Return button texture
static shared_ptr<Texture> goBack_tex;


void CreditsScene::Load()
{
	// Background
	background_tex = make_shared<Texture>();
	background_tex->loadFromFile("res/menus/credits_background.png");
	{
		background = makeEntity();
		background->setPosition(Vector2f(GAMEX / 2.0f,  GAMEY / 2.0f));
		// Sprite
		auto sprite = background->addComponent<SpriteComponent>();
		sprite->setTexure(background_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 384, 224));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		float scaleX = (float) GAMEX / (sprite->getSprite().getTextureRect().width);
		float scaleY = (float) GAMEY / (sprite->getSprite().getTextureRect().height);
		sprite->getSprite().scale(scaleX, scaleY);
	}
	// Return 
	goBack_tex = make_shared<Texture>();
	goBack_tex->loadFromFile("res/menus/return.png");
	{
		goBack_btn = makeEntity();
		goBack_btn->setPosition(Vector2f((GAMEX / 2.0f) - (goBack_tex->getSize().x / 2.0f) - 75.0f, 650.0f));
		// sprite
		auto sprite = goBack_btn->addComponent<SpriteComponent>();
		sprite->setTexure(goBack_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 55, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// button component
		auto button = goBack_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 55, 15));
		button->setHovered(sf::IntRect(0, 15, 55, 15));
		button->setScene(&main_menu);
	}
	setLoaded(true);
}

void CreditsScene::Update(const double& dt)
{
	Scene::Update(dt);
}

void CreditsScene::Render()
{
	Scene::Render();
}

void CreditsScene::UnLoad()
{
	Audio::clearAllSounds();
	Scene::UnLoad();
}