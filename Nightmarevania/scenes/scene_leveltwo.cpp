#include "scene_leveltwo.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../components/cmp_player_combat.h"
#include "../components/cmp_door.h"
#include "../components/cmp_key.h"
#include "../animation_states.h"
#include "../game.h"
#include <iostream>
#include <LevelSystem.h>
#include <system_controller.h>
#include <system_resolution.h>
#include "../components/cmp_door.h"
#include "../components/cmp_key.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> pause_background;
static shared_ptr<Texture> pause_background_tex;

static shared_ptr<Entity> returnToMenu_btn;
static shared_ptr<Texture> returnToMenu_tex;

static shared_ptr<Entity> resume_btn;
static shared_ptr<Texture> resume_tex;

static vector<shared_ptr<ButtonComponent>> buttonsForController;
static int buttonsCurrentIndex;

sf::Vector2f curCentre; //debugging

void LevelTwo::Load()
{
	// Controller starts at button 0
	buttonsCurrentIndex = 0;
	// The scene is not paused at the beginning
	_paused = false;
	// Disable cursor
	Engine::GetWindow().setMouseCursorVisible(false);
	// Level file
	ls::loadLevelFile("res/levels/level_two.txt", 60.0f);
	// Tiles offset
	auto ho = GAMEY - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));

	//DOOR
	shared_ptr<Entity> door;
	if (ls::doesTileExist(ls::DOOR)) {
		door = makeEntity();
		auto doorCmp = door->addComponent<DoorComponent>(true, ls::getTilePosition(ls::findTiles(ls::DOOR)[0]));
		//false for L1, but starts as true for L2 and stays true in L3
		auto doorSprite = door->addComponent<SpriteComponent>();
		doorSprite->setTexure(doorCmp->getTexture());
		doorSprite->getSprite().setOrigin(doorSprite->getSprite().getTextureRect().width * 0.5f, 0.0f);
		doorSprite->getSprite().setTextureRect(doorCmp->getRect());
	}

	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer_sword.png");
	spriteSheet = make_shared<Texture>();
	spriteSheet->loadFromFile("res/img/adventurer.png");

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
		
		//KEY - level 2 only
		if (ls::doesTileExist(ls::DOOR)) {
			door->GetCompatibleComponent<DoorComponent>()[0]->setPlayer(player);
		}

		//if (keyExists) {
		if (ls::doesTileExist(ls::KEY)) {
			auto key = player->addComponent<KeyComponent>(false, ls::getTilePosition(ls::findTiles(ls::KEY)[0]));
		}

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


	//MOVING CAMERA STUFF
	screenSize = static_cast<sf::Vector2f>(Engine::GetWindow().getSize());
	curCentre = player->getPosition();
	centrePoint = sf::Vector2f(leftBoundary, bottomBoundary);

	setLoaded(true);
}

void LevelTwo::Update(const double& dt)
{

	//sf::Vector2f size = static_cast<sf::Vector2f>(Engine::GetWindow().getSize());
	
	if (ls::getTileAt(player->getPosition()) == ls::KEY) { // && keyExists
		player->GetCompatibleComponent<KeyComponent>()[0]->setHeld(true);
	}
	

	if (ls::getTileAt(player->getPosition()) == ls::KEY) {
		player->GetCompatibleComponent<KeyComponent>()[0]->setHeld(true);
	}

	// Pause game
	if (Controller::isPressed(Controller::PauseButton))
	{
		// Enable cursor when game is paused
		Engine::GetWindow().setMouseCursorVisible(true);
		_paused = true;
	}
	if (_paused)
	{
		ButtonComponent::ButtonNavigation(buttonsForController, buttonsCurrentIndex, dt);
	}

	if (player->getPosition().x > leftBoundary && player->getPosition().x < rightBoundary) {
		centrePoint.x = player->getPosition().x;
	}

	if (player->getPosition().y > topBoundary && player->getPosition().y < bottomBoundary) {
		centrePoint.y = player->getPosition().y;
	}

	//debug:
	if (player->getPosition() != curCentre) {
		printf("(%f,%f)\n", player->getPosition().x, player->getPosition().y);
		curCentre = player->getPosition();
	}

	followPlayer = sf::View(sf::FloatRect(0.f, 0.f, screenSize.x, screenSize.y));
	followPlayer.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	followPlayer.setCenter(centrePoint);

	if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&levelTwo);
	}

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

void LevelTwo::UnLoad()
{
	buttonsForController.clear();
	player.reset();
	ls::unload();
	Scene::UnLoad();
}