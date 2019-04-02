#include "scene_main_menu.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <system_renderer.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> main_menu_background;
static shared_ptr<Entity> title;
static shared_ptr<Entity> start_btn;
static shared_ptr<Entity> settings_btn;
static shared_ptr<Entity> credits_btn;
static shared_ptr<Entity> exit_btn;

shared_ptr<Texture> main_menu_background_tex;
shared_ptr<Texture> title_tex;
shared_ptr<Texture> start_tex;
shared_ptr<Texture> settings_tex;
shared_ptr<Texture> credits_tex;
shared_ptr<Texture> exit_tex;

void MainMenuScene::Load()
{
	// Background
	main_menu_background_tex = make_shared<Texture>();
	main_menu_background_tex->loadFromFile("res/menus/main_menu_background.png");
	{
		main_menu_background = makeEntity();
		main_menu_background->setPosition(Vector2f(GAMEX / 2.0f,  GAMEY / 2.0f));
		main_menu_background->addTag("Background");
		// Sprite
		auto sprite = main_menu_background->addComponent<SpriteComponent>();
		sprite->setTexure(main_menu_background_tex);
		sprite->getSprite().setOrigin(main_menu_background_tex->getSize().x * 0.5f, main_menu_background_tex->getSize().y * 0.5f);
		float scaleX = (float) GAMEX / (main_menu_background_tex->getSize().x);
		float scaleY = (float) GAMEY / (main_menu_background_tex->getSize().y);
		sprite->getSprite().scale(scaleX, scaleY);
	}
	// Title
	title_tex = make_shared<Texture>();
	title_tex->loadFromFile("res/menus/nightmarevania_title.png");
	{
		title = makeEntity();
		title->setPosition(Vector2f((GAMEX / 2.0f) - (title_tex->getSize().x / 2.0f), 0.5 * (GAMEY / 2.0f)));
		// Sprite
		auto sprite = title->addComponent<SpriteComponent>();
		sprite->setTexure(title_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 37 * 4, 235, 37));
		sprite->getSprite().scale(Vector2f(2.0f, 2.0f));
		title->setPosition(Vector2f(title->getPosition().x / 1.35f, title->getPosition().y));
	}
	// Start button
	start_tex = make_shared<Texture>();
	start_tex->loadFromFile("res/menus/start.png");
	{
		start_btn = makeEntity();
		start_btn->setPosition(Vector2f((GAMEX/2.0f) - (start_tex->getSize().x / 2.0f) - 17.0f, (GAMEY / 2.0f) + 130.0f));
		// sprite
		auto sprite = start_btn->addComponent<SpriteComponent>();
		sprite->setTexure(start_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 125, 37));
		// button component
		auto button = start_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 125, 37));
		button->setHovered(sf::IntRect(0, 37, 125, 37));
		button->setScene(&testing);
	}
	// Settings button 
	settings_tex = make_shared<Texture>();
	settings_tex->loadFromFile("res/menus/settings.png");
	{
		settings_btn = makeEntity();
		settings_btn->setPosition(Vector2f((GAMEX / 2.0f) - (start_tex->getSize().x / 2.0f) - 40.0f, (GAMEY / 2.0f) + 180.0f));
		// sprite
		auto sprite = settings_btn->addComponent<SpriteComponent>();
		sprite->setTexure(settings_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 200, 40));
		// button component
		auto button = settings_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 200, 40));
		button->setHovered(sf::IntRect(0, 40, 200, 40));
		button->setScene(&settings);
	}
	// Credits button 
	credits_tex = make_shared<Texture>();
	credits_tex->loadFromFile("res/menus/credits.png");
	{
		credits_btn = makeEntity();
		credits_btn->setPosition(Vector2f((GAMEX / 2.0f) - (credits_tex->getSize().x / 2.0f) - 18.0f, (GAMEY / 2.0f) + 230.0f));
		// sprite
		auto sprite = credits_btn->addComponent<SpriteComponent>();
		sprite->setTexure(credits_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 150, 40));
		// button component
		auto button = credits_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 150, 40));
		button->setHovered(sf::IntRect(0, 40, 150, 40));
		button->setScene(&testing);
	}
	// Exit button
	exit_tex = make_shared<Texture>();
	exit_tex->loadFromFile("res/menus/exit.png");
	{
		exit_btn = makeEntity();
		exit_btn->setPosition(Vector2f((GAMEX / 2.0f) - (exit_tex->getSize().x / 2.0f) - 18.0f, (GAMEY / 2.0f) + 280.0f));
		// sprite
		auto sprite = exit_btn->addComponent<SpriteComponent>();
		sprite->setTexure(exit_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 100, 37));
		// button component
		auto button = exit_btn->addComponent<ExitButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 100, 37));
		button->setHovered(sf::IntRect(0, 40, 100, 37));
	}

	setLoaded(true);
}


void MainMenuScene::Update(const double& dt)
{
	Scene::Update(dt);
}

void MainMenuScene::Render()
{
	Scene::Render();
}

void MainMenuScene::UnLoad()
{
	Scene::UnLoad();
}