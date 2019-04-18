#pragma once

#include "engine.h"

class LevelSword : public Scene
{
public:
	LevelSword() = default;
	~LevelSword() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;

private:
	shared_ptr<Entity> door;
};

