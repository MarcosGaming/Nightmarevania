#pragma once

#include "engine.h"

class LevelThree : public Scene
{
public:
	LevelThree() = default;
	~LevelThree() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};

