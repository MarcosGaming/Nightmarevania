#pragma once
#include "components\cmp_animation_machine.h"

// Player Animations

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
	void runFrames(Entity*, float) override;
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

// Get up animation
class GetUpAnimation : public DeathAnimation
{
public:
	GetUpAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

//Animation for L1 ghost
class GhostAnimation : public Animation
{
public:
	GhostAnimation() = default;
	void runFrames(Entity* owner, float waitTime) override;
};

class GhostTakeOffAnimation : public GhostAnimation {
public:
	GhostTakeOffAnimation() = default;
	void execute(Entity* owner, double dt) noexcept override;
};

class GhostFlyingAnimation : public GhostAnimation {
public:
	GhostFlyingAnimation() = default;
	void execute(Entity* owner, double dt) noexcept override;
};

class GhostIdleAnimation : public GhostAnimation {
public:
	GhostIdleAnimation() = default;
	void execute(Entity* owner, double dt) noexcept override;
};

// Skeleton Animations

// Iddle animation
class SkeletonIdleAnimation : public Animation
{
public:
	SkeletonIdleAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Attack animation
class SkeletonAttackAnimation : public Animation
{
public:
	SkeletonAttackAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Slow walk animation
class SkeletonSlowWalkAnimation : public Animation
{
public:
	SkeletonSlowWalkAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Fast walk animation
class SkeletonFastWalkAnimation : public Animation
{
public:
	SkeletonFastWalkAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Hurt animation
class SkeletonHurtAnimation : public Animation
{
public:
	SkeletonHurtAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Die animation
class SkeletonDeathAnimation : public DeathAnimation
{
public:
	SkeletonDeathAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Revive animation
class SkeletonReviveAnimation : public DeathAnimation
{
public:
	SkeletonReviveAnimation() = default;
	void execute(Entity*, double) noexcept override;
};

// Open portal animation
class PortalOpenAnimation : public Animation
{
public:
	PortalOpenAnimation() = default;
	void runFrames(Entity*, float) override;
	void execute(Entity*, double) noexcept override;
};

// Static portal animation
class PortalStaticAnimation : public Animation
{
public:
	PortalStaticAnimation() = default;
	void runFrames(Entity*, float) override;
	void execute(Entity*, double) noexcept override;
};