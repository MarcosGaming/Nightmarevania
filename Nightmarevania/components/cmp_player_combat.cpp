#include "cmp_player_combat.h"
#include <LevelSystem.h>
#include <engine.h>
#include <iostream>
using namespace sf;
using namespace std;

PlayerCombatComponent::PlayerCombatComponent(Entity* p) : Component(p) {}

void PlayerCombatComponent::update(double dt) 
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		_attacking = true;
	}
	else if (Mouse::isButtonPressed(Mouse::Right))
	{
		_defending = true;
	}
}

bool PlayerCombatComponent::isAttacking() const
{
	return _attacking;
}

void PlayerCombatComponent::setAttacking(bool attacking)
{
	_attacking = attacking;
}

bool PlayerCombatComponent::isDefending() const
{
	return _defending;
}

void PlayerCombatComponent::setDefending(bool defending)
{
	_defending = defending;
}