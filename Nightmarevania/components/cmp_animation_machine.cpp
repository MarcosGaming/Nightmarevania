#include "cmp_animation_machine.h"
#include "cmp_player_physics.h"

// ANIMATION
void Animation::addFrame(sf::IntRect frame) { frames.push_back(frame); }

void Animation::runFrames(Entity* owner, float waitTime)
{
	auto sprite = owner->get_components<SpriteComponent>()[0];
	sprite->getSprite().setTextureRect(frames[current_frame]);
	if (clock.getElapsedTime().asSeconds() > waitTime)
	{
		current_frame++;
		clock.restart();
	}
	if (current_frame >= frames.size())
	{
		current_frame = 0;
	}
	// Set in which direction the sprite should be facing
	setFacing(owner);
}

void Animation::setFacing(Entity* owner)
{
	auto sprite = owner->get_components<SpriteComponent>()[0];
	auto movement = owner->get_components<PlayerPhysicsComponent>()[0];
	// Rotate sprite based on where the character is looking at
	if (movement->getVelocity().x > 0.1f && !facingRight)
	{
		sprite->getSprite().setRotation(0.0f);
		facingRight = true;
	}
	else if (movement->getVelocity().x < -0.1f && facingRight)
	{
		sprite->getSprite().setRotation(90.0f);
		facingRight = false;
	}
}

// ANIMATION MACHINE COMPONENT
AnimationMachineComponent::AnimationMachineComponent(Entity* p) : _current_animation(nullptr), Component(p) {}

void AnimationMachineComponent::update(double dt)
{
	if (_current_animation != nullptr)
	{
		_current_animation->execute(_parent, dt);
	}
}

void AnimationMachineComponent::addAnimation(const std::string& name, std::shared_ptr<Animation> animation) noexcept
{ 
	_animations[name] = animation; 
}

std::shared_ptr<Animation> AnimationMachineComponent::getAnimation(const std::string& name) const noexcept
{
	auto found = _animations.find(name);
	if (found != _animations.end())
	{
		return found->second;
	}
	else
	{
		return nullptr;
	}
}

void AnimationMachineComponent::removeAnimation(const std::string& name) noexcept
{
	_animations.erase(name);
}

void AnimationMachineComponent::changeAnimation(const std::string& name) noexcept
{
	auto found = _animations.find(name);
	if (found != _animations.end())
	{
		_current_animation = found->second;
		_current_animation_name = name;
	}
}