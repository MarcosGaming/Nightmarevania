#include "scene_settings.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_button.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>
#include <system_resolution.h>
#include <system_controller.h>

using namespace std;
using namespace sf;

// Background
static shared_ptr<Entity> background;
// Sound buttons
static shared_ptr<Entity> music_on_btn;
static shared_ptr<Entity> music_off_btn;
static shared_ptr<Entity> effects_on_btn;
static shared_ptr<Entity> effects_off_btn;
// Resolution buttons
static shared_ptr<Entity> resolution1_btn;
static shared_ptr<Entity> resolution2_btn;
static shared_ptr<Entity> resolution3_btn;
static shared_ptr<Entity> resolution4_btn;
static shared_ptr<Entity> fullscreen_btn;
static shared_ptr<Entity> borderless_btn;
// Controls buttons
static shared_ptr<Entity> move_right_btn;
static shared_ptr<Entity> move_left_btn;
static shared_ptr<Entity> jump_btn;
static shared_ptr<Entity> basic_attack_btn;
static shared_ptr<Entity> defend_btn;
static shared_ptr<Entity> up_attack_btn;
static shared_ptr<Entity> down_attack_btn;
static shared_ptr<Entity> pause_btn;
// Return button
static shared_ptr<Entity> goBack_btn;

// Background texture
static shared_ptr<Texture> background_tex;
// Sound buttons textures
static shared_ptr<Texture> on_tex;
static shared_ptr<Texture> off_tex;
// Resolution buttons textures
static shared_ptr<Texture> resolution1_tex;
static shared_ptr<Texture> resolution2_tex;
static shared_ptr<Texture> resolution3_tex;
static shared_ptr<Texture> resolution4_tex;
static shared_ptr<Texture> fullscreen_tex;
static shared_ptr<Texture> borderless_tex;
// Control buttons textures
static shared_ptr<Texture> move_right_tex;
static shared_ptr<Texture> move_left_tex;
static shared_ptr<Texture> jump_tex;
static shared_ptr<Texture> basic_attack_tex;
static shared_ptr<Texture> defend_tex;
static shared_ptr<Texture> up_attack_tex;
static shared_ptr<Texture> down_attack_tex;
static shared_ptr<Texture> pause_tex;
// Return button texture
static shared_ptr<Texture> goBack_tex;

// Mediators
shared_ptr<MediatorResolutionButtons> mediator_resolution;
shared_ptr<MediatorSoundButtons> mediator_sound;

// Elements for controller navigation
static vector<shared_ptr<ButtonComponent>> buttonsForController;
static int buttonsCurrentIndex;

// Obeserver for the buttons that cannot be modified by the controller
shared_ptr<ObserverControls> observer_controls;


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

	// Mediator for sound buttons
	mediator_sound = make_shared<MediatorSoundButtons>();
	// On and Off textures
	on_tex = make_shared<Texture>();
	on_tex->loadFromFile("res/menus/on.png");
	off_tex = make_shared<Texture>();
	off_tex->loadFromFile("res/menus/off.png");
	// Music On
	{
		music_on_btn = makeEntity();
		music_on_btn->setPosition(Vector2f(260.0f, 114.0f));
		// Sprite
		auto sprite = music_on_btn->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 25, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Music button
		auto musicButton = music_on_btn->addComponent<MusicsButtonComponent>();
		musicButton->setNormal(IntRect(0, 0, 25, 15));
		musicButton->setHovered(IntRect(0, 15, 25, 15));
		musicButton->setPressed(IntRect(0, 30, 25, 15));
		musicButton->setAsOnButton();
		if (Audio::isMusicOn())
		{
			musicButton->setActive(true);
		}
		// Sound mediator
		musicButton->setMediator(mediator_sound);
		mediator_sound->addMusicButton(musicButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(musicButton);
	}
	// Music Off
	{
		music_off_btn = makeEntity();
		music_off_btn->setPosition(Vector2f(410.0f, 110.0f));
		// Sprite
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
		// Sound mediator
		musicButton->setMediator(mediator_sound);
		mediator_sound->addMusicButton(musicButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(musicButton);
	}
	// Effects On 
	{
		effects_on_btn = makeEntity();
		effects_on_btn->setPosition(Vector2f(260.0f, 188.0f));
		// Sprite
		auto sprite = effects_on_btn->addComponent<SpriteComponent>();
		sprite->setTexure(on_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 25, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Effects button
		auto effectsButton = effects_on_btn->addComponent<EffectsButtonComponent>();
		effectsButton->setNormal(IntRect(0, 0, 25, 15));
		effectsButton->setHovered(IntRect(0, 15, 25, 15));
		effectsButton->setPressed(IntRect(0, 30, 25, 15));
		effectsButton->setAsOnButton();
		if (Audio::areEffectsOn())
		{
			effectsButton->setActive(true);
		}
		// Sound mediator
		effectsButton->setMediator(mediator_sound);
		mediator_sound->addEffectsButton(effectsButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(effectsButton);
	}
	// Effects Off
	{
		effects_off_btn = makeEntity();
		effects_off_btn->setPosition(Vector2f(410.0f, 184.0f));
		// Sprite
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
		// Sound mediator
		effectsButton->setMediator(mediator_sound);
		mediator_sound->addEffectsButton(effectsButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(effectsButton);
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
		// Resolution mediator
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(resolutionButton);
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
		// Resolution mediator
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(resolutionButton);
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
		if (Resolution::isResolution1280x720On())
		{
			resolutionButton->setActive(true);
		}
		// Resolution mediator
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(resolutionButton);
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
		// Resolution mediator
		resolutionButton->setMediator(mediator_resolution);
		mediator_resolution->addResolutionButton(resolutionButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(resolutionButton);
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
		fullscreenButton->setCanHoverActive(true);
		if (Resolution::isFullScreenOn())
		{
			fullscreenButton->setActive(true);
		}
		// Resolution mediator
		fullscreenButton->setMediator(mediator_resolution);
		mediator_resolution->addFullScreenButton(fullscreenButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(fullscreenButton);
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
		borderlessButton->setNormal(IntRect(0, 0, 80, (int)14.8));
		borderlessButton->setHovered(IntRect(0, 15, 80, (int)14.8));
		borderlessButton->setPressed(IntRect(0, 30, 80, (int) 14.8));
		borderlessButton->setHoveredActive(IntRect(0, 45, 80, (int)14.8));
		borderlessButton->setCanHoverActive(true);
		if (Resolution::isBorderlessOn())
		{
			borderlessButton->setActive(true);
		}
		// Resolution mediator
		borderlessButton->setMediator(mediator_resolution);
		mediator_resolution->addBorderlessButton(borderlessButton);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(borderlessButton);
	}

	// Observer for controls
	observer_controls = make_shared<ObserverControls>();
	// Move right / right attack
	move_right_tex = make_shared<Texture>();
	move_right_tex->loadFromFile("res/menus/move_right.png");
	{
		move_right_btn = makeEntity();
		move_right_btn->setPosition(Vector2f(629.0f, 100.0f));
		// Button
		auto controlsButton = move_right_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::MoveRightButton);
		controlsButton->setNormal(IntRect(0, 0, 205, 20));
		controlsButton->setHovered(IntRect(0, 20, 205, 20));
		controlsButton->setPressed(IntRect(0, 40, 205, 20));
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Add button to observer as it cannot be modified when the controller is connected
		observer_controls->attach(controlsButton);
		// Sprite
		auto sprite = move_right_btn->addComponent<SpriteComponent>();
		sprite->setTexure(move_right_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 205, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Text
		auto text = move_right_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::MoveRightButton);
		text->setInitialPosition(Vector2f(1165.0f, 108.0f));
	}
	// Move left / left attack
	move_left_tex = make_shared<Texture>();
	move_left_tex->loadFromFile("res/menus/move_left.png");
	{
		move_left_btn = makeEntity();
		move_left_btn->setPosition(Vector2f(695.0f, 160.0f));
		// Button
		auto controlsButton = move_left_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::MoveLeftButton);
		controlsButton->setNormal(IntRect(0, 0, 183, 20));
		controlsButton->setHovered(IntRect(0, 20, 183, 20));
		controlsButton->setPressed(IntRect(0, 40, 183, 20));
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Add button to observer as it cannot be modified when the controller is connected
		observer_controls->attach(controlsButton);
		// Sprite
		auto sprite = move_left_btn->addComponent<SpriteComponent>();
		sprite->setTexure(move_left_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 183, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Text
		auto text = move_left_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::MoveLeftButton);
		text->setInitialPosition(Vector2f(1165.0f, 168.0f));
	}
	// Jump
	jump_tex = make_shared<Texture>();
	jump_tex->loadFromFile("res/menus/jump.png");
	{
		jump_btn = makeEntity();
		jump_btn->setPosition(Vector2f(956.0f, 220.0f));
		// Button
		auto controlsButton = jump_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::JumpButton);
		controlsButton->setNormal(IntRect(0, 0, 96, 20));
		controlsButton->setHovered(IntRect(0, 20, 96, 20));
		controlsButton->setCanHoverActive(true);
		controlsButton->setPressed(IntRect(0, 40, 96, 20));
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Sprite
		auto sprite = jump_btn->addComponent<SpriteComponent>();
		sprite->setTexure(jump_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 96, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Text
		auto text = jump_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::JumpButton);
		text->setInitialPosition(Vector2f(1165.0f, 228.0f));
	}
	// Basic attack
	basic_attack_tex = make_shared<Texture>();
	basic_attack_tex->loadFromFile("res/menus/basic_attack.png");
	{
		basic_attack_btn = makeEntity();
		basic_attack_btn->setPosition(Vector2f(851.0f, 280.0f));
		// Button
		auto controlsButton = basic_attack_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::AttackButton);
		controlsButton->setNormal(IntRect(0, 0, 131, 20));
		controlsButton->setHovered(IntRect(0, 20, 131, 20));
		controlsButton->setPressed(IntRect(0, 40, 131, 20));
		controlsButton->setCanHoverActive(true);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Sprite
		auto sprite = basic_attack_btn->addComponent<SpriteComponent>();
		sprite->setTexure(basic_attack_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 131, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Text
		auto text = basic_attack_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::AttackButton);
		text->setInitialPosition(Vector2f(1165.0f, 288.0f));
	}
	// Defend
	defend_tex = make_shared<Texture>();
	defend_tex->loadFromFile("res/menus/defend.png");
	{
		defend_btn = makeEntity();
		defend_btn->setPosition(Vector2f(941.0f, 340.0f));
		// Button
		auto controlsButton = defend_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::DefendButton);
		controlsButton->setNormal(IntRect(0, 0, 101, 20));
		controlsButton->setHovered(IntRect(0, 20, 101, 20));
		controlsButton->setCanHoverActive(true);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Sprite
		auto sprite = defend_btn->addComponent<SpriteComponent>();
		sprite->setTexure(defend_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 101, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		controlsButton->setPressed(IntRect(0, 40, 101, 20));
		// Text
		auto text = defend_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::DefendButton);
		text->setInitialPosition(Vector2f(1165.0f, 348.0f));
	}
	// Up attack
	up_attack_tex = make_shared<Texture>();
	up_attack_tex->loadFromFile("res/menus/up_attack.png");
	{
		up_attack_btn = makeEntity();
		up_attack_btn->setPosition(Vector2f(896.0f, 400.0f));
		// Button
		auto controlsButton = up_attack_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::UpAttackButton);
		controlsButton->setNormal(IntRect(0, 0, 116, 20));
		controlsButton->setHovered(IntRect(0, 20, 116, 20));
		controlsButton->setPressed(IntRect(0, 40, 116, 20));
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Add button to observer as it cannot be modified when the controller is connected
		observer_controls->attach(controlsButton);
		// Sprite
		auto sprite = up_attack_btn->addComponent<SpriteComponent>();
		sprite->setTexure(up_attack_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 116, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Text
		auto text = up_attack_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::UpAttackButton);
		text->setInitialPosition(Vector2f(1165.0f, 408.0f));
	}
	// Down attack
	down_attack_tex = make_shared<Texture>();
	down_attack_tex->loadFromFile("res/menus/down_attack.png");
	{
		down_attack_btn = makeEntity();
		down_attack_btn->setPosition(Vector2f(860.0f, 460.0f));
		// Button
		auto controlsButton = down_attack_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::DownAttackButton);
		controlsButton->setNormal(IntRect(0, 0, 128, 20));
		controlsButton->setHovered(IntRect(0, 20, 128, 20));
		controlsButton->setPressed(IntRect(0, 40, 128, 20));
		// Add button to observer as it cannot be modified when the controller is connected
		observer_controls->attach(controlsButton);
		// Sprite
		auto sprite = down_attack_btn->addComponent<SpriteComponent>();
		sprite->setTexure(down_attack_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 128, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Text
		auto text = down_attack_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::DownAttackButton);
		text->setInitialPosition(Vector2f(1165.0f, 468.0f));
	}
	// Pause
	pause_tex = make_shared<Texture>();
	pause_tex->loadFromFile("res/menus/pause.png");
	{
		pause_btn = makeEntity();
		pause_btn->setPosition(Vector2f(956.0f, 520.0f));
		// Button
		auto controlsButton = pause_btn->addComponent<ControlsButton>();
		controlsButton->setAction(Controller::PauseButton);
		controlsButton->setNormal(IntRect(0, 0, 96, 20));
		controlsButton->setHovered(IntRect(0, 20, 96, 20));
		controlsButton->setPressed(IntRect(0, 40, 96, 20));
		controlsButton->setCanHoverActive(true);
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(controlsButton);
		// Sprite
		auto sprite = pause_btn->addComponent<SpriteComponent>();
		sprite->setTexure(pause_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 96, 20));
		sprite->getSprite().scale(3.0f, 3.0f);
		// Text
		auto text = pause_btn->addComponent<ControlsTextComponent>();
		text->setAction(Controller::PauseButton);
		text->setInitialPosition(Vector2f(1165.0f, 528.0f));
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
		// Add button to the vector used when the controller is connected
		buttonsForController.push_back(button);
	}
	// Controller starts at first button that is not active
	buttonsCurrentIndex = 0;
	while (buttonsForController[buttonsCurrentIndex]->isActive())
	{
		buttonsCurrentIndex++;
	}
	setLoaded(true);
}

void SettingsScene::Update(const double& dt)
{
	Scene::Update(dt);
	ButtonComponent::ButtonNavigation(buttonsForController, buttonsCurrentIndex, dt);
	// Observer activates/deactivates some buttons according to which device is connected
	if (Joystick::isConnected(0) && !observer_controls->isControllerActiveSet())
	{
		observer_controls->setControllerActive(true);
	}
	else if(!Joystick::isConnected(0) && observer_controls->isControllerActiveSet())
	{
		observer_controls->setControllerActive(false);
	}
	// Settings music
	if (Audio::isMusicOn())
	{
		Audio::playMusic("main_menu_music");
	}
	else
	{
		Audio::stopMusic("main_menu_music");
	}
}

void SettingsScene::Render()
{
	Scene::Render();
}

void SettingsScene::UnLoad()
{
	observer_controls->UnLoad();
	mediator_resolution->UnLoad();
	buttonsForController.clear();
	mediator_sound->UnLoad();
	Scene::UnLoad();
}