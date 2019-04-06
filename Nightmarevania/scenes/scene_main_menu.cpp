#include "scene_main_menu.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> background;
static shared_ptr<Entity> title;
static shared_ptr<Entity> new_game_btn;
static shared_ptr<Entity> continue_btn;
static shared_ptr<Entity> settings_btn;
static shared_ptr<Entity> credits_btn;
static shared_ptr<Entity> exit_btn;

static shared_ptr<Texture> background_tex;
static shared_ptr<Texture> title_tex;
static shared_ptr<Texture> new_game_tex;
static shared_ptr<Texture> continue_tex;
static shared_ptr<Texture> settings_tex;
static shared_ptr<Texture> credits_tex;
static shared_ptr<Texture> exit_tex;

void MainMenuScene::Load()
{
	// Background
	background_tex = make_shared<Texture>();
	background_tex->loadFromFile("res/menus/main_menu_background.png");
	{
		background = makeEntity();
		background->setPosition(Vector2f(GAMEX / 2.0f,  GAMEY / 2.0f));
		background->addTag("Background");
		// Sprite
		auto sprite = background->addComponent<SpriteComponent>();
		sprite->setTexure(background_tex);
		sprite->getSprite().setOrigin(background_tex->getSize().x * 0.5f, background_tex->getSize().y * 0.5f);
		float scaleX = (float) GAMEX / (background_tex->getSize().x);
		float scaleY = (float) GAMEY / (background_tex->getSize().y);
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
	// New game button
	new_game_tex = make_shared<Texture>();
	new_game_tex->loadFromFile("res/menus/new_game.png");
	{
		new_game_btn = makeEntity();
		new_game_btn->setPosition(Vector2f((GAMEX/2.0f) - (new_game_tex->getSize().x / 2.0f) - 20.0f, (GAMEY / 2.0f) + 100.0f));
		// sprite
		auto sprite = new_game_btn->addComponent<SpriteComponent>();
		sprite->setTexure(new_game_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 200, 42));
		// button component
		auto button = new_game_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 200, 42));
		button->setHovered(sf::IntRect(0, 42, 200, 42));
		button->setScene(&testing);
	}
	// Continue button
	continue_tex = make_shared<Texture>();
	continue_tex->loadFromFile("res/menus/continue.png");
	{
		continue_btn = makeEntity();
		continue_btn->setPosition(Vector2f((GAMEX / 2.0f) - (continue_tex->getSize().x / 2.0f) - 7.0f, (GAMEY / 2.0f) + 150.0f));
		// sprite
		auto sprite = continue_btn->addComponent<SpriteComponent>();
		sprite->setTexure(continue_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 200, 37));
		// button component
		auto button = continue_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 200, 37));
		button->setHovered(sf::IntRect(0, 37, 200, 37));
		button->setPressed(sf::IntRect(0, 74, 200, 37));
		// Active depends in having something stored in the save file
		button->setActive(true);
		// The scene is the one in the save file
		button->setScene(&testing);
	}
	// Settings button 
	settings_tex = make_shared<Texture>();
	settings_tex->loadFromFile("res/menus/settings.png");
	{
		settings_btn = makeEntity();
		settings_btn->setPosition(Vector2f((GAMEX / 2.0f) - (settings_tex->getSize().x / 2.0f) - 5.0f, (GAMEY / 2.0f) + 200.0f));
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
		credits_btn->setPosition(Vector2f((GAMEX / 2.0f) - (credits_tex->getSize().x / 2.0f) - 18.0f, (GAMEY / 2.0f) + 250.0f));
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
		exit_btn->setPosition(Vector2f((GAMEX / 2.0f) - (exit_tex->getSize().x / 2.0f) - 18.0f, (GAMEY / 2.0f) + 300.0f));
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
	Audio::clearAllSounds();
	Scene::UnLoad();
}