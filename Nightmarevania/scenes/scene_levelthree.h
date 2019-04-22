#pragma once

#include "engine.h"
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

class LevelThree : public Scene
{
public:
	LevelThree() = default;
	~LevelThree() = default;

	void Load() override;
	void UnLoad() override;

	void Update(const double& dt) override;
	void Render() override;

private:
	shared_ptr<Texture> background_tex;
	shared_ptr<Sprite> background_image;
};

