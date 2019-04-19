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

private:
	Vector2f screenSize;
	View followPlayer;
	Vector2f centrePoint;
	shared_ptr<Entity> door;
	shared_ptr<Entity> ghost;
	float leftBoundary = 640.0f;
	float rightBoundary = 21795.0f;
};

