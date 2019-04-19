#pragma once

#include "engine.h"

class LevelOutside : public Scene
{
public:
	LevelOutside() = default;
	~LevelOutside() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};

