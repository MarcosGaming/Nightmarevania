#include "cmp_animation_machine.h"
#include "cmp_player_physics.h"

// ANIMATION
Animation::Animation() : _current_frame(0) {}

void Animation::addFrame(sf::IntRect frame) { _frames.push_back(frame); }

void Animation::runFrames(Entity* owner, float waitTime)
{
	auto sprite = owner->get_components<SpriteComponent>()[0];
	// Set the frame
	sprite->getSprite().setTextureRect(_frames[_current_frame]);
	// Set in which direction the sprite should be facing
	setFacing(owner);
	// Change frame
	if (_clock.getElapsedTime().asSeconds() > waitTime)
	{
		_current_frame++;
		_clock.restart();
	}
	if (_current_frame >= _frames.size())
	{
		_current_frame = 0;
	}
}

void Animation::setFacing(Entity* owner)
{
	auto sprite = owner->get_components<SpriteComponent>()[0];
	auto movement = owner->GetCompatibleComponent<PhysicsComponent>()[0];
	// Rotate sprite based on the character movement
	if (movement->getVelocity().x > 0.5f && !owner->isFacingRight())
	{
		sprite->getSprite().scale(-1.0f, 1.0f);
		owner->setFacingRight(true);
	}
	else if (movement->getVelocity().x < -0.5f && owner->isFacingRight())
	{
		sprite->getSprite().scale(-1.0f, 1.0f);
		owner->setFacingRight(false);
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