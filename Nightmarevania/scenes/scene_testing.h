#pragma once

#include "engine.h"

class TestingScene : public Scene 
{
public:
	TestingScene() = default;
	~TestingScene() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};
