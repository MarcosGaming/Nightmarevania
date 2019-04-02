#include "cmp_button.h"
#include "cmp_sprite.h"
#include "../game.h"
#include <system_renderer.h>

// Basic button

ButtonComponent::ButtonComponent(Entity* p) : Component(p) { }

void ButtonComponent::setNormal(sf::IntRect normal) { _normal = normal; }

void ButtonComponent::setHovered(sf::IntRect hovered) { _hovered = hovered; }

void ButtonComponent::update(double dt)
{
	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (sprite->getSprite().getGlobalBounds().contains(worldPos))
	{
		sprite->getSprite().setTextureRect(_hovered);
	}
	else
	{
		sprite->getSprite().setTextureRect(_normal);
	}
}

// Button that changes scenes
ChangeSceneButtonComponent::ChangeSceneButtonComponent(Entity* p) : ButtonComponent(p) { }

void ChangeSceneButtonComponent::setScene(Scene* scene) { _scene_to_change = scene; }

void ChangeSceneButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (sprite->getSprite().getGlobalBounds().contains(worldPos))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Engine::ChangeScene(_scene_to_change);
		}
	}
}

// Button that exits the game
ExitButtonComponent::ExitButtonComponent(Entity* p) : ButtonComponent(p) { }

void ExitButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (sprite->getSprite().getGlobalBounds().contains(worldPos))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Engine::GetWindow().close();
		}
	}
}