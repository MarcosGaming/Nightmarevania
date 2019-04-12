#pragma once

#include <../lib_ecm/ecm.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class DoorComponent : public Component
{
private:
	int tileSize = 32;
	bool _locked;
	std::shared_ptr<sf::Sprite> _sprite;
	shared_ptr<sf::Texture> _texture;
	Vector2f doorSize = Vector2f(96.0f, 96.0f);
	Vector2f _position = Vector2f(-1000, 5000);
	sf::IntRect lockedImage;
	sf::IntRect unlockedImage;

protected:
	

public:
	
	DoorComponent() = delete;
	explicit DoorComponent(Entity * p, bool isLocked, Vector2f position);

	void update(double dt) override;
	void render() override;

	void setLocked(bool isLocked);
	bool getLocked();
	shared_ptr <sf::Texture> getTexture();
	sf::IntRect getRect();
	Vector2f getSize();
	void setPlayer(shared_ptr<Entity> player);
	//void changeDoor();
};