#include "cmp_door.h"
#include "system_renderer.h"
#include "engine.h"
#include "cmp_key.h"

using namespace std;
using namespace sf;

// Abstract base key component class
KeyComponent::KeyComponent(Entity* p) : Component(p) {}

void KeyComponent::setHeld(bool hasKey) { _hasKey = hasKey; }
bool KeyComponent::getHeld() { return _hasKey; }

shared_ptr<sf::Texture> KeyComponent::getTexture() { return _keyTexture; }


// Normal key component
NormalKeyComponent::NormalKeyComponent(Entity * p, bool hasKey, Vector2f position) : KeyComponent(p) {
	i = sf::IntRect((2 * tileSize), (2 * tileSize), tileSize, tileSize);
	
	_hasKey = hasKey;
	_position = Vector2f(position);

	_keyTexture = make_shared<Texture>();
	_keyTexture->loadFromFile("res/img/foreground_tiles.png");

	_keySprite = make_shared<Sprite>();
	_keySprite->setPosition(_position);
	_keySprite->setTexture(*_keyTexture);
	_keySprite->setTextureRect(i);
}

void NormalKeyComponent::render() {
	if (!_hasKey) {
		_keySprite->setScale(sf::Vector2f(2.0f, 2.0f));
		_keySprite->setTextureRect(i);
		Renderer::queue(_keySprite.get());
	}
}

sf::IntRect NormalKeyComponent::getRect() {
	return i;
}

// Sword key component
SwordKeyComponent::SwordKeyComponent(Entity * p, bool hasKey, Vector2f position) : KeyComponent(p) {
	_hasKey = hasKey;
	_position = Vector2f(position.x, position.y - 35.0f);

	_keyTexture = make_shared<Texture>();
	_keyTexture->loadFromFile("res/img/sword_alone.png");

	_keySprite = make_shared<Sprite>();
	_keySprite->setPosition(_position);
	_keySprite->setTexture(*_keyTexture);
	_keySprite->setScale(sf::Vector2f(1.5f, 1.5f));
}

void SwordKeyComponent::render() {
	if (!_hasKey) {
		Renderer::queue(_keySprite.get());
	}
}
