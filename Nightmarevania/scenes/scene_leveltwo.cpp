#include "scene_leveltwo.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../animation_states.h"
#include "../components/cmp_player_combat.h"
#include <iostream>
#include <LevelSystem.h>

using namespace std;
using namespace sf;

//sf::View followPC;
//sf::Vector2f curCentre; //debugging
/*sf::Vector2f screenSize;


float leftBoundary = 967.0f;
float rightBoundary = 2795.0f;
float topBoundary = -470.0f;
float bottomBoundary = 607.0f;
sf::Vector2f centrePoint;*/

void LevelTwo::Load()
{
	// Level file
	ls::loadLevelFile("res/levels/level_two.txt", 60.0f);
	// Tiles offset
	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));
	// Adventurer textures
	spriteSheet = make_shared<Texture>();
	spriteSheet->loadFromFile("res/img/adventurer.png");
	playerAnimations = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");

	// Player for levels 1 and 2
	{
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

	//MOVING CAMERA STUFF
	//for debugging:
	//printf("(%f, %f)\n", player->getPosition().x, player->getPosition().y);
	//curCentre = player->getPosition();

	screenSize = static_cast<sf::Vector2f>(Engine::GetWindow().getSize());
	centrePoint = sf::Vector2f(leftBoundary, bottomBoundary);//Engine::GetWindow().getView().getCenter();// = player->getPosition();

	setLoaded(true);
}

void LevelTwo::UnLoad()
{
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void LevelTwo::Update(const double& dt)
{
	//sf::Vector2f size = static_cast<sf::Vector2f>(Engine::GetWindow().getSize());
	
	
	

	
	if (player->getPosition().x > leftBoundary && player->getPosition().x < rightBoundary){
		centrePoint.x = player->getPosition().x;
	}

	if (player->getPosition().y > topBoundary && player->getPosition().y < bottomBoundary){
		centrePoint.y = player->getPosition().y;
	}

	/*if (curCentre != followPlayer.getCenter()) {
		printf("(%f, %f)\n", followPlayer.getCenter().x, followPlayer.getCenter().y);
		curCentre = followPlayer.getCenter();
	}*/


	followPlayer = sf::View(sf::FloatRect(0.f, 0.f, screenSize.x, screenSize.y));
	followPlayer.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	followPlayer.setCenter(centrePoint); //LEVEL2 - Follows player in all directions
	//Engine::GetWindow().setView(followPC);

	Scene::Update(dt);
}

void LevelTwo::Render()
{
	Engine::GetWindow().setView(followPlayer);
	ls::render(Engine::GetWindow()); //render the enviro tiles
	//TODO - render bg and fg seperately (in diff views)
	/* Something like:
	* sf::View background;
	* background = sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y)); //how to do parallax?
	* Engine::GetWindow().setView(background);
	* ls::render(Engine::GetWindow()); //prob not ls unless I can find a way to change tile input and spritesheet depending on fg or bg
	* Engine::GetWindow().setView(followPlayer);
	* ls::render(Engine::GetWindow());
	*/
	Scene::Render();
}