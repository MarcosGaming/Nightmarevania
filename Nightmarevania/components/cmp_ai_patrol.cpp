#include "cmp_ai_patrol.h"
#include "LevelSystem.h"

AIPatrolComponent::AIPatrolComponent(Entity* p, sf::Vector2f pointA, sf::Vector2f pointB) : _positionA(pointA), _positionB(pointB), Component(p) 
{
	_patrol = make_shared<Patrol>(p, pointA, pointB);
}

void AIPatrolComponent::update(double dt)
{
	if (_parent->getPosition().x <= _positionA.x || ls::getTileAt(Vector2f(_parent->getPosition().x + 60.0f, _parent->getPosition().y)) == ls::WALL || ls::getTileAt(Vector2f(_parent->getPosition().x + 60.0f, _parent->getPosition().y)) == ls::FLOOR || ls::getTileAt(Vector2f(_parent->getPosition().x + 60.0f, _parent->getPosition().y - 60.0f)) == ls::EMPTY)
	{
		_patrol->setWasLastPositionA(true);
	}
	else if(_parent->getPosition().x >= _positionB.x || ls::getTileAt(Vector2f(_parent->getPosition().x - 60.0f, _parent->getPosition().y)) == ls::WALL || ls::getTileAt(Vector2f(_parent->getPosition().x - 60.0f, _parent->getPosition().y)) == ls::FLOOR || ls::getTileAt(Vector2f(_parent->getPosition().x - 60.0f, _parent->getPosition().y - 60.0f)) == ls::EMPTY)
	{
		_patrol->setWasLastPositionA(false);
	}
}

std::shared_ptr<Patrol> AIPatrolComponent::getPatrol() const
{
	return _patrol;
}