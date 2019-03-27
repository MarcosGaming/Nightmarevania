#include "scene_testing.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../animation_states.h"
#include "../components/cmp_player_combat.h"
#include <iostream>
#include <LevelSystem.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
shared_ptr<Texture> playerAnimations;
shared_ptr<Texture> combatIcons;

void TestingScene::Load()
{
	// Level file
	ls::loadLevelFile("res/levels/level_test.txt", 60.0f);
	// Tiles offset
	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));
	// Adventurer texture
	playerAnimations = make_shared<Texture>();
	//spriteSheet->loadFromFile("res/img/adventurer.png");
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");
	
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
		player->addComponent<PlayerCombatComponent>();
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

	setLoaded(true);
}

void TestingScene::UnLoad()
{
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void TestingScene::Update(const double& dt)
{
	Scene::Update(dt);
}

void TestingScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}