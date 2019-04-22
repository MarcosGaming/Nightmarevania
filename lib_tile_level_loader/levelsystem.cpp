#include "LevelSystem.h"
#include <fstream>
#include "../Nightmarevania/components/cmp_door.h"

using namespace std;
using namespace sf;

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;

size_t LevelSystem::_width;
size_t LevelSystem::_height;

Vector2f LevelSystem::_offset(0.0f, 30.0f);

vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

float LevelSystem::_tileSize(100.f);
int spriteSize = 32;

sf::Texture foreground;

/*std::map<LevelSystem::Tile, sf::IntRect> LevelSystem::_background{

	{WINDOWa, sf::IntRect((5 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{WINDOWb, sf::IntRect((5 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{WINDOWc, sf::IntRect((5 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},

	{STATUEa, sf::IntRect((7 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{STATUEb, sf::IntRect((7 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{STATUEc, sf::IntRect((7 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{STATUEd, sf::IntRect((8 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{STATUEe, sf::IntRect((8 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{STATUEf, sf::IntRect((8 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},

	{COLUMNa, sf::IntRect((13 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{COLUMNb, sf::IntRect((13 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{COLUMNc, sf::IntRect((13 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
};*/

std::map<LevelSystem::Tile, sf::IntRect> LevelSystem::_images{

	{SPIKES1, sf::IntRect((2 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{SPIKES2, sf::IntRect((3 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{SPIKES3, sf::IntRect((4 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{SPIKES4, sf::IntRect((5 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{SPIKES5, sf::IntRect((6 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},

	{WAYPOINTA, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{WAYPOINTB, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{DOOR, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{KEY, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{ENEMY, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},

	{END, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{START, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{EMPTY, sf::IntRect((0 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{WALL, sf::IntRect((2 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)},
	{FLOOR, sf::IntRect((1 * spriteSize), (0 * spriteSize), spriteSize, spriteSize)}
};

sf::IntRect LevelSystem::getSprite(LevelSystem::Tile t)
{
	auto it = _images.find(t);
	if (it == _images.end())
	{
		_images[t] = sf::IntRect(0, 0, 32, 32); //set to the black background tile
	}
	return _images[t];
}

void LevelSystem::setSprite(LevelSystem::Tile t, sf::IntRect i)
{
	_images[t] = i;
}

void LevelSystem::loadLevelFile(const std::string& path, float tileSize)
{
	_tileSize = tileSize;
	size_t w = 0, h = 0;
	string buffer;

	// Load in file to buffer
	ifstream f(path);
	if (f.good())
	{
		f.seekg(0, std::ios::end);
		buffer.resize(f.tellg());
		f.seekg(0);
		f.read(&buffer[0], buffer.size());
		f.close();
	}
	else
	{
		throw string("Couldn't open level file: ") + path;
	}

	std::vector<Tile> temp_tiles;
	int widthCheck = 0;
	for (int i = 0; i < buffer.size(); ++i)
	{
		const char c = buffer[i];
		if (c == '\0') { break; }
		// newline
		if (c == '\n')
		{
			// if we haven't written width yet set it
			if (w == 0)
			{
				w = i;
			}
			else if (w != (widthCheck - 1))
			{
				throw string("non uniform width:" + to_string(h) + " ") + path;
			}
			widthCheck = 0;
			// increment height
			h++;
		}
		else
		{
			temp_tiles.push_back((Tile)c);
		}
		++widthCheck;
	}

	if (temp_tiles.size() != (w * h))
	{
		throw string("Can't parse level file") + path;
	}
	_tiles = std::make_unique<Tile[]>(w * h);
	// set static class vars
	_width = w;
	_height = h;
	std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
	buildSprites();
}

void LevelSystem::buildSprites()
{
	if (!(foreground.loadFromFile("res/img/foreground_tiles.png"))) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	_sprites.clear();

	struct tp
	{
		sf::Vector2f p;
		sf::Vector2f s;
		sf::IntRect i;
	};

	vector<tp> tps;

	const auto tls = Vector2f(_tileSize, _tileSize);
	for (size_t y = 0; y < _height; ++y)
	{
		for (size_t x = 0; x < _width; ++x)
		{
			Tile t = getTile({ x, y });
			if (t == EMPTY)
			{
				continue;
			}
			tps.push_back({ getTilePosition({x, y}), tls, getSprite(t) });
		}
	}

	const auto nonempty = tps.size();

	for (auto& t : tps)
	{
		auto s = make_unique<sf::RectangleShape>();
		s->setPosition(t.p);
		s->setSize(t.s);
		s->setTexture(&foreground);
		s->setTextureRect(t.i);
		_sprites.push_back(move(s));
	}
}

void LevelSystem::render(RenderWindow& window)
{
	for (auto& t : _sprites)
	{
		window.draw(*t);
	}
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p)
{
	if (p.x > _width || p.y > _height)
	{
		throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y) + ")";
	}
	return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p)
{
	return (Vector2f((float)p.x, (float)p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type)
{
	auto v = vector<sf::Vector2ul>();
	for (size_t i = 0; i < _width * _height; ++i)
	{
		if (_tiles[i] == type)
		{
			v.push_back({ i % _width, i / _width });
		}
	}

	return v;
}

bool LevelSystem::doesTileExist(LevelSystem::Tile type)
{
	for (size_t i = 0; i < _width * _height; ++i)
	{
		if (_tiles[i] == type)
		{
			return true;
		}
	}

	return false;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v)
{
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0)
	{
		throw string("Tile out of range ");
	}
	return getTile(Vector2ul((v - _offset) / (_tileSize)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v)
{
	auto a = v - _offset;
	if (a.x < 0 || a.y < 0)
	{
		return false;
	}
	auto p = Vector2ul((v - _offset) / (_tileSize));
	if (p.x > _width || p.y > _height)
	{
		return false;
	}
	return true;
}

void LevelSystem::setOffset(const Vector2f& _offset)
{
	LevelSystem::_offset = _offset;
	buildSprites();
}

void LevelSystem::unload()
{
	_sprites.clear();
	_tiles.reset();
	_width = 0;
	_height = 0;
	_offset = { 0, 0 };
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }
