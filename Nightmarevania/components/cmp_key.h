#pragma once

#include <../lib_ecm/ecm.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class KeyComponent : public Component
{
protected:
	bool _hasKey;
	shared_ptr<sf::Texture> _keyTexture;
public:
	KeyComponent() = delete;
	explicit KeyComponent(Entity* p);

	void update(double) override { };
	virtual void render() override = 0;

	void setHeld(bool hasKey);
	bool getHeld();

	shared_ptr <sf::Texture> getTexture();
};

class NormalKeyComponent : public KeyComponent
{
private:
	int tileSize = 32;
	shared_ptr<sf::Sprite> _keySprite;
	shared_ptr<sf::Texture> _keyTexture;
	Vector2f _position = Vector2f(-1000, 5000);
	Vector2f radiusOffset = Vector2f(1.0f, 1.0f);
	IntRect i;

public:
	
	NormalKeyComponent() = delete;
	explicit NormalKeyComponent(Entity * p, bool hasKey, Vector2f position);

	void render() override;

	sf::IntRect getRect();
};

class SwordKeyComponent : public KeyComponent
{
private:
	shared_ptr<sf::Sprite> _keySprite;
	shared_ptr<sf::Texture> _keyTexture;
	Vector2f _position;

public:

	SwordKeyComponent() = delete;
	explicit SwordKeyComponent(Entity * p, bool hasKey, Vector2f position);

	void render() override;
};