#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PlayerHurtComponent : public Component 
{
protected:
	std::weak_ptr<Entity> _player;

public:
	PlayerHurtComponent() = delete;
	explicit PlayerHurtComponent(Entity* p);

	void update(double dt) override;
	void render() override {}
};

class BossHurtComponent : public Component
{
protected:
	std::weak_ptr<Entity> _boss;

public:
	BossHurtComponent() = delete;
	explicit BossHurtComponent(Entity* p);

	void update(double dt) override;
	void render() override {}
};

class PlayerKillComponent : public Component
{
protected:
	std::weak_ptr<Entity> _player;

public:
	PlayerKillComponent() = delete;
	explicit PlayerKillComponent(Entity* p);

	void update(double dt) override;
	void render() override {}
};
