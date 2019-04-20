#include "steering.h"

// Returns the direction and rotation that needs to be used for the movement
SteeringOutput Seek::getSteering() const noexcept
{
	SteeringOutput steering;
	steering.direction = normalize(_target->getPosition() - _character->getPosition());
	steering.rotation = 0.0f;
	steering.direction *= _speed;
	return steering;
}

// Returns the direction of the point that needs to be visited next
SteeringOutput Patrol::getSteering() const noexcept
{
	SteeringOutput steering;
	// Direction of travel is going to be towards B
	if (_wasLastPositionA)
	{
		steering.direction = normalize(_positionB - _character->getPosition());
		//steering.direction.x = _positionB.x - _character->getPosition().x;
		//steering.rotation = 180.0f;
	}
	// Direction of travel is going to be towards A
	else
	{
		steering.direction = normalize(_positionA - _character->getPosition());
		//steering.direction.x = _positionA.x - _character->getPosition().x;
		//steering.rotation = 0.0f;
	}

	steering.rotation = 0.0f;
	return steering;
}

void Patrol::setWasLastPositionA(const bool status)
{
	_wasLastPositionA = status;
}