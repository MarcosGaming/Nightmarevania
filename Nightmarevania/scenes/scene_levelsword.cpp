#include "scene_levelsword.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../animation_states.h"
#include "../components/cmp_door.h"
#include "../components/cmp_key.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <LevelSystem.h>
#include <system_controller.h>
#include <system_resolution.h>
#include <system_sound.h>
#include <system_saving.h>

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

static shared_ptr<Entity> short_dialogue;

//Texture background_tex;
//Sprite background_image;

vector<shared_ptr<Entity>> levelDoors;

void LevelSword::Load()
{
	// Save this level as the last one played
	Saving::saveLevel("3");
	// Stop music from main menu in the case that we come from there
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
		background_tex->loadFromFile("res/img/level_sword.png");
		float scaleX = (float)GAMEX / (background_tex->getSize().x);
		float scaleY = (float)GAMEY / (background_tex->getSize().y);
		background_image->scale(scaleX, scaleY);
		background_image->setTexture(*background_tex);
	}

	// Level file
	ls::loadLevelFile("res/levels/level_sword.txt", 60.0f);
	// Tiles offset
	auto ho = GAMEY - (ls::getHeight() * 60.0f);
	ls::setOffset(Vector2f(0, ho));

	// Adventurer textures
	playerAnimations = make_shared<Texture>();
	playerAnimations->loadFromFile("res/img/adventurer.png");

	// Door
	auto doors = ls::findTiles(ls::DOOR);
	//if (ls::doesTileExist(ls::DOOR))
	for (auto d : doors)
	{
		door = makeEntity();
		auto doorCmp = door->addComponent<DoorComponent>(true, ls::getTilePosition(d));
		auto doorSprite = door->addComponent<SpriteComponent>();
		doorSprite->setTexure(doorCmp->getTexture());
		doorSprite->getSprite().setOrigin(doorSprite->getSprite().getTextureRect().width * 0.5f, 0.0f);
		doorSprite->getSprite().setTextureRect(doorCmp->getRect());
		levelDoors.push_back(door);
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
		// Component that manages player animations
		auto anim = player->addComponent<AnimationMachineComponent>();
		anim->addAnimation("Idle", idle);
		anim->addAnimation("Run", run);
		anim->addAnimation("Jump", jump);
		anim->addAnimation("Fall", fall);
		anim->addAnimation("DoubleJump", doubleJump);
		anim->changeAnimation("Idle");

		// Physics component
		auto physics = player->addComponent<PlayerPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 2.8f));
		physics->setRestitution(0.0f);

		// Sword component
		if (ls::doesTileExist(ls::KEY)) 
		{
			Vector2f tilePos = ls::getTilePosition(ls::findTiles(ls::KEY)[0]);
			Vector2f swordPos = Vector2f(tilePos.x, tilePos.y-33.0f);
			auto key = player->addComponent<SwordKeyComponent>(false,swordPos);
		}
		// Set the player for the door component of the door entity
		if (ls::doesTileExist(ls::DOOR))
		{
			levelDoors.back()->GetCompatibleComponent<DoorComponent>()[0]->setPlayer(player);
		}
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

	// Short dialogue dialogue
	{
		short_dialogue = makeEntity();
		short_dialogue->setUpdatable(true);
		// Dialogue text component
		auto text = short_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Mysterious voice: Ahead awaits you the final challenge Serah. Grab the sword and reclaim\nthe strength of your ancestors, you will need it to defeat Erebus.");
		text->setFunction([&]() {text->moveWhileDialogueUpdate(); });
	}
	setLoaded(true);
}

void LevelSword::Update(const double& dt)
{
	// Pause game
	if (Controller::isPressed(Controller::PauseButton))
	{
		_paused = true;
		// Pause mucis
		Audio::pauseMusic("mystic_music");
		// Enable cursor when game is paused
		Engine::GetWindow().setMouseCursorVisible(true);
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
		// Disable cursor
		Engine::GetWindow().setMouseCursorVisible(false);
	}
	// The player can proceed to the next level after picking up the sword key
	if (ls::getTileAt(player->getPosition()) == ls::KEY && !player->GetCompatibleComponent<KeyComponent>()[0]->getHeld())
	{
		player->GetCompatibleComponent<KeyComponent>()[0]->setHeld(true);
		Audio::playEffect("pick_up_effect");
	}
	else if (ls::getTileAt(player->getPosition()) == ls::END && player->GetCompatibleComponent<KeyComponent>()[0]->getHeld())
	{
		Engine::ChangeScene((Scene*)&levelThree);
	}
	Scene::Update(dt);
}

void LevelSword::Render()
{
	Engine::GetWindow().draw(*background_image);

	ls::render(Engine::GetWindow());
	Scene::Render();
}

void LevelSword::UnLoad()
{
	buttonsForController.clear();
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

