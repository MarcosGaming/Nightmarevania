#include "cmp_door.h"
#include "system_renderer.h"

using namespace std;
using namespace sf;

int tileSize = 32;
Vector2f doorSize = Vector2f(96.0f, 96.0f);
sf::IntRect lockedImage = sf::IntRect((0 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));
sf::IntRect unlockedImage = sf::IntRect((3 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));
//auto _sprite = make_unique<sf::RectangleShape>();

//Door::Door(Entity * p) : Component(p), _sprite(make_shared<sf::RectangleShape>()) {}
Door::Door() {}

/*Door::Door(Entity * p, bool _locked, std::shared_ptr<sf::Texture> tex, Vector2f pos) : Component(p), _sprite(make_shared<sf::Sprite>())
{
	//Door::_locked = locked;
	//Door::setLocked(_locked);

	_sprite->setSize(doorSize);
	_sprite->setPosition(pos);
	_sprite->setTexture(&tex);
}*/

//void Door::update(double dt) {}

void Door::render() {
	Renderer::queue(_sprite.get());
}

bool Door::isLocked() const { return _locked; }

void Door::setLocked(bool _locked) { 
	Door::_locked = _locked; 

	if (_locked) {
		//set sprite to the locked door
		createDoor(lockedImage);
	}
	else {
		//set it to unlocked
		createDoor(unlockedImage);
	}
}

/*void Door::setTexure(std::shared_ptr<sf::Texture> tex)
{
	_texture = tex;
	_sprite->setTexture(_texture);
}*/



void Door::setPosition(Vector2f pos) {
	_sprite->setPosition(pos);
}

void Door::createDoor(IntRect i) {

	//auto s = make_unique<sf::RectangleShape>();
	_texture = make_shared<Texture>();
	_texture->loadFromFile("res/img/foreground_tiles.png");

	_sprite->setSize(doorSize);
	//_sprite->setTexture(tex);
	_sprite->setTextureRect(i);
}
