/*
using namespace sf;

SteeringComponent::SteeringComponent(Entity* p, Entity* player)
	: _player(player), _seek(Seek(p, player, 100.0f)),
	_flee(Flee(p, player, 100.0f)), Component(p) {}

bool SteeringComponent::validMove(const Vector2f& pos) const
{
	if (pos.x < 0.0f || pos.x > Engine::getWindowSize().x ||
		pos.y < 0.0f || pos.y > Engine::getWindowSize().y)
	{
		return false;
	}
	return true;
}

void SteeringComponent::update(double dt)
{
	// If the target(player) is more than 100 pixels away then seek
	if (length(_parent->getPosition() - _player->getPosition()) > 100.0f)
	{
		auto output = _seek.getSteering();
		move(output.direction * (float)dt);
	}
	// If target(player) is less than 50 pixels away flee
	if (length(_parent->getPosition() - _player->getPosition()) < 50.0f)
	{
		auto output = _flee.getSteering();
		move(output.direction * (float)dt);
	}

}

void SteeringComponent::move(const sf::Vector2f& pos)
{
	auto new_pos = _parent->getPosition() + pos;
	if (validMove(new_pos))
	{
		_parent->setPosition(new_pos);
	}
}

void SteeringComponent::move(float x, float y)
{
	move(Vector2f(x, y));
}*/