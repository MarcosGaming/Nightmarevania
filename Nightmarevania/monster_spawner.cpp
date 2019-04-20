//#include "monster_spawner.h"
//#include "ecm.h"
//#include "engine.h"
//#include "SFML/Graphics.hpp"
//#include <LevelSystem.h>
////#include "../lib_tile_level_loader/LevelSystem.h"
//#include "components/cmp_sprite.h"
//#include "components/cmp_animation_machine.h"
//#include "../Nightmarevania/animation_states.h"
//#include "components/cmp_enemy_ai.h"
//#include "components/cmp_state_machine.h"
//#include "components/cmp_ai_patrol.h"
//#include "enemy_decisions.h"
//#include "components/cmp_enemy_physics .h"
//#include "components/cmp_hurt.h"
//#include "enemy_states.h"
//#include "enemy_decisions.h"
//
//using namespace std;
//using namespace sf;
//
//static shared_ptr<Entity> skeleton_soldier;
//static shared_ptr<Texture> skeleton_soldier_tex;
//
//MonsterSpawner::MonsterSpawner()
//{
//}
//
//MonsterSpawner::~MonsterSpawner()
//{
//}
//
//shared_ptr<Entity> MonsterSpawner::spawnSkeleton(float pointA, float pointB, shared_ptr<Entity> player, Scene* scene)
//{
//	skeleton_soldier_tex = make_shared<Texture>();
//	skeleton_soldier_tex->loadFromFile("res/img/skeleton_soldier.png");
//
//	skeleton_soldier = make_shared<Entity>();
//	skeleton_soldier = scene->makeEntity();
//	auto position = ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]);
//	skeleton_soldier->setPosition(position);
//	// Sprite component
//	auto sprite = skeleton_soldier->addComponent<SpriteComponent>();
//	sprite->setTexure(skeleton_soldier_tex);
//	sprite->getSprite().setTextureRect(IntRect(0, 0, 24, 37));
//	sprite->getSprite().scale(sf::Vector2f(3.0f, 3.0f));
//	sprite->getSprite().setOrigin(sprite->getSprite().getTextureRect().width * 0.5f, sprite->getSprite().getTextureRect().height * 0.5f);
//	// Animations
//	shared_ptr<SkeletonIdleAnimation> idle = make_shared<SkeletonIdleAnimation>();
//	for (int i = 0; i < 11; i++)
//	{
//		idle->addFrame(IntRect(24 * i, 0, 24, 37));
//	}
//	shared_ptr<SkeletonSlowWalkAnimation> walk = make_shared<SkeletonSlowWalkAnimation>();
//	for (int i = 0; i < 13; i++)
//	{
//		walk->addFrame(IntRect(22 * i, 37, 22, 37));
//	}
//	shared_ptr<SkeletonAttackAnimation> attack = make_shared<SkeletonAttackAnimation>();
//	attack->setSoundFrame(7);
//	for (int i = 0; i < 18; i++)
//	{
//		attack->addFrame(IntRect(43 * i, 37 * 2, 43, 37));
//	}
//	shared_ptr<SkeletonFakeDeathAnimation> death = make_shared<SkeletonFakeDeathAnimation>();
//	for (int i = 1; i < 15; i++)
//	{
//		death->addFrame(IntRect(33 * i, 37 * 3, 33, 37));
//	}
//	shared_ptr<SkeletonReviveAnimation> revive = make_shared<SkeletonReviveAnimation>();
//	for (int i = 14; i > 0; i--)
//	{
//		revive->addFrame(IntRect(33 * i, 37 * 3, 33, 37));
//	}
//	// Component that manages player animations
//	auto anim = skeleton_soldier->addComponent<AnimationMachineComponent>();
//	anim->addAnimation("Idle", idle);
//	anim->addAnimation("WalkSlow", walk);
//	anim->addAnimation("Attack", attack);
//	anim->addAnimation("Death", death);
//	anim->addAnimation("Revive", revive);
//	anim->changeAnimation("Idle");
//	// AI component
//	auto AI = skeleton_soldier->addComponent<SkeletonAIComponent>();
//	// Patrol component
//	auto patrol = skeleton_soldier->addComponent<AIPatrolComponent>(position, position + sf::Vector2f(500.0f, 0.0f));
//	// State machine component
//	auto sm = skeleton_soldier->addComponent<StateMachineComponent>();
//	sm->addState("Stationary", make_shared<StationaryState>());
//	sm->addState("SeekSlow", make_shared<SeekLimitsState>(skeleton_soldier, player, position, position + sf::Vector2f(500.0f, 0.0f)));
//	sm->addState("Patrol", make_shared<PatrolState>());
//	sm->addState("Death", make_shared<FakeDeathState>());
//	sm->addState("Attack", make_shared<AttackState>());
//	sm->addState("Revive", make_shared<ReviveState>());
//	sm->changeState("Idle");
//	// Decision subtree 1
//	auto weightedDecisionSubtree1 = make_shared<WeightedBinaryDecision>(90, make_shared<ReviveDecision>(), make_shared<DeathDecision>());
//	// Decision subtree 2
//	auto distanceDecision1Subtree2 = make_shared<DistanceDecision>(player, 120, make_shared<AttackDecision>(), make_shared<SeekSlowDecision>());
//	auto weightedDecision1Subtree2 = make_shared<WeightedBinaryDecision>(70, make_shared<StationaryDecision>(), make_shared<DeathDecision>());
//	auto weightedDecision2Subtree2 = make_shared<WeightedBinaryDecision>(90, make_shared<PatrolDecision>(), weightedDecision1Subtree2);
//	auto distanceDecision2Subtree2 = make_shared<DistanceDecision>(player, 300, distanceDecision1Subtree2, weightedDecision2Subtree2);
//	// Complete tree
//	auto fakeDeathDecision = make_shared<FakeDeathDecision>(skeleton_soldier, weightedDecisionSubtree1, distanceDecision2Subtree2);
//	// Decision tree component
//	skeleton_soldier->addComponent<DecisionTreeComponent>(fakeDeathDecision);
//	// Physics component
//	auto physics = skeleton_soldier->addComponent<EnemyPhysicsComponent>(Vector2f(sprite->getSprite().getTextureRect().width, sprite->getSprite().getTextureRect().height * 2.8f));
//	// Player kill component
//	skeleton_soldier->addComponent<PlayerKillComponent>();
//
//	return skeleton_soldier;
//}
