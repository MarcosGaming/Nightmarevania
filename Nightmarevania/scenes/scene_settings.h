#pragma once

#include "engine.h"

class SettingsScene : public Scene 
{
public:
	SettingsScene() = default;
	~SettingsScene() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};
