#pragma once

#include <SFML\Window.hpp>

namespace Controller
{
	void initialise();

	const std::string MoveRightButton = "MoveRight";
	const std::string MoveLeftButton = "MoveLeft";
	const std::string JumpButton = "Jump";
	const std::string AttackButton = "Attack";
	const std::string DefendButton = "Defend";
	const std::string UpAttackButton = "UpAttack";
	const std::string DownAttackButton = "DownAttack";
	const std::string PauseButton = "Pause";

	sf::Keyboard::Key* getActionKey(std::string&);
	sf::Mouse::Button* getActionMouseButton(std::string&);
	int* getActionControllerButton(std::string&);

	void processEvent(sf::Event);

	bool isAttackButtonReleased();
	void setAttackButtonReleased(bool);

	bool isDefendButtonReleased();
	void setDefendButtonReleased(bool);

	bool isJumpButtonReleased();
	void setJumpButtonReleased(bool);

	bool isPressed(std::string);

	void mapInputToAction(std::string& action);
	void mapInputToActionController(std::string& action);

};