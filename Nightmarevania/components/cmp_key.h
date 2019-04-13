#pragma once

#include <../lib_ecm/ecm.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class KeyComponent : public Component
{
private:
	int tileSize = 32;
	shared_ptr<sf::Sprite> _keySprite;
	shared_ptr<sf::Texture> _keyTexture;
	Vector2f _position = Vector2f(-1000, 5000);
	bool _hasKey;
	Vector2f radiusOffset = Vector2f(1.0f, 1.0f);

protected:
	
	//std::shared_ptr<sf::Sprite> _sprite;

public:
	
	KeyComponent() = delete;
	explicit KeyComponent(Entity * p, bool hasKey, Vector2f position);

	void update(double dt) override;
	void render() override;

	void setHeld(bool hasKey);
	bool getHeld();
	shared_ptr <sf::Texture> getTexture();
	sf::IntRect getRect();
};