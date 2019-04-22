#include "cmp_door.h"
#include "cmp_key.h"
#include "cmp_sprite.h"
#include "system_renderer.h"
#include "engine.h"

using namespace std;
using namespace sf;

// Constructor:
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
	if (auto pl = _player.lock())
	{
		if (pl != 0 && pl->GetCompatibleComponent<KeyComponent>().size() != 0) {
			for (int i = 0; i < pl->GetCompatibleComponent<KeyComponent>().size(); i++){
				// If any of the keys that the player might have is not held return
				if (!pl->GetCompatibleComponent<KeyComponent>()[i]->getHeld()) {
					return;
				}
			}
			_locked = false;
		}
	}
}

void DoorComponent::render() {
	
	_sprite->setScale(sf::Vector2f(2.0f, 2.0f));
	if (_locked) {
		_sprite->setTextureRect(lockedImage);
	}
	else {
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

void DoorComponent::setPlayer(std::shared_ptr<Entity> player) {
	_player = player;
}

// Door orbs

DoorOrbsComponent::DoorOrbsComponent(Entity * p, int keysNumber) : _keysCollected(0), _totalKeys(keysNumber), Component(p) { }

void DoorOrbsComponent::update(double dt){
	_parent->get_components<SpriteComponent>()[0]->getSprite().setTextureRect(_orbsIntRect[_keysCollected]);
}

void DoorOrbsComponent::addOrbIntRect(sf::IntRect rect){
	_orbsIntRect.push_back(rect);
}

void DoorOrbsComponent::increaseKeysCollected() { _keysCollected++; }

bool DoorOrbsComponent::allKeysCollected() const {
	if (_keysCollected == _totalKeys)
	{
		return true;
	}
	return false;
}