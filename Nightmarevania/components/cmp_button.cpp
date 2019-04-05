#include "cmp_button.h"
#include "cmp_sprite.h"
#include "../game.h"
#include <system_renderer.h>
#include <system_sound.h>

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
ResolutionButtonComponent::ResolutionButtonComponent(Entity* p) : ButtonComponent(p) { }

void ResolutionButtonComponent::setResolution(int width, int height) { _width = width; _height = height; }

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
				if (_mediator->isBorderlessActive())
				{
					Engine::GetWindow().create(sf::VideoMode(_width, _height), "Nightmarevania", sf::Style::None);
					ChangeScreenResolution(_width, _height);
				}
				else
				{
					Engine::GetWindow().create(sf::VideoMode(_width, _height), "Nightmarevania", sf::Style::Close);
					ChangeScreenResolution(_width, _height);
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
				int x = sf::VideoMode::getDesktopMode().width;
				int y = sf::VideoMode::getDesktopMode().height;
				Engine::GetWindow().create(sf::VideoMode(x, y), "Nightmarevania", sf::Style::Fullscreen);
				ChangeScreenResolution(x, y);
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
				int x = sf::VideoMode::getDesktopMode().width;
				int y = sf::VideoMode::getDesktopMode().height;
				Engine::GetWindow().create(sf::VideoMode(x, y), "Nightmarevania", sf::Style::Close);
				ChangeScreenResolution(x, y);
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
				int previousX = Engine::GetWindow().getSize().x;
				int previousY = Engine::GetWindow().getSize().y;
				Engine::GetWindow().create(sf::VideoMode(previousX, previousY), "Nightmarevania", sf::Style::None);
				ChangeScreenResolution(previousX, previousY);
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
				int previousX = Engine::GetWindow().getSize().x;
				int previousY = Engine::GetWindow().getSize().y;
				Engine::GetWindow().create(sf::VideoMode(previousX, previousY), "Nightmarevania", sf::Style::Close);
				ChangeScreenResolution(previousX, previousY);
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
					SoundSystem::turnMusicOn();
				}
				else
				{
					_mediator->deactivateOtherMusicButton(this);
					SoundSystem::turnMusicOff();
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
					SoundSystem::turnEffectsOn();
				}
				else
				{
					_mediator->deactivateOtherEffectsButton(this);
					SoundSystem::turnEffectsOff();
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

// Create FloatRect to fits Game into Screen while preserving aspect
sf::FloatRect CalculateViewport(const sf::Vector2u& screensize, const sf::Vector2u& gamesize)
{

	const sf::Vector2f screensf(screensize.x, screensize.y);
	const sf::Vector2f gamesf(gamesize.x, gamesize.y);
	const float gameAspect = gamesf.x / gamesf.y;
	const float screenAspect = screensf.x / screensf.y;
	// Final size.x of game viewport in pixels
	float scaledWidth;
	// Final size.y of game viewport in pixels
	float scaledHeight;
	// False = scale to screen.x, True = screen.y
	bool scaleSide = false;

	//Work out which way to scale
	if (gamesize.x > gamesize.y)
	{
		// Game is wider than tall, can we use full width?
		if (screensf.y < (screensf.x / gameAspect)) {
			// No, not high enough to fit game height
			scaleSide = true;
		}
		else {
			// Yes, use all width available
			scaleSide = false;
		}
	}
	else
	{
		// Game is Square or Taller than Wide, can we use full height?
		if (screensf.x < (screensf.y * gameAspect))
		{
			// No, screensize not wide enough to fit game width
			scaleSide = false;
		}
		else
		{
			// Yes, use all height available
			scaleSide = true;
		}
	}

	if (scaleSide)
	{
		// Use max screen height
		scaledHeight = screensf.y;
		scaledWidth = floor(scaledHeight * gameAspect);
	}
	else
	{
		// Use max screen width
		scaledWidth = screensf.x;
		scaledHeight = floor(scaledWidth / gameAspect);
	}

	// Calculate as percent of screen
	const float widthPercent = (scaledWidth / screensf.x);
	const float heightPercent = (scaledHeight / screensf.y);

	return sf::FloatRect(0, 0, widthPercent, heightPercent);
}
// Change screen resolution
void ChangeScreenResolution(int width, int height)
{
	const sf::Vector2u screensize(width, height);
	const sf::Vector2u gamesize(GAMEX, GAMEY);
	// Set View as normal
	Engine::GetWindow().setSize(screensize);
	sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
	auto v = sf::View(visibleArea);
	// Figure out how to scale and maintain aspect;
	auto viewport = CalculateViewport(screensize, gamesize);
	// Optionally Center it
	bool centered = true;
	if (centered)
	{
		viewport.left = (1.0 - viewport.width) * 0.5;
		viewport.top = (1.0 - viewport.height) * 0.5;
	}
	v.setViewport(viewport);
	Engine::GetWindow().setView(v);
}