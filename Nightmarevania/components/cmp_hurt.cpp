#include "cmp_hurt.h"
#include "cmp_player_combat.h"
#include "cmp_enemy_ai.h"
#include <engine.h>
#include "cmp_player_physics.h"

using namespace std;
using namespace sf;

PlayerHurtComponent::PlayerHurtComponent(Entity* p) : Component(p), _player(_parent->scene->ents.find("Player")[0]) {}

void PlayerHurtComponent::update(double dt) 
{
	static float attackDelay = 0.5f;
	if (auto player = _player.lock())
	{
		auto boss = _parent->get_components<BossAIComponent>()[0];
		auto combat = player->get_components<PlayerCombatComponent>()[0];
		if (boss->isAttacking() && !combat->isDefending())
		{
			attackDelay -= dt;
			if (attackDelay < 0.0f && length(player->getPosition() - _parent->getPosition()) < 180.0f)
			{
				combat->hurtPlayer(1);
				attackDelay = 1.2f;
			}
		}
		else
		{
			attackDelay = 0.5f;
		}
	}
}


// Component that hurts the boss (goes in the player)
BossHurtComponent::BossHurtComponent(Entity* p) : Component(p), _boss(_parent->scene->ents.find("Boss")[0]) {}

void BossHurtComponent::update(double dt)
{
	if (auto boss = _boss.lock())
	{
		auto player = _parent->get_components<PlayerCombatComponent>()[0];
		if (player->isAttacking() && length(boss->getPosition() - _parent->getPosition()) < 120.0f)
		{
			auto AI = boss->get_components<BossAIComponent>()[0];
			AI->hurtEnemy(player->getAttackDamage());
		}
	}
}