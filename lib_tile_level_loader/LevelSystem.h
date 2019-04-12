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
		/*EMPTY = ' ',
		START = 's',
		END = 'e',
		WALL = 'w',
		ENEMY = 'n',
		WAYPOINT = '+'*/

		BG1 = 'r',
		BG2 = 't',
		BG3 = 'y',
		BG4 = 'u',
		BG5 = 'i',
		BG6 = 'o',
		BG7 = 'p',
		BG8 = '[',
		BG9 = ']',
		BG10 = 'd',
		BG11 = 'v',
		BG12 = 'g',
		BG13 = 'h',
		BG14 = 'j',
		BG15 = 'k',
		BG16 = 'l',
		BG17 = ';',
		BG18 = '#',

		SPIKES1 = '!',
		SPIKES2 = '$',
		DOOR = '1',
		/*UDOOR1 = '!',
		UDOOR2 = '$',
		UDOOR3 = '&',
		UDOOR4 = '*',
		UDOOR5 = '%',
		UDOOR6 = '"',
		UDOOR7 = '�',
		UDOOR8 = '^',
		UDOOR9 = '(',*/

		/*LDOOR1 = '1',
		LDOOR2 = '4',
		LDOOR3 = '7',
		LDOOR4 = '8',
		LDOOR5 = '5',
		LDOOR6 = '2',
		LDOOR7 = '3',
		LDOOR8 = '6',
		LDOOR9 = '9',*/

		TORCHa = 'z', //top
		TORCHb = 'x', //middle
		TORCHc = 'c', //bottom

		COLUMN1a = '<', //top
		COLUMN1b = '>', //middle
		COLUMN1c = '?', //bottom

		COLUMN2a = ',', //top
		COLUMN2b = '.', //middle
		COLUMN2c = '/', //bottom

		END = 'e',
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
