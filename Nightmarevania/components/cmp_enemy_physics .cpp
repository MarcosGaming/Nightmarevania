#include "cmp_enemy_physics .h"
#include "system_physics.h"

using namespace std;
using namespace sf;
using namespace Physics;

EnemyPhysicsComponent::EnemyPhysicsComponent(Entity* p, const Vector2f& size) : PhysicsComponent(p, true, size)
{
	_size = sv2_to_bv2(size, true);
	_maxSlowVelocity = Vector2f(50.0f, 100.0f);
	_maxFastVelocity = Vector2f(200.0f, 100.0f);
	_moveDirection = Vector2f(1.0f, 0.0f);
	_groundspeed = 30.f;
	_grounded = false;
	_slow = true;
	_canMove = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
}

void EnemyPhysicsComponent::update(double dt)
{
	if (_canMove)
	{
		static float waitForImpulseUp = 0.2f;
		waitForImpulseUp -= (float)dt;
		if (_slow)
		{
			if (getVelocity().x < _maxSlowVelocity.x)
			{
				impulse(Vector2f(_moveDirection.x * (float)(dt*10.0f), 0));
			}
			// Clamp velocity.
			auto v = getVelocity();
			v.x = copysign(min(abs(v.x), _maxSlowVelocity.x), v.x);
			v.y = copysign(min(abs(v.y), _maxSlowVelocity.y), v.y);
			setVelocity(v);
		}
		else
		{
			if (getVelocity().x < _maxFastVelocity.x)
			{
				impulse(Vector2f(_moveDirection.x * (float)(dt*20.0f), 0));
			}
			// Clamp velocity.
			auto v = getVelocity();
			v.x = copysign(min(abs(v.x), _maxFastVelocity.x), v.x);
			v.y = copysign(min(abs(v.y), _maxFastVelocity.y), v.y);
			setVelocity(v);
		}
		// Impulse up so the enemy does not gets stuck on the ground
		if (waitForImpulseUp <= 0.0f)
		{
			impulse(Vector2f(0.0f, -1.0f));
			waitForImpulseUp = 0.2f;
		}
	}
	else
	{
		setVelocity(Vector2f(0.0f, 0.0f));
	}
	//Are we in air?
	if (!_grounded)
	{
		// Check to see if we have landed yet
		_grounded = isGrounded();
		// disable friction while jumping
		setFriction(0.f);
	}
	else
	{
		setFriction(0.1f);
	}

	PhysicsComponent::update(dt);
}

bool EnemyPhysicsComponent::isGrounded() const
{
	auto touch = getTouching();
	const auto& pos = _body->GetPosition();
	const float halfPlrHeigt = _size.y * .5f;
	const float halfPlrWidth = _size.x * .52f;
	b2WorldManifold manifold;
	for (const auto& contact : touch)
	{
		contact->GetWorldManifold(&manifold);
		const int numPoints = contact->GetManifold()->pointCount;
		bool onTop = numPoints > 0;
		// If all contacts are below the player.
		for (int j = 0; j < numPoints; j++)
		{
			onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
		}
		if (onTop)
		{
			return true;
		}
	}

	return false;
}

void EnemyPhysicsComponent::setCanMove(bool canMove) { _canMove = canMove; }
bool EnemyPhysicsComponent::canMove() const { return _canMove; }

void EnemyPhysicsComponent::setSlow(bool slow) { _slow = slow; }
bool EnemyPhysicsComponent::isSlowMove() const { return _slow; }

void EnemyPhysicsComponent::setMoveDirection(Vector2f direction) { _moveDirection = direction; }
