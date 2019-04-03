#include "scene_settings.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <system_renderer.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> settings_background;
shared_ptr<Entity> music_on;
shared_ptr<Entity> music_off;
shared_ptr<Entity> effects_on;
shared_ptr<Entity> effects_off;
shared_ptr<Entity> resolution1;
shared_ptr<Entity> resolution2;
shared_ptr<Entity> resolution3;
shared_ptr<Entity> resolution4;
shared_ptr<Entity> fullscreen;
shared_ptr<Entity> fullscreen_on;
shared_ptr<Entity> fullscreen_off;
shared_ptr<Entity> borderless;
shared_ptr<Entity> borderless_on;
shared_ptr<Entity> borderless_off;


shared_ptr<Texture> settings_background_tex;
shared_ptr<Texture> on_tex;
shared_ptr<Texture> off_tex;
shared_ptr<Texture> resolution1_tex;
shared_ptr<Texture> resolution2_tex;
shared_ptr<Texture> resolution3_tex;
shared_ptr<Texture> resolution4_tex;
shared_ptr<Texture> fullscreen_tex;
shared_ptr<Texture> borderless_tex;

shared_ptr<MediatorResolutionButtons> mediator_resolution;

void SettingsScene::Load()
{
	// On and Off textures
	on_tex = make_shared<Texture>();
	on_tex->loadFromFile("res/menus/on.png");
	off_tex = make_shared<Texture>();
	off_tex->loadFromFile("res/menus/off.png");
	// Background
	settings_background_tex = make_shared<Texture>();
	settings_background_tex->loadFromFile("res/menus/settings_background.png");
	{
		settings_background = makeEntity();
		settings_background->setPosition(Vector2f(GAMEX / 2.0f,  GAMEY / 2.0f));
		// Sprite
		auto sprite = settings_background->addComponent<SpriteComponent>();
		sprite->setTexure(settings_background_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 384, 224));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		float scaleX = (float) GAMEX / (sprite->getSprite().getTextureRect().width);
		float scaleY = (float) GAMEY / (sprite->getSprite().getTextureRect().height);
		sprite->getSprite().scale(scaleX, scaleY);
	}
	// Music On
	{
		music_on = makeEntity();
		music_on->setPosition(Vector2f(240.0f, 114.0f));
		// sprite
		auto sprite = music_on->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 40, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
	}
	// Music Off
	{
		music_off = makeEntity();
		music_off->setPosition(Vector2f(390.0f, 110.0f));
		// sprite
		auto sprite = music_off->addComponent<SpriteComponent>();
		sprite->setTexure(off_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 40, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
	}
	// Effects On 
	{
		effects_on = makeEntity();
		effects_on->setPosition(Vector2f(240.0f, 188.0f));
		// sprite
		auto sprite = effects_on->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 40, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
	}
	// Effects Off
	{
		effects_off = makeEntity();
		effects_off->setPosition(Vector2f(390.0f, 184.0f));
		// sprite
		auto sprite = effects_off->addComponent<SpriteComponent>();
		sprite->setTexure(off_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 40, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
	}
	// Mediator for resolution buttons
	mediator_resolution = make_shared<MediatorResolutionButtons>();
	// Resolution 1920x1080
	resolution1_tex = make_shared<Texture>();
	resolution1_tex->loadFromFile("res/menus/1920x1080.png");
	{
		resolution1 = makeEntity();
		resolution1->setPosition(Vector2f(5.0f, 370.0f));
		// Sprite
		auto sprite = resolution1->addComponent<SpriteComponent>();
		sprite->setTexure(resolution1_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Active button component
		auto resolutionButton = resolution1->addComponent<SpecificResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolution(1920, 1080);
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Resolution 1600x900
	resolution2_tex = make_shared<Texture>();
	resolution2_tex->loadFromFile("res/menus/1600x900.png");
	{
		resolution2 = makeEntity();
		resolution2->setPosition(Vector2f(300.0f, 370.0f));
		// Sprite
		auto sprite = resolution2->addComponent<SpriteComponent>();
		sprite->setTexure(resolution2_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Active button component
		auto resolutionButton = resolution2->addComponent<SpecificResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolution(1600, 900);
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Resolution 1280x720
	resolution3_tex = make_shared<Texture>();
	resolution3_tex->loadFromFile("res/menus/1280x720.png");
	{
		resolution3 = makeEntity();
		resolution3->setPosition(Vector2f(5.0f, 460.0f));
		// Sprite
		auto sprite = resolution3->addComponent<SpriteComponent>();
		sprite->setTexure(resolution3_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Active button component
		auto resolutionButton = resolution3->addComponent<SpecificResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolution(1280, 720);
		resolutionButton->setMediator(mediator_resolution);
		resolutionButton->setActive(true);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Resolution 1024x576
	resolution4_tex = make_shared<Texture>();
	resolution4_tex->loadFromFile("res/menus/1024x576.png");
	{
		resolution4 = makeEntity();
		resolution4->setPosition(Vector2f(300.0f, 460.0f));
		// Sprite
		auto sprite = resolution4->addComponent<SpriteComponent>();
		sprite->setTexure(resolution4_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Active button component
		auto resolutionButton = resolution4->addComponent<SpecificResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolution(1024, 576);
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Fullscreen
	fullscreen_tex = make_shared<Texture>();
	fullscreen_tex->loadFromFile("res/menus/fullscreen.png");
	{
		fullscreen = makeEntity();
		fullscreen->setPosition(Vector2f(10.0f, 550.0f));
		// Sprite
		auto sprite = fullscreen->addComponent<SpriteComponent>();
		sprite->setTexure(fullscreen_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 84, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
	}
	// Fullscreen
	borderless_tex = make_shared<Texture>();
	borderless_tex->loadFromFile("res/menus/borderless.png");
	{
		borderless = makeEntity();
		borderless->setPosition(Vector2f(305.0f, 550.0f));
		// Sprite
		auto sprite = borderless->addComponent<SpriteComponent>();
		sprite->setTexure(borderless_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 80, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
	}

	setLoaded(true);
}

sf::FloatRect CalculateViewport(const sf::Vector2u& screensize, const sf::Vector2u& gamesize)
{

	const sf::Vector2f screensf(screensize.x, screensize.y);
	const sf::Vector2f gamesf(gamesize.x, gamesize.y);
	const float gameAspect = gamesf.x / gamesf.y;
	const float screenAspect = screensf.x / screensf.y;
	// Final size.x of game viewport in pixels
	float scaledWidth;
	// Final size.y of game viewport in pixels
	float scaledHeight;
	// False = scale to screen.x, True = screen.y
	bool scaleSide = false;

	//Work out which way to scale
	if (gamesize.x > gamesize.y)
	{
		// Game is wider than tall, can we use full width?
		if (screensf.y < (screensf.x / gameAspect)) {
			// No, not high enough to fit game height
			scaleSide = true;
		}
		else {
			// Yes, use all width available
			scaleSide = false;
		}
	}
	else
	{
		// Game is Square or Taller than Wide, can we use full height?
		if (screensf.x < (screensf.y * gameAspect))
		{
			// No, screensize not wide enough to fit game width
			scaleSide = false;
		}
		else
		{
			// Yes, use all height available
			scaleSide = true;
		}
	}

	if (scaleSide)
	{
		// Use max screen height
		scaledHeight = screensf.y;
		scaledWidth = floor(scaledHeight * gameAspect);
	}
	else
	{
		// Use max screen width
		scaledWidth = screensf.x;
		scaledHeight = floor(scaledWidth / gameAspect);
	}

	//calculate as percent of screen
	const float widthPercent = (scaledWidth / screensf.x);
	const float heightPercent = (scaledHeight / screensf.y);

	return sf::FloatRect(0, 0, widthPercent, heightPercent);
}

// Change screen resolution
void ChangeScreenResolution(int _width, int _height)
{
	const sf::Vector2u screensize(_width, _height);
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

void SettingsScene::Update(const double& dt)
{
	Scene::Update(dt);

	if (Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		int previousX = Engine::GetWindow().getSize().x;
		int previousY = Engine::GetWindow().getSize().y;
		Engine::GetWindow().create(sf::VideoMode(previousX, previousY), "Nightmarevania", sf::Style::None);
		ChangeScreenResolution(previousX, previousY);
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		int previousX = Engine::GetWindow().getSize().x;
		int previousY = Engine::GetWindow().getSize().y;
		Engine::GetWindow().create(sf::VideoMode(previousX, previousY), "Nightmarevania", sf::Style::Close);
		ChangeScreenResolution(previousX, previousY);
	}
	else if (Keyboard::isKeyPressed(sf::Keyboard::Num7))
	{
		int x = sf::VideoMode::getDesktopMode().width;
		int y = sf::VideoMode::getDesktopMode().height;
		Engine::GetWindow().create(sf::VideoMode(x, y), "Nightmarevania", sf::Style::Fullscreen);
		ChangeScreenResolution(x, y);
	}
}

void SettingsScene::Render()
{
	Scene::Render();
}

void SettingsScene::UnLoad()
{
	mediator_resolution->UnLoad();
	Scene::UnLoad();
}