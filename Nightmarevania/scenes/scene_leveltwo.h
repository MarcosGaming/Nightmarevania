#pragma once

#include "engine.h"
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

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
	vector<shared_ptr<Entity>> levelDoors;
	float leftBoundary = 640.0f;
	float rightBoundary = 4820.0f;
	float topBoundary = -1500.0f;
	float bottomBoundary = 355.0f;
	shared_ptr<Texture> background_tex;
	shared_ptr<Sprite> background_image;
};

