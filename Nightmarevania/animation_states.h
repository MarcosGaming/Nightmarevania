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