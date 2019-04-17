#pragma once

#include "ecm.h"
#include <SFML/Graphics/Sprite.hpp>

class SkeletonAIComponent : public Component 
{
protected:
	bool _attacking;

	bool _death;

public:
	SkeletonAIComponent() = delete;
	explicit SkeletonAIComponent(Entity* p);

	virtual void update(double dt) override;
	virtual void render() override;

	bool isAttacking() const;
	void setAttacking(bool);

	bool isDeath() const;
	void setDeath(bool);
};

class BossAIComponent : public SkeletonAIComponent
{
private:
	int _healthTaken;
	int _maxHealth;

	bool _hurt;
	bool _isIddle;

	bool _hideHealthBar;

	std::shared_ptr<sf::Texture> _healthBarTexture;
	std::shared_ptr<sf::Sprite> _healthBarSprite;

	std::shared_ptr<sf::Texture> _foregroundTexture;
	std::shared_ptr<sf::Sprite> _foregroundSprite;

public:
	BossAIComponent() = delete;
	explicit BossAIComponent(Entity* p);

	void update(double dt) override final;
	void render() override final;

	bool isHurt()const;
	void resetHurt();
	void hurtEnemy(int);

	void setIsIddle(bool);

	void setHealthBarTexture(std::shared_ptr<sf::Texture>);
	sf::Sprite& getHealthBarSprite() const;

	void setForegroundTexture(std::shared_ptr<sf::Texture>);
	sf::Sprite& getForegroundSprite() const;

	int getHealthTaken() const;

};
