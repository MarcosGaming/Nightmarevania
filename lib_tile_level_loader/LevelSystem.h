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

	enum TILES
	{
		/*WALL1 = '1',
		WALL2 = '2',
		WALL3 = '3',
		WALL4 = '4',
		WALL5 = '5',
		WALL6 = '6',
		WALL7 = '7',
		WALL8 = '8',
		CORNER1 = '9',
		CORNER2 = '0',
		CORNER3 = '-',
		CORNER4 = '=',
		PLATFORM = 'p',*/

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

		/*TORCHa = 'z', //top
		TORCHb = 'x', //middle
		TORCHc = 'c', //bottom

		COLUMN1a = '<', //top
		COLUMN1b = '>', //middle
		COLUMN1c = '?', //bottom

		COLUMN2a = ',', //top
		COLUMN2b = '.', //middle
		COLUMN2c = '/', //bottom*/

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

	//static sf::Color getColor(Tile t);
	static sf::IntRect getSprite(Tile t);

	//static void setColor(Tile t, sf::Color c);
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

	static void buildSprites(bool optimise = true);

	static float _tileSize; // for rendering
	static std::map<Tile, sf::Color> _colours;

private:
	LevelSystem() = delete;

	~LevelSystem() = delete;
};
