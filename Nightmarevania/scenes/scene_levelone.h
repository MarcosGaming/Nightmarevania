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
	sf::Vector2f screenSize;
	sf::View followPlayer;
	sf::Vector2f centrePoint;
	shared_ptr<Entity> door;
	float leftBoundary = 640.0f;// 967.0f;
	float rightBoundary = 12795.0f;
};

