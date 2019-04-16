#include "cmp_ai_steering.h"
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "../game.h"

using namespace std;
using namespace sf;

//debugging
/*float playerPosX = -1000.0f;
float ghostPosX = -1000.0f;
float dist = -1000.0f;*/

void AISteeringComponent::update(double dt) {
	
	isMoving = false;

	//debugging:
	/*if (playerPosX != _player->getPosition().x) {
		playerPosX = _player->getPosition().x;
		printf("player pos (x): %f\n", playerPosX);
	}
	if (ghostPosX != _parent->getPosition().x) {
		ghostPosX = _parent->getPosition().x;
		printf("ghost pos (x): %f\n", ghostPosX);
	}
	if (dist != length(_parent->getPosition() - _player->getPosition())) {
		dist = length(_parent->getPosition() - _player->getPosition());
		printf("distance between ghost and player: %f\n", dist);
	}*/
	
	float curDist = length(_parent->getPosition() - _player->getPosition());
	printf("%f\n", curDist);

	//check if player should be dead:
	if (curDist < 30.0f) { //the player is dead
		//show player death then reset the scene
		//Engine::ChangeScene(&levelOne); //this works here but should probably be in the player code somewhere after the death animation has played
		printf("player should be dead...\n");
	}


	//if not, check if target (player) is less than _distance pixels away, chase
	if (curDist < _distance) {
		auto output = _seek.getSteering();
		if (curDist > 400.0f) { //if the distance gets too great, increase the speed a little to make the ghost catch up
			move(output.direction * (float)dt * 1.5f); //TODO create some sort of curve so that speed transitions are nicer
		}
		else if (curDist < 50.0f) { //else, if the ghost is REALLY close, slow it down a little to give the player a bit of a chance
			move(output.direction * (float)dt * 0.75f);
		}
		else {
			move(output.direction * (float)dt);
		}

		
		
		isMoving = true;
		//printf("I should be moving\n");
	}
}

AISteeringComponent::AISteeringComponent(Entity* p, Entity* player, float speed, float distance)
	: _player(player), _seek(Seek(p, player, speed)), _distance(distance), isMoving(false), Component(p) {}

bool AISteeringComponent::validMove(const sf::Vector2f& pos) const {
	//Whilst this will be good practice to have, in the case of the ghost, 
	//it will never manage to make it outside the game boundaries 
	//because the player won't either
	/*if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x ||
		pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y) {
		return false;
	}*/
	return true;
}

void AISteeringComponent::move(const sf::Vector2f &p) {
	auto new_pos = _parent->getPosition() + p;
	if (validMove(new_pos)) {
		_parent->setPosition(new_pos);
	}
}

void AISteeringComponent::move(float x, float y) { move(Vector2f(x, y)); }

bool AISteeringComponent::getMovement() {
	return isMoving;
}