#include "cmp_enemy_ai.h"
#include "../steering.h"
#include <system_renderer.h>
#include <system_sound.h>

static float hurtCoolDown = 0.2f;

// Base skeleton AI component
SkeletonAIComponent::SkeletonAIComponent(Entity* p) : _attacking(false), Component(p) {}

void SkeletonAIComponent::update(double dt) {}
void SkeletonAIComponent::render() {}

bool SkeletonAIComponent::isAttacking() const { return _attacking; }
void SkeletonAIComponent::setAttacking(bool attacking) { _attacking = attacking; }

// The boss AI component is going to add a healthbar to the skeleton
BossAIComponent::BossAIComponent(Entity* p) : _healthTaken(0), _maxHealth(100), _hurt(false), _isIdle(false), SkeletonAIComponent(p)
{
	_healthBarTexture = std::make_shared<sf::Texture>();
	_foregroundTexture = std::make_shared<sf::Texture>();
	_healthBarSprite = std::make_shared<sf::Sprite>();
	_foregroundSprite = std::make_shared<sf::Sprite>();
}

void BossAIComponent::update(double dt)
{
	// Hurt cooldown needed to give some time between hits
	hurtCoolDown -= (float)dt;
	// Dies
	if(_parent->isDead())
	{
		_hurt = false;
	}
}

// The health bar is going to change based on how much life the enemy has left
void BossAIComponent::render()
{
	if (!_parent->isDead())
	{
		_healthBarSprite->setTextureRect(sf::IntRect(0, 0, 196 - (int)(_healthTaken * 1.96f), 16));
		// Only render health bar while the boss still has health left
		if (_healthTaken < _maxHealth)
		{
			Renderer::queue(_healthBarSprite.get());
		}
		Renderer::queue(_foregroundSprite.get());
	}
}

bool BossAIComponent::isHurt()const { return _hurt; }
void BossAIComponent::resetHurt() { _hurt = false; }
void BossAIComponent::hurtEnemy(int damage)
{
	// The boss receives more damage when is iddle
	if (hurtCoolDown <= 0.0f)
	{
		_hurt = true;
		if (!_isIdle)
		{
			_healthTaken += 1;
			hurtCoolDown = 0.4f;
		}
		else
		{
			_healthTaken += damage;
			hurtCoolDown = 0.2f;
			Audio::playEffect("boss_hurt_effect");
		}
	}
}

void BossAIComponent::setHealthBarTexture(std::shared_ptr<sf::Texture> tex) { _healthBarTexture = tex; _healthBarSprite->setTexture(*_healthBarTexture); }
sf::Sprite& BossAIComponent::getHealthBarSprite() const { return *_healthBarSprite; }

void BossAIComponent::setForegroundTexture(std::shared_ptr<sf::Texture> tex) 
{ 
	_foregroundTexture = tex; 
	_foregroundSprite->setTexture(*_foregroundTexture); 
	_foregroundSprite->setTextureRect(sf::IntRect(0, 0, 256, 32));
}
sf::Sprite& BossAIComponent::getForegroundSprite() const { return *_foregroundSprite; }

void BossAIComponent::setIsIdle(bool idle) { _isIdle = idle; }

int BossAIComponent::getHealthTaken() const { return _healthTaken; }

//Ghost for level 1
GhostAIComponent::GhostAIComponent(Entity* p) : Component(p) {}

void GhostAIComponent::update(double dt)
{

}

void GhostAIComponent::setTarget(shared_ptr<Entity> target) {
	_target = target;
}

void GhostAIComponent::render() {}