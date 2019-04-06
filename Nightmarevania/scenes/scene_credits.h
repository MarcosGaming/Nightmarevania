#pragma once

#include "engine.h"

class CreditsScene : public Scene 
{
public:
	CreditsScene() = default;
	~CreditsScene() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};
