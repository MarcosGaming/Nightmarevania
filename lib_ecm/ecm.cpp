#include "ecm.h"

using namespace std;

// ENTITY

Entity::Entity(Scene* const s) : _position({0, 0}), _rotation(0), _death(false), _updatable(true), _visible(true), scene(s), _fordeletion(false), _facingRight(true) {}

void Entity::update(double dt) 
{
	if (!_updatable) 
	{
		return;
	}
	for (size_t i = 0; i < _components.size(); i++) 
	{
		if (_components[i]->is_fordeletion()) 
		{
			_components.erase(_components.begin() + i);
			--i;
		}
		_components[i]->update(dt);
	}
}

void Entity::render() 
{
	if (!_visible) 
	{
		return;
	}
	for (auto& c : _components) 
	{
		c->render();
	}
}

void Entity::addTag(const std::string& t) { _tags.insert(t); }
const std::set<std::string>& Entity::getTags() const { return _tags; }

const sf::Vector2f& Entity::getPosition() const { return _position; }
void Entity::setPosition(const sf::Vector2f& _position) { Entity::_position = _position;}

float Entity::getRotation() const { return _rotation; }
void Entity::setRotation(float _rotation) { Entity::_rotation = _rotation; }

bool Entity::isDead() const { return _death; }
void Entity::setDeath(bool _death) { Entity::_death = _death; }

bool Entity::isUpdatable() const { return _updatable; }
void Entity::setUpdatable(bool _alive) { Entity::_updatable = _alive; }

bool Entity::is_fordeletion() const { return _fordeletion; }
void Entity::setForDelete() 
{
	_fordeletion = true;
	_updatable = false;
	_visible = false;
}

bool Entity::isVisible() const { return _visible; }
void Entity::setVisible(bool _visible) { Entity::_visible = _visible; }

bool Entity::isFacingRight() const { return _facingRight; }
void Entity::setFacingRight(bool _facingRight) { Entity::_facingRight = _facingRight; }

Entity::~Entity() 
{
	// Components can inter-depend on each other, so deleting them may take multiple passes. We Keep deleting components until we can't delete any more
	int deli = 0;
	while (deli != _components.size()) 
	{
		deli = (int)_components.size();
		_components.erase(remove_if(_components.begin(), _components.end(), [](auto& sp) { return (sp.use_count() <= 1); }), _components.end());
	}

	 if (_components.size() > 0) 
	 {
		throw std::runtime_error("Can't delete entity, someone is grabbing a component!");
	 }

	_components.clear();
}

// COMPONENT

Component::Component(Entity* const p) : _parent(p), _fordeletion(false) {}

Component::~Component() {}

bool Component::is_fordeletion() const { return _fordeletion; }

// ENTITY MANAGER

void EntityManager::update(double dt) 
{
	for (size_t i = 0; i < list.size(); i++) 
	{
		if (list[i]->is_fordeletion()) 
		{
			 list.erase(list.begin() + i);
			--i;
			continue;
		}
		if (list[i]->_updatable) 
		{
			list[i]->update(dt);
		}
	}
}

void EntityManager::render() 
{
	for (auto& e : list) 
	{
		if (e->_visible) 
		{
			e->render();
		}
	}
}

vector<shared_ptr<Entity>> EntityManager::find(const string& tag) const 
{
	vector<shared_ptr<Entity>> ret;
	for (auto& e : list) 
	{
		const auto tgs = e->_tags;
		if (tgs.find(tag) != tgs.end()) 
		{
			ret.push_back(e);
		}
	}
	return ret;
}

vector<shared_ptr<Entity>> EntityManager::find(const vector<string>& tags) const 
{
	vector<shared_ptr<Entity>> ret;
	for (auto& e : list) 
	{
		const auto tgs = e->_tags;
		if (any_of(tags.begin(), tags.end(), [&tgs](auto t) { return tgs.find(t) != tgs.end(); })) 
		{
			ret.push_back(e);
		}
	}
	return ret;
}
