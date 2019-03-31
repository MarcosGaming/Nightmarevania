#include "cmp_player_combat.h"
#include "cmp_player_physics.h"
#include "system_renderer.h"
#include <engine.h>

using namespace sf;
using namespace std;

PlayerCombatComponent::PlayerCombatComponent(Entity* p) : Component(p) 
{
	_healthTaken = 0;
	_maxHealth = 10;

	_hurt = false;

	_attackDamage = 1;

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
	_upAttackSprite = make_shared<Sprite>();
	_circularAttackSprite = make_shared<Sprite>();
	_downAttackSprite = make_shared<Sprite>();
	_defendSprite = make_shared<Sprite>();
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
	if ((Mouse::isButtonPressed(Mouse::Left)) && !_defending &&!_hurt)
	{
		Engine::setAttackButtonReleased(false);
		_attackDamage = 1;
		_attacking = true;
		_basicAttack = true;
		_circularAttackRight = false;
		_circularAttackLeft = false;
		_upAttack = false;
		_downAttack = false;
		// Possible special ground attacks
		if (_parent->get_components<PlayerPhysicsComponent>()[0]->isGrounded())
		{
			if ((Keyboard::isKeyPressed(Keyboard::W)) && _upAttackCooldown <= 0.0f)
			{
				_attackDamage = 2;
				_basicAttack = false;
				_upAttack = true;
				_upAttackCooldown = 3.0f;
			}
			if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::A)) && _circularAttackCooldown <= 0.0f)
			{
				_attackDamage = 3;
				_basicAttack = false;
				if (Keyboard::isKeyPressed(Keyboard::D))
				{
					_circularAttackRight = true;
				}
				else
				{
					_circularAttackLeft = true;
				}
				_circularAttackCooldown = 2.0f;
			}
		}
		// Possible special air attack
		else
		{
			if ((Keyboard::isKeyPressed(Keyboard::S)) && _downAttackCooldown <= 0.0f)
			{
				_attackDamage = 5;
				_basicAttack = false;
				_downAttack = true;
				_downAttackCooldown = 5.0f;
			}
		}

	}
	else if (Engine::isAttackButtonReleased() || _hurt)
	{
		_attackDamage = 1;
		_attacking = false;
		_basicAttack = false;
		_circularAttackRight = false;
		_circularAttackLeft = false;
		_upAttack = false;
		_downAttack = false;
	}
	// Defending behaviour
	if ((Mouse::isButtonPressed(Mouse::Right)) && !_attacking && !_hurt && _parent->get_components<PlayerPhysicsComponent>()[0]->isGrounded() && _defendingCooldown <= 0.0f)
	{
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
	else if(Engine::isDefendButtonReleased() || _hurt)
	{
		if (_defendingTime != 3.0f)
		{
			_defendingCooldown = 2.0f;
			_defendingTime = 3.0f;
		}
		_defending = false;
	}
	// Player death
	if (_healthTaken >= _maxHealth)
	{
		_hurt = false;
		_parent->setDeath(true);
	}
}

void PlayerCombatComponent::render()
{
	// Health bar icon
	if (_healthTaken < _maxHealth)
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[_healthTaken]);
	}
	else
	{
		_healthBarSprite->setTextureRect(_healthBarIcons[10]);
	}
	Renderer::queue(_healthBarSprite.get());
	// Up attack icon
	if (_upAttackCooldown <= 0)
	{
		_upAttackSprite->setTextureRect(_upAttackIcons[0]);
	}
	else
	{
		_upAttackSprite->setTextureRect(_upAttackIcons[1]);
	}
	Renderer::queue(_upAttackSprite.get());
	// Circular attack icon
	if (_circularAttackCooldown <= 0)
	{
		_circularAttackSprite->setTextureRect(_circularAttackIcons[0]);
	}
	else
	{
		_circularAttackSprite->setTextureRect(_circularAttackIcons[1]);
	}
	Renderer::queue(_circularAttackSprite.get());
	// Down attack icon
	if (_downAttackCooldown <= 0)
	{
		_downAttackSprite->setTextureRect(_downAttackIcons[0]);
	}
	else
	{
		_downAttackSprite->setTextureRect(_downAttackIcons[1]);
	}
	Renderer::queue(_downAttackSprite.get());
	// Defend icon
	if (_defendingCooldown <= 0)
	{
		_defendSprite->setTextureRect(_defendIcons[0]);
	}
	else
	{
		_defendSprite->setTextureRect(_defendIcons[1]);
	}
	Renderer::queue(_defendSprite.get());
}

bool PlayerCombatComponent::isAttacking() const { return _attacking; }
bool PlayerCombatComponent::isDefending() const { return _defending; }
bool PlayerCombatComponent::isBasicAttack() const{ return _basicAttack; }
bool PlayerCombatComponent::isCircularAttackRight() const { return _circularAttackRight; }
bool PlayerCombatComponent::isCircularAttackLeft() const { return _circularAttackLeft; }
bool PlayerCombatComponent::isUpAttack() const { return _upAttack; }
bool PlayerCombatComponent::isDownAttack() const { return _downAttack; }

void PlayerCombatComponent::resetHealth() { _healthTaken = 0; }

int PlayerCombatComponent::getAttackDamage() const { return _attackDamage; }

bool PlayerCombatComponent::isHurt() const { return _hurt; }
void PlayerCombatComponent::resetHurt() { _hurt = false; }
void PlayerCombatComponent::hurtPlayer(int damage)
{
	if (_hurtCooldown <= 0.0f)
	{
		_hurt = true;
		_healthTaken += damage;
		_hurtCooldown = 1.0f;
	}
}

void PlayerCombatComponent::setTexture(std::shared_ptr<sf::Texture> tex)
{
	_combatIconsTexture = tex;
	_healthBarSprite->setTexture(*_combatIconsTexture);
	_upAttackSprite->setTexture(*_combatIconsTexture);
	_circularAttackSprite->setTexture(*_combatIconsTexture);
	_downAttackSprite->setTexture(*_combatIconsTexture);
	_defendSprite->setTexture(*_combatIconsTexture);
}

sf::Sprite& PlayerCombatComponent::getHealthBarSprite() const { return *_healthBarSprite; }
sf::Sprite& PlayerCombatComponent::getUpAttackSprite() const { return *_upAttackSprite; }
sf::Sprite& PlayerCombatComponent::getCircularAttackSprite() const { return *_circularAttackSprite; }
sf::Sprite& PlayerCombatComponent::getDownAttackSprite() const { return *_downAttackSprite; }
sf::Sprite& PlayerCombatComponent::getDefendSprite() const { return *_defendSprite; }

void PlayerCombatComponent::addHealthBarIcon(sf::IntRect icon) { _healthBarIcons.push_back(icon); }
void PlayerCombatComponent::addUpAttackIcon(sf::IntRect icon) { _upAttackIcons.push_back(icon); }
void PlayerCombatComponent::addDownAttackIcon(sf::IntRect icon) { _downAttackIcons.push_back(icon); }
void PlayerCombatComponent::addCircularAttackIcon(sf::IntRect icon) { _circularAttackIcons.push_back(icon); }
void PlayerCombatComponent::addDefendIcon(sf::IntRect icon) { _defendIcons.push_back(icon); }
