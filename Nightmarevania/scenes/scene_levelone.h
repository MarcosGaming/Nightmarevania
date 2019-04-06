#pragma once

#include "engine.h"

class LevelOne : public Scene
{
public:
	LevelOne() = default;
	~LevelOne() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};

