#include "cmp_enemy_ai.h"
#include "../steering.h"

// The skeleton AI component is going to manage the attacking and fake death
SkeletonAIComponent::SkeletonAIComponent(Entity* p) : _attacking(false), _fakeDeath(false), Component(p) {}

void SkeletonAIComponent::update(double dt) {}
void SkeletonAIComponent::render() {}

bool SkeletonAIComponent::isAttacking() const { return _attacking; }
void SkeletonAIComponent::setAttacking(bool attacking) { _attacking = attacking; }

bool SkeletonAIComponent::isFakeDeath() const { return _fakeDeath; }
void SkeletonAIComponent::setFakeDeath(bool fakeDeath) { _fakeDeath = fakeDeath; }



// The boss AI component is going to add a healthbar to the skeleton
BossAIComponent::BossAIComponent(Entity* p) : _healthTaken(0), _maxHealth(100), _hurt(false), _isIdle(false), SkeletonAIComponent(p) {}

// Enemy dies when health taken equals max health
void BossAIComponent::update(double dt)
{

}

// The health bar is going to change based on how much life the enemy has left
void BossAIComponent::render()
{
	
}

void BossAIComponent::setHealthBarTexture(std::shared_ptr<sf::Texture> tex) { _healthBarTexture = tex; }
sf::Sprite& BossAIComponent::getHealthBarSprite() const { return *_healthBarSprite; }
void BossAIComponent::addHealthBarIcon(sf::IntRect icon) { _healthBarIcons.push_back(icon); }

bool BossAIComponent::isHurt()const { return _hurt; }
void BossAIComponent::resetHurt() { _hurt = false; }
void BossAIComponent::hurtEnemy(int damage)
{
	// The boss can only be hurt when is idle
	if (_isIdle)
	{
		_hurt = true;
		_healthTaken += damage;
	}
}

void BossAIComponent::setIsIdle(bool idle) { _isIdle = idle; }


//Ghost for level 1
GhostAIComponent::GhostAIComponent(Entity* p) : Component(p) {}

void GhostAIComponent::update(double dt)
{

}

void GhostAIComponent::setTarget(shared_ptr<Entity> target) {
	_target = target;
}

void GhostAIComponent::render() {}
