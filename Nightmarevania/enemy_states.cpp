#include "enemy_states.h"
#include "components\cmp_enemy_physics .h"
#include "components\cmp_enemy_ai.h"
#include "system_sound.h"

// Stationary state
void StationaryState::execute(Entity* owner, double dt) noexcept
{
	// Physics component
	auto physics = owner->get_components<EnemyPhysicsComponent>()[0];
	physics->setCanMove(false);
	// AI component
	auto AI = owner->GetCompatibleComponent<SkeletonAIComponent>()[0];
	AI->setAttacking(false);
	// Boss AI
	auto bossAI = owner->get_components<BossAIComponent>();
	if (!bossAI.empty())
	{
		bossAI[0]->setIsIddle(true);
	}
}

// Seek slow state
SeekSlowState::SeekSlowState(std::shared_ptr<Entity> owner, std::shared_ptr<Entity> player) : _steering(owner.get(), player.get()) {}
void SeekSlowState::execute(Entity* owner, double dt) noexcept
{
	// Physics component
	auto physics = owner->get_components<EnemyPhysicsComponent>()[0];
	auto output = _steering.getSteering();
	physics->setCanMove(true);
	physics->setSlow(true);
	physics->setMoveDirection(output.direction);
	// AI component
	auto AI = owner->GetCompatibleComponent<SkeletonAIComponent>()[0];
	AI->setAttacking(false);
	// Boss AI
	auto bossAI = owner->get_components<BossAIComponent>();
	if (!bossAI.empty())
	{
		bossAI[0]->setIsIddle(false);
	}
}

// Seek fast state
SeekFastState::SeekFastState(std::shared_ptr<Entity> owner, std::shared_ptr<Entity> player) : _steering(owner.get(), player.get()) {}
void SeekFastState::execute(Entity* owner, double dt) noexcept
{
	// Physics component
	auto physics = owner->get_components<EnemyPhysicsComponent>()[0];
	auto output = _steering.getSteering();
	physics->setCanMove(true);
	physics->setSlow(false);
	physics->setMoveDirection(output.direction);
	// AI component
	auto AI = owner->GetCompatibleComponent<SkeletonAIComponent>()[0];
	AI->setAttacking(false);
	// Boss AI
	auto bossAI = owner->get_components<BossAIComponent>();
	if (!bossAI.empty())
	{
		bossAI[0]->setIsIddle(false);
	}
}

// Attack state
void AttackState::execute(Entity* owner, double dt) noexcept
{
	// Physics component
	auto physics = owner->get_components<EnemyPhysicsComponent>()[0];
	physics->setCanMove(false);
	// AI component
	auto AI = owner->GetCompatibleComponent<SkeletonAIComponent>()[0];
	AI->setAttacking(true);
	// Boss AI
	auto bossAI = owner->get_components<BossAIComponent>();
	if (!bossAI.empty())
	{
		bossAI[0]->setIsIddle(false);
	}
}

// Fake death state
void DeathState::execute(Entity* owner, double dt) noexcept
{
	// Physics component
	auto physics = owner->get_components<EnemyPhysicsComponent>()[0];
	physics->setCanMove(false);
	// AI component
	auto AI = owner->GetCompatibleComponent<SkeletonAIComponent>()[0];
	AI->setAttacking(false);
	owner->setDeath(true);
	// Boss AI
	auto bossAI = owner->get_components<BossAIComponent>();
	if (!bossAI.empty())
	{
		bossAI[0]->setIsIddle(false);
	}
	static bool played = false;
	if (!played)
	{
		Audio::playEffect("boss_death_effect");
		played = true;
	}
}

// Revive state
void ReviveState::execute(Entity* owner, double dt) noexcept
{
	// Physics component
	auto physics = owner->get_components<EnemyPhysicsComponent>()[0];
	physics->setCanMove(false);
	// AI component
	auto AI = owner->GetCompatibleComponent<SkeletonAIComponent>()[0];
	AI->setAttacking(false);
	// Boss AI
	auto bossAI = owner->get_components<BossAIComponent>();
	if (!bossAI.empty())
	{
		bossAI[0]->setIsIddle(false);
	}
}

