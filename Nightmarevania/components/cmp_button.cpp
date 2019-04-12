#include "cmp_button.h"
#include "cmp_sprite.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>
#include <system_resolution.h>
#include <system_controller.h>

static float PressedCooldown = 0.5;

// Basic button
ButtonComponent::ButtonComponent(Entity* p) : _active(false), _controllerHovered(false), _controllerPressed(false), _canHoverActive(false), Component(p) { }

void ButtonComponent::setNormal(sf::IntRect normal) { _normal = normal; }
void ButtonComponent::setHovered(sf::IntRect hovered) { _hovered = hovered; }
void ButtonComponent::setPressed(sf::IntRect pressed) { _pressed = pressed; }

void ButtonComponent::setControllerHovered(bool status) { _controllerHovered = status; }
void ButtonComponent::setControllerPressed(bool status) { _controllerPressed = status; }

void ButtonComponent::setActive(bool active) { _active = active; }
bool ButtonComponent::isActive() const { return _active; }

void ButtonComponent::setCanHoverActive(bool status) { _canHoverActive = status; }

void ButtonComponent::ButtonNavigation(const std::vector<std::shared_ptr<ButtonComponent>>& buttons, int& index, const double& dt)
{
	static float coolDown = 0.2f;
	coolDown -= dt;
	if (sf::Joystick::isConnected(0) && !buttons.empty())
	{
		if (sf::Joystick::isButtonPressed(0, 0) && PressedCooldown <= 0.0f)
		{
			buttons[index]->_controllerPressed = true;
			PressedCooldown = 0.2f;
		}
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 70.0f && coolDown <= 0.0f)
		{
			buttons[index]->_controllerHovered = false;
			index++;
			coolDown = 0.2f;
			// Skip buttons that are active when going down
			while (index < buttons.size() - 1 && buttons[index]->isActive() && !buttons[index]->_canHoverActive)
			{
				index++;
			}
		}
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -70.0f && coolDown <= 0.0f)
		{
			buttons[index]->_controllerHovered = false;
			index--;
			coolDown = 0.2f;
			// Skip buttons that are active when going up
			while (index >= 0 && buttons[index]->isActive() && !buttons[index]->_canHoverActive)
			{
				index--;
			}
		}
		// If the index is less than 0 set it to be the last element in the vector
		if (index < 0)
		{
			index = buttons.size() - 1;
		}
		// If the index is greater than the vector size then set it to be the first element in the vector
		else if (index > buttons.size() - 1)
		{
			index = 0;
		}
		buttons[index]->_controllerHovered = true;
	}
	// If the controller gets disconnected removed the hovered status from the current button
	else if (!buttons.empty())
	{
		buttons[index]->_controllerHovered = false;
	}
}

void ButtonComponent::update(double dt)
{
	// Control button selection freezes the game causing delta time to increase too much and affecting button selection
	if (dt < 0.1)
	{
		PressedCooldown -= dt;
	}
	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	// Change button sprite based on its status
	if (_active)
	{
		sprite->getSprite().setTextureRect(_pressed);
	}
	else if (sprite->getSprite().getGlobalBounds().contains(worldPos) || _controllerHovered)
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && PressedCooldown <= 0.0f && !_active)
		{
			Engine::ChangeScene(_scene_to_change);
			PressedCooldown = 0.5f;
		}
	}
	else if (_controllerPressed && _controllerHovered && PressedCooldown <= 0.0f)
	{
		Engine::ChangeScene(_scene_to_change);
		PressedCooldown = 1.0f;
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && PressedCooldown <= 0.0f)
		{
			Engine::GetWindow().close();
		}
	}
	else if (_controllerPressed && _controllerHovered && PressedCooldown <= 0.0f)
	{
		Engine::GetWindow().close();
	}
}

// Button that changes the screen to a specific resolution
ResolutionButtonComponent::ResolutionButtonComponent(Entity* p) : _is1920x1080(false), _is1600x900(false), _is1280x720(false), _is1024x576(false),  ButtonComponent(p) { }

void ResolutionButtonComponent::setResolutionTo1920x1080() { _is1920x1080 = true; }
void ResolutionButtonComponent::setResolutionTo1600x900() { _is1600x900 = true; }
void ResolutionButtonComponent::setResolutionTo1280x720() { _is1280x720 = true; }
void ResolutionButtonComponent::setResolutionTo1024x576() { _is1024x576 = true; }

void ResolutionButtonComponent::setMediator(std::shared_ptr<MediatorResolutionButtons> mediator) { _mediator = mediator; }

void ResolutionButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	bool changeResolution = false;
	if(!_active && PressedCooldown <= 0.0f)
	{
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				changeResolution = true;
			}
		}
		else if (_controllerPressed && _controllerHovered)
		{
			_controllerPressed = false;
			_controllerHovered = false;
			changeResolution = true;
		}
	}
	if (changeResolution)
	{
		_active = true;
		_mediator->deactivateOtherResolutionButtons(this);
		_mediator->deactivateFullScreen();
		// Change to the corresponding resolution
		if (_is1920x1080)
		{
			Resolution::changeTo1920x1080();
		}
		else if (_is1600x900)
		{
			Resolution::changeTo1600x900();
		}
		else if (_is1280x720)
		{
			Resolution::changeTo1280x720();
		}
		else if (_is1024x576)
		{
			Resolution::changeTo1024x576();
		}
		PressedCooldown = 4.0f;
	}
}

// Button that sets the game to fullsscreen
FullScreenButtonComponent::FullScreenButtonComponent(Entity* p) : ButtonComponent(p) { }

void FullScreenButtonComponent::setMediator(std::shared_ptr<MediatorResolutionButtons> mediator) { _mediator = mediator; }
void FullScreenButtonComponent::setHoveredActive(sf::IntRect hoveredActive) { _hoveredActive = hoveredActive; }

void FullScreenButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (!_active && PressedCooldown <= 0.0f)
	{
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = true;
				_mediator->deactivateAllResolutionButtons();
				_mediator->activateBorderless();
				Resolution::turnFullScreenOn();
				PressedCooldown = 2.5f;
			}
		}
		else if (_controllerPressed && _controllerHovered)
		{
			_controllerPressed = false;
			_active = true;
			_mediator->deactivateAllResolutionButtons();
			_mediator->activateBorderless();
			Resolution::turnFullScreenOn();
			PressedCooldown = 2.5f;
		}
	}
	else if (PressedCooldown <= 0.0f)
	{
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			sprite->getSprite().setTextureRect(_hoveredActive);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = false;
				_mediator->deactivateAllResolutionButtons();
				_mediator->deactivateBorderless();
				Resolution::turnFullScreenOff();
				PressedCooldown = 2.5f;
			}
		}
		else if (_controllerHovered)
		{
			sprite->getSprite().setTextureRect(_hoveredActive);
			if (_controllerPressed)
			{
				_controllerPressed = false;
				_active = false;
				_mediator->deactivateAllResolutionButtons();
				_mediator->deactivateBorderless();
				Resolution::turnFullScreenOff();
				PressedCooldown = 2.5f;
			}
		}
		else
		{
			sprite->getSprite().setTextureRect(_pressed);
		}
	}
}
// Buttons that sets the game to borderless window
BorderlessButtonComponent::BorderlessButtonComponent(Entity* p) : ButtonComponent(p) { }

void BorderlessButtonComponent::setMediator(std::shared_ptr<MediatorResolutionButtons> mediator) { _mediator = mediator; }

void BorderlessButtonComponent::setHoveredActive(sf::IntRect hoveredActive) { _hoveredActive = hoveredActive; }

void BorderlessButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (!_active && PressedCooldown <= 0.0f)
	{
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = true;
				Resolution::turnBorderlessOn();
				PressedCooldown = 2.5f;
			}
		}
		else if (_controllerPressed && _controllerHovered)
		{
			_controllerPressed = false;
			_active = true;
			Resolution::turnBorderlessOn();
			PressedCooldown = 2.5f;
		}
	}
	else if (PressedCooldown <= 0.0f)
	{
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			sprite->getSprite().setTextureRect(_hoveredActive);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = false;
				_mediator->deactivateFullScreen();
				Resolution::turnBorderlessOff();
				PressedCooldown = 2.5f;
			}
		}
		else if (_controllerHovered)
		{
			sprite->getSprite().setTextureRect(_hoveredActive);
			if (_controllerPressed)
			{
				_controllerPressed = false;
				_active = false;
				_mediator->deactivateFullScreen();
				Resolution::turnBorderlessOff();
				PressedCooldown = 2.5f;
			}
		}
		else
		{
			sprite->getSprite().setTextureRect(_pressed);
		}
	}
}

// Mediator design pattern that is going to handle communication between the resolution buttons
void MediatorResolutionButtons::addResolutionButton(std::shared_ptr<ResolutionButtonComponent> button) { _resolutionButtons.push_back(button); }

void MediatorResolutionButtons::addFullScreenButton(std::shared_ptr<FullScreenButtonComponent> button) { _fullscreenButton = button; }

void MediatorResolutionButtons::addBorderlessButton(std::shared_ptr<BorderlessButtonComponent> button) { _borderlessButton = button; }

void MediatorResolutionButtons::UnLoad() { _resolutionButtons.clear(); _fullscreenButton.reset(); _borderlessButton.reset(); }

// Deactivate all the buttons that are not the caller
void MediatorResolutionButtons::deactivateOtherResolutionButtons(ResolutionButtonComponent* caller)
{
	for (int i = 0; i < _resolutionButtons.size(); i++)
	{
		if (_resolutionButtons[i].get() != caller)
		{
			_resolutionButtons[i]->setActive(false);
		}
	}
}

void MediatorResolutionButtons::deactivateAllResolutionButtons()
{
	for (int i = 0; i < _resolutionButtons.size(); i++)
	{
		_resolutionButtons[i]->setActive(false);
	}
}

void MediatorResolutionButtons::deactivateBorderless() { _borderlessButton->setActive(false); }
void MediatorResolutionButtons::activateBorderless() { _borderlessButton->setActive(true); }
bool MediatorResolutionButtons::isBorderlessActive() const { return _borderlessButton->isActive(); }

void MediatorResolutionButtons::deactivateFullScreen() { _fullscreenButton->setActive(false); }
void MediatorResolutionButtons::activateFullScreen() { _fullscreenButton->setActive(true); }

// Base sound button
SoundButtonComponent::SoundButtonComponent(Entity* p) : _On(false),  ButtonComponent(p) { }

void SoundButtonComponent::setMediator(std::shared_ptr<MediatorSoundButtons> mediator) { _mediator = mediator; }

void SoundButtonComponent::setAsOnButton() { _On = true; }

// Button that handles turning the music on/off
MusicsButtonComponent::MusicsButtonComponent(Entity* p) : SoundButtonComponent(p) { }

void MusicsButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	if (!_active && PressedCooldown <= 0.0f)
	{
		bool changeMusicStatus = false;
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				changeMusicStatus = true;
			}
		}
		else if (_controllerPressed && _controllerHovered)
		{
			_controllerPressed = false;
			_controllerHovered = false;
			changeMusicStatus = true;
		}
		if (changeMusicStatus)
		{
			_active = true;
			if (_On)
			{
				_mediator->deactivateOtherMusicButton(this);
				Audio::turnMusicOn();
			}
			else
			{
				_mediator->deactivateOtherMusicButton(this);
				Audio::turnMusicOff();
			}
			PressedCooldown = 2.5f;
		}
	}

}
// Button that handles turning the effects on/off
EffectsButtonComponent::EffectsButtonComponent(Entity* p) : SoundButtonComponent(p) { }

void EffectsButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	if (!_active && PressedCooldown <= 0.0f)
	{
		bool changeEffectsStatus = false;
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				changeEffectsStatus = true;
			}
		}
		else if (_controllerPressed && _controllerHovered)
		{
			_controllerPressed = false;
			_controllerHovered = false;
			changeEffectsStatus = true;
		}
		if (changeEffectsStatus)
		{
			_active = true;
			if (_On)
			{
				_mediator->deactivateOtherEffectsButton(this);
				Audio::turnEffectsOn();
			}
			else
			{
				_mediator->deactivateOtherEffectsButton(this);
				Audio::turnEffectsOff();
			}
			PressedCooldown = 2.5f;
		}
	}
}

// Mediator that handles communication between sound buttons
void MediatorSoundButtons::addMusicButton(std::shared_ptr<MusicsButtonComponent> button) { _musicButtons.push_back(button); }

void MediatorSoundButtons::addEffectsButton(std::shared_ptr<EffectsButtonComponent> button) { _effectsButtons.push_back(button); }

// Deactivate all the music buttons that are not the caller
void MediatorSoundButtons::deactivateOtherMusicButton(MusicsButtonComponent* caller)
{
	for (int i = 0; i < _musicButtons.size(); i++)
	{
		if (_musicButtons[i].get() != caller)
		{
			_musicButtons[i]->setActive(false);
		}
	}
}

// Deactivate all the effect buttons that are not the caller
void MediatorSoundButtons::deactivateOtherEffectsButton(EffectsButtonComponent* caller)
{
	for (int i = 0; i < _effectsButtons.size(); i++)
	{
		if (_effectsButtons[i].get() != caller)
		{
			_effectsButtons[i]->setActive(false);
		}
	}
}

void MediatorSoundButtons::UnLoad()
{
	_musicButtons.clear();
	_effectsButtons.clear();
}

// Button that handles control mapping
ControlsButton::ControlsButton(Entity* p) : _unnactive(false), ButtonComponent(p) { }

void ControlsButton::setAction(std::string action) { _action = action; }

void ControlsButton::setDeactive(bool deactivate) { _unnactive = deactivate; }

void ControlsButton::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	// In the first just set active to true with the purpose of allowing the sprite to change
	if (!_active && PressedCooldown <= 0.0f)
	{
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = true;
				PressedCooldown = 2.5f;
				return;
			}
		}
		else if (_controllerPressed && _controllerHovered)
		{
			_active = true;
			PressedCooldown = 2.5f;
			return;
		}
	}
	// Control mapping
	if (!_unnactive && _active && PressedCooldown <= 0.0f)
	{
		if (sf::Joystick::isConnected(0))
		{
			Controller::mapInputToActionController(_action);
		}
		else
		{
			Controller::mapInputToAction(_action);
		}
		_active = false;
		_controllerPressed = false;
		PressedCooldown = 3.5f;
	}
}

// Button that resumes the game when this is paused
ResumeButton::ResumeButton(Entity* p) : ButtonComponent(p) { }

void ResumeButton::setCurrentScene(Scene* scene) { _currentScene = scene; }

void ResumeButton::update(double dt)
{
	ButtonComponent::update(dt);

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (sprite->getSprite().getGlobalBounds().contains(worldPos))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && PressedCooldown <= 0.0f && !_active)
		{
			_currentScene->setPause(false);
			PressedCooldown = 0.2f;
		}
	}
	else if(_controllerPressed && _controllerHovered && PressedCooldown <= 0.0f)
	{
		_controllerPressed = false;
		_currentScene->setPause(false);
		PressedCooldown = 0.2f;
	}
}

// Observer that is going to deactivate/reactivate the control buttons that cannot be modified by the controller
void ObserverControls::setControllerActive(bool active) 
{
	_controllerActive = active; 
	this->notify();
}

bool ObserverControls::isControllerActiveSet() const { return _controllerActive; }

void ObserverControls::notify()
{
	for (int i = 0; i < _views.size(); i++)
	{
		_views[i]->setActive(_controllerActive);
		_views[i]->setDeactive(_controllerActive);
	}
}

void ObserverControls::attach(std::shared_ptr<ControlsButton> button)
{
	_views.push_back(button);
}

void ObserverControls::UnLoad()
{
	_views.clear();
}
