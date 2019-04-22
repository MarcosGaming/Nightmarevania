#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>

#define ls LevelSystem

class LevelSystem
{
public:
	static void loadLevelFile(const std::string&, float tileSize = 100.0f);
	static void unload();
	static void render(sf::RenderWindow& window);

	typedef unsigned char Tile;

	/*enum BACKGROUND
	{
		WINDOWa = 'w', //top
		WINDOWb = 's', //middle
		WINDOWc = 'x', //bottom

		STATUEa = 'e', //top left
		STATUEb = 'd', //middle left
		STATUEc = 'c', //bottom left
		STATUEd = 'r', //top right
		STATUEe = 'f', //middle right
		STATUEf = 'v', //bottom right

		COLUMNa = 'q', //top
		COLUMNb = 'a', //middle
		COLUMNc = 'z', //bottom
	};*/

	enum TILES
	{
		WAYPOINTA = 'a',
		WAYPOINTB = 'b',

		SPIKES1 = '^',
		SPIKES2 = 'V',
		SPIKES3 = 'v',
		SPIKES4 = '>',
		SPIKES5 = '<',
		DOOR = 'd',
		KEY = 'k',
		ENEMY = 'e',

		END = '+',
		START = 's',
		EMPTY = ' ',
		WALL = 'w',
		FLOOR = 'f'
	};

	static Tile getTile(sf::Vector2ul);

	static Tile getTileAt(sf::Vector2f);

	static bool isOnGrid(sf::Vector2f);

	static size_t getWidth();

	static size_t getHeight();

	static sf::Vector2f getTilePosition(sf::Vector2ul);

	static std::vector<sf::Vector2ul> findTiles(Tile);

	static bool doesTileExist(LevelSystem::Tile type);

	static sf::IntRect getSprite(Tile t);
	static void setSprite(Tile t, sf::IntRect s);

	static void setOffset(const sf::Vector2f& _offset);

	static const sf::Vector2f& getOffset();

	static float getTileSize();

protected:
	static std::unique_ptr<Tile[]> _tiles;
	static size_t _width;
	static size_t _height;
	static sf::Vector2f _offset;

	static std::vector<std::unique_ptr<sf::RectangleShape>> _sprites;

	static std::map<Tile, sf::IntRect> _images;

	static void buildSprites();

	static float _tileSize; // for rendering
	static std::map<Tile, sf::Color> _colours;

private:
	LevelSystem() = delete;

	~LevelSystem() = delete;
};
