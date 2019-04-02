#include "scene_settings.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <system_renderer.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> settings_background;
/*shared_ptr<Entity> title;
shared_ptr<Entity> start_btn;
shared_ptr<Entity> settings_btn;
shared_ptr<Entity> credits_btn;
shared_ptr<Entity> exit_btn;*/

shared_ptr<Texture> settings_background_tex;
/*shared_ptr<Texture> title_tex;
shared_ptr<Texture> start_tex;
shared_ptr<Texture> settings_tex;
shared_ptr<Texture> credits_tex;
shared_ptr<Texture> exit_tex;*/

void SettingsScene::Load()
{
	// Background
	settings_background_tex = make_shared<Texture>();
	settings_background_tex->loadFromFile("res/menus/settings_background.png");
	{
		settings_background = makeEntity();
		settings_background->setPosition(Vector2f(GAMEX / 2.0f,  GAMEY / 2.0f));
		settings_background->addTag("Background");
		// Sprite
		auto sprite = settings_background->addComponent<SpriteComponent>();
		sprite->setTexure(settings_background_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 384, 224));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		float scaleX = (float) GAMEX / (sprite->getSprite().getTextureRect().width);
		float scaleY = (float) GAMEY / (sprite->getSprite().getTextureRect().height);
		sprite->getSprite().scale(scaleX, scaleY);
	}
	// Music On 40 x 15

	// Music Off 40 x 15

	// Effects On 40 x 15

	// Effects Off 40 x 15
	/*title_tex = make_shared<Texture>();
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
		button->setScene(&testing);
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
	}*/

	setLoaded(true);
}

// Create FloatRect to fits Game into Screen while preserving aspect
sf::FloatRect CalculateViewport(const sf::Vector2u& screensize, const sf::Vector2u& gamesize) {

	const Vector2f screensf(screensize.x, screensize.y);
	const Vector2f gamesf(gamesize.x, gamesize.y);
	const float gameAspect = gamesf.x / gamesf.y;
	const float screenAspect = screensf.x / screensf.y;
	float scaledWidth;  // final size.x of game viewport in piels
	float scaledHeight; //final size.y of game viewport in piels
	bool scaleSide = false; // false = scale to screen.x, true = screen.y

							//Work out which way to scale
	if (gamesize.x > gamesize.y) { // game is wider than tall
								   // Can we use full width?
		if (screensf.y < (screensf.x / gameAspect)) {
			//no, not high enough to fit game height
			scaleSide = true;
		}
		else {
			//Yes, use all width available
			scaleSide = false;
		}
	}
	else { // Game is Square or Taller than Wide
		   // Can we use full height?
		if (screensf.x < (screensf.y * gameAspect)) {
			// No, screensize not wide enough to fit game width
			scaleSide = false;
		}
		else {
			// Yes, use all height available
			scaleSide = true;
		}
	}

	if (scaleSide) { // use max screen height
		scaledHeight = screensf.y;
		scaledWidth = floor(scaledHeight * gameAspect);
	}
	else { //use max screen width
		scaledWidth = screensf.x;
		scaledHeight = floor(scaledWidth / gameAspect);
	}

	//calculate as percent of screen
	const float widthPercent = (scaledWidth / screensf.x);
	const float heightPercent = (scaledHeight / screensf.y);

	return sf::FloatRect(0, 0, widthPercent, heightPercent);
}


void SettingsScene::Update(const double& dt)
{
	Scene::Update(dt);

	if (Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		// All together now in a reusable solution.
		const sf::Vector2u screensize(1024, 576);
		const sf::Vector2u gamesize(GAMEX, GAMEY);
		//set View as normal
		Engine::GetWindow().setSize(screensize);
		sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
		auto v = sf::View(visibleArea);
		// figure out how to scale and maintain aspect;
		auto viewport = CalculateViewport(screensize, gamesize);
		//Optionally Center it
		bool centered = true;
		if (centered)
		{
			viewport.left = (1.0 - viewport.width) * 0.5;
			viewport.top = (1.0 - viewport.height) * 0.5;
		}
		v.setViewport(viewport);
		Engine::GetWindow().setView(v);
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num3)) 
	{
		// All together now in a reusable solution.
		const sf::Vector2u screensize(1600, 900);
		const sf::Vector2u gamesize(GAMEX, GAMEY);
		//set View as normal
		Engine::GetWindow().setSize(screensize);
		sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
		auto v = sf::View(visibleArea);
		// figure out how to scale and maintain aspect;
		auto viewport = CalculateViewport(screensize, gamesize);
		//Optionally Center it
		bool centered = true;
		if (centered) 
		{
			viewport.left = (1.0 - viewport.width) * 0.5;
			viewport.top = (1.0 - viewport.height) * 0.5;
		}
		v.setViewport(viewport);
		Engine::GetWindow().setView(v);
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		// All together now in a reusable solution.
		const sf::Vector2u screensize(1920, 1080);
		const sf::Vector2u gamesize(GAMEX, GAMEY);
		//set View as normal
		Engine::GetWindow().setSize(screensize);
		sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
		auto v = sf::View(visibleArea);
		// figure out how to scale and maintain aspect;
		auto viewport = CalculateViewport(screensize, gamesize);
		//Optionally Center it
		bool centered = true;
		if (centered)
		{
			viewport.left = (1.0 - viewport.width) * 0.5;
			viewport.top = (1.0 - viewport.height) * 0.5;
		}
		v.setViewport(viewport);
		Engine::GetWindow().setView(v);
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		Engine::GetWindow().create(sf::VideoMode(Engine::getWindowSize().x, Engine::getWindowSize().y), "Nightmarevania", sf::Style::Fullscreen);
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		Engine::GetWindow().create(sf::VideoMode(Engine::getWindowSize().x, Engine::getWindowSize().y), "Nightmarevania", sf::Style::Default);
	}
}

void SettingsScene::Render()
{
	Scene::Render();
}

void SettingsScene::UnLoad()
{
	Scene::UnLoad();
}