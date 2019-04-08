#include "system_controller.h"
#include <unordered_map>

static std::unordered_map<std::string, sf::Keyboard::Key> keyboard;
static std::unordered_map<std::string, sf::Mouse::Button> mouse;
static std::unordered_map<std::string, int> controller;

static bool attackButtonReleased;
static bool defendButtonReleased;
static bool jumpButtonReleased;

void Controller::initialise()
{
	attackButtonReleased = true;
	defendButtonReleased = true;
	jumpButtonReleased = true;
	// Standatd keyboard and mouse mapping
	keyboard[MoveRightButton] = sf::Keyboard::D;
	keyboard[MoveLeftButton] = sf::Keyboard::A;
	keyboard[JumpButton] = sf::Keyboard::Space;
	mouse[AttackButton] = sf::Mouse::Left;
	mouse[DefendButton] = sf::Mouse::Right;
	keyboard[UpAttackButton] = sf::Keyboard::W;
	keyboard[DownAttackButton] = sf::Keyboard::S;
	// Standard controller mapping
	controller[JumpButton] = 0;
	controller[AttackButton] = 2;
	controller[DefendButton] = 1;
}

void Controller::mapActionToKey(std::string action, sf::Keyboard::Key key) 
{ 
	for (auto element = keyboard.begin(); element != keyboard.end(); element++)
	{
		// Check if the same key is already map to another action
		if (element->second == key)
		{
			// Get the action that the key was mapped to
			std::string previousAction = element->first;
			// Map previous action to the key of the action to change before mapping the action to the new key 
			keyboard[previousAction] = keyboard[action];
		}
	}
	// Map action to new key
	keyboard[action] = key;
	// Remove the action if exits in the mouse
	auto found = mouse.find(action);
	if (found != mouse.end())
	{
		mouse.erase(found);
	}
}

void Controller::mapActionToMouseButton(std::string action, sf::Mouse::Button button)
{
	for (auto element = mouse.begin(); element != mouse.end(); element++)
	{
		// Check if the same key is already map to another action
		if (element->second == button)
		{
			// Get the action that the button was mapped to
			std::string previousAction = element->first;
			// Map previous action to the button of the action to change before mapping the action to the new button 
			keyboard[previousAction] = keyboard[action];
		}
	}
	// Map action to button
	mouse[action] = button;
	// Remove the action if exits in the keyboard
	auto found = keyboard.find(action);
	if (found != keyboard.end())
	{
		keyboard.erase(found);
	}
}

void Controller::mapActionToControllerButton(std::string action, int button)
{
	for (auto element = controller.begin(); element != controller.end(); element++)
	{
		// Check if the same key is already map to another action
		if (element->second == button)
		{
			// Get the action that the button was mapped to
			std::string previousAction = element->first;
			// Map previous action to the button of the action to change before mapping the action to the new button 
			keyboard[previousAction] = keyboard[action];
		}
	}
	// Map action to button
	controller[action] = button;
}

bool Controller::isPressed(std::string action)
{
	// Controller
	if (sf::Joystick::isConnected(0))
	{
		// With a controller some actions are fixed to the joystick
		if (action == MoveRightButton && sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 15.0f)
		{
			return true;
		}
		else if (action == MoveLeftButton && sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -15.0f)
		{
			return true;
		}
		if (action == UpAttackButton && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -35.0f)
		{
			return true;
		}
		else if (action == DownAttackButton && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 35.0f)
		{
			return true;
		}
		// Actions with the controller that the player can modify
		auto found = controller.find(action);
		if (found != controller.end() && sf::Joystick::isButtonPressed(0, controller[action]))
		{
			return true;
		}
	}
	// Keyboard and mouse
	else
	{
		// Check if the action belongs to the mouse or to the keyboard
		auto foundMouse = mouse.find(action);
		if (foundMouse != mouse.end() && sf::Mouse::isButtonPressed(mouse[action]))
		{
			return true;
		}
		auto foundKeyboard = keyboard.find(action);
		if (foundKeyboard != keyboard.end() && sf::Keyboard::isKeyPressed(keyboard[action]))
		{
			return true;
		}
	}
	return false;
}

bool Controller::isAttackButtonReleased() { return attackButtonReleased; }
void Controller::setAttackButtonReleased(bool status) { attackButtonReleased = status; }

bool Controller::isDefendButtonReleased() { return defendButtonReleased; }
void Controller::setDefendButtonReleased(bool status) { defendButtonReleased = status; }

bool Controller::isJumpButtonReleased() { return jumpButtonReleased; }
void Controller::setJumpButtonReleased(bool status) { jumpButtonReleased = status; }

// Jump, attack and defend implementation takes into account if their corresponding buttons have been released
void Controller::processEvent(sf::Event eventReleased)
{
	if (sf::Joystick::isConnected(0))
	{
		if (eventReleased.joystickButton.button == controller[JumpButton])
		{
			jumpButtonReleased = true;
		}
		else if (eventReleased.joystickButton.button == controller[AttackButton])
		{
			attackButtonReleased = true;
		}
		else if (eventReleased.joystickButton.button == controller[DefendButton])
		{
			defendButtonReleased = true;
		}
	}
	else
	{
		// Check if the event belongs to the keyboard
		auto foundKeyboard = keyboard.find(JumpButton);
		if (foundKeyboard != keyboard.end())
		{
			// Check if the event released corresponds to the jump button
			if (eventReleased.key.code == foundKeyboard->second)
			{
				jumpButtonReleased = true;
				return;
			}
		}
		foundKeyboard = keyboard.find(AttackButton);
		if (foundKeyboard != keyboard.end())
		{
			// Check if the event released corresponds to the attack button
			if (eventReleased.key.code == foundKeyboard->second)
			{
				attackButtonReleased = true;
				return;
			}
		}
		foundKeyboard = keyboard.find(DefendButton);
		if (foundKeyboard != keyboard.end())
		{
			// Check if the event released corresponds to the defend button
			if (eventReleased.key.code == foundKeyboard->second)
			{
				defendButtonReleased = true;
				return;
			}
		}
		// Check if the event belongs to the mouse
		auto foundMouse = mouse.find(JumpButton);
		if (foundMouse != mouse.end())
		{
			// Check if the event released corresponds to the jump button
			if (eventReleased.mouseButton.button == foundMouse->second)
			{
				jumpButtonReleased = true;
				return;
			}
		}
		foundMouse = mouse.find(AttackButton);
		if (foundMouse != mouse.end())
		{
			// Check if the event released corresponds to the attack button
			if (eventReleased.mouseButton.button == foundMouse->second)
			{
				attackButtonReleased = true;
				return;
			}
		}
		foundMouse = mouse.find(DefendButton);
		if (foundMouse != mouse.end())
		{
			// Check if the event released corresponds to the defend button
			if (eventReleased.mouseButton.button == foundMouse->second)
			{
				defendButtonReleased = true;
				return;
			}
		}
	}
}

