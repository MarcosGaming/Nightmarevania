#pragma once
#include <ecm.h>
#include <engine.h>

class ButtonComponent : public Component
{
protected:
	sf::IntRect _normal;
	sf::IntRect _hovered;
	sf::IntRect _pressed;

	bool _active;
	bool _controllerHovered;
	bool _controllerPressed;
	bool _canHoverActive;

public:
	ButtonComponent() = delete;
	explicit ButtonComponent(Entity* p);

	virtual void update(double dt) override = 0;

	void render() override final { }

	void setNormal(sf::IntRect);

	void setHovered(sf::IntRect);

	void setPressed(sf::IntRect);

	void setControllerHovered(bool);

	void setControllerPressed(bool);

	void setCanHoverActive(bool);

	static void ButtonNavigation(const std::vector<std::shared_ptr<ButtonComponent>>&, int&, const double& dt);

	void setActive(bool);
	bool isActive() const;

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

class MediatorResolutionButtons;

class ResolutionButtonComponent : public ButtonComponent
{
private:
	std::shared_ptr<MediatorResolutionButtons> _mediator;
	bool _is1920x1080;
	bool _is1600x900;
	bool _is1280x720;
	bool _is1024x576;

public:
	ResolutionButtonComponent() = delete;
	explicit ResolutionButtonComponent(Entity* p);

	void update(double dt) override final;

	void setMediator(std::shared_ptr<MediatorResolutionButtons>);

	void setResolutionTo1920x1080();
	void setResolutionTo1600x900();
	void setResolutionTo1280x720();
	void setResolutionTo1024x576();

};

class FullScreenButtonComponent : public ButtonComponent
{
private:
	sf::IntRect _hoveredActive;
	std::shared_ptr<MediatorResolutionButtons> _mediator;

public:
	FullScreenButtonComponent() = delete;
	explicit FullScreenButtonComponent(Entity* p);

	void setMediator(std::shared_ptr<MediatorResolutionButtons>);

	void update(double dt) override final;

	void setHoveredActive(sf::IntRect);
};

class BorderlessButtonComponent : public ButtonComponent
{
private:
	sf::IntRect _hoveredActive;
	std::shared_ptr<MediatorResolutionButtons> _mediator;

public:
	BorderlessButtonComponent() = delete;
	explicit BorderlessButtonComponent(Entity* p);

	void setMediator(std::shared_ptr<MediatorResolutionButtons>);

	void update(double dt) override final;

	void setHoveredActive(sf::IntRect);
};

class MediatorResolutionButtons
{
private:
	std::vector<std::shared_ptr<ResolutionButtonComponent>> _resolutionButtons;
	std::shared_ptr<FullScreenButtonComponent> _fullscreenButton;
	std::shared_ptr<BorderlessButtonComponent> _borderlessButton;

public:
	MediatorResolutionButtons() = default;

	void addResolutionButton(std::shared_ptr<ResolutionButtonComponent>);

	void addFullScreenButton(std::shared_ptr<FullScreenButtonComponent>);

	void addBorderlessButton(std::shared_ptr<BorderlessButtonComponent>);

	void deactivateOtherResolutionButtons(ResolutionButtonComponent*);

	void deactivateAllResolutionButtons();

	void deactivateBorderless();
	void activateBorderless();
	bool isBorderlessActive() const;

	void deactivateFullScreen();
	void activateFullScreen();

	void UnLoad();
};

class MediatorSoundButtons;
class SoundButtonComponent : public ButtonComponent
{
protected:
	bool _On;
	std::shared_ptr<MediatorSoundButtons> _mediator;
	
public:
	SoundButtonComponent() = delete;
	explicit SoundButtonComponent(Entity* p);

	void setMediator(std::shared_ptr<MediatorSoundButtons>);

	void setAsOnButton();
};

class MusicsButtonComponent : public SoundButtonComponent
{
public:
	MusicsButtonComponent() = delete;
	explicit MusicsButtonComponent(Entity* p);

	void update(double dt) override final;
};


class EffectsButtonComponent : public SoundButtonComponent
{
public:
	EffectsButtonComponent() = delete;
	explicit EffectsButtonComponent(Entity* p);

	void update(double dt) override final;
};

class MediatorSoundButtons
{
private:
	std::vector<std::shared_ptr<MusicsButtonComponent>> _musicButtons;
	std::vector<std::shared_ptr<EffectsButtonComponent>> _effectsButtons;

public:
	MediatorSoundButtons() = default;

	void addMusicButton(std::shared_ptr<MusicsButtonComponent>);

	void addEffectsButton(std::shared_ptr<EffectsButtonComponent>);

	void deactivateOtherMusicButton(MusicsButtonComponent*);

	void deactivateOtherEffectsButton(EffectsButtonComponent*);

	void UnLoad();
};

class ControlsButton : public ButtonComponent
{
private:
	std::string _action;
	bool _unnactive;
public:
	ControlsButton() = delete;
	explicit ControlsButton(Entity* p);

	void update(double dt) override final;

	void setAction(std::string);

	void setDeactive(bool);
};

class ResumeButton : public ButtonComponent
{
private:
	Scene* _currentScene;
public:
	ResumeButton() = delete;
	explicit ResumeButton(Entity* p);

	void setCurrentScene(Scene*);

	void update(double dt) override final;
};

class ObserverControls
{
private:
	std::vector<std::shared_ptr<ControlsButton>> _views;
	bool _controllerActive;

	void notify();

public:
	ObserverControls() = default;

	void setControllerActive(bool);
	bool isControllerActiveSet() const;

	void attach(std::shared_ptr<ControlsButton>);

	void UnLoad();
};