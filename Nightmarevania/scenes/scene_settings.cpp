#include "scene_settings.h"
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
static shared_ptr<Entity> music_on_btn;
static shared_ptr<Entity> music_off_btn;
static shared_ptr<Entity> effects_on_btn;
static shared_ptr<Entity> effects_off_btn;
static shared_ptr<Entity> resolution1_btn;
static shared_ptr<Entity> resolution2_btn;
static shared_ptr<Entity> resolution3_btn;
static shared_ptr<Entity> resolution4_btn;
static shared_ptr<Entity> fullscreen_btn;
static shared_ptr<Entity> borderless_btn;
static shared_ptr<Entity> goBack_btn;


static shared_ptr<Texture> background_tex;
static shared_ptr<Texture> on_tex;
static shared_ptr<Texture> off_tex;
static shared_ptr<Texture> resolution1_tex;
static shared_ptr<Texture> resolution2_tex;
static shared_ptr<Texture> resolution3_tex;
static shared_ptr<Texture> resolution4_tex;
static shared_ptr<Texture> fullscreen_tex;
static shared_ptr<Texture> borderless_tex;
static shared_ptr<Texture> goBack_tex;

shared_ptr<MediatorResolutionButtons> mediator_resolution;
shared_ptr<MediatorSoundButtons> mediator_sound;


void SettingsScene::Load()
{
	// Background
	background_tex = make_shared<Texture>();
	background_tex->loadFromFile("res/menus/settings_background.png");
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
		goBack_btn->setPosition(Vector2f((GAMEX / 2.0f) - (goBack_tex->getSize().x / 2.0f) - 100.0f, 650.0f));
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
	// On and Off textures
	on_tex = make_shared<Texture>();
	on_tex->loadFromFile("res/menus/on.png");
	off_tex = make_shared<Texture>();
	off_tex->loadFromFile("res/menus/off.png");
	// Mediator for sound buttons
	mediator_sound = make_shared<MediatorSoundButtons>();
	// Music On
	{
		music_on_btn = makeEntity();
		music_on_btn->setPosition(Vector2f(260.0f, 114.0f));
		// sprite
		auto sprite = music_on_btn->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 25, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Music button
		auto musicButton = music_on_btn->addComponent<MusicsButtonComponent>();
		musicButton->setNormal(IntRect(0, 0, 25, 15));
		musicButton->setHovered(IntRect(0, 15, 25, 15));
		musicButton->setPressed(IntRect(0, 30, 25, 15));
		if (Audio::isMusicOn())
		{
			musicButton->setActive(true);
		}
		musicButton->setAsOnButton();
		musicButton->setMediator(mediator_sound);
		mediator_sound->addMusicButton(musicButton);
	}
	// Music Off
	{
		music_off_btn = makeEntity();
		music_off_btn->setPosition(Vector2f(410.0f, 110.0f));
		// sprite
		auto sprite = music_off_btn->addComponent<SpriteComponent>();
		sprite->setTexure(off_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 28, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Music button
		auto musicButton = music_off_btn->addComponent<MusicsButtonComponent>();
		musicButton->setNormal(IntRect(0, 0, 28, 15));
		musicButton->setHovered(IntRect(0, 15, 28, 15));
		musicButton->setPressed(IntRect(0, 30, 28, 15));
		if (!Audio::isMusicOn())
		{
			musicButton->setActive(true);
		}
		musicButton->setMediator(mediator_sound);
		mediator_sound->addMusicButton(musicButton);
	}
	// Effects On 
	{
		effects_on_btn = makeEntity();
		effects_on_btn->setPosition(Vector2f(260.0f, 188.0f));
		// sprite
		auto sprite = effects_on_btn->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 25, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Effects button
		auto effectsButton = effects_on_btn->addComponent<EffectsButtonComponent>();
		effectsButton->setNormal(IntRect(0, 0, 25, 15));
		effectsButton->setHovered(IntRect(0, 15, 25, 15));
		effectsButton->setPressed(IntRect(0, 30, 25, 15));
		if (Audio::areEffectsOn())
		{
			effectsButton->setActive(true);
		}
		effectsButton->setAsOnButton();
		effectsButton->setMediator(mediator_sound);
		mediator_sound->addEffectsButton(effectsButton);
	}
	// Effects Off
	{
		effects_off_btn = makeEntity();
		effects_off_btn->setPosition(Vector2f(410.0f, 184.0f));
		// sprite
		auto sprite = effects_off_btn->addComponent<SpriteComponent>();
		sprite->setTexure(off_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 28, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Effects button
		auto effectsButton = effects_off_btn->addComponent<EffectsButtonComponent>();
		effectsButton->setNormal(IntRect(0, 0, 28, 15));
		effectsButton->setHovered(IntRect(0, 15, 28, 15));
		effectsButton->setPressed(IntRect(0, 30, 28, 15));
		if (!Audio::areEffectsOn())
		{
			effectsButton->setActive(true);
		}
		effectsButton->setMediator(mediator_sound);
		mediator_sound->addEffectsButton(effectsButton);
	}
	// Mediator for resolution buttons
	mediator_resolution = make_shared<MediatorResolutionButtons>();
	// Resolution 1920x1080
	resolution1_tex = make_shared<Texture>();
	resolution1_tex->loadFromFile("res/menus/1920x1080.png");
	{
		resolution1_btn = makeEntity();
		resolution1_btn->setPosition(Vector2f(5.0f, 370.0f));
		// Sprite
		auto sprite = resolution1_btn->addComponent<SpriteComponent>();
		sprite->setTexure(resolution1_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Resolution button
		auto resolutionButton = resolution1_btn->addComponent<ResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolutionTo1920x1080();
		if (Resolution::isResolution1920x1080On())
		{
			resolutionButton->setActive(true);
		}
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Resolution 1600x900
	resolution2_tex = make_shared<Texture>();
	resolution2_tex->loadFromFile("res/menus/1600x900.png");
	{
		resolution2_btn = makeEntity();
		resolution2_btn->setPosition(Vector2f(300.0f, 370.0f));
		// Sprite
		auto sprite = resolution2_btn->addComponent<SpriteComponent>();
		sprite->setTexure(resolution2_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Resolution button
		auto resolutionButton = resolution2_btn->addComponent<ResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolutionTo1600x900();
		if (Resolution::isResolution1600x900On())
		{
			resolutionButton->setActive(true);
		}
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Resolution 1280x720
	resolution3_tex = make_shared<Texture>();
	resolution3_tex->loadFromFile("res/menus/1280x720.png");
	{
		resolution3_btn = makeEntity();
		resolution3_btn->setPosition(Vector2f(5.0f, 460.0f));
		// Sprite
		auto sprite = resolution3_btn->addComponent<SpriteComponent>();
		sprite->setTexure(resolution3_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Resolution button
		auto resolutionButton = resolution3_btn->addComponent<ResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolutionTo1280x720();
		resolutionButton->setMediator(mediator_resolution);
		if (Resolution::isResolution1280x720On())
		{
			resolutionButton->setActive(true);
		}
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Resolution 1024x576
	resolution4_tex = make_shared<Texture>();
	resolution4_tex->loadFromFile("res/menus/1024x576.png");
	{
		resolution4_btn = makeEntity();
		resolution4_btn->setPosition(Vector2f(300.0f, 460.0f));
		// Sprite
		auto sprite = resolution4_btn->addComponent<SpriteComponent>();
		sprite->setTexure(resolution4_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 87, 22));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Resolution button
		auto resolutionButton = resolution4_btn->addComponent<ResolutionButtonComponent>();
		resolutionButton->setNormal(IntRect(0, 0, 87, 22));
		resolutionButton->setHovered(IntRect(0, 22, 87, 22));
		resolutionButton->setPressed(IntRect(0, 22*2, 87, 22));
		resolutionButton->setResolutionTo1024x576();
		if (Resolution::isResolution1024x576On())
		{
			resolutionButton->setActive(true);
		}
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
	}
	// Fullscreen
	fullscreen_tex = make_shared<Texture>();
	fullscreen_tex->loadFromFile("res/menus/fullscreen.png");
	{
		fullscreen_btn = makeEntity();
		fullscreen_btn->setPosition(Vector2f(10.0f, 550.0f));
		// Sprite
		auto sprite = fullscreen_btn->addComponent<SpriteComponent>();
		sprite->setTexure(fullscreen_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 84, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Fullscreen button
		auto fullscreenButton = fullscreen_btn->addComponent<FullScreenButtonComponent>();
		fullscreenButton->setNormal(IntRect(0, 0, 84, 15));
		fullscreenButton->setHovered(IntRect(0, 15, 84, 15));
		fullscreenButton->setPressed(IntRect(0, 30, 84, 15));
		fullscreenButton->setHoveredActive(IntRect(0, 45, 84, 15));
		if (Resolution::isFullScreenOn())
		{
			fullscreenButton->setActive(true);
		}
		fullscreenButton->setMediator(mediator_resolution);
		mediator_resolution->addFullScreenButton(fullscreenButton);

	}
	// Borderless
	borderless_tex = make_shared<Texture>();
	borderless_tex->loadFromFile("res/menus/borderless.png");
	{
		borderless_btn = makeEntity();
		borderless_btn->setPosition(Vector2f(305.0f, 550.0f));
		// Sprite
		auto sprite = borderless_btn->addComponent<SpriteComponent>();
		sprite->setTexure(borderless_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 80, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Borderless button
		auto borderlessButton = borderless_btn->addComponent<BorderlessButtonComponent>();
		borderlessButton->setNormal(IntRect(0, 0, 80, 14.8));
		borderlessButton->setHovered(IntRect(0, 15, 80, 14.8));
		borderlessButton->setPressed(IntRect(0, 30, 80, 14.8));
		borderlessButton->setHoveredActive(IntRect(0, 45, 80, 14.8));
		if (Resolution::isBorderlessOn())
		{
			borderlessButton->setActive(true);
		}
		borderlessButton->setMediator(mediator_resolution);
		mediator_resolution->addBorderlessButton(borderlessButton);
	}

	setLoaded(true);
}

void SettingsScene::Update(const double& dt)
{
	Scene::Update(dt);
}

void SettingsScene::Render()
{
	Scene::Render();
}

void SettingsScene::UnLoad()
{
	mediator_resolution->UnLoad();
	mediator_sound->UnLoad();
	Audio::clearAllSounds();
	Scene::UnLoad();
}