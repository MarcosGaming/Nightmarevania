#pragma once

#include "engine.h"

class LevelTwo : public Scene
{
public:
	LevelTwo() = default;
	~LevelTwo() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;

private:
	sf::Vector2f screenSize;
	sf::View followPlayer;
	sf::Vector2f centrePoint;
	shared_ptr<Entity> door;
	float leftBoundary = 0.0f;
	float rightBoundary = 50795.0f;
	float topBoundary = -470.0f;
	float bottomBoundary = 360.0f;//607.0f;
};

