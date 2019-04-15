#include "system_controller.h"
#include "engine.h"
#include <unordered_map>
#include "../lib_conversor/conversor.h"

static std::unordered_map<std::string, sf::Keyboard::Key> keyboard;
static std::unordered_map<std::string, sf::Mouse::Button> mouse;
static std::unordered_map<std::string, int> controller;

static bool attackButtonReleased;
static bool defendButtonReleased;
static bool jumpButtonReleased;

// Function that maps the controls to the standard form
static void setControlsToStandard();

// Internal funcions that handle mapping
static void mapActionToKeyboardKey(std::string, sf::Keyboard::Key);
static void mapActionToMouseButton(std::string, sf::Mouse::Button);
static void mapActionToControllerButton(std::string, int);

void Controller::initialise(std::vector<std::string>& keyMouseSettings, std::vector<std::string>& controllerSettings)
{
	// All actions begin being released
	attackButtonReleased = true;
	defendButtonReleased = true;
	jumpButtonReleased = true;
	// If any of vectors is empty or if their size do not correspond with the amount of elements they should have perform standard mapping
	if (keyMouseSettings.empty() || controllerSettings.empty() || keyMouseSettings.size() != 8 || controllerSettings.size() != 4)
	{
		setControlsToStandard();
		return;
	}
	// Set keyboard/mouse mapping based on the keyMouseSettings saved
	for (int i = 0; i < keyMouseSettings.size(); i++)
	{
		// The mapping follows this order: MoveRight,MoveLeft,Jump,Attack,Defend,UpAttack,DownAttack,Pause
		std::string string;
		switch (i)
		{
		case 0:
			string = MoveRightButton;
			break;
		case 1:
			string = MoveLeftButton;
			break;
		case 2:
			string = JumpButton;
			break;
		case 3:
			string = AttackButton;
			break;
		case 4:
			string = DefendButton;
			break;
		case 5:
			string = UpAttackButton;
			break;
		case 6:
			string = DownAttackButton;
			break;
		case 7:
			string = PauseButton;
			break;
		}
		// Try to map the element in the vector to a keyboard key or to a mouse button
		sf::Keyboard::Key key = Conversor::StringToKeyboardKey(keyMouseSettings[i]);
		// If the key is mapped to unknown with the keyboard try with the mouse
		if (key == sf::Keyboard::Unknown)
		{
			sf::Mouse::Button button = Conversor::StringToMouseButton(keyMouseSettings[i]);
			// If button maps to button count perform standard mapping
			if (button == sf::Mouse::Button::ButtonCount)
			{
				setControlsToStandard();
				return;
			}
			else
			{
				mouse[string] = button;
			}
		}
		else
		{
			keyboard[string] = key;
		}
	}
	// Set controller mapping based on the controller settings saved
	for (int i = 0; i < controllerSettings.size(); i++)
	{
		// The mapping follows this order: Jump,Attack,Defend,Pause
		std::string string;
		switch (i)
		{
		case 0:
			string = JumpButton;
			break;
		case 1:
			string = AttackButton;
			break;
		case 2:
			string = DefendButton;
			break;
		case 3:
			string = PauseButton;
			break;
		}
		// If the integer retrieve from the conversion is 10 than perform standard mapping
		int button = Conversor::StringToControllerButton(controllerSettings[i]);
		if (button == 10)
		{
			setControlsToStandard();
			return;
		}
		else
		{
			controller[string] = button;
		}
	}
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

// Returns the key that corresponds to the action 
sf::Keyboard::Key* Controller::getActionKey(std::string& action)
{
	auto found = keyboard.find(action);
	if (found != keyboard.end())
	{
		return &keyboard[action];
	}
	return NULL;
}
// Returns the button that corresponds to the action 
sf::Mouse::Button* Controller::getActionMouseButton(std::string& action)
{
	auto found = mouse.find(action);
	if (found != mouse.end())
	{
		return &mouse[action];
	}
	return NULL;
}
// Returns the int button that corresponds to the action 
int* Controller::getActionControllerButton(std::string& action)
{
	auto found = controller.find(action);
	if (found != controller.end())
	{
		return &controller[action];
	}
	return NULL;
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

// Wait for user input and map it to the keyboard or mouse map
void Controller::mapInputToAction(std::string& action)
{
	while (true)
	{
		sf::Event event;
		while (Engine::GetWindow().pollEvent(event))
		{
			if (event.type == event.KeyPressed)
			{
				mapActionToKeyboardKey(action, event.key.code);
				return;
			}
			else if (event.type == event.MouseButtonPressed)
			{
				mapActionToMouseButton(action, event.mouseButton.button);
				return;
			}
		}
	}
}
// Wait for user input and map it to the controller map
void Controller::mapInputToActionController(std::string& action)
{
	while (true)
	{
		sf::Event event;
		while (Engine::GetWindow().pollEvent(event))
		{
			if (event.type == event.JoystickButtonPressed)
			{
				mapActionToControllerButton(action, event.joystickButton.button);
				return;
			}
		}
		if (!sf::Joystick::isConnected(0))
		{
			return;
		}
	}
}

// Handles mapping to keyboard
void mapActionToKeyboardKey(std::string action, sf::Keyboard::Key key)
{
	for (auto element = keyboard.begin(); element != keyboard.end(); element++)
	{
		// Check if the same key is already map to another action
		if (element->second == key)
		{
			// Check if the action is mapped in the mouse
			auto foundInMouse = mouse.find(action);
			if (foundInMouse != mouse.end())
			{
				sf::Mouse::Button previousMouseMapping = mouse[action];
				// Get the action that the button was mapped to
				std::string previousAction = element->first;
				// Map previous action to the button of the action to change before mapping the action to the new button 
				mouse[previousAction] = previousMouseMapping;
				// Erase previous action from mouse
				keyboard.erase(previousAction);
				break;
			}
			else
			{
				// Get the action that the key was mapped to
				std::string previousAction = element->first;
				// Map previous action to the key of the action to change before mapping the action to the new key 
				keyboard[previousAction] = keyboard[action];
			}
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
// Handles mapping to mouse
void mapActionToMouseButton(std::string action, sf::Mouse::Button button)
{
	for (auto element = mouse.begin(); element != mouse.end(); element++)
	{
		// Check if the same key is already map to another action
		if (element->second == button)
		{
			// Check if the action is mapped in the keyboard
			auto foundInKeyboard = keyboard.find(action);
			if (foundInKeyboard != keyboard.end())
			{
				sf::Keyboard::Key previousKeyboardMapping = keyboard[action];
				// Get the action that the button was mapped to
				std::string previousAction = element->first;
				// Map previous action to the button of the action to change before mapping the action to the new button 
				keyboard[previousAction] = previousKeyboardMapping;
				// Erase previous action from mouse
				mouse.erase(previousAction);
				break;
			}
			else
			{
				// Get the action that the button was mapped to
				std::string previousAction = element->first;
				// Map previous action to the button of the action to change before mapping the action to the new button 
				mouse[previousAction] = mouse[action];
			}
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
// Handles mapping to controller
void mapActionToControllerButton(std::string action, int button)
{
	for (auto element = controller.begin(); element != controller.end(); element++)
	{
		// Check if the same key is already map to another action
		if (element->second == button)
		{
			// Get the action that the button was mapped to
			std::string previousAction = element->first;
			// Map previous action to the button of the action to change before mapping the action to the new button 
			controller[previousAction] = controller[action];
		}
	}
	// Map action to button
	controller[action] = button;
}

// Maps the controls to the standard form
void setControlsToStandard()
{
	// Standatd keyboard and mouse mapping
	keyboard[Controller::MoveRightButton] = sf::Keyboard::D;
	keyboard[Controller::MoveLeftButton] = sf::Keyboard::A;
	keyboard[Controller::JumpButton] = sf::Keyboard::Space;
	mouse[Controller::AttackButton] = sf::Mouse::Left;
	mouse[Controller::DefendButton] = sf::Mouse::Right;
	keyboard[Controller::UpAttackButton] = sf::Keyboard::W;
	keyboard[Controller::DownAttackButton] = sf::Keyboard::S;
	keyboard[Controller::PauseButton] = sf::Keyboard::Escape;
	// Standard controller mapping
	controller[Controller::JumpButton] = 0;
	controller[Controller::AttackButton] = 2;
	controller[Controller::DefendButton] = 1;
	controller[Controller::PauseButton] = 7;
}
