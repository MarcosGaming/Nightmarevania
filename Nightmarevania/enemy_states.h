#pragma once

#include "steering.h"
#include "components\cmp_state_machine.h"

class StationaryState : public State
{
public:
	StationaryState() = default;
	void execute(Entity*, double) noexcept override;
};

class SeekSlowState : public State
{
private:
	Seek _steering;

public:
	SeekSlowState(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
	void execute(Entity*, double) noexcept override;
};

class SeekFastState : public State
{
private:
	Seek _steering;

public:
	SeekFastState(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
	void execute(Entity*, double) noexcept override;
};

class SeekLimitsState : public State
{
private:
	Seek _steering;
	sf::Vector2f _positionA;
	sf::Vector2f _positionB;

public:
	SeekLimitsState(std::shared_ptr<Entity>, std::shared_ptr<Entity>, sf::Vector2f, sf::Vector2f);
	void execute(Entity*, double) noexcept override;
};

class PatrolState : public State
{
public:
	PatrolState() = default;
	void execute(Entity*, double) noexcept override;
};

class AttackState : public State
{
public:
	AttackState() = default;
	void execute(Entity*, double) noexcept override;
};

class DeathState : public State
{
public:
	DeathState() = default;
	void execute(Entity*, double) noexcept override;
};

class FakeDeathState : public State
{
public:
	FakeDeathState() = default;
	void execute(Entity*, double) noexcept override;
};

class ReviveState : public State
{
public:
	ReviveState() = default;
	void execute(Entity*, double) noexcept override;
};
