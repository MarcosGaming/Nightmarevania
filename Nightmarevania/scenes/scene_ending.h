#pragma once

#include "engine.h"

class EndingScene : public Scene 
{
public:
	EndingScene() = default;
	~EndingScene() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;
};
