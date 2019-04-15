#pragma once
#include "components\cmp_animation_machine.h"

// Idle animation
class IdleAnimation : public Animation
{
public:
	IdleAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Run animation
class RunAnimation : public Animation
{
public:
	RunAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Jump animation
class JumpAnimation : public Animation
{
public:
	JumpAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Fall animation
class FallAnimation : public Animation
{
public:
	FallAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Double jump animation
class DoubleJumpAnimation : public Animation
{
public:
	DoubleJumpAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Ground attack animation
class GroundAttackAnimation : public Animation
{
public:
	GroundAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Circular attack animation
class CircularAttackAnimation : public Animation
{
public:
	CircularAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Air attack animation
class AirAttackAnimation : public Animation
{
public:
	AirAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// UP attack animation
class UpAttackAnimation : public Animation
{
public:
	UpAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Down attack animation 
class DownAttackAnimation : public Animation
{
public:
	DownAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Smasher after down attack animation
class SmasherDownAttackAnimation : public Animation
{
public:
	SmasherDownAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Defending animation
class DefendingAnimation : public Animation
{
public:
	DefendingAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Hurt animation
class HurtAnimation : public Animation
{
public:
	HurtAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// General death animation
class DeathAnimation : public Animation
{
public:
	DeathAnimation() = default;
	void runFrames(Entity* owner, float waitTime) override;
};

// Death fall animation
class DeathAnimationFall : public DeathAnimation
{
public:
	DeathAnimationFall() = default;
	void execute(Entity*, double) noexcept override;
};

// Death ground animation
class DeathAnimationGround : public DeathAnimation
{
public:
	DeathAnimationGround() = default;
	void execute(Entity*, double) noexcept override;
};
