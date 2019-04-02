#pragma once
#include <ecm.h>
#include <engine.h>

class ButtonComponent : public Component
{
protected:
	sf::IntRect _normal;
	sf::IntRect _hovered;

public:
	ButtonComponent() = delete;
	explicit ButtonComponent(Entity* p);

	virtual void update(double dt) override;

	void setNormal(sf::IntRect);

	void setHovered(sf::IntRect);
};

class ChangeSceneButtonComponent : public ButtonComponent 
{
protected:
	Scene* _scene_to_change;

public:
	ChangeSceneButtonComponent() = delete;
	explicit ChangeSceneButtonComponent(Entity* p);

	void update(double dt) override;

	void render() override {}

	void setScene(Scene*);
};

class ExitButtonComponent : public ButtonComponent
{
public:
	ExitButtonComponent() = delete;
	explicit ExitButtonComponent(Entity* p);

	void update(double dt) override;

	void render() override {}
};
