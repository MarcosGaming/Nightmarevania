#include "cmp_player_physics.h"
#include "system_physics.h"
#include "cmp_player_combat.h"
#include <engine.h>
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const Vector2f& size) : PhysicsComponent(p, true, size) 
{
	_size = sv2_to_bv2(size, true);
	_maxVelocity = Vector2f(200.f, 400.f);
	_groundspeed = 30.f;
	_grounded = false;
	_secondJump = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
}

void PlayerPhysicsComponent::update(double dt)
{
	const auto pos = _parent->getPosition();

	//Teleport to start if we fall off map or if is not alive
	if (pos.y > ls::getHeight() * ls::getTileSize() || !_parent->isAlive())
	{
		teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		_parent->setAlive(true);
		_parent->setDeath(false);
	}

	// When the player is attacking the physics behave differently
	auto combat = _parent->get_components<PlayerCombatComponent>();
	if (!combat.empty() && combat[0]->isAttacking())
	{
		if (isGrounded())
		{
			if (combat[0]->isCircularAttackRight())
			{
				impulse(Vector2f(1.0f, 0.0f));
			}
			else if (combat[0]->isCircularAttackLeft())
			{
				impulse(Vector2f(-1.0f, 0.0f));
			}
			else if (combat[0]->isUpAttack())
			{
				impulse(Vector2f(0.0f, -8.0f));
			}
			else
			{
				auto v = getVelocity();
				v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
				v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
				setVelocity(v);
			}
		}
		else
		{
			if (combat[0]->isDownAttack())
			{
				impulse(Vector2f(0.0f, 8.0f));
			}
			setVelocity(Vector2f(0.0f, getVelocity().y));
		}
		
	}
	else if (!combat.empty() && combat[0]->isDefending())
	{
		setVelocity(Vector2f(0.0f, 0.0f));
	}
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D))
		{
			// Moving Either Left or Right
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				if (getVelocity().x < _maxVelocity.x)
					impulse({ (float)(dt * _groundspeed), 0 });
			}
			else
			{
				if (getVelocity().x > -_maxVelocity.x)
					impulse({ -(float)(dt * _groundspeed), 0 });
			}
		}
		else
		{
			// Dampen X axis movement
			dampen({ 0.9f, 1.0f });
		}

		static bool impulseDown = false;
		// Handle Jump
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			_grounded = isGrounded();
			if (_grounded)
			{
				setVelocity(Vector2f(getVelocity().x, 0.f));
				teleport(Vector2f(pos.x, pos.y - 2.0f));
				impulse(Vector2f(0, -6.8f));
				impulseDown = false;
			}
			// Only double jump once after releasing space
			else if (Physics::getCanDoubleJump() && !_secondJump)
			{
				impulse(Vector2f(0, -6.8f + _body->GetLinearVelocity().y));
				_secondJump = true;
				impulseDown = false;
			}
		}
		// Make the player fall faster
		if (getVelocity().y < -1.0f && !impulseDown)
		{
			impulse(Vector2f(0, 2.0f));
			impulseDown = true;
		}

		// Clamp velocity.
		auto v = getVelocity();
		v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
		v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
		setVelocity(v);
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
		Physics::setCanDoubleJump(false);
		_secondJump = false;
	}

	// Death testing
	if (Keyboard::isKeyPressed(Keyboard::U) && !combat[0]->isDefending())
	{
		combat[0]->hurtPlayer(10);
	}
	// If the player is dead velocity is 0 on x and 0 on y if not in the air
	if (_parent->isDead() && isGrounded())
	{
		setVelocity(Vector2f(0.0f, 0.0f));
	}
	else if (_parent->isDead() && !isGrounded())
	{
		setVelocity(Vector2f(0.0f, getVelocity().y));
	}

	PhysicsComponent::update(dt);
}

bool PlayerPhysicsComponent::isGrounded() const
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

bool PlayerPhysicsComponent::canSecondJump() const
{
	return _secondJump;
}
