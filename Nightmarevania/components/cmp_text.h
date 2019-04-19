#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>
#include <functional>

class TextComponent : public Component 
{
public:
  TextComponent() = delete;
  explicit TextComponent(Entity* p, const std::string& str = "");
  virtual void update(double dt) override;

  void render() override;

  ~TextComponent() override = default;

  void SetText(const std::string& str);

  void setTextSize(const int);

protected:
  std::shared_ptr<sf::Font> _font;
  std::string _string;
  sf::Text _text;
};

// Text used for the controls in the settings scene
class ControlsTextComponent : public TextComponent
{
private:
	std::string _action;
	sf::Vector2f _initialPosition;
public:
	ControlsTextComponent() = delete;
	explicit ControlsTextComponent(Entity*, const std::string& str = "");

	void update(double) override;

	void setInitialPosition(sf::Vector2f);

	void setAction(std::string);
};

// Text used for the dialogue boxes
class DialogueBoxComponent : public TextComponent
{
private:
	std::string _completeText;
	int _currentChar;
	bool _finished;

	std::function<void()> _func;

	double _dt;

public:
	DialogueBoxComponent() = delete;
	explicit DialogueBoxComponent(Entity*, const std::string& str = "");

	void update(double) override;

	void setCompleteText(const std::string&);

	void setFunction(std::function<void()>);

	void serahGetUpDialogueUpdate();

	void outsideLevelDialogueUpdate();

	void moveWhileDialogueUpdate();

	void swordDialogueUpdate();

	void beginBattleDialogueUpdate(bool&);

	void endBattleDialogueUpdate();

	void endingDialogueUpdate();
	
	void theEndDialogueUpdate();
};