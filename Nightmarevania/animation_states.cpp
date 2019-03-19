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
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
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
	else if (movement->getVelocity().y < -0.1f)
	{
		animation->changeAnimation("Fall");
		current_frame = 0;
	}
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void JumpAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.4f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (movement->canSecondJump())
	{
		animation->changeAnimation("DoubleJump");
		current_frame = 0;
	}
	if (movement->getVelocity().y < -0.1f)
	{
		animation->changeAnimation("Fall");
		current_frame = 0;
	}
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
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
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
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
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

// Death animations have a different runFrames method
void DeathAnimation::runFrames(Entity* owner, float waitTime)
{
	// If the death animation has finished return
	if (current_frame >= frames.size())
	{
		return;
	}
	auto sprite = owner->get_components<SpriteComponent>()[0];
	// Set the frame
	sprite->getSprite().setTextureRect(frames[current_frame]);
	// Change frame
	if (clock.getElapsedTime().asSeconds() > waitTime)
	{
		current_frame++;
		clock.restart();
	}
}
void DeathAnimationFall::execute(Entity* owner, double dt) noexcept
{
	// Animation changes based on the character movement
	runFrames(owner, 0.2f);
	// Animation changes based on the character movement
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	if (movement->getVelocity().y == 0 && current_frame >= frames.size())
	{
		animation->changeAnimation("DeathGround");
		current_frame = 0;
	}
}
void DeathAnimationGround::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.2f);
	if (current_frame >= frames.size())
	{
		owner->setAlive(false);
		auto animation = owner->get_components<AnimationMachineComponent>()[0];
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
}