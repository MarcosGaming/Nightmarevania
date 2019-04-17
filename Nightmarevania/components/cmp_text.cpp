#include "cmp_text.h"
#include "cmp_player_physics.h"
#include <system_renderer.h>
#include <system_resources.h>
#include <system_controller.h>
#include <system_sound.h>
#include <conversor.h>
#include <iostream>
#include <engine.h>

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

// The text changes dynamically based on which device is connected
void ControlsTextComponent::update(double dt)
{
	if (sf::Joystick::isConnected(0))
	{
		if (_action == Controller::MoveRightButton)
		{
			SetText("LS Right");
		}
		else if (_action == Controller::MoveLeftButton)
		{
			SetText("LS Left");
		}
		else if (_action == Controller::UpAttackButton)
		{
			SetText("LS Up");
		}
		else if (_action == Controller::DownAttackButton)
		{
			SetText("LS Down");
		}
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

// Text component for the dialogue boxes
DialogueBoxComponent::DialogueBoxComponent(Entity* p, const std::string& str) : _currentChar(0), _finished(false),  TextComponent(p, str) 
{ 
	_text.setCharacterSize(20); 
}

void DialogueBoxComponent::update(double dt) 
{
	_dt = dt;
	_func();
}

void DialogueBoxComponent::setCompleteText(const std::string& text)
{
	_completeText = text;
}

void DialogueBoxComponent::setFunction(std::function<void()> function) { _func = function; }

// Function for sword dialogue
void DialogueBoxComponent::swordDialogueUpdate()
{
	Audio::playMusic("mystic_music");
	// While the dialogue box is active do not allow the player to move
	std::weak_ptr<Entity> player = _parent->scene->ents.find("Player")[0];
	if (auto pl = player.lock())
	{
		pl->get_components<PlayerPhysicsComponent>()[0]->setCanMove(false);
	}
	// The letters are placed in the text one by one
	static float stringCountDown = 0.1f;
	stringCountDown -= _dt;
	// Render the dialogue letter by letter.
	if (stringCountDown <= 0.0f && !_finished)
	{
		this->SetText(_completeText.substr(0, _currentChar));
		_currentChar++;
		stringCountDown = 0.1f;
	}
	// Once the text is fully rendered wait a bit before removing it
	static float hideCountDown = 0.8f;
	static float finalCountDown = 0.8f;
	if (_currentChar > _completeText.size())
	{
		_finished = true;
		hideCountDown -= _dt;
		if (hideCountDown <= 0.0f)
		{
			if (auto pl = player.lock())
			{
				pl->get_components<PlayerPhysicsComponent>()[0]->setCanMove(true);
			}
			_parent->setVisible(false);
			finalCountDown -= _dt;
			// Begin battle text starts
			if (finalCountDown <= 0.0f)
			{
				_parent->setAlive(false);
				_parent->scene->ents.find("BeginBattleText")[0]->setAlive(true);
				Audio::stopMusic("mystic_music");
				Audio::playEffect("boss_roar_effect");
			}
		}
	}
	_text.setPosition(sf::Vector2f(GAMEX*0.5f - (_text.getLocalBounds().width * 0.5f), GAMEY*0.5f - 100.0f));
}
// Function for final boss begin battle
void DialogueBoxComponent::beginBattleDialogueUpdate(bool& activateMusicInLevel)
{
	static float musicCountDown = 1.5f;
	musicCountDown -= _dt;
	if (musicCountDown <= 0.0f)
	{
		activateMusicInLevel = true;
	}
	// While the dialogue box is active do not allow the player to move
	std::weak_ptr<Entity> player = _parent->scene->ents.find("Player")[0];
	if (auto pl = player.lock())
	{
		pl->get_components<PlayerPhysicsComponent>()[0]->setCanMove(false);
	}
	// The letters are placed in the text one by one
	static float stringCountDown = 0.1f;
	stringCountDown -= _dt;
	// Render the dialogue letter by letter.
	if (stringCountDown <= 0.0f && !_finished)
	{
		this->SetText(_completeText.substr(0, _currentChar));
		_currentChar++;
		stringCountDown = 0.1f;
	}
	// Once the text is fully rendered wait a bit before removing it
	static float finalCountDown = 0.8f;
	if (_currentChar > _completeText.size())
	{
		_finished = true;
		finalCountDown -= _dt;
		if (finalCountDown <= 0.0f)
		{
			if (auto pl = player.lock())
			{
				pl->get_components<PlayerPhysicsComponent>()[0]->setCanMove(true);
			}
			_parent->setAlive(false);
			_parent->setVisible(false);
			// Fight begins
			_parent->scene->ents.find("Boss")[0]->setAlive(true);
			_parent->scene->ents.find("Boss")[0]->setDeath(false);
			musicCountDown = 1.5f;
		}
	}
	_text.setPosition(sf::Vector2f(GAMEX*0.5f - (_text.getLocalBounds().width * 0.5f), GAMEY*0.5f - 100.0f));
}
// Function for final boss end battle
void DialogueBoxComponent::endBattleDialogueUpdate()
{
	// While the dialogue box is active do not allow the player to move
	std::weak_ptr<Entity> player = _parent->scene->ents.find("Player")[0];
	if (auto pl = player.lock())
	{
		pl->get_components<PlayerPhysicsComponent>()[0]->setCanMove(false);
	}
	// The letters are placed in the text one by one
	static float stringCountDown = 0.1f;
	stringCountDown -= _dt;
	// Render the dialogue letter by letter.
	if (stringCountDown <= 0.0f && !_finished)
	{
		this->SetText(_completeText.substr(0, _currentChar));
		_currentChar++;
		stringCountDown = 0.1f;
	}
	// Once the text is fully rendered wait a bit before removing it
	static float finalCountDown = 0.8f;
	if (_currentChar > _completeText.size())
	{
		_finished = true;
		finalCountDown -= _dt;
		if (finalCountDown <= 0.0f)
		{
			if (auto pl = player.lock())
			{
				pl->get_components<PlayerPhysicsComponent>()[0]->setCanMove(true);
			}
			_parent->setAlive(false);
			_parent->setVisible(false);
		}
	}
	_text.setPosition(sf::Vector2f(GAMEX*0.5f - (_text.getLocalBounds().width * 0.5f), GAMEY*0.5f - 100.0f));
}