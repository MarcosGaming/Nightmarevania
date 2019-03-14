#include "animation_states.h"
#include "components\cmp_player_physics.h"
#include "maths.h"
#include <SFML\Graphics.hpp>

void IddleAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.3f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (movement->getVelocity().y > 0.1f)
	{
		animation->changeAnimation("Jump");
	}
	else if (movement->getVelocity().y < -0.1f)
	{
		animation->changeAnimation("Fall");
	}
	else if (std::abs(movement->getVelocity().x) > 0.1f)
	{
		animation->changeAnimation("Run");
	}
}

void RunAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.1f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (std::abs(movement->getVelocity().x) < 0.1f)
	{
		animation->changeAnimation("Iddle");
	}
	else if (movement->getVelocity().y > 0.1f)
	{
		animation->changeAnimation("Jump");
	}
}

void JumpAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.4f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (movement->_secondJump)
	{
		animation->changeAnimation("DoubleJump");
		current_frame = 0;
	}
	if (movement->getVelocity().y < -0.1f)
	{
		animation->changeAnimation("Fall");
		current_frame = 0;
	}
}

void DoubleJumpAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.14f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (movement->getVelocity().y < -0.1f)
	{
		animation->changeAnimation("Fall");
		current_frame = 0;
	}
}

void FallAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.2f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (movement->getVelocity().y > -0.1f)
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
}