#include "animation_states.h"
#include "components\cmp_player_physics.h"
#include "components/cmp_player_combat.h"
#include "maths.h"
#include <SFML\Graphics.hpp>

void IddleAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.3f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>();
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the character movement
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
	// Animation changes based on the combat
	if (!combat.empty() && combat[0]->isBasicAttack())
	{
		animation->changeAnimation("GroundAttack");
	}
	else if (!combat.empty() && (combat[0]->isCircularAttackRight() || combat[0]->isCircularAttackLeft()))
	{
		animation->changeAnimation("CircularAttack");
	}
	else if (!combat.empty() && combat[0]->isUpAttack())
	{
		animation->changeAnimation("UpAttack");
	}
	else if (!combat.empty() && combat[0]->isDefending())
	{
		animation->changeAnimation("Defending");
		current_frame = 0;
	}
	// Animation changes based on entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void RunAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.1f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>();
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the character movement
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
	// Animation changes based on the combat component
	if (!combat.empty() && combat[0]->isBasicAttack())
	{
		movement->setVelocity(sf::Vector2f(0.0f, 0.0f));
		animation->changeAnimation("GroundAttack");
	}
	else if (!combat.empty() && (combat[0]->isCircularAttackRight() || combat[0]->isCircularAttackLeft()))
	{
		movement->setVelocity(sf::Vector2f(0.0f, 0.0f));
		animation->changeAnimation("CircularAttack");
	}
	else if (!combat.empty() && combat[0]->isUpAttack())
	{
		movement->setVelocity(sf::Vector2f(0.0f, 0.0f));
		animation->changeAnimation("UpAttack");
	}
	else if (!combat.empty() && combat[0]->isDefending())
	{
		animation->changeAnimation("Defending");
		current_frame = 0;
	}
	// Aniamtion changes based on entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void JumpAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.4f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>();
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the character movement
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
	// Animation changes based on the combat component
	if (!combat.empty() && combat[0]->isBasicAttack())
	{
		animation->changeAnimation("AirAttack");
		current_frame = 0;
	}
	if (!combat.empty() && combat[0]->isDownAttack())
	{
		animation->changeAnimation("DownAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void DoubleJumpAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.14f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>();
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the character movement
	if (movement->getVelocity().y < -0.1f)
	{
		animation->changeAnimation("Fall");
		current_frame = 0;
	}
	// Animation changes based on the combat component
	if (!combat.empty() && combat[0]->isBasicAttack())
	{
		animation->changeAnimation("AirAttack");
		current_frame = 0;
	}
	if (!combat.empty() && combat[0]->isDownAttack())
	{
		animation->changeAnimation("DownAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void FallAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.2f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>();
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the character movement
	if (movement->getVelocity().y > -0.1f)
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	// Animation changes based on the combat component
	if (!combat.empty() && combat[0]->isBasicAttack())
	{
		animation->changeAnimation("AirAttack");
	}
	if (!combat.empty() && (combat[0]->isCircularAttackRight() || combat[0]->isCircularAttackLeft()))
	{
		animation->changeAnimation("CircularAttack");
	}
	if (!combat.empty() && combat[0]->isDownAttack())
	{
		animation->changeAnimation("DownAttack");
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void GroundAttackAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.08f);
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the combat component
	if (!combat->isAttacking())
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	if (combat->isCircularAttackRight() || combat->isCircularAttackLeft())
	{
		animation->changeAnimation("CircularAttack");
		current_frame = 0;
	}
	if (combat->isUpAttack())
	{
		animation->changeAnimation("UpAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
		current_frame = 0;
	}
}

void CircularAttackAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.15f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the combat component
	if (!combat->isAttacking())
	{
		movement->setVelocity(sf::Vector2f(0.0f, 0.0f));
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	if (combat->isBasicAttack() && current_frame == 0)
	{
		movement->setVelocity(sf::Vector2f(0.0f, 0.0f));
		animation->changeAnimation("GroundAttack");
	}
	if (combat->isUpAttack())
	{
		animation->changeAnimation("UpAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
		current_frame = 0;
	}
}

void AirAttackAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.1f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// ANimation changes based on combat and wheter the character is on the ground or not
	if (!combat->isAttacking())
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	if (movement->isGrounded() && combat->isAttacking())
	{
		animation->changeAnimation("GroundAttack");
		current_frame = 0;
	}
	if (movement->isGrounded() && (combat->isCircularAttackLeft() || combat->isCircularAttackRight()))
	{
		animation->changeAnimation("CircularAttack");
		current_frame = 0;
	}
	if (combat->isDownAttack())
	{
		animation->changeAnimation("DownAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}

void UpAttackAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.18f);
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on the combat component
	if (!combat->isAttacking())
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	if (combat->isBasicAttack() && current_frame == 0)
	{
		animation->changeAnimation("AirAttack");
		current_frame = 0;
	}
	if (combat->isDownAttack())
	{
		animation->changeAnimation("DownAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
		current_frame = 0;
	}
}

void DownAttackAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.15f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// ANimation changes based on combat and wheter the character is on the ground or not
	if (!combat->isAttacking())
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	if (movement->isGrounded())
	{
		animation->changeAnimation("SmasherDownAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
	}
}
void SmasherDownAttackAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.15f);
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// ANimation changes based on combat 
	if (!combat->isAttacking())
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
	if (combat->isBasicAttack() && current_frame == 0)
	{
		animation->changeAnimation("GroundAttack");
		current_frame = 0;
	}
	if ((combat->isCircularAttackLeft() || combat->isCircularAttackRight()) && current_frame == 0)
	{
		animation->changeAnimation("CircularAttack");
		current_frame = 0;
	}
	// Animation changes based on the entity state
	if (owner->isDead())
	{
		animation->changeAnimation("DeathFall");
		current_frame = 0;
	}
}

void DefendingAnimation::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.15f);
	auto combat = owner->get_components<PlayerCombatComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes based on combat
	if (!combat->isDefending())
	{
		animation->changeAnimation("Iddle");
		current_frame = 0;
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
	// Set in which direction the sprite should be facing
	setFacing(owner);
	// Change frame
	if (clock.getElapsedTime().asSeconds() > waitTime)
	{
		current_frame++;
		clock.restart();
	}
}
void DeathAnimationFall::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.2f);
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	auto animation = owner->get_components<AnimationMachineComponent>()[0];
	// Animation changes when character is on the ground and the animation has finished
	if (movement->getVelocity().y == 0 && current_frame >= frames.size())
	{
		animation->changeAnimation("DeathGround");
		current_frame = 0;
	}
}
void DeathAnimationGround::execute(Entity* owner, double dt) noexcept
{
	runFrames(owner, 0.2f);
	// Animation changes when it finishes
	if (current_frame >= frames.size())
	{
		owner->setAlive(false);
		auto animation = owner->get_components<AnimationMachineComponent>()[0];
		animation->changeAnimation("Iddle");
		current_frame = 0;
	}
}