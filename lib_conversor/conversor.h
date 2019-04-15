#pragma once

#include "SFML\Window.hpp"

namespace Conversor
{
	std::string KeyboardKeyToString(sf::Keyboard::Key);

	std::string MouseButtonToString(sf::Mouse::Button);

	std::string ControllerButtonToString(int);

	sf::Keyboard::Key StringToKeyboardKey(std::string&);

	sf::Mouse::Button StringToMouseButton(std::string&);

	int StringToControllerButton(std::string&);
}
