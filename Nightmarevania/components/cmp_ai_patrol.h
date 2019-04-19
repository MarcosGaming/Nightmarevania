#pragma once
#include "ecm.h"
#include "../steering.h"

class AIPatrolComponent : public Component
{
protected:
	std::shared_ptr<Patrol> _patrol;
	sf::Vector2f _positionA;
	sf::Vector2f _positionB;

public:
	AIPatrolComponent() = delete;
	explicit AIPatrolComponent(Entity*, sf::Vector2f, sf::Vector2f);
	
	void update(double) override;
	void render() override { }

	std::shared_ptr<Patrol> getPatrol() const;
};