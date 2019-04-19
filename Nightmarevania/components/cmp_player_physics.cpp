#include "cmp_player_physics.h"
#include "cmp_player_combat.h"
#include "system_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <system_sound.h>
#include <system_controller.h>
#include <engine.h>

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
	_canMove = true;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	//Bullet items have higher-res collision detection
	_body->SetBullet(true);
}

void PlayerPhysicsComponent::update(double dt)
{
	const auto pos = _parent->getPosition();

	auto combat = _parent->get_components<PlayerCombatComponent>();

	// This works as what sets the alive state of the character is the animation which is updated before this
	if (!_parent->isAlive())
	{
		if (_parent->scene == &levelOne || _parent->scene == &levelTwo)
		{
			Engine::ChangeScene(&levelOutside);
		}
		else
		{
			Engine::ChangeScene(&levelSword);
		}
	}
	// When the player is attacking the physics behave differently
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
	else if (!combat.empty() && combat[0]->isHurt())
	{
		setVelocity(Vector2f(0.0f, getVelocity().y));
	}
	else
	{
		if (_canMove && (Controller::isPressed(Controller::MoveRightButton) || Controller::isPressed(Controller::MoveLeftButton)))
		{
			// Moving Either Left or Right
			if (Controller::isPressed(Controller::MoveRightButton))
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
		if (_canMove && Controller::isPressed(Controller::JumpButton))
		{
			_grounded = isGrounded();
			if (_grounded)
			{
				setVelocity(Vector2f(getVelocity().x, 0.f));
				teleport(Vector2f(pos.x, pos.y - 2.0f));
				impulse(Vector2f(0, -7.2f));
				impulseDown = false;
			}
			// Only double jump once after releasing space
			else if (Controller::isJumpButtonReleased() && !_secondJump)
			{
				impulse(Vector2f(0, -7.2f + _body->GetLinearVelocity().y));
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
		Controller::setJumpButtonReleased(false);
		_secondJump = false;
	}

	// Death 
	if (ls::getTileAt(pos) == ls::SPIKES1 || ls::getTileAt(pos) == ls::SPIKES2) {
		_parent->setDeath(true);
	}
	// If the player is dead velocity is 0 on x and 0 on y if not in the air
	if (_parent->isDead() && isGrounded())
	{
		setVelocity(Vector2f(0.0f, 0.0f));
		//teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}
	else if (_parent->isDead() && !isGrounded())
	{
		setVelocity(Vector2f(0.0f, getVelocity().y));
		//teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
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

bool PlayerPhysicsComponent::isSecondJump() const
{
	return _secondJump;
}

void PlayerPhysicsComponent::setCanMove(bool canMove)
{
	_canMove = canMove;
}
