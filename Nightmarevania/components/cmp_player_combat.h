#pragma once
#include <ecm.h>
#include <SFML/Graphics/Sprite.hpp>

class PlayerCombatComponent : public Component 
{
protected:
	bool _attacking;

	int _attackDamage;

	bool _defending;
	float _defendingTime;
	float _defendingCooldown;

	bool _basicAttack;

	bool _circularAttackRight;
	bool _circularAttackLeft;
	float _circularAttackCooldown;

	bool _upAttack;
	float _upAttackCooldown;

	bool _downAttack;
	float _downAttackCooldown;

	int _healthTaken;
	int _maxHealth;

	bool _hurt;
	float _hurtCooldown;

	std::shared_ptr<sf::Texture> _combatIconsTexture;

	std::shared_ptr<sf::Sprite> _healthBarSprite;
	std::shared_ptr<sf::Sprite> _upAttackSprite;
	std::shared_ptr<sf::Sprite> _circularAttackSprite;
	std::shared_ptr<sf::Sprite> _downAttackSprite;
	std::shared_ptr<sf::Sprite> _defendSprite;

	std::vector<sf::IntRect> _healthBarIcons;
	std::vector<sf::IntRect> _upAttackIcons;
	std::vector<sf::IntRect> _circularAttackIcons;
	std::vector<sf::IntRect> _downAttackIcons;
	std::vector<sf::IntRect> _defendIcons;

public:
	PlayerCombatComponent() = delete;
	explicit PlayerCombatComponent(Entity* p);

	void update(double dt) override;
	void render() override;

	bool isAttacking() const;

	bool isDefending() const;

	bool isBasicAttack() const;

	bool isCircularAttackRight() const;

	bool isCircularAttackLeft() const;

	bool isUpAttack() const;

	bool isDownAttack() const;

	bool isHurt() const;

	void resetHurt();

	void hurtPlayer(int);

	void resetHealth();

	int getAttackDamage() const;

	void setTexture(std::shared_ptr<sf::Texture> tex);

	sf::Sprite& getHealthBarSprite() const;

	sf::Sprite& getUpAttackSprite() const;

	sf::Sprite& getCircularAttackSprite() const;

	sf::Sprite& getDownAttackSprite() const;

	sf::Sprite& getDefendSprite() const;

	void addHealthBarIcon(sf::IntRect);

	void addUpAttackIcon(sf::IntRect);

	void addDownAttackIcon(sf::IntRect);

	void addCircularAttackIcon(sf::IntRect);

	void addDefendIcon(sf::IntRect);
};
