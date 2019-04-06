#include "scene_levelthree.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../animation_states.h"
#include "../components/cmp_player_combat.h"
#include <iostream>
#include <LevelSystem.h>


using namespace std;
using namespace sf;

//static shared_ptr<Entity> player;
//shared_ptr<Texture> playerAnimations;
//shared_ptr<Texture> combatIcons;

void LevelThree::Load()
{
	// Level file
	ls::loadLevelFile("res/levels/level_three.txt", 60.0f);
	// Tiles offset
	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));
	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");
	combatIcons = make_shared<Texture>();
	combatIcons->loadFromFile("res/img/combat_icons.png");

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

	setLoaded(true);
}

void LevelThree::UnLoad()
{
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void LevelThree::Update(const double& dt)
{
	//TODO Do we need to zoom the camera out and have a bigger room for fighting?

	Scene::Update(dt);
}

void LevelThree::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}