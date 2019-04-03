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

	virtual void update(double dt) override = 0;

	void render() override final { }

	void setNormal(sf::IntRect);

	void setHovered(sf::IntRect);

};

class ChangeSceneButtonComponent : public ButtonComponent 
{
private:
	Scene* _scene_to_change;

public:
	ChangeSceneButtonComponent() = delete;
	explicit ChangeSceneButtonComponent(Entity* p);

	void update(double dt) override final;

	void setScene(Scene*);
};

class ExitButtonComponent : public ButtonComponent
{
public:
	ExitButtonComponent() = delete;
	explicit ExitButtonComponent(Entity* p);

	void update(double dt) override final;
};

class ActiveButtonComponent : public ButtonComponent
{
protected:
	sf::IntRect _pressed;
	bool _active;

public:
	ActiveButtonComponent() = delete;
	explicit ActiveButtonComponent(Entity* p);

	virtual void update(double dt) override = 0;

	void setPressed(sf::IntRect);

	void setActive(bool);
	bool isActive() const;
};

class BaseResolutionButtonComponent : public ActiveButtonComponent
{
protected:
	std::shared_ptr<MediatorResolutionButtons> _mediator;

public:
	BaseResolutionButtonComponent() = delete;
	explicit BaseResolutionButtonComponent(Entity* p);

	void setMediator(std::shared_ptr<MediatorResolutionButtons>);

protected:
	sf::FloatRect calculateViewport(const sf::Vector2u& screensize, const sf::Vector2u& gamesize);
	void changeScreenResolution(int width, int height);

};


class SpecificResolutionButtonComponent : public BaseResolutionButtonComponent
{
private:
	int _width;
	int _height;

public:
	SpecificResolutionButtonComponent() = delete;
	explicit SpecificResolutionButtonComponent(Entity* p);

	void update(double dt) override final;

	void setResolution(int, int);
};

class FullScreenButtonComponent : public BaseResolutionButtonComponent
{
public:
	FullScreenButtonComponent() = delete;
	explicit FullScreenButtonComponent(Entity* p);

	void update(double dt) override final;
};

class BorderlessButtonComponent : public BaseResolutionButtonComponent
{
	BorderlessButtonComponent() = delete;
	explicit BorderlessButtonComponent(Entity* p);

	void update(double dt) override final;
};

class MediatorResolutionButtons
{
private:
	std::vector<std::shared_ptr<SpecificResolutionButtonComponent>> _resolutionButtons;
	std::shared_ptr<FullScreenButtonComponent> _fullscreenButton;
	std::shared_ptr<BorderlessButtonComponent> _borderlessButton;

public:
	MediatorResolutionButtons() = default;

	void addResolutionButton(std::shared_ptr<SpecificResolutionButtonComponent>);

	void deactivateOtherResolutionButtons(SpecificResolutionButtonComponent*);

	void deactivateAllResolutionButtons();

	void UnLoad();
};

/*class FullScreenButtonComponent : public ActiveButtonComponent
{
private:
	bool _isOn;

public:
	FullScreenButtonComponent() = delete;
	explicit FullScreenButtonComponent(Entity* p);

	void update(double dt) final override;

};*/

/*class MusicButtonComponent : public ActiveButtonComponent
{
private:
	bool _value;

public:
	MusicButtonComponent() = delete;
	explicit MusicButtonComponent(Entity* p);

	void update(double dt) override final;
};*/
