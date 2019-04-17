#include "scene_levelthree.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../components/cmp_player_combat.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_physics .h"
#include "../components/cmp_decision_tree.h"
#include "../components/cmp_state_machine.h"
#include "../components/cmp_hurt.h"
#include "../components/cmp_text.h"
#include "../animation_states.h"
#include "../enemy_states.h"
#include "../enemy_decisions.h"
#include "../game.h"
#include <iostream>
#include <LevelSystem.h>
#include <system_controller.h>
#include <system_resolution.h>
#include <system_sound.h>
#include <system_saving.h>
#include <conversor.h>

using namespace std;
using namespace sf;

shared_ptr<Entity> boss;
static shared_ptr<Texture> boss_spritesheet;
static shared_ptr<Texture> boss_healthbar;
static shared_ptr<Texture> boss_foreground;

static shared_ptr<Entity> pause_background;
static shared_ptr<Texture> pause_background_tex;

static shared_ptr<Entity> returnToMenu_btn;
static shared_ptr<Texture> returnToMenu_tex;

static shared_ptr<Entity> resume_btn;
static shared_ptr<Texture> resume_tex;

static shared_ptr<Entity> sword_alone;
static shared_ptr<Texture> sword_alone_tex;

static vector<shared_ptr<ButtonComponent>> buttonsForController;
static int buttonsCurrentIndex;

static shared_ptr<Entity> sword_dialogue;
static shared_ptr<Entity> begin_fight_dialogue;
static shared_ptr<Entity> end_fight_dialogue;

static bool begin_fight_dialogue_musicOn;

void LevelThree::Load()
{
	// Save this level as the last one played
	Saving::saveLevel("4");
	// Stops music from main menu
	Audio::stopMusic("main_menu_music");
	// Controller starts at button 0
	buttonsCurrentIndex = 0;
	// The scene is not paused at the beginning
	_paused = false;
	// Disable cursor
	Engine::GetWindow().setMouseCursorVisible(false);
	// Level file
	ls::loadLevelFile("res/levels/level_three.txt", 60.0f);
	// Tiles offset
	auto ho = GAMEY - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));
	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	combatIcons = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");
	combatIcons->loadFromFile("res/img/combat_icons.png");
	// Boss textures
	boss_spritesheet = make_shared<Texture>();
	boss_spritesheet->loadFromFile("res/img/skeleton_boss.png");
	boss_healthbar = make_shared<Texture>();
	boss_healthbar->loadFromFile("res/img/boss_healthbar.png");
	boss_foreground = make_shared<Texture>();
	boss_foreground->loadFromFile("res/img/boss_foreground.png");
	// Sword alone texture
	sword_alone_tex = make_shared<Texture>();
	sword_alone_tex->loadFromFile("res/img/sword_alone.png");
	// Music does not start playing until the second dialogue has started
	begin_fight_dialogue_musicOn = false;

	// Sword
	{
		sword_alone = makeEntity();
		sword_alone->setPosition(Vector2f(GAMEX * 0.45f, 540.0f));
		auto sprite = sword_alone->addComponent<SpriteComponent>();
		sprite->setTexure(sword_alone_tex);
		sprite->getSprite().scale(Vector2f(2.0f, 2.0f));
	}

	// Player
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
		shared_ptr<IdleAnimation> iddle = make_shared<IdleAnimation>();
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

	// Boss for level 3
	{
		boss = makeEntity();
		boss->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
		boss->addTag("Boss");
		// Sprite component
		auto sprite = boss->addComponent<SpriteComponent>();
		sprite->setTexure(boss_spritesheet);
		sprite->getSprite().setTextureRect(IntRect(128 * 3, 64 * 6, 128, 64));
		sprite->getSprite().scale(sf::Vector2f(3.0f, 3.0f));
		sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.4f, sprite->getSprite().getTextureRect().height * 0.56f);
		sprite->getSprite().setPosition(boss->getPosition());
		// Animations, each frame has a size of 128x64
		shared_ptr<SkeletonIdleAnimation> iddle = make_shared<SkeletonIdleAnimation>();
		for (int i = 0; i < 7; i++)
		{
		iddle->addFrame(IntRect(128 * i, 0, 128, 64));
		}
		shared_ptr<SkeletonSlowWalkAnimation> slowWalk = make_shared<SkeletonSlowWalkAnimation>();
		for (int i = 3; i < 7; i++)
		{
			slowWalk->addFrame(IntRect(128 * i, 64 * 3, 128, 64));
		}
		for (int i = 0; i < 7; i++)
		{
			slowWalk->addFrame(IntRect(128 * i, 64 * 4, 128, 64));
		}
		shared_ptr<SkeletonFastWalkAnimation> fastWalk = make_shared<SkeletonFastWalkAnimation>();
		for (int i = 3; i < 7; i++)
		{
			fastWalk->addFrame(IntRect(128 * i, 64 * 3, 128, 64));
		}
		for (int i = 0; i < 7; i++)
		{
			fastWalk->addFrame(IntRect(128 * i, 64 * 4, 128, 64));
		}
		shared_ptr<SkeletonAttackAnimation> attack = make_shared<SkeletonAttackAnimation>();
		for (int i = 0; i < 7; i++)
		{
			attack->addFrame(IntRect(128 * i, 64, 128, 64));
		}
		for (int i = 0; i < 7; i++)
		{
			attack->addFrame(IntRect(128 * i, 64 * 2, 128, 64));
		}
		for (int i = 0; i < 3; i++)
		{
			attack->addFrame(IntRect(128 * i, 64 * 3, 128, 64));
		}
		shared_ptr<SkeletonHurtAnimation> hurt = make_shared<SkeletonHurtAnimation>();
		hurt->addFrame(IntRect(128 * 6, 64 * 4, 128, 64));
		for (int i = 4; i < 6; i++)
		{
			hurt->addFrame(IntRect(128 * i, 64 * 6, 128, 64));
		}
		shared_ptr<SkeletonDeathAnimation> death = make_shared<SkeletonDeathAnimation>();
		death->addFrame(IntRect(128 * 6, 64 * 4, 128, 64));
		for (int i = 0; i < 7; i++)
		{
			death->addFrame(IntRect(128 * i, 64 * 5, 128, 64));
		}
		for (int i = 0; i < 4; i++)
		{
			death->addFrame(IntRect(128 * i, 64 * 6, 128, 64));
		}
		shared_ptr<SkeletonReviveAnimation> revive = make_shared<SkeletonReviveAnimation>();
		for (int i = 3; i >= 0; i--)
		{
			revive->addFrame(IntRect(128 * i, 64 * 6, 128, 64));
		}
		for (int i = 6; i >= 0; i--)
		{
			revive->addFrame(IntRect(128 * i, 64 * 5, 128, 64));
		}
		revive->addFrame(IntRect(128 * 6, 64 * 4, 128, 64));
		// Animation machine component
		auto anim = boss->addComponent<AnimationMachineComponent>();
		anim->addAnimation("Iddle", iddle);
		anim->addAnimation("WalkSlow", slowWalk);
		anim->addAnimation("WalkFast", fastWalk);
		anim->addAnimation("Attack", attack);
		anim->addAnimation("Hurt", hurt);
		anim->addAnimation("Death", death);
		anim->addAnimation("Revive", revive);
		anim->changeAnimation("Revive");
		// Boss AI component
		auto AI = boss->addComponent<BossAIComponent>();
		AI->setHealthBarTexture(boss_healthbar);
		AI->getHealthBarSprite().setOrigin(AI->getHealthBarSprite().getTextureRect().width * 0.5f, AI->getHealthBarSprite().getTextureRect().height * 0.5f);
		AI->getHealthBarSprite().scale(Vector2f(4.0f, 1.0f));
		AI->getHealthBarSprite().setPosition(Vector2f(GAMEX / 2.0f, GAMEY - 25.0f));
		AI->setForegroundTexture(boss_foreground);
		AI->getForegroundSprite().setOrigin(AI->getForegroundSprite().getTextureRect().width * 0.5f, AI->getForegroundSprite().getTextureRect().height * 0.5f);
		AI->getForegroundSprite().scale(Vector2f(4.0f, 1.0f));
		AI->getForegroundSprite().setPosition(Vector2f(GAMEX / 2.0f, GAMEY - 25.0f));
		// State machine component
		auto sm = boss->addComponent<StateMachineComponent>();
		sm->addState("Stationary", make_shared<StationaryState>());
		sm->addState("SeekSlow", make_shared<SeekSlowState>(boss, player));
		sm->addState("SeekFast", make_shared<SeekFastState>(boss, player));
		sm->addState("Death", make_shared<DeathState>());
		sm->addState("Attack", make_shared<AttackState>());
		sm->addState("Revive", make_shared<ReviveState>());
		sm->changeState("Revive");
		// Decisions subtree 1 (Health > 50)
		auto weightedBinaryDecision1Tree1 = make_shared<WeightedBinaryDecision>(50, make_shared<StationaryDecision>(), make_shared<AttackDecision>());
		auto weightedBinaryDecision2Tree1 = make_shared<WeightedBinaryDecision>(80, make_shared<SeekSlowDecision>(), make_shared<StationaryDecision>());
		auto distanceDecisionTree1 = make_shared<DistanceDecision>(player, 200.0f, weightedBinaryDecision1Tree1, weightedBinaryDecision2Tree1);
		// Decisions subtree 2 (25 < Health < 50)
		auto weightedBinaryDecision1Tree2 = make_shared<WeightedBinaryDecision>(40, make_shared<StationaryDecision>(), make_shared<AttackDecision>());
		auto weightedBinaryDecision2Tree2 = make_shared<WeightedBinaryDecision>(80, make_shared<SeekFastDecision>(), make_shared<StationaryDecision>());
		auto distanceDecisionTree2 = make_shared<DistanceDecision>(player, 200.0f, weightedBinaryDecision1Tree2, weightedBinaryDecision2Tree2);
		// Decisions subtree 3 (Health < 25)
		auto weightedBinaryDecision1Tree3 = make_shared<WeightedBinaryDecision>(30, make_shared<StationaryDecision>(), make_shared<AttackDecision>());
		auto distanceDecisionTree3 = make_shared<DistanceDecision>(player, 200.0f, weightedBinaryDecision1Tree3, make_shared<SeekFastDecision>());
		// Complete tree
		auto lowHealthDecision = make_shared<HealthDecision>(boss, 100, distanceDecisionTree3, make_shared<DeathDecision>());
		auto mediumHealthDecision = make_shared<HealthDecision>(boss, 65, distanceDecisionTree2, lowHealthDecision);
		auto highHealthDecision = make_shared<HealthDecision>(boss, 30, distanceDecisionTree1, mediumHealthDecision);
		// Decision tree component
		boss->addComponent<DecisionTreeComponent>(highHealthDecision);
		// Physics component
		auto physics = boss->addComponent<EnemyPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width *0.5f, sprite->getSprite().getTextureRect().height * 2.3f));
		// Boss is not update until the dialogues have finished
		boss->setAlive(false);
		boss->setDeath(true);
	}

	{
		// Player Hurt Component
		player->addComponent<BossHurtComponent>();
		// Boss Hurt Component
		boss->addComponent<PlayerHurtComponent>();
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

	// Sword of dawn dialogue
	{
		sword_dialogue = makeEntity();
		// Dialogue text component
		auto text = sword_dialogue->addComponent<DialogueBoxComponent>();
		// Some text changes based on what controller is connected
		string dialogue;
		string attack = Controller::AttackButton;
		string defend = Controller::DefendButton;
		// Dialogue when the joystick is connected
		if (sf::Joystick::isConnected(0))
		{
			string attackButton = Conversor::ControllerButtonToString(*Controller::getActionControllerButton(attack));
			string defendButton = Conversor::ControllerButtonToString(*Controller::getActionControllerButton(defend));
			// Dialogue
			dialogue = "Mysterious voice: What you picked up in the previous room was the Sword of Dawn, with it, you will be able to\nshed light on this darkness Serah. Press " 
				+ defendButton + " to defend yourself, maintain press " + attackButton + " \nto attack and while "
				"pressing it, use the L.Stick to perform special attacks.";
		}
		// Dialogue when the mouse/keyboard are connected
		else
		{
			// Attack button
			string attack = Controller::AttackButton;
			string attackButton;
			if (Controller::getActionKey(attack) != NULL)
			{
				attackButton = Conversor::KeyboardKeyToString(*Controller::getActionKey(attack));
			}
			else
			{
				attackButton = Conversor::MouseButtonToString(*Controller::getActionMouseButton(attack));
			}
			// Defend button
			string defend = Controller::DefendButton;
			string defendButton;
			if (Controller::getActionKey(defend) != NULL)
			{
				defendButton = Conversor::KeyboardKeyToString(*Controller::getActionKey(defend));
			}
			else
			{
				defendButton = Conversor::MouseButtonToString(*Controller::getActionMouseButton(defend));
			}
			// Right attack/movement button
			string right = Controller::MoveRightButton;
			string rightAttack;
			if (Controller::getActionKey(right) != NULL)
			{
				rightAttack = Conversor::KeyboardKeyToString(*Controller::getActionKey(right));
			}
			else
			{
				rightAttack = Conversor::MouseButtonToString(*Controller::getActionMouseButton(right));
			}
			// Left attack/movement button
			string left = Controller::MoveLeftButton;
			string leftAttack;
			if (Controller::getActionKey(left) != NULL)
			{
				leftAttack = Conversor::KeyboardKeyToString(*Controller::getActionKey(left));
			}
			else
			{
				leftAttack = Conversor::MouseButtonToString(*Controller::getActionMouseButton(left));
			}
			// Down attack button
			string down = Controller::DownAttackButton;
			string downAttack;
			if (Controller::getActionKey(down) != NULL)
			{
				downAttack = Conversor::KeyboardKeyToString(*Controller::getActionKey(down));
			}
			else
			{
				downAttack = Conversor::MouseButtonToString(*Controller::getActionMouseButton(down));
			}
			// Up attack button
			string up = Controller::UpAttackButton;
			string upAttack;
			if (Controller::getActionKey(up) != NULL)
			{
				upAttack = Conversor::KeyboardKeyToString(*Controller::getActionKey(up));
			}
			else
			{
				upAttack = Conversor::MouseButtonToString(*Controller::getActionMouseButton(up));
			}
			// Dialogue
			dialogue = "Mysterious voice: What you picked up in the previous room was the Sword of Dawn, with it, you will be able to\nshed light on this darkness Serah. Press " 
				+ defendButton +" to defend yourself, maintain press " + attackButton + " \nto attack and while "
				"pressing it, use " + rightAttack + ", " + leftAttack + ", " + upAttack + ", " + downAttack + " to perform special attacks.";
		}
		text->setCompleteText(dialogue);
		text->setFunction([&]() {text->swordDialogueUpdate();});
	}
	// Begin fight dialogue
	{
		begin_fight_dialogue = makeEntity();
		begin_fight_dialogue->addTag("BeginBattleText");
		begin_fight_dialogue->setAlive(false);
		// Dialogue text component
		auto text = begin_fight_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Erebus: Hahahaha! The new guardian is just a young, little girl.\nYou won't be able to stop me as your ancestors did!\nI, Erebus, will obliterate you from existance\nand then your world will follow you!");
		text->setFunction([&]() {text->beginBattleDialogueUpdate(begin_fight_dialogue_musicOn); });
	}
	// End fight dialogue
	{
		end_fight_dialogue = makeEntity();
		end_fight_dialogue->setAlive(false);
		// Dialogue text component
		auto text = end_fight_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Erebus: How... How can this be? I have been defeated by you? Next time...\nNext time I will make sure to end with your lineage once and for all.");
		text->setFunction([&]() {text->endBattleDialogueUpdate(); });
	}

	setLoaded(true);
}

void LevelThree::Update(const double& dt)
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
	else if(begin_fight_dialogue_musicOn)
	{
		Audio::playMusic("level_3_music");
	}
	if (!boss->isAlive() && !begin_fight_dialogue->isAlive() && !sword_dialogue->isAlive())
	{
		end_fight_dialogue->setAlive(true);
	}
	Scene::Update(dt);
}

void LevelThree::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void LevelThree::UnLoad()
{
	Audio::stopMusic("level_3_music");
	Audio::stopMusic("mystic_music");
	buttonsForController.clear();
	player.reset();
	boss.reset();
	ls::unload();
	Scene::UnLoad();
}

