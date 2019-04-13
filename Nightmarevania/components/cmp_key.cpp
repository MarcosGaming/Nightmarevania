#include "cmp_door.h"
#include "system_renderer.h"
#include "engine.h"
#include "cmp_key.h"
//#include "cmp_sprite.h"

using namespace std;
using namespace sf;

//int tileSize = 32;
sf::IntRect i;// = sf::IntRect((2 * tileSize), (2 * tileSize), tileSize, tileSize);

//bool _hasKey;
//shared_ptr<sf::Sprite> _sprite;
//shared_ptr<sf::Texture> _texture;
//Vector2f _position = Vector2f(-1000,5000);
//int radiusOffset;

KeyComponent::KeyComponent(Entity * p, bool hasKey, Vector2f position) : Component(p) {
	i = sf::IntRect((2 * tileSize), (2 * tileSize), tileSize, tileSize);
	
	_hasKey = hasKey;
	_position = Vector2f(position); //check if offset required

	_keyTexture = make_shared<Texture>();
	_keyTexture->loadFromFile("res/img/foreground_tiles.png");

	_keySprite = make_shared<Sprite>();
	_keySprite->setPosition(_position);
	_keySprite->setTexture(*_keyTexture);
	_keySprite->setTextureRect(i);
}

void KeyComponent::update(double dt) {
	//if playerpos > pos+offset || <pos-offset //double check direction here...
	//{ unload? set visible == false? something... }
	//printf("player: (%d, %d), key: (%d, %d)\n", _parent->getPosition().x, _parent->getPosition().y, _position.x, _position.y);
	//if (_parent->getPosition().x < (_position.x + radiusOffset.x) && 
	//	_parent->getPosition().x > (_position.x - radiusOffset.x) &&
	//	_parent->getPosition().y < (_position.y + radiusOffset.y) &&
	//	_parent->getPosition().y >(_position.y - radiusOffset.y) ) {
	//printf("%d", _hasKey);
	/*if(_hasKey){
		//_hasKey == true;
		this->_parent->setVisible(false);
	}*/
}

void KeyComponent::render() {
	if (!_hasKey) {
		_keySprite->setScale(sf::Vector2f(2.0f, 2.0f));
		_keySprite->setTextureRect(i);
		Renderer::queue(_keySprite.get());
	}

	/*_keySprite->setScale(sf::Vector2f(2.0f, 2.0f));
	_keySprite->setTextureRect(i);
	Renderer::queue(_keySprite.get());*/
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
