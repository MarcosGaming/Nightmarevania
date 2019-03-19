#pragma once 

#include <ecm.h>
#include "../components/cmp_sprite.h"
#include <unordered_map>
#include <string>

class Animation
{
protected:
	std::vector<sf::IntRect> frames;
	std::size_t current_frame = 0;
	sf::Clock clock;
	static bool facingRight;

	virtual void runFrames(Entity*, float);
public:
	Animation();
	virtual ~Animation() = default;

	void addFrame(sf::IntRect);
	void setFacing(Entity*);

	virtual void execute(Entity*, double) noexcept = 0;
};

class AnimationMachineComponent : public Component
{
private:
	std::unordered_map<std::string, std::shared_ptr<Animation>> _animations;
	std::shared_ptr<Animation> _current_animation;
	std::string _current_animation_name;

public:
	AnimationMachineComponent() = delete;
	explicit AnimationMachineComponent(Entity* p);

	void update(double) override;
	void render() override {}

	void addAnimation(const std::string&, std::shared_ptr<Animation>) noexcept;
	std::shared_ptr<Animation> getAnimation(const std::string&) const noexcept;
	void removeAnimation(const std::string&) noexcept;
	void changeAnimation(const std::string&) noexcept;
	const std::string& currentAnimation() const { return _current_animation_name; }
	
};