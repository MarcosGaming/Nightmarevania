#pragma once

#include "ecm.h"
#include <SFML/Graphics/Sprite.hpp>

using namespace std;

class SkeletonAIComponent : public Component 
{
protected:
	bool _attacking;

	bool _fakeDeath;

public:
	SkeletonAIComponent() = delete;
	explicit SkeletonAIComponent(Entity* p);

	virtual void update(double dt) override;
	virtual void render() override;

	bool isAttacking() const;
	void setAttacking(bool);

	bool isFakeDeath() const;
	void setFakeDeath(bool);
};

class BossAIComponent : public SkeletonAIComponent
{
private:
	int _healthTaken;
	int _maxHealth;

	bool _hurt;
	bool _isIdle;

	std::shared_ptr<sf::Texture> _healthBarTexture;
	std::shared_ptr<sf::Sprite> _healthBarSprite;
	std::vector<sf::IntRect> _healthBarIcons;

public:
	BossAIComponent() = delete;
	explicit BossAIComponent(Entity* p);

	void update(double dt) override final;
	void render() override final;

	bool isHurt()const;
	void resetHurt();
	void hurtEnemy(int);

	void setIsIdle(bool);

	void setHealthBarTexture(std::shared_ptr<sf::Texture>);
	sf::Sprite& getHealthBarSprite() const;
	void addHealthBarIcon(sf::IntRect);
};

class GhostAIComponent : public Component
{
protected:
	//bool _isFiring; //not sure if this will be used yet
	float _speed;
	shared_ptr<Entity> _target;

public:
	GhostAIComponent() = delete;
	explicit GhostAIComponent(Entity* p);

	virtual void update(double dt) override;
	void setTarget(shared_ptr<Entity> target);
	virtual void render() override;
};