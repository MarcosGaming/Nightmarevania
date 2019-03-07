#pragma once

#include "engine.h"

class MenuScene : public Scene 
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void Load() override;

	void Update(const double& dt) override;
};
