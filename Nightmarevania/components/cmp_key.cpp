#include "cmp_door.h"
#include "system_renderer.h"
#include "engine.h"
#include "cmp_key.h"

using namespace std;
using namespace sf;

sf::IntRect i;

KeyComponent::KeyComponent(Entity * p, bool hasKey, Vector2f position) : Component(p) {
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

void KeyComponent::update(double dt) {}

void KeyComponent::render() {
	if (!_hasKey) {
		_keySprite->setScale(sf::Vector2f(2.0f, 2.0f));
		_keySprite->setTextureRect(i);
		Renderer::queue(_keySprite.get());
	}
}

void KeyComponent::setHeld(bool hasKey) {
	_hasKey = hasKey;
}

bool KeyComponent::getHeld() {
	return _hasKey;
}

shared_ptr<sf::Texture> KeyComponent::getTexture() {
	return _keyTexture;
}

sf::IntRect KeyComponent::getRect() {
	return i;
}
