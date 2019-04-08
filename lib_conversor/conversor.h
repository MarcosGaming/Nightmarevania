#pragma once

#include "SFML\Window.hpp"

namespace Conversor
{
	std::string KeyboardKeyToString(sf::Keyboard::Key);

	std::string MouseButtonToString(sf::Mouse::Button);

	std::string ControllerButtonToString(int);
}
