#include "scene_leveltwo.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_button.h"
#include "../components/cmp_player_combat.h"
#include "../components/cmp_enemy_physics .h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_state_machine.h"
#include "../components/cmp_decision_tree.h"
#include "../components/cmp_ai_patrol.h"
#include "../components/cmp_hurt.h"
#include "../enemy_decisions.h"
#include "../enemy_states.h"
#include "../animation_states.h"
#include "../components/cmp_door.h"
#include "../components/cmp_key.h"
#include "../game.h"
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

static shared_ptr<Entity> skeleton_soldier;
static shared_ptr<Texture> skeleton_soldier_tex;
vector<shared_ptr<Entity>> skeletonSoldiers;

int keyCount = 0;

//Texture background_tex;
//Sprite background_image;

void LevelTwo::Load()
{
	// Need to initialise phyiscs to reset the world otherwise the player dead body will block the path
	Physics::initialise();
	// Save this level as the last one played
	Saving::saveLevel("2");
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
		background_tex->loadFromFile("res/img/level_two.png");
		float scaleX = (float)GAMEX / (background_tex->getSize().x);
		float scaleY = (float)GAMEY / (background_tex->getSize().y);
		background_image->scale(scaleX, scaleY);
		background_image->setTexture(*background_tex);
	}

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

		auto physics = player->addComponent<PlayerPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 2.8f));
		physics->setRestitution(0.0f);

		//KEY - level 2 only
		if (ls::doesTileExist(ls::DOOR)) {
			door->GetCompatibleComponent<DoorComponent>()[0]->setPlayer(player);
		}

		//if (keyExists) {
		if (ls::doesTileExist(ls::KEY)) {
			auto keys = ls::findTiles(ls::KEY);
			for (auto k : keys) { //START OF MULTI KEY CODE
				auto key = player->addComponent<NormalKeyComponent>(false, ls::getTilePosition(k));
			}
		}

	}

	//skeletons
	if (ls::doesTileExist(ls::ENEMY)) {
		skeleton_soldier_tex = make_shared<Texture>();
		skeleton_soldier_tex->loadFromFile("res/img/skeleton_soldier.png");
		auto skeletons = ls::findTiles(ls::ENEMY);
		// Animations
		shared_ptr<SkeletonIdleAnimation> idle = make_shared<SkeletonIdleAnimation>();
		for (int i = 0; i < 11; i++)
		{
			idle->addFrame(IntRect(24 * i, 0, 24, 37));
		}
		shared_ptr<SkeletonSlowWalkAnimation> walk = make_shared<SkeletonSlowWalkAnimation>();
		for (int i = 0; i < 13; i++)
		{
			walk->addFrame(IntRect(22 * i, 37, 22, 37));
		}
		shared_ptr<SkeletonAttackAnimation> attack = make_shared<SkeletonAttackAnimation>();
		attack->setSoundFrame(7);
		for (int i = 0; i < 18; i++)
		{
			attack->addFrame(IntRect(43 * i, 37 * 2, 43, 37));
		}
		shared_ptr<SkeletonFakeDeathAnimation> death = make_shared<SkeletonFakeDeathAnimation>();
		for (int i = 1; i < 15; i++)
		{
			death->addFrame(IntRect(33 * i, 37 * 3, 33, 37));
		}
		shared_ptr<SkeletonReviveAnimation> revive = make_shared<SkeletonReviveAnimation>();
		for (int i = 14; i > 0; i--)
		{
			revive->addFrame(IntRect(33 * i, 37 * 3, 33, 37));
		}

		
		int i = 0;
		for (auto s : skeletons)
		{
			auto waypointsA = ls::findTiles(ls::WAYPOINTA);
			auto waypointsB = ls::findTiles(ls::WAYPOINTB);

			skeleton_soldier = makeEntity();
			auto position = ls::getTilePosition(s);
			skeleton_soldier->setPosition(position);

			// Sprite component
			auto sprite = skeleton_soldier->addComponent<SpriteComponent>();
			sprite->setTexure(skeleton_soldier_tex);
			sprite->getSprite().setTextureRect(IntRect(0, 0, 24, 37));
			sprite->getSprite().scale(sf::Vector2f(3.0f, 3.0f));
			sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);

			// Component that manages player animations
			auto anim = skeleton_soldier->addComponent<AnimationMachineComponent>();
			anim->addAnimation("Idle", idle);
			anim->addAnimation("WalkSlow", walk);
			anim->addAnimation("Attack", attack);
			anim->addAnimation("Death", death);
			anim->addAnimation("Revive", revive);
			anim->changeAnimation("Idle");
			// AI component
			auto AI = skeleton_soldier->addComponent<SkeletonAIComponent>();
			// Patrol component
			//auto patrol = skeleton_soldier->addComponent<AIPatrolComponent>(position, position + sf::Vector2f(500.0f, 0.0f));
			auto patrol = skeleton_soldier->addComponent<AIPatrolComponent>(ls::getTilePosition(waypointsA[i]), ls::getTilePosition(waypointsB[i]));
			// State machine component
			auto sm = skeleton_soldier->addComponent<StateMachineComponent>();
			sm->addState("Stationary", make_shared<StationaryState>());
			//sm->addState("SeekSlow", make_shared<SeekLimitsState>(skeleton_soldier, player, position, position + sf::Vector2f(500.0f, 0.0f)));
			sm->addState("SeekSlow", make_shared<SeekLimitsState>(skeleton_soldier, player, ls::getTilePosition(waypointsA[i]), ls::getTilePosition(waypointsB[i])));
			sm->addState("Patrol", make_shared<PatrolState>());
			sm->addState("Death", make_shared<FakeDeathState>());
			sm->addState("Attack", make_shared<AttackState>());
			sm->addState("Revive", make_shared<ReviveState>());
			sm->changeState("Idle");
			// Decision subtree 1
			auto weightedDecisionSubtree1 = make_shared<WeightedBinaryDecision>(90, make_shared<ReviveDecision>(), make_shared<DeathDecision>());
			// Decision subtree 2
			auto distanceDecision1Subtree2 = make_shared<DistanceDecision>(player, 120, make_shared<AttackDecision>(), make_shared<SeekSlowDecision>());
			auto weightedDecision1Subtree2 = make_shared<WeightedBinaryDecision>(90, make_shared<StationaryDecision>(), make_shared<DeathDecision>());
			auto weightedDecision2Subtree2 = make_shared<WeightedBinaryDecision>(90, make_shared<PatrolDecision>(), weightedDecision1Subtree2);
			auto distanceDecision2Subtree2 = make_shared<DistanceDecision>(player, 350, distanceDecision1Subtree2, weightedDecision2Subtree2);
			// Complete tree
			auto fakeDeathDecision = make_shared<FakeDeathDecision>(skeleton_soldier, weightedDecisionSubtree1, distanceDecision2Subtree2);
			// Decision tree component
			skeleton_soldier->addComponent<DecisionTreeComponent>(fakeDeathDecision);
			// Physics component
			auto physics = skeleton_soldier->addComponent<EnemyPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width, sprite->getSprite().getTextureRect().height * 2.8f));
			physics->setMaxSlowVelocity(Vector2f(70, 100));
			// Player kill component
			skeleton_soldier->addComponent<PlayerKillComponent>();
			// Add skeleton to vector
			skeletonSoldiers.push_back(skeleton_soldier);
			i++;
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


	//MOVING CAMERA STUFF
	centrePoint = sf::Vector2f(rightBoundary, topBoundary);

	if (player->getPosition().x > leftBoundary && player->getPosition().x < rightBoundary) {
		centrePoint.x = player->getPosition().x;
	}

	if (player->getPosition().y > topBoundary && player->getPosition().y < bottomBoundary) {
		centrePoint.y = player->getPosition().y;
	}
	Engine::GetWindow().setView(followPlayer);

	setLoaded(true);
}

void LevelTwo::Update(const double& dt)
{
	
	if (ls::getTileAt(player->getPosition()) == ls::KEY) { // && keyExists
		player->GetCompatibleComponent<NormalKeyComponent>()[0]->setHeld(true);
		keyCount++;
		Audio::playEffect("pick_up_effect");
	}

	// Pause game
	if (Controller::isPressed(Controller::PauseButton))
	{
		_paused = true;
		// Pause music
		Audio::pauseMusic("level_2_music");
		// Enable cursor when game is paused
		Engine::GetWindow().setMouseCursorVisible(true);
	}
	if (_paused)
	{
		ButtonComponent::ButtonNavigation(buttonsForController, buttonsCurrentIndex, dt);
	}
	else
	{
		Audio::playMusic("level_2_music");
		// Disable cursor
		Engine::GetWindow().setMouseCursorVisible(true);
	}

	if (player->getPosition().x > leftBoundary && player->getPosition().x < rightBoundary) 
	{
		centrePoint.x = player->getPosition().x;
	}

	if (player->getPosition().y > topBoundary && player->getPosition().y < bottomBoundary) 
	{
		centrePoint.y = player->getPosition().y;
	}

	followPlayer = sf::View(sf::FloatRect(0.f, 0.f, GAMEX, GAMEY));
	followPlayer.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	followPlayer.setCenter(centrePoint);

	// Move pause menu
	pause_background->setPosition(Vector2f(followPlayer.getCenter().x, followPlayer.getCenter().y));
	resume_btn->setPosition(Vector2f((followPlayer.getCenter().x) - (resume_tex->getSize().x / 2.0f) - 90.0f, followPlayer.getCenter().y + 50.0f));
	returnToMenu_btn->setPosition(Vector2f(((followPlayer.getCenter().x)) - (returnToMenu_tex->getSize().x / 2.0f) - 150.0f, followPlayer.getCenter().y + 150.0f));

	// Player death
	if (!player->isUpdatable() && player->isDead())
	{
		Engine::ChangeScene(&levelTwo);
	}
	// Change level if player reaches the door with the key
	else if (ls::getTileAt(player->getPosition()) == ls::END && keyCount > 0)
	{
		Engine::ChangeScene((Scene*)&levelSword);
	}

	Scene::Update(dt);
}

void LevelTwo::Render()
{
	Engine::GetWindow().draw(*background_image);

	Engine::GetWindow().setView(followPlayer);
	ls::render(Engine::GetWindow());

	Scene::Render();
}

void LevelTwo::UnLoad()
{
	Engine::GetWindow().setView(sf::View(sf::FloatRect(0.0f, 0.0f, GAMEX, GAMEY)));
	Audio::stopMusic("level_2_music");
	buttonsForController.clear();
	player.reset();
	skeleton_soldier.reset();
	skeletonSoldiers.clear();
	ls::unload();
	Scene::UnLoad();
}