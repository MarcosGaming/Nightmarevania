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

	void mapActionToKey(std::string, sf::Keyboard::Key);

	void mapActionToMouseButton(std::string, sf::Mouse::Button);

	void mapActionToControllerButton(std::string, int);

	void processEvent(sf::Event);

	bool isAttackButtonReleased();
	void setAttackButtonReleased(bool);

	bool isDefendButtonReleased();
	void setDefendButtonReleased(bool);

	bool isJumpButtonReleased();
	void setJumpButtonReleased(bool);

	bool isPressed(std::string);

};