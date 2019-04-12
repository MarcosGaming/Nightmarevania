#include "cmp_door.h"
#include "system_renderer.h"
#include "engine.h"
//#include "cmp_sprite.h"

using namespace std;
using namespace sf;

int tileSize = 32;
Vector2f doorSize = Vector2f(96.0f, 96.0f);
sf::IntRect lockedImage = sf::IntRect((0 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));
sf::IntRect unlockedImage = sf::IntRect((3 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));


bool _locked;
//shared_ptr<sf::RectangleShape> _sprite;
shared_ptr<sf::Sprite> _sprite;
shared_ptr<sf::Texture> _texture;
Vector2f _position = Vector2f(-1000,5000);

/*
void Door::render() {
	//Renderer::queue(_sprite.get());
}

bool Door::getLocked() const { return _locked; }

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

void Door::setPosition(Vector2f pos) {
	_sprite->setPosition(pos);
}

void Door::createDoor(IntRect i) {

	//doorSize = Vector2f(96.0f, 96.0f);

	_texture = make_shared<Texture>();
	_texture->loadFromFile("res/img/foreground_tiles.png");

	render();
}

*/

//constructor:
DoorComponent::DoorComponent(Entity * p, bool isLocked, Vector2f position) : Component(p) {
	_locked = isLocked;
	_position = Vector2f(position.x, position.y-131.5f);
	
	_texture = make_shared<Texture>();
	_texture->loadFromFile("res/img/foreground_tiles.png");
	
	_sprite = make_shared<Sprite>();
	_sprite->setPosition(_position);
	_sprite->setTexture(*_texture);
	_sprite->setTextureRect(getRect());
}

/*DoorComponent::DoorComponent(Entity * p) : Component(p) {

	_texture = make_shared<Texture>();
	_texture->loadFromFile("res/img/foreground_tiles.png");
	_sprite->setPosition(_position);
	_sprite = make_shared<Sprite>();
	_sprite->setTexture(*_texture);
}*/

void DoorComponent::update(double dt)
{
	//if player hasKey == isLocked then change the door (because if hasKey == true, than isLocked should == false)
}

void DoorComponent::render()
{
	sf::IntRect texRect;
	if (_locked) {
		texRect = lockedImage;
	}
	else {
		texRect = unlockedImage;
	}

	_sprite->setScale(sf::Vector2f(2.0f,2.0f));
	_sprite->setTextureRect(texRect);
	Renderer::queue(_sprite.get());
}

void DoorComponent::setLocked(bool isLocked) {
	_locked = isLocked;
}

bool DoorComponent::getLocked() {
	return _locked;
}

shared_ptr <sf::Texture> DoorComponent::getTexture() {
	return _texture;
}

sf::IntRect DoorComponent::getRect() {
	if (_locked) {
		return lockedImage;
	}

	return unlockedImage;
}

Vector2f DoorComponent::getSize() {
	return doorSize;
}

void DoorComponent::changeDoor() {

}

/*void setPosition(Vector2f position) {
	_position = position;
}*/