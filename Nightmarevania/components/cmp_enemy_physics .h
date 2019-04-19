#pragma once

#include "cmp_physics.h"

class EnemyPhysicsComponent : public PhysicsComponent 
{
protected:
  b2Vec2 _size;

  sf::Vector2f _maxSlowVelocity;
  sf::Vector2f _maxFastVelocity;

  sf::Vector2f _moveDirection;

  bool _grounded;
  float _groundspeed;

  bool _canMove;
  
  bool _slow;

public:
  void update(double dt) override;

  explicit EnemyPhysicsComponent(Entity* p, const sf::Vector2f& size);

  EnemyPhysicsComponent() = delete;

  bool isGrounded() const;

  void setCanMove(bool);
  bool canMove() const;

  void setSlow(bool);
  bool isSlowMove() const;

  void setMoveDirection(sf::Vector2f);
};
