#pragma once
#include "ecm.h"
#include "../steering.h"

using namespace std;

class AISteeringComponent : public Component
{
protected:
	Seek _seek;
	float _distance; //closest distance at which player needs to be to trigger seek behaviour
	//Flee _flee; //not currently using flee
	Entity *_player;
	bool validMove(const sf::Vector2f&) const;
	bool isMoving;

public:
	void update(double) override;
	void move(const sf::Vector2f&);
	void move(float x, float y);
	void render() override { }
	//explicit AISteeringComponent(Entity * p, Entity * player, float speed, float distance);
	explicit AISteeringComponent(Entity * p, Entity * player, float speed, float distance);
	AISteeringComponent() = delete;
	bool getMovement();
};