#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>
#include <system_controller.h>
#include <conversor.h>
#include <iostream>

// Base text component
void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str) : Component(p), _string(str) 
{
  _text.setString(_string);
  _font = Resources::get<sf::Font>("Gothic.ttf");
  _text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str) 
{
  _string = str;
  _text.setString(_string);
}

// Text component for the controls
ControlsTextComponent::ControlsTextComponent(Entity* p, const std::string& str) : TextComponent(p, str) { }

void ControlsTextComponent::setAction(std::string action) { _action = action; }

void ControlsTextComponent::setInitialPosition(sf::Vector2f position) { _initialPosition = position; }

void ControlsTextComponent::update(double dt)
{
	if (sf::Joystick::isConnected(0))
	{
		int* button = Controller::getActionControllerButton(_action);
		if (button != NULL)
		{
			SetText(Conversor::ControllerButtonToString(*button));
		}
	}
	else
	{
		sf::Keyboard::Key* key = Controller::getActionKey(_action);
		if (key != NULL)
		{
			SetText(Conversor::KeyboardKeyToString(*key));
		}
		else
		{
			sf::Mouse::Button* button = Controller::getActionMouseButton(_action);
			if (button != NULL)
			{
				SetText(Conversor::MouseButtonToString(*button));
			}
		}
	}
	_text.setPosition(sf::Vector2f(_initialPosition.x - (_text.getLocalBounds().width * 0.5f), _initialPosition.y));
}
