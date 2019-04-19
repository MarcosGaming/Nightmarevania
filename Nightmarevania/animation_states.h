#pragma once
#include "components\cmp_animation_machine.h"

// Player Animations
class IdleAnimation : public Animation
{
public:
	IdleAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class RunAnimation : public Animation
{
public:
	RunAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class JumpAnimation : public Animation
{
public:
	JumpAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class FallAnimation : public Animation
{
public:
	FallAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class DoubleJumpAnimation : public Animation
{
public:
	DoubleJumpAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class GroundAttackAnimation : public Animation
{
public:
	GroundAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class CircularAttackAnimation : public Animation
{
public:
	CircularAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class AirAttackAnimation : public Animation
{
public:
	AirAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class UpAttackAnimation : public Animation
{
public:
	UpAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class DownAttackAnimation : public Animation
{
public:
	DownAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SmasherDownAttackAnimation : public Animation
{
public:
	SmasherDownAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class DefendingAnimation : public Animation
{
public:
	DefendingAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class HurtAnimation : public Animation
{
public:
	HurtAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class DeathAnimation : public Animation
{
public:
	DeathAnimation() = default;
	void runFrames(Entity*, float) override;
};
class DeathAnimationFall : public DeathAnimation
{
public:
	DeathAnimationFall() = default;
	void execute(Entity*, double) noexcept override;
};
class DeathAnimationGround : public DeathAnimation
{
public:
	DeathAnimationGround() = default;
	void execute(Entity*, double) noexcept override;
};
class GetUpAnimation : public DeathAnimation
{
public:
	GetUpAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Animations for level 1 ghost
class GhostAnimation : public Animation
{
public:
	GhostAnimation() = default;
	void runFrames(Entity* owner, float waitTime) override;
};

class GhostTakeOffAnimation : public GhostAnimation 
{
public:
	GhostTakeOffAnimation() = default;
	void execute(Entity* owner, double dt) noexcept override;
};

class GhostFlyingAnimation : public GhostAnimation 
{
public:
	GhostFlyingAnimation() = default;
	void execute(Entity* owner, double dt) noexcept override;
};

class GhostIdleAnimation : public GhostAnimation 
{
public:
	GhostIdleAnimation() = default;
	void execute(Entity* owner, double dt) noexcept override;
};

// Skeleton Animations
class SkeletonIdleAnimation : public Animation
{
public:
	SkeletonIdleAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SkeletonAttackAnimation : public Animation
{
public:
	SkeletonAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SkeletonSlowWalkAnimation : public Animation
{
public:
	SkeletonSlowWalkAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SkeletonFastWalkAnimation : public Animation
{
public:
	SkeletonFastWalkAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SkeletonHurtAnimation : public Animation
{
public:
	SkeletonHurtAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SkeletonDeathAnimation : public DeathAnimation
{
public:
	SkeletonDeathAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

class SkeletonReviveAnimation : public DeathAnimation
{
public:
	SkeletonReviveAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Portal animations
class PortalOpenAnimation : public Animation
{
public:
	PortalOpenAnimation() = default;
	void runFrames(Entity*, float) override;
	void execute(Entity*, double) noexcept override;
};

class PortalStaticAnimation : public Animation
{
public:
	PortalStaticAnimation() = default;
	void runFrames(Entity*, float) override;
	void execute(Entity*, double) noexcept override;
};