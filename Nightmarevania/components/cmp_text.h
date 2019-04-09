#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextComponent : public Component 
{
public:
  TextComponent() = delete;
  explicit TextComponent(Entity* p, const std::string& str = "");
  virtual void update(double dt) override;

  void render() override;

  ~TextComponent() override = default;

  void SetText(const std::string& str);

protected:
  std::shared_ptr<sf::Font> _font;
  std::string _string;
  sf::Text _text;
};

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
