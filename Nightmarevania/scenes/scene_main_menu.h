#pragma once

#include "engine.h"

class MainMenuScene : public Scene 
{
public:
	MainMenuScene() = default;
	~MainMenuScene() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};
