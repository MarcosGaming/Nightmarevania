#include "cmp_player_combat.h"
#include "cmp_player_physics.h"
#include "system_renderer.h"
#include <LevelSystem.h>
#include <engine.h>
#include <iostream>
using namespace sf;
using namespace std;

PlayerCombatComponent::PlayerCombatComponent(Entity* p) : Component(p) 
{
	_health = 100;

	_attacking = false;
	_defending = false;
	_basicAttack = false;
	_circularAttackRight = false;
	_circularAttackLeft = false;
	_upAttack = false;
	_downAttack = false;

	_defendingTime = 3.0f;
	_defendingCooldown = 0.0f;
	_circularAttackCooldown = 0.0f;
	_upAttackCooldown = 0.0f;
	_downAttackCooldown = 0.0f;
	_hurtCooldown = 0.0f;

	_healthBarSprite = make_shared<Sprite>();
}

void PlayerCombatComponent::update(double dt) 
{
	// Cooldown decreases with delta time
	_hurtCooldown -= dt;
	_defendingCooldown -= dt;
	_circularAttackCooldown -= dt;
	_upAttackCooldown -= dt;
	_downAttackCooldown -= dt;
	// Attacking behaviour
	if ((Mouse::isButtonPressed(Mouse::Left)) && !_defending)
	{
		Engine::setAttackButtonReleased(false);
		_attacking = true;
		_basicAttack = true;
		_circularAttackRight = false;
		_circularAttackLeft = false;
		_upAttack = false;
		_downAttack = false;
		// Some attacks can only be done while the player is on the ground or on the air
		if (_parent->get_components<PlayerPhysicsComponent>()[0]->isGrounded())
		{
			if ((Keyboard::isKeyPressed(Keyboard::W)) && _upAttackCooldown <= 0.0f)
			{
				_basicAttack = false;
				_upAttack = true;
				_upAttackCooldown = 1.0f;
			}
			if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::A)) && _circularAttackCooldown <= 0.0f)
			{
				_basicAttack = false;
				if (Keyboard::isKeyPressed(Keyboard::D))
				{
					_circularAttackRight = true;
				}
				else
				{
					_circularAttackLeft = true;
				}
				_circularAttackCooldown = 1.0f;
			}
		}
		else
		{
			if ((Keyboard::isKeyPressed(Keyboard::S)) && _downAttackCooldown <= 0.0f)
			{
				_basicAttack = false;
				_downAttack = true;
				_downAttackCooldown = 1.0f;
			}
		}

	}
	else if (Engine::isAttackButtonReleased())
	{
		_attacking = false;
		_basicAttack = false;
		_circularAttackRight = false;
		_circularAttackLeft = false;
		_upAttack = false;
		_downAttack = false;
	}
	// Defending behaviour
	if ((Mouse::isButtonPressed(Mouse::Right)) && !_attacking && _parent->get_components<PlayerPhysicsComponent>()[0]->isGrounded() && _defendingCooldown <= 0.0f)
	{
		std::cout << "here" << endl;
		Engine::setDefendButtonReleased(false);
		_defending = true;
		_defendingTime -= dt;
		if (_defendingTime <= 0)
		{
			_defendingCooldown = 2.0f;
			_defendingTime = 3.0f;
			_defending = false;
		}
	}
	else if(Engine::isDefendButtonReleased())
	{
		if (_defendingTime != 3.0f)
		{
			_defendingCooldown = 2.0f;
			_defendingTime = 3.0f;
		}
		_defending = false;
	}
	// Player dies if health is <= 0
	if (_health <= 0)
	{
		_parent->setDeath(true);
		_health = 100;
	}
}

void PlayerCombatComponent::render()
{
	if (_health == 100)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[0]);
	}
	else if (_health == 90)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[1]);
	}
	else if (_health == 80)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[2]);
	}
	else if (_health == 70)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[3]);
	}
	else if (_health == 60)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[4]);
	}
	else if (_health == 50)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[5]);
	}
	else if (_health == 40)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[6]);
	}
	else if (_health == 30)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[7]);
	}
	else if (_health == 20)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[8]);
	}
	else if (_health == 10)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[9]);
	}
	else if (_health == 0)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[10]);
	}
	Renderer::queue(_healthBarSprite.get());
}

bool PlayerCombatComponent::isAttacking() const
{
	return _attacking;
}

bool PlayerCombatComponent::isDefending() const
{
	return _defending;
}

bool PlayerCombatComponent::isBasicAttack() const
{
	return _basicAttack;
}

bool PlayerCombatComponent::isCircularAttackRight() const
{
	return _circularAttackRight;
}

bool PlayerCombatComponent::isCircularAttackLeft() const
{
	return _circularAttackLeft;
}

bool PlayerCombatComponent::isUpAttack() const
{
	return _upAttack;;
}

bool PlayerCombatComponent::isDownAttack() const
{
	return _downAttack;
}

void PlayerCombatComponent::hurtPlayer(int damage)
{
	if (_hurtCooldown <= 0.0f)
	{
		_health -= damage;
		_hurtCooldown = 1.0f;
	}
}


void PlayerCombatComponent::setTexture(std::shared_ptr<sf::Texture> tex)
{
	_combatIconsTexture = tex;
	_healthBarSprite->setTexture(*_combatIconsTexture);
	/*_upAttackSprite->setTexture(*_combatIconsTexture);
	_circularAttackSprite->setTexture(*_combatIconsTexture);
	_downAttackSprite->setTexture(*_combatIconsTexture);
	_defendSprite->setTexture(*_combatIconsTexture);*/
}

void PlayerCombatComponent::addHealthBarIcon(sf::IntRect icon) { _healthBarIcons.push_back(icon); }

void PlayerCombatComponent::addUpAttackIcon(sf::IntRect icon) { _upAttackIcons.push_back(icon); }

void PlayerCombatComponent::addDownAttackIcon(sf::IntRect icon) { _downAttackIcons.push_back(icon); }

void PlayerCombatComponent::addCircularAttackIcon(sf::IntRect icon) { _circularAttackIcons.push_back(icon); }

void PlayerCombatComponent::addDefendIcon(sf::IntRect icon) { _defendIcons.push_back(icon); }

sf::Sprite& PlayerCombatComponent::getHealthBarSprite() const { return *_healthBarSprite; }