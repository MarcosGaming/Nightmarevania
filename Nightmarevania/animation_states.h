#pragma once
#include "components\cmp_animation_machine.h"

// Iddle animation
class IddleAnimation : public Animation
{
public:
	IddleAnimation() = default;
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

// Static attack animation
class StaticAttackAnimation : public Animation
{
public:
	StaticAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Run attack animation
class RunAttackAnimation : public Animation
{
public:
	RunAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Jump attack animation
class JumpAttackAnimation : public Animation
{
public:
	JumpAttackAnimation() = default;
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