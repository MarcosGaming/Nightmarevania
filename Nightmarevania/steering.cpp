#include "steering.h"

using namespace sf;

SteeringOutput Seek::getSteering() const noexcept
{
	SteeringOutput steering;
	steering.direction = normalize(_target->getPosition() - _character->getPosition());
	steering.rotation = 0.0f;
	steering.direction *= _speed;
	return steering;
}
