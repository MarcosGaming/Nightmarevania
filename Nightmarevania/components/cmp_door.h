#pragma once

//#include "ecm.h"
#include "../lib_ecm/ecm.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

using namespace std;
using namespace sf;

class Door
{
protected:
	bool _locked;
	//shared_ptr<Sprite> _sprite;
	//unique_ptr<sf::RectangleShape> _sprite;
	shared_ptr <sf::RectangleShape> _sprite;
	shared_ptr<Texture> _texture;

public:
	//Door() = delete;
	//explicit Door(Entity* p);
	explicit Door();
	//explicit Door(Entity * p, bool _locked, std::shared_ptr<sf::Texture> tex, Vector2f pos);
	~Door() = default;

	//void setTexure(std::shared_ptr<sf::Texture> tex);
	void setPosition(Vector2f pos);
	void createDoor(IntRect i);

	

	//void update(double dt);
	void render();
	
	bool isLocked() const;
	void setLocked(bool _locked);

};