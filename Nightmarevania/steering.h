#pragma once

#include <engine.h>

struct SteeringOutput
{
	// Direction of travel
	sf::Vector2f direction;
	// Rotation of travel
	float rotation;
};

class SteeringBehaviour
{
public:
	virtual ~SteeringBehaviour() = default;

	// Get the output from a steering behaviour
	virtual SteeringOutput getSteering() const noexcept = 0;
};

class Seek : public SteeringBehaviour
{
private:
	Entity * _character;
	Entity*  _target;
	float _speed;

public:
	Seek() = delete;
	Seek(Entity* character, Entity* target, float speed) : _character(character), _target(target),  _speed(speed){};

	SteeringOutput getSteering() const noexcept override;
};