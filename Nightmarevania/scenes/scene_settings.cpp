#include "scene_settings.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>

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
shared_ptr<Entity> borderless;
shared_ptr<Entity> goBack;


shared_ptr<Texture> settings_background_tex;
shared_ptr<Texture> on_tex;
shared_ptr<Texture> off_tex;
shared_ptr<Texture> resolution1_tex;
shared_ptr<Texture> resolution2_tex;
shared_ptr<Texture> resolution3_tex;
shared_ptr<Texture> resolution4_tex;
shared_ptr<Texture> fullscreen_tex;
shared_ptr<Texture> borderless_tex;
shared_ptr<Texture> goBack_tex;

shared_ptr<MediatorResolutionButtons> mediator_resolution;
shared_ptr<MediatorSoundButtons> mediator_sound;


void SettingsScene::Load()
{
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
	// Return 
	goBack_tex = make_shared<Texture>();
	goBack_tex->loadFromFile("res/menus/return.png");
	{
		goBack = makeEntity();
		goBack->setPosition(Vector2f((GAMEX / 2.0f) - (goBack_tex->getSize().x / 2.0f) - 100.0f, 650.0f));
		// sprite
		auto sprite = goBack->addComponent<SpriteComponent>();
		sprite->setTexure(goBack_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 55, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// button component
		auto button = goBack->addComponent<ChangeSceneButtonComponent>();
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
		music_on = makeEntity();
		music_on->setPosition(Vector2f(260.0f, 114.0f));
		// sprite
		auto sprite = music_on->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 25, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Music button
		auto musicButton = music_on->addComponent<MusicsButtonComponent>();
		musicButton->setNormal(IntRect(0, 0, 25, 15));
		musicButton->setHovered(IntRect(0, 15, 25, 15));
		musicButton->setPressed(IntRect(0, 30, 25, 15));
		musicButton->setActive(true);
		musicButton->setAsOnButton();
		musicButton->setMediator(mediator_sound);
		mediator_sound->addMusicButton(musicButton);
	}
	// Music Off
	{
		music_off = makeEntity();
		music_off->setPosition(Vector2f(410.0f, 110.0f));
		// sprite
		auto sprite = music_off->addComponent<SpriteComponent>();
		sprite->setTexure(off_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 28, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Music button
		auto musicButton = music_off->addComponent<MusicsButtonComponent>();
		musicButton->setNormal(IntRect(0, 0, 28, 15));
		musicButton->setHovered(IntRect(0, 15, 28, 15));
		musicButton->setPressed(IntRect(0, 30, 28, 15));
		musicButton->setMediator(mediator_sound);
		mediator_sound->addMusicButton(musicButton);
	}
	// Effects On 
	{
		effects_on = makeEntity();
		effects_on->setPosition(Vector2f(260.0f, 188.0f));
		// sprite
		auto sprite = effects_on->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 25, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Effects button
		auto effectsButton = effects_on->addComponent<EffectsButtonComponent>();
		effectsButton->setNormal(IntRect(0, 0, 25, 15));
		effectsButton->setHovered(IntRect(0, 15, 25, 15));
		effectsButton->setPressed(IntRect(0, 30, 25, 15));
		effectsButton->setActive(true);
		effectsButton->setAsOnButton();
		effectsButton->setMediator(mediator_sound);
		mediator_sound->addEffectsButton(effectsButton);
	}
	// Effects Off
	{
		effects_off = makeEntity();
		effects_off->setPosition(Vector2f(410.0f, 184.0f));
		// sprite
		auto sprite = effects_off->addComponent<SpriteComponent>();
		sprite->setTexure(off_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 28, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Effects button
		auto effectsButton = effects_off->addComponent<EffectsButtonComponent>();
		effectsButton->setNormal(IntRect(0, 0, 28, 15));
		effectsButton->setHovered(IntRect(0, 15, 28, 15));
		effectsButton->setPressed(IntRect(0, 30, 28, 15));
		effectsButton->setMediator(mediator_sound);
		mediator_sound->addEffectsButton(effectsButton);
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
		// Resolution button
		auto resolutionButton = resolution1->addComponent<ResolutionButtonComponent>();
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
		// Resolution button
		auto resolutionButton = resolution2->addComponent<ResolutionButtonComponent>();
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
		// Resolution button
		auto resolutionButton = resolution3->addComponent<ResolutionButtonComponent>();
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
		// Resolution button
		auto resolutionButton = resolution4->addComponent<ResolutionButtonComponent>();
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
		// Fullscreen button
		auto fullscreenButton = fullscreen->addComponent<FullScreenButtonComponent>();
		fullscreenButton->setNormal(IntRect(0, 0, 84, 15));
		fullscreenButton->setHovered(IntRect(0, 15, 84, 15));
		fullscreenButton->setPressed(IntRect(0, 30, 84, 15));
		fullscreenButton->setHoveredActive(IntRect(0, 45, 84, 15));
		fullscreenButton->setMediator(mediator_resolution);
		mediator_resolution->addFullScreenButton(fullscreenButton);

	}
	// Borderless
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
		// Borderless button
		auto borderlessButton = borderless->addComponent<BorderlessButtonComponent>();
		borderlessButton->setNormal(IntRect(0, 0, 80, 14.8));
		borderlessButton->setHovered(IntRect(0, 15, 80, 14.8));
		borderlessButton->setPressed(IntRect(0, 30, 80, 14.8));
		borderlessButton->setHoveredActive(IntRect(0, 45, 80, 14.8));
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
	SoundSystem::clearAllSounds();
	Scene::UnLoad();
}