#include "cmp_door.h"
#include "system_renderer.h"
#include "engine.h"
#include "cmp_key.h"

using namespace std;
using namespace sf;

//int tileSize = 32;
//Vector2f doorSize = Vector2f(96.0f, 96.0f);
sf::IntRect lockedImage;// = sf::IntRect((0 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));
sf::IntRect unlockedImage;// = sf::IntRect((3 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));

shared_ptr<Entity> _player = NULL;
sf::IntRect texRect;

//bool _locked;
//shared_ptr<sf::Sprite> _sprite;
//shared_ptr<sf::Texture> _texture;
//Vector2f _position = Vector2f(-1000,5000);

//constructor:
DoorComponent::DoorComponent(Entity * p, bool isLocked, Vector2f position) : Component(p) {
	lockedImage = sf::IntRect((0 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));
	unlockedImage = sf::IntRect((3 * tileSize), (7 * tileSize), (3 * tileSize), (3 * tileSize));
	
	_locked = isLocked;
	_position = Vector2f(position.x, position.y-131.5f);
	
	_texture = make_shared<Texture>();
	_texture->loadFromFile("res/img/foreground_tiles.png");
	
	_sprite = make_shared<Sprite>();
	_sprite->setPosition(_position);
	_sprite->setTexture(*_texture);
	_sprite->setTextureRect(getRect());
}

void DoorComponent::update(double dt) {
	
	if ((_player != 0) && _player->GetCompatibleComponent<KeyComponent>()[0]->getHeld()) {
		_locked = false;
	}
}

void DoorComponent::render() {
	
	_sprite->setScale(sf::Vector2f(2.0f, 2.0f));
	if (_locked) {
		//texRect = lockedImage;
		_sprite->setTextureRect(lockedImage);
	}
	else {
		//texRect = unlockedImage;
		_sprite->setTextureRect(unlockedImage);
	}
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

void DoorComponent::setPlayer(shared_ptr<Entity> player) {
	_player = player;
}

/*void setPosition(Vector2f position) {
	_position = position;
}*/