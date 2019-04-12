#pragma once

#include <../lib_ecm/ecm.h>
#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Sprite.hpp>
//#include <SFML/Graphics/Shape.hpp>
//#include <SFML/Graphics/RectangleShape.hpp>
//#include <memory>

using namespace std;
using namespace sf;

class DoorComponent : public Component
{
protected:
//private:
	bool _locked;
	std::shared_ptr<sf::Sprite> _sprite;
	//std::shared_ptr<sf::RectangleShape> _sprite;
	//std::shared_ptr<sf::Texture> _texture;

public:
	
	DoorComponent() = delete;
	explicit DoorComponent(Entity * p, bool isLocked, Vector2f position);
	//explicit DoorComponent(Entity* p, bool _isLocked);
	//explicit DoorComponent(Entity* p);

	void update(double dt) override;
	void render() override;

	void setLocked(bool isLocked);
	bool getLocked();
	shared_ptr <sf::Texture> getTexture();
	sf::IntRect getRect();
	Vector2f getSize();
	void changeDoor();
};