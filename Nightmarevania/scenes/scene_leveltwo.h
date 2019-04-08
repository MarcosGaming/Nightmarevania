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
	float leftBoundary = 967.0f;
	float rightBoundary = 2795.0f;
	float topBoundary = -470.0f;
	float bottomBoundary = 607.0f;
};

