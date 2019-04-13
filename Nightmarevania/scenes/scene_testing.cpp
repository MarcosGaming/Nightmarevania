#include "scene_testing.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../components/cmp_player_combat.h"
#include "../animation_states.h"
#include "../game.h"
#include <iostream>
#include <LevelSystem.h>
#include <system_controller.h>
#include <system_resolution.h>
#include <system_sound.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
shared_ptr<Texture> playerAnimations;
shared_ptr<Texture> combatIcons;

static shared_ptr<Entity> pause_background;
static shared_ptr<Texture> pause_background_tex;

static shared_ptr<Entity> returnToMenu_btn;
static shared_ptr<Texture> returnToMenu_tex;

static shared_ptr<Entity> resume_btn;
static shared_ptr<Texture> resume_tex;

static vector<shared_ptr<ButtonComponent>> buttonsForController;
static int buttonsCurrentIndex;

void TestingScene::Load()
{
	// Stop menu music
	Audio::stopMusic("main_menu_music");
	// Controller starts at button 0
	buttonsCurrentIndex = 0;
	// The scene is not paused at the beginning
	_paused = false;
	// Disable cursor
	Engine::GetWindow().setMouseCursorVisible(false);
	// Level file
	ls::loadLevelFile("res/levels/level_test.txt", 60.0f);
	// Tiles offset
	auto ho = GAMEY - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));
	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	combatIcons = make_shared<Texture>();
	//spriteSheet->loadFromFile("res/img/adventurer.png");
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");
	combatIcons->loadFromFile("res/img/combat_icons.png");
	
	// Player for levels 1 and 2
	/*{
		player = makeEntity();
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		player->addTag("Player");
		// Sprite component
		auto sprite = player->addComponent<SpriteComponent>();
		sprite->setTexure(spriteSheet);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 50, 37));
		sprite->getSprite().scale(sf::Vector2f(3.0f, 3.0f));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		// Animations, each frame has a size of 50x37
		shared_ptr<IddleAnimation> iddle = make_shared<IddleAnimation>();
		for (int i = 0; i < 4; i++)
		{
			iddle->addFrame(IntRect(50 * i, 0, 50, 37));
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
		shared_ptr<DeathAnimationFall> deathFall = make_shared<DeathAnimationFall>();
		for (int i = 0; i < 3; i++)
		{
			deathFall->addFrame(IntRect(50 * i, 37 * 11, 50, 37));
		}
		shared_ptr<DeathAnimationGround> deathGround = make_shared<DeathAnimationGround>();
		for (int i = 2; i < 7; i++)
		{
			deathGround->addFrame(IntRect(50 * i, 37 * 11, 50, 37));
		}
		// Component that manages player animations
		auto anim = player->addComponent<AnimationMachineComponent>();
		anim->addAnimation("Iddle", iddle);
		anim->addAnimation("Run", run);
		anim->addAnimation("Jump", jump);
		anim->addAnimation("Fall", fall);
		anim->addAnimation("DoubleJump", doubleJump);
		anim->addAnimation("DeathFall", deathFall);
		anim->addAnimation("DeathGround", deathGround);
		anim->changeAnimation("Iddle");
		
		auto physics = player->addComponent<PlayerPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 2.8f));
	}*/

	// Player for level 3
	{
		player = makeEntity();
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		player->addTag("Player");
		// Sprite component
		auto sprite = player->addComponent<SpriteComponent>();
		sprite->setTexure(playerAnimations);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 50, 37));
		sprite->getSprite().scale(sf::Vector2f(3.0f, 3.0f));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
		// Combat component
		auto combat = player->addComponent<PlayerCombatComponent>();
		combat->setTexture(combatIcons);
		// Health bar sprite
		combat->getHealthBarSprite().setTextureRect((IntRect(0, 0, 100, 37)));
		combat->getHealthBarSprite().scale(Vector2f(5.0f, 5.0f));
		combat->getHealthBarSprite().setOrigin(Vector2f(50.0f, 18.0f));
		combat->getHealthBarSprite().setPosition(Vector2f(350.0f, 100.0f));
		for (int i = 0; i < 11; i++)
		{
			combat->addHealthBarIcon(IntRect(100 * i, 0, 100, 37));
		}
		// Circular attack sprite
		combat->getCircularAttackSprite().setTextureRect((IntRect(50 * 2, 37, 50, 37)));
		combat->getCircularAttackSprite().setOrigin(Vector2f(25.0f, 18.0f));
		combat->getCircularAttackSprite().setPosition(Vector2f(240.0f, 160.0f));
		for (int i = 2; i < 4; i++)
		{
			combat->addCircularAttackIcon(IntRect(50 * i, 37, 50, 37));
		}
		// Up attack sprite
		combat->getUpAttackSprite().setTextureRect((IntRect(0, 37, 50, 37)));
		combat->getUpAttackSprite().setOrigin(Vector2f(25.0f, 18.0f));
		combat->getUpAttackSprite().setPosition(Vector2f(300.0f, 160.0f));
		for (int i = 0; i < 2; i++)
		{
			combat->addUpAttackIcon(IntRect(50 * i, 37, 50, 37));
		}
		// Down attack sprite
		combat->getDownAttackSprite().setTextureRect((IntRect(50 * 4, 37, 50, 37)));
		combat->getDownAttackSprite().setOrigin(Vector2f(25.0f, 18.0f));
		combat->getDownAttackSprite().setPosition(Vector2f(360.0f, 160.0f));
		for (int i = 4; i < 6; i++)
		{
			combat->addDownAttackIcon(IntRect(50 * i, 37, 50, 37));
		}
		// Defend sprite
		combat->getDefendSprite().setTextureRect((IntRect(50 * 8, 37, 50, 37)));
		combat->getDefendSprite().setOrigin(Vector2f(25.0f, 18.0f));
		combat->getDefendSprite().setPosition(Vector2f(420.0f, 160.0f));
		for (int i = 8; i < 10; i++)
		{
			combat->addDefendIcon(IntRect(50 * i, 37, 50, 37));
		}
		// Animations, each frame has a size of 50x37
		shared_ptr<IddleAnimation> iddle = make_shared<IddleAnimation>();
		for (int i = 0; i < 4; i++)
		{
			iddle->addFrame(IntRect(50 * i, 0, 50, 37));
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
		shared_ptr<DeathAnimationFall> deathFall = make_shared<DeathAnimationFall>();
		for (int i = 0; i < 3; i++)
		{
			deathFall->addFrame(IntRect(50 * i, 37 * 16, 50, 37));
		}
		shared_ptr<DeathAnimationGround> deathGround = make_shared<DeathAnimationGround>();
		for (int i = 2; i < 7; i++)
		{
			deathGround->addFrame(IntRect(50 * i, 37 * 16, 50, 37));
		}
		shared_ptr<GroundAttackAnimation> groundAttack = make_shared<GroundAttackAnimation>();
		for (int i = 1; i < 7; i++)
		{
			groundAttack->addFrame(IntRect(50 * i, 37 * 6, 50, 37));
		}
		for (int i = 0; i < 4; i++)
		{
			groundAttack->addFrame(IntRect(50 * i, 37 * 7, 50, 37));
		}
		shared_ptr<CircularAttackAnimation> circularAttack = make_shared<CircularAttackAnimation>();
		for (int i = 4; i < 7; i++)
		{
			circularAttack->addFrame(IntRect(50 * i, 37 * 7, 50, 37));
		}
		for (int i = 0; i < 3; i++)
		{
			circularAttack->addFrame(IntRect(50 * i, 37 * 8, 50, 37));
		}
		shared_ptr<AirAttackAnimation> airAttack = make_shared<AirAttackAnimation>();
		for (int i = 4; i < 7; i++)
		{
			airAttack->addFrame(IntRect(50 * i, 37 * 13, 50, 37));
		}
		for (int i = 0; i < 2; i++)
		{
			airAttack->addFrame(IntRect(50 * i, 37 * 14, 50, 37));
		}
		shared_ptr<UpAttackAnimation> upAttack = make_shared<UpAttackAnimation>();
		for (int i = 1; i < 5; i++)
		{
			upAttack->addFrame(IntRect(50 * i, 37 * 14, 50, 37));
		}
		shared_ptr<DownAttackAnimation> downAttack = make_shared<DownAttackAnimation>();
		for (int i = 5; i < 7; i++)
		{
			downAttack->addFrame(IntRect(50 * i, 37 * 14, 50, 37));
		}
		shared_ptr<SmasherDownAttackAnimation> smasherDownAttack = make_shared<SmasherDownAttackAnimation>();
		for (int i = 0; i < 4; i++)
		{
			smasherDownAttack->addFrame(IntRect(50 * i, 37 * 15, 50, 37));
		}
		shared_ptr<DefendingAnimation> defending = make_shared<DefendingAnimation>();
		for (int i = 3; i < 7; i++)
		{
			defending->addFrame(IntRect(50 * i, 37 * 8, 50, 37));
		}
		shared_ptr<HurtAnimation> hurt = make_shared<HurtAnimation>();
		for (int i = 0; i < 2; i++)
		{
			hurt->addFrame(IntRect(50 * i, 37 * 17, 50, 37));
		}
		// Component that manages player animations
		auto anim = player->addComponent<AnimationMachineComponent>();
		anim->addAnimation("Iddle", iddle);
		anim->addAnimation("Run", run);
		anim->addAnimation("Jump", jump);
		anim->addAnimation("Fall", fall);
		anim->addAnimation("DoubleJump", doubleJump);
		anim->addAnimation("DeathFall", deathFall);
		anim->addAnimation("DeathGround", deathGround);
		anim->addAnimation("GroundAttack", groundAttack);
		anim->addAnimation("CircularAttack", circularAttack);
		anim->addAnimation("AirAttack", airAttack);
		anim->addAnimation("UpAttack", upAttack);
		anim->addAnimation("DownAttack", downAttack);
		anim->addAnimation("SmasherDownAttack", smasherDownAttack);
		anim->addAnimation("Defending", defending);
		anim->addAnimation("Hurt", hurt);
		anim->changeAnimation("Iddle");
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

	setLoaded(true);
}


void TestingScene::Update(const double& dt)
{
	// Pause game
	if (Controller::isPressed(Controller::PauseButton))
	{
		// Enable cursor when game is paused
		Engine::GetWindow().setMouseCursorVisible(true);
		_paused = true;
		// Pause music
		Audio::pauseMusic("level_3_music");
	}
	if (_paused)
	{
		ButtonComponent::ButtonNavigation(buttonsForController, buttonsCurrentIndex, dt);
	}
	else
	{
		// Level 3 music
		Audio::playMusic("level_3_music");
	}
	Scene::Update(dt);
}

void TestingScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void TestingScene::UnLoad()
{
	Audio::stopMusic("level_3_music");
	buttonsForController.clear();
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

