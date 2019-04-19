#include "scene_leveloutside.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../animation_states.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <LevelSystem.h>
#include <system_controller.h>
#include <system_resolution.h>
#include <system_sound.h>
#include <conversor.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> background;
static shared_ptr<Texture> background_tex;

static shared_ptr<Entity> portal;
static shared_ptr<Texture> portal_tex;

static shared_ptr<Entity> pause_background;
static shared_ptr<Texture> pause_background_tex;

static shared_ptr<Entity> returnToMenu_btn;
static shared_ptr<Texture> returnToMenu_tex;

static shared_ptr<Entity> resume_btn;
static shared_ptr<Texture> resume_tex;

static vector<shared_ptr<ButtonComponent>> buttonsForController;
static int buttonsCurrentIndex;

static shared_ptr<Entity> serah_dialogue;
static shared_ptr<Entity> intro_dialogue;
static shared_ptr<Entity> controls_dialogue;

void LevelOutside::Load()
{
	// Stop music from main menu in the case that we come from there
	Audio::stopMusic("main_menu_music");
	// Controller starts at button 0
	buttonsCurrentIndex = 0;
	// The scene is not paused at the beginning
	_paused = false;
	// Disable cursor
	Engine::GetWindow().setMouseCursorVisible(false);
	// Level file
	ls::loadLevelFile("res/levels/level_outside.txt", 60.0f);
	// Tiles offset
	auto ho = GAMEY - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));
	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");
	spriteSheet = make_shared<Texture>();
	spriteSheet->loadFromFile("res/img/adventurer.png");

	// Background
	background_tex = make_shared<Texture>();
	background_tex->loadFromFile("res/img/initial_level.png");
	{
		background = makeEntity();
		background->setPosition(Vector2f(GAMEX / 2.0f, GAMEY / 2.0f));
		background->addTag("Background");
		// Sprite
		auto sprite = background->addComponent<SpriteComponent>();
		sprite->setTexure(background_tex);
		sprite->getSprite().setOrigin(background_tex->getSize().x * 0.5f, background_tex->getSize().y * 0.5f);
		float scaleX = (float)GAMEX / (background_tex->getSize().x);
		float scaleY = (float)GAMEY / (background_tex->getSize().y);
		sprite->getSprite().scale(scaleX, scaleY);
	}

	// Portal
	portal_tex = make_shared<Texture>();
	portal_tex->loadFromFile("res/img/portal.png");
	{
		portal = makeEntity();
		portal->setPosition(ls::getTilePosition(ls::findTiles(ls::END)[0]));
		portal->setPosition(Vector2f(portal->getPosition().x, portal->getPosition().y - 80.0f));
		portal->addTag("Portal");
		// Sprite component
		auto sprite = portal->addComponent<SpriteComponent>();
		sprite->setTexure(portal_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 64, 64, 64));
		sprite->getSprite().scale(sf::Vector2f(-5.0f, 5.0f));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		// Animations
		shared_ptr<PortalOpenAnimation> portalOpen = make_shared<PortalOpenAnimation>();
		for (int i = 0; i < 8; i++)
		{
			portalOpen->addFrame(IntRect(64 * i, 64, 64, 64));
		}
		shared_ptr<PortalStaticAnimation> portalStatic = make_shared<PortalStaticAnimation>();
		for (int i = 0; i < 8; i++)
		{
			portalStatic->addFrame(IntRect(64 * i, 0, 64, 64));
		}
		// Animation machine component
		auto anim = portal->addComponent<AnimationMachineComponent>();
		anim->addAnimation("Open", portalOpen);
		anim->addAnimation("Static", portalStatic);
		anim->changeAnimation("Open");
		// Portal is initially not going to be render and updated
		portal->setVisible(false);
		portal->setAlive(false);
	}

	// Player
	{
		player = makeEntity();
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		player->addTag("Player");
		// Sprite component
		auto sprite = player->addComponent<SpriteComponent>();
		sprite->setTexure(spriteSheet);
		sprite->getSprite().setTextureRect(IntRect(0, 37 * 12, 50, 37));
		sprite->getSprite().scale(sf::Vector2f(3.0f, 3.0f));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		// Animations, each frame has a size of 50x37
		shared_ptr<IdleAnimation> idle = make_shared<IdleAnimation>();
		for (int i = 0; i < 4; i++)
		{
			idle->addFrame(IntRect(50 * i, 0, 50, 37));
		}
		shared_ptr<RunAnimation> run = make_shared<RunAnimation>();
		for (int i = 1; i < 7; i++)
		{
			run->addFrame(IntRect(50 * i, 37, 50, 37));
		}
		shared_ptr<JumpAnimation> jump = make_shared<JumpAnimation>();
		for (int i = 1; i < 4; i++)
		{
			jump->addFrame(IntRect(50 * i, 37 * 2, 50, 37));
		}
		shared_ptr<FallAnimation> fall = make_shared<FallAnimation>();
		for (int i = 1; i < 3; i++)
		{
			fall->addFrame(IntRect(50 * i, 37 * 3, 50, 37));
		}
		shared_ptr<DoubleJumpAnimation> doubleJump = make_shared<DoubleJumpAnimation>();
		for (int i = 3; i < 7; i++)
		{
			doubleJump->addFrame(IntRect(50 * i, 37 * 2, 50, 37));
		}
		doubleJump->addFrame(IntRect(0, 37 * 3, 50, 37));
		shared_ptr<GetUpAnimation> getUp = make_shared<GetUpAnimation>();
		for (int i = 0; i < 7; i++)
		{
			getUp->addFrame(IntRect(50 * i, 37 * 12, 50, 37));
		}
		// Component that manages player animations
		auto anim = player->addComponent<AnimationMachineComponent>();
		anim->addAnimation("Idle", idle);
		anim->addAnimation("Run", run);
		anim->addAnimation("Jump", jump);
		anim->addAnimation("Fall", fall);
		anim->addAnimation("DoubleJump", doubleJump);
		anim->addAnimation("GetUp", getUp);
		anim->changeAnimation("GetUp");
		// Physics component
		auto physics = player->addComponent<PlayerPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 2.8f));
	}


	// Add physics colliders to level tiles.
	{
		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls)
		{
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(30.0f, 30.0f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			auto physics = e->addComponent<PhysicsComponent>(false, Vector2f(60.0f, 60.0f));
		}

		auto floor = ls::findTiles(ls::FLOOR);
		for (auto f : floor)
		{
			auto pos = ls::getTilePosition(f);
			pos += Vector2f(30.0f, 30.0f); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			auto physics = e->addComponent<PhysicsComponent>(false, Vector2f(60.0f, 60.0f));
		}
	}

	// Pause background
	pause_background_tex = make_shared<Texture>();
	pause_background_tex->loadFromFile("res/menus/pause_background.png");
	{
		pause_background = makePausedEntity();
		pause_background->setPosition(Vector2f(GAMEX / 2.0f, GAMEY / 2.0f));
		// Sprite
		auto sprite = pause_background->addComponent<SpriteComponent>();
		sprite->setTexure(pause_background_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 384, 224));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		float scaleX = (float)GAMEX / (sprite->getSprite().getTextureRect().width);
		float scaleY = (float)GAMEY / (sprite->getSprite().getTextureRect().height);
		sprite->getSprite().scale(scaleX, scaleY);
	}
	// Resume
	resume_tex = make_shared<Texture>();
	resume_tex->loadFromFile("res/menus/resume.png");
	{
		resume_btn = makePausedEntity();
		resume_btn->setPosition(Vector2f((GAMEX / 2.0f) - (resume_tex->getSize().x / 2.0f) - 90.0f, 400.0f));
		// sprite
		auto sprite = resume_btn->addComponent<SpriteComponent>();
		sprite->setTexure(resume_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 60, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// button component
		auto button = resume_btn->addComponent<ResumeButton>();
		button->setNormal(sf::IntRect(0, 0, 60, 15));
		button->setHovered(sf::IntRect(0, 15, 60, 15));
		button->setCurrentScene(this);
		buttonsForController.push_back(button);
	}
	// Return to menu
	returnToMenu_tex = make_shared<Texture>();
	returnToMenu_tex->loadFromFile("res/menus/returnToMenu.png");
	{
		returnToMenu_btn = makePausedEntity();
		returnToMenu_btn->setPosition(Vector2f((GAMEX / 2.0f) - (returnToMenu_tex->getSize().x / 2.0f) - 150.0f, 500.0f));
		// sprite
		auto sprite = returnToMenu_btn->addComponent<SpriteComponent>();
		sprite->setTexure(returnToMenu_tex);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 117, 15));
		sprite->getSprite().scale(3.0f, 3.0f);
		// button component
		auto button = returnToMenu_btn->addComponent<ChangeSceneButtonComponent>();
		button->setNormal(sf::IntRect(0, 0, 117, 15));
		button->setHovered(sf::IntRect(0, 15, 117, 15));
		button->setScene(&main_menu);
		buttonsForController.push_back(button);
	}

	// Serah get up dialogue
	{
		serah_dialogue = makeEntity();
		// Dialogue text component
		auto text = serah_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Serah: My head...Where... Where am I?.");
		text->setFunction([&]() {text->serahGetUpDialogueUpdate(); });
		text->setTextSize(30);
	}

	// Mysterious voice intro dialogue
	{
		intro_dialogue = makeEntity();
		intro_dialogue->addTag("Intro");
		intro_dialogue->setAlive(false);
		// Dialogue text component
		auto text = intro_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Mysterious voice: Serah... Erebus has transported you to the Netherworld during your nightmare.\nWe don't have much time left and he needs to be stopped. Cross the portal I have opened to get\nto his castle.");
		text->setFunction([&]() {text->outsideLevelDialogueUpdate(); });
	}

	// End fight dialogue
	{
		controls_dialogue = makeEntity();
		controls_dialogue->addTag("ControlsDialogue");
		controls_dialogue->setAlive(false);
		// Dialogue text component
		auto text = controls_dialogue->addComponent<DialogueBoxComponent>();
		// Some text changes based on what controller is connected
		string dialogue;
		string jump = Controller::JumpButton;
		// Dialogue when the joystick is connected
		if (sf::Joystick::isConnected(0))
		{
			string jumpButton = Conversor::ControllerButtonToString(*Controller::getActionControllerButton(jumpButton));
			// Dialogue
			dialogue = "Mysterious voice: Use the L.Stick to move," + jumpButton + " to perform a single jump\nand release it and press it again in the air to perform a double jump.";
		}
		// Dialogue when the mouse/keyboard are connected
		else
		{
			// Jump button
			string jumpButton;
			if (Controller::getActionKey(jump) != NULL)
			{
				jumpButton = Conversor::KeyboardKeyToString(*Controller::getActionKey(jump));
			}
			else
			{
				jumpButton = Conversor::MouseButtonToString(*Controller::getActionMouseButton(jump));
			}
			// Right movement button
			string right = Controller::MoveRightButton;
			string moveRight;
			if (Controller::getActionKey(right) != NULL)
			{
				moveRight = Conversor::KeyboardKeyToString(*Controller::getActionKey(right));
			}
			else
			{
				moveRight = Conversor::MouseButtonToString(*Controller::getActionMouseButton(right));
			}
			// Left movement button
			string left = Controller::MoveLeftButton;
			string moveLeft;
			if (Controller::getActionKey(left) != NULL)
			{
				moveLeft = Conversor::KeyboardKeyToString(*Controller::getActionKey(left));
			}
			else
			{
				moveLeft = Conversor::MouseButtonToString(*Controller::getActionMouseButton(left));
			}
			// Dialogue
			dialogue = "Mysterious voice: Use " + moveRight + " and " + moveLeft + " to move," + jumpButton + " to perform a single jump\nand release it and press it again in the air to perform a double jump.";
		}
		text->setCompleteText(dialogue);
		text->setFunction([&]() {text->moveWhileDialogueUpdate(); });
	}
	setLoaded(true);
}

void LevelOutside::Update(const double& dt)
{
	// Pause game
	if (Controller::isPressed(Controller::PauseButton))
	{
		// Enable cursor when game is paused
		Engine::GetWindow().setMouseCursorVisible(true);
		_paused = true;
		Audio::pauseMusic("mystic_music");
	}
	// Controller button navigation
	if (_paused)
	{
		ButtonComponent::ButtonNavigation(buttonsForController, buttonsCurrentIndex, dt);
	}
	else
	{
		// Music for this level
		Audio::playMusic("mystic_music");
	}
	// Change scene to next level
	if (ls::getTileAt(player->getPosition()) == ls::END)
	{
		Engine::ChangeScene(&levelOne);
	}

	Scene::Update(dt);
}

void LevelOutside::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void LevelOutside::UnLoad()
{
	buttonsForController.clear();
	player.reset();
	ls::unload();
	Scene::UnLoad();
}
