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

// Button that can be active
ActiveButtonComponent::ActiveButtonComponent(Entity* p) : _active(false), ButtonComponent(p) { }

void ActiveButtonComponent::setPressed(sf::IntRect pressed) { _pressed = pressed; }

bool ActiveButtonComponent::isActive() const { return _active; }
void ActiveButtonComponent::setActive(bool active) { _active = active; }

void ActiveButtonComponent::update(double dt)
{
	ButtonComponent::update(dt);
	auto sprite = _parent->get_components<SpriteComponent>()[0];
	if (_active)
	{
		sprite->getSprite().setTextureRect(_pressed);
	}
}

// Mediator design pattern that is going to handle desactivating connected buttons
void MediatorResolutionButtons::addResolutionButton(std::shared_ptr<SpecificResolutionButtonComponent> button) { _resolutionButtons.push_back(button); }

void MediatorResolutionButtons::UnLoad() { _resolutionButtons.clear(); }

void MediatorResolutionButtons::deactivateOtherResolutionButtons(SpecificResolutionButtonComponent* caller)
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

// Base class for buttons that handle the screen resolution
BaseResolutionButtonComponent::BaseResolutionButtonComponent(Entity* p) : ActiveButtonComponent(p) { }

void BaseResolutionButtonComponent::setMediator(std::shared_ptr<MediatorResolutionButtons> mediator) { _mediator = mediator; }

// Button that changes the screen to a specific resolution
SpecificResolutionButtonComponent::SpecificResolutionButtonComponent(Entity* p) : BaseResolutionButtonComponent(p) { }

void SpecificResolutionButtonComponent::setResolution(int width, int height) { _width = width; _height = height; }

void SpecificResolutionButtonComponent::update(double dt)
{
	ActiveButtonComponent::update(dt);

	static float pressedCooldown = 1.0f;
	pressedCooldown -= dt;

	auto sprite = _parent->get_components<SpriteComponent>()[0];
	if(!_active && pressedCooldown <= 0.0f)
	{
		sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_active = true;
				changeScreenResolution(_width, _height);
				_mediator->deactivateOtherResolutionButtons(this);
				pressedCooldown = 1.0f;
			}
		}
	}
}

// Button that sets the game to fullsscreen
FullScreenButtonComponent::FullScreenButtonComponent(Entity* p) : BaseResolutionButtonComponent(p) { }

void FullScreenButtonComponent::update(double dt)
{
	ActiveButtonComponent::update(dt);

	static float pressedCooldown = 1.0f;
	pressedCooldown -= dt;


	auto sprite = _parent->get_components<SpriteComponent>()[0];
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Engine::GetWindow()));
	if (!_active && pressedCooldown <= 0.0f)
	{
		if (sprite->getSprite().getGlobalBounds().contains(worldPos))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

			}
		}
	}
}
// Buttons that sets the game to borderless window
BorderlessButtonComponent::BorderlessButtonComponent(Entity* p) : BaseResolutionButtonComponent(p) { }

void BorderlessButtonComponent::update(double dt)
{
	ActiveButtonComponent::update(dt);
}

// Create FloatRect to fits Game into Screen while preserving aspect
sf::FloatRect BaseResolutionButtonComponent::calculateViewport(const sf::Vector2u& screensize, const sf::Vector2u& gamesize)
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

	//calculate as percent of screen
	const float widthPercent = (scaledWidth / screensf.x);
	const float heightPercent = (scaledHeight / screensf.y);

	return sf::FloatRect(0, 0, widthPercent, heightPercent);
}
// Change screen resolution
void BaseResolutionButtonComponent::changeScreenResolution(int width, int height)
{
	const sf::Vector2u screensize(width, height);
	const sf::Vector2u gamesize(GAMEX, GAMEY);
	//set View as normal
	Engine::GetWindow().setSize(screensize);
	sf::FloatRect visibleArea(0.f, 0.f, gamesize.x, gamesize.y);
	auto v = sf::View(visibleArea);
	// figure out how to scale and maintain aspect;
	auto viewport = calculateViewport(screensize, gamesize);
	//Optionally Center it
	bool centered = true;
	if (centered)
	{
		viewport.left = (1.0 - viewport.width) * 0.5;
		viewport.top = (1.0 - viewport.height) * 0.5;
	}
	v.setViewport(viewport);
	Engine::GetWindow().setView(v);
}