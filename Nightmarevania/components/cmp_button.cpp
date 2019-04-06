#include "cmp_button.h"
#include "cmp_sprite.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>
#include <system_resolution.h>

static float PressedCooldown = 0.5f;

// Basic button
ButtonComponent::ButtonComponent(Entity* p) : _active(false), Component(p) { }

void ButtonComponent::setNormal(sf::IntRect normal) { _normal = normal; }
void ButtonComponent::setHovered(sf::IntRect hovered) { _hovered = hovered; }
void ButtonComponent::setPressed(sf::IntRect pressed) { _pressed = pressed; }

void ButtonComponent::setActive(bool active) { _active = active; }
bool ButtonComponent::isActive() const { return _active; }

void ButtonComponent::update(double dt)
{
	PressedCooldown -= dt;

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (_active)
	{
		sprite->getSprite().setTextureRect(_pressed);
	}
	else if (sprite->getSprite().getGlobalBounds().contains(worldPos))
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && PressedCooldown <= 0.0f)
		{
			Engine::ChangeScene(_scene_to_change);
			PressedCooldown = 1.0f;
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
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && PressedCooldown <= 0.0f)
		{
			Engine::GetWindow().close();
		}
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
	if(!_active && PressedCooldown <= 0.0f)
	{
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = true;
				_mediator->deactivateOtherResolutionButtons(this);
				_mediator->deactivateFullScreen();
				// Check if the window is currently borderless
				bool borderless = false;
				if (_mediator->isBorderlessActive())
				{
					borderless = true;
				}
				// Change to the corresponding resolution
				if (_is1920x1080)
				{
					Resolution::changeTo1920x1080(borderless);
				}
				else if (_is1600x900)
				{
					Resolution::changeTo1600x900(borderless);
				}
				else if (_is1280x720)
				{
					Resolution::changeTo1280x720(borderless);
				}
				else if (_is1024x576)
				{
					Resolution::changeTo1024x576(borderless);
				}
				PressedCooldown = 1.0f;
			}
		}
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
				PressedCooldown = 1.0f;
			}
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
				PressedCooldown = 1.0f;
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
				PressedCooldown = 1.5f;
			}
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
				PressedCooldown = 1.5f;
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

void MediatorResolutionButtons::deactivateOtherResolutionButtons(ResolutionButtonComponent* caller)
{
	// Deactivate all the buttons that are not the caller
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
	// Deactivate all resolution buttons
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
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
				PressedCooldown = 1.0f;
			}
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
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
				PressedCooldown = 1.0f;
			}
		}
	}
}

// Mediator that handles communication between sound buttons
void MediatorSoundButtons::addMusicButton(std::shared_ptr<MusicsButtonComponent> button) { _musicButtons.push_back(button); }

void MediatorSoundButtons::addEffectsButton(std::shared_ptr<EffectsButtonComponent> button) { _effectsButtons.push_back(button); }

void MediatorSoundButtons::deactivateOtherMusicButton(MusicsButtonComponent* caller)
{
	// Deactivate all the buttons that are not the caller
	for (int i = 0; i < _musicButtons.size(); i++)
	{
		if (_musicButtons[i].get() != caller)
		{
			_musicButtons[i]->setActive(false);
		}
	}
}

void MediatorSoundButtons::deactivateOtherEffectsButton(EffectsButtonComponent* caller)
{
	// Deactivate all the buttons that are not the caller
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