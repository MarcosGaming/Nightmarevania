#include "scene_levelone.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../components/cmp_player_combat.h"
#include "../animation_states.h"
#include "../components/cmp_door.h"
#include "../components/cmp_key.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_ai_steering.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <iostream>
#include <LevelSystem.h>
#include <system_controller.h>
#include <system_resolution.h>
#include <system_sound.h>
#include <system_saving.h>
#include <system_physics.h>

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

static shared_ptr<Entity> run_dialogue;

void LevelOne::Load()
{
	// Need to initialise phyiscs to reset the world otherwise the player dead body will block the path
	Physics::initialise();
	// Save this level as the last one played
	Saving::saveLevel("1");
	// Stop music from main menu
	Audio::stopMusic("main_menu_music");
	// Controller starts at button 0
	buttonsCurrentIndex = 0;
	// The scene is not paused at the beginning
	_paused = false;
	// Disable cursor
	Engine::GetWindow().setMouseCursorVisible(false);

	// Background
	{
		background_tex = make_shared<Texture>();
		background_image = make_shared<Sprite>();
		background_tex->loadFromFile("res/img/level_one.png");
		float scaleX = (float)GAMEX / (background_tex->getSize().x);
		float scaleY = (float)GAMEY / (background_tex->getSize().y);
		background_image->scale(scaleX*17.0f, scaleY);
		background_image->setTexture(*background_tex);
	}

	// Level file
	ls::loadLevelFile("res/levels/level_one.txt", 60.0f);
	// Tiles offset
	auto ho = GAMEY - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));

	// Door
	if (ls::doesTileExist(ls::DOOR)) {
		door = makeEntity();
		auto doorCmp = door->addComponent<DoorComponent>(true, ls::getTilePosition(ls::findTiles(ls::DOOR)[0]));
		auto doorSprite = door->addComponent<SpriteComponent>();
		doorSprite->setTexure(doorCmp->getTexture());
		doorSprite->getSprite().setOrigin(doorSprite->getSprite().getTextureRect().width * 0.5f, 0.0f);
		doorSprite->getSprite().setTextureRect(doorCmp->getRect());
	}

	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer.png");

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
		anim->addAnimation("Idle", idle);
		anim->addAnimation("Run", run);
		anim->addAnimation("Jump", jump);
		anim->addAnimation("Fall", fall);
		anim->addAnimation("DoubleJump", doubleJump);
		anim->addAnimation("DeathFall", deathFall);
		anim->addAnimation("DeathGround", deathGround);
		anim->changeAnimation("Idle");
		// Physics component
		auto physics = player->addComponent<PlayerPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 2.8f));
		physics->setRestitution(0.0f);

		// Door
		if (ls::doesTileExist(ls::DOOR)) {
			door->GetCompatibleComponent<DoorComponent>()[0]->setPlayer(player);
		}

		// Key
		if (ls::doesTileExist(ls::KEY)) {
			auto key = player->addComponent<NormalKeyComponent>(false, ls::getTilePosition(ls::findTiles(ls::KEY)[0]));
		}
	}

	// Ghost
	{
		ghost = makeEntity();
		ghost->addTag("Ghost");
		ghost->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
		// AI component
		auto AIcmp = ghost->addComponent<GhostAIComponent>();
		// Sprite component
		auto sprite = ghost->addComponent<SpriteComponent>();
		shared_ptr<Texture> ghostSprites = make_shared<Texture>();
		ghostSprites->loadFromFile("res/img/ghost.png");
		sprite->setTexure(ghostSprites);
		sprite->getSprite().setTextureRect(IntRect(0, 0, 71, 58));
		sprite->getSprite().scale(sf::Vector2f(5.0f, 5.0f));
		sprite->getSprite().setOrigin((float)sprite->getSprite().getTextureRect().width * 0.5f, (float)sprite->getSprite().getTextureRect().height);
		// Steering component
		auto steering = ghost->addComponent<AISteeringComponent>(player.get(), 190.0f, 1000.0f);
		// Animations
		shared_ptr<GhostTakeOffAnimation> ghostTakeOff = make_shared<GhostTakeOffAnimation>();
		for (int i = 1; i < 7; i++)
		{
			ghostTakeOff->addFrame(IntRect(71 * i, 0, 71, 58));
		}
		shared_ptr<GhostFlyingAnimation> ghostFly = make_shared<GhostFlyingAnimation>();
		ghostFly->addFrame(IntRect(71 * 5, 0, 71, 58));
		ghostFly->addFrame(IntRect(71 * 6, 0, 71, 58));
		shared_ptr<GhostIdleAnimation> ghostIdle = make_shared<GhostIdleAnimation>();
		ghostIdle->addFrame(IntRect(71 * 0, 0, 71, 58));
		ghostIdle->addFrame(IntRect(71 * 1, 0, 71, 58));
		// Animation machine component
		auto animations = ghost->addComponent<AnimationMachineComponent>();
		animations->addAnimation("GhostIdle", ghostIdle);
		animations->addAnimation("GhostTakeOff", ghostTakeOff);
		animations->addAnimation("GhostFly", ghostFly);
		animations->changeAnimation("GhostIdle");
		// Ghost is initially not visible and updatable
		ghost->setUpdatable(false);
		ghost->setVisible(false);
	}

	// Add physics colliders to level tiles.
	{
		vector<Vector2f> checkedTiles;

		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls)
		{
			auto pos = ls::getTilePosition(w);
			int nextTile = 1;
			float width = 60.0f;
			float height = 60.0f;

			if (find(checkedTiles.begin(), checkedTiles.end(), pos) == checkedTiles.end()) {
				//not found so collider not added yet
				checkedTiles.push_back(pos);
				bool end = false;
				while (!end) {
					Vector2f nextPos = Vector2f(pos.x + (60 * nextTile), pos.y);
					size_t mapWidth = ls::getWidth(); //for debugging
					if (nextPos.x > (ls::getWidth()*60.0f) || ls::getTileAt(nextPos) != ls::WALL || find(checkedTiles.begin(), checkedTiles.end(), nextPos) != checkedTiles.end()) {
						end = true;
					}
					else {
						//then the next tile is the same so
						nextTile++;
						width += 60.0f;
						checkedTiles.push_back(nextPos);
					}

				}
				pos += Vector2f(width / 2, height / 2); //offset to center
				auto e = makeEntity();
				e->setPosition(pos);
				auto physics = e->addComponent<PhysicsComponent>(false, Vector2f(width, height));
				physics->setRestitution(0.0f);
			}
		}

		auto floor = ls::findTiles(ls::FLOOR);
		for (auto f : floor)
		{
			auto pos = ls::getTilePosition(f);
			int nextTile = 1;
			float width = 60.0f;
			float height = 60.0f;

			if (find(checkedTiles.begin(), checkedTiles.end(), pos) == checkedTiles.end()) {
				//not found so collider not added yet
				checkedTiles.push_back(pos);
				bool end = false;
				while (!end) {
					Vector2f nextPos = Vector2f(pos.x + (60 * nextTile), pos.y);
					size_t mapWidth = ls::getWidth(); //for debugging
					if (nextPos.x > (ls::getWidth()*60.0f) || ls::getTileAt(nextPos) != ls::FLOOR || find(checkedTiles.begin(), checkedTiles.end(), nextPos) != checkedTiles.end()) {
						end = true;
					}
					else {
						//then the next tile is the same so
						nextTile++;
						width += 60.0f;
						checkedTiles.push_back(nextPos);
					}

				}
				pos += Vector2f(width / 2, height / 2); //offset to center
				auto e = makeEntity();
				e->setPosition(pos);
				auto physics = e->addComponent<PhysicsComponent>(false, Vector2f(width, height));
				physics->setRestitution(0.0f);
			}
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

	// Mysterius voice dialogue
	{
		run_dialogue = makeEntity();
		// Dialogue text component
		auto text = run_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Mysterious Voice: Serah, you need to find the Sword of Dawn to defeat Erebus, with out it\nyou are just defendless. Wait, something is coming... RUN!!!!");
		text->setFunction([&]() {text->level1DialogueUpdate(); });
	}

	//Set moving camera
	screenSize = static_cast<sf::Vector2f>(Engine::GetWindow().getSize());
	//curCentre = player->getPosition();
	//centrePoint = sf::Vector2f(leftBoundary, (screenSize.y / 2)-60.0f);
	centrePoint = sf::Vector2f(leftBoundary, (GAMEY / 2)-60.0f);
	if (player->getPosition().x > leftBoundary && player->getPosition().x < rightBoundary) {
		centrePoint.x = player->getPosition().x;
	}

	player->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->setRestitution(0.0f);


	setLoaded(true);
}

void LevelOne::Update(const double& dt)
{

	if (ls::getTileAt(player->getPosition()) == ls::KEY) {
		player->GetCompatibleComponent<NormalKeyComponent>()[0]->setHeld(true);
	}


	// Pause game
	if (Controller::isPressed(Controller::PauseButton))
	{
		_paused = true;
		// Pause music
		Audio::pauseMusic("level_1_music");
		Audio::pauseMusic("mystic_music");
		// Enable cursor when game is paused
		Engine::GetWindow().setMouseCursorVisible(true);
	}
	if (_paused)
	{
		ButtonComponent::ButtonNavigation(buttonsForController, buttonsCurrentIndex, dt);
	}
	else
	{
		// Disable cursor
		Engine::GetWindow().setMouseCursorVisible(false);
		if (!run_dialogue->isUpdatable())
		{
			Audio::playMusic("level_1_music");
		}
	}

	// Player caught by ghost
	if (ghost->GetCompatibleComponent<AISteeringComponent>()[0]->getPlayerDeath() && !player->isDead())
	{
		player->setDeath(true);
	}

	// Pick up key
	if (ls::getTileAt(player->getPosition()) == ls::KEY) 
	{
		player->GetCompatibleComponent<NormalKeyComponent>()[0]->setHeld(true);
	}

	// Camera
	if (player->getPosition().x > leftBoundary && player->getPosition().x < rightBoundary) {
		centrePoint.x = player->getPosition().x;
	}
	followPlayer = sf::View(sf::FloatRect(0.f, 0.f, GAMEX, GAMEY));
	followPlayer.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	followPlayer.setCenter(centrePoint);


	// Player death
	if (!player->isUpdatable() && player->isDead())
	{
		Engine::ChangeScene(&levelOne);
	}
	else if (ls::getTileAt(player->getPosition()) == ls::END && player->GetCompatibleComponent<KeyComponent>()[0]->getHeld()) {
		Engine::ChangeScene(&levelTwo);
	}

	// Move pause menu
	pause_background->setPosition(Vector2f(followPlayer.getCenter().x, followPlayer.getCenter().y));
	resume_btn->setPosition(Vector2f((followPlayer.getCenter().x) - (resume_tex->getSize().x / 2.0f) - 90.0f, followPlayer.getCenter().y + 50.0f));
	returnToMenu_btn->setPosition(Vector2f(((followPlayer.getCenter().x)) - (returnToMenu_tex->getSize().x / 2.0f) - 150.0f, followPlayer.getCenter().y + 150.0f));

	Scene::Update(dt);
}

void LevelOne::Render()
{
	Engine::GetWindow().draw(*background_image);
	Engine::GetWindow().setView(followPlayer);
	ls::render(Engine::GetWindow()); 

	Scene::Render();
}

void LevelOne::UnLoad()
{
	Engine::GetWindow().setView(sf::View(sf::FloatRect(0.0f, 0.0f, GAMEX, GAMEY)));
	Audio::stopMusic("level_1_music");
	buttonsForController.clear();
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

