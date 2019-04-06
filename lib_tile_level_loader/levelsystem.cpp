#include "LevelSystem.h"
#include <fstream>

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

std::map<LevelSystem::Tile, sf::IntRect> LevelSystem::_images{ 

	{BG1, sf::IntRect((4 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{BG2, sf::IntRect((4 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{BG3, sf::IntRect((4 * spriteSize), (3 * spriteSize), spriteSize, spriteSize)},
	{BG4, sf::IntRect((5 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{BG5, sf::IntRect((5 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{BG6, sf::IntRect((5 * spriteSize), (3 * spriteSize), spriteSize, spriteSize)},
	{BG7, sf::IntRect((6 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{BG8, sf::IntRect((6 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{BG9, sf::IntRect((6 * spriteSize), (3 * spriteSize), spriteSize, spriteSize)},
	{BG10, sf::IntRect((7 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{BG11, sf::IntRect((7 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{BG12, sf::IntRect((7 * spriteSize), (3 * spriteSize), spriteSize, spriteSize)},
	{BG13, sf::IntRect((8 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{BG14, sf::IntRect((8 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{BG15, sf::IntRect((8 * spriteSize), (3 * spriteSize), spriteSize, spriteSize)},
	{BG16, sf::IntRect((9 * spriteSize), (1 * spriteSize), spriteSize, spriteSize)},
	{BG17, sf::IntRect((9 * spriteSize), (2 * spriteSize), spriteSize, spriteSize)},
	{BG18, sf::IntRect((9 * spriteSize), (3 * spriteSize), spriteSize, spriteSize)},

	{UDOOR1, sf::IntRect((3 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},
	{UDOOR2, sf::IntRect((3 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{UDOOR3, sf::IntRect((3 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{UDOOR4, sf::IntRect((4 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{UDOOR5, sf::IntRect((4 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{UDOOR6, sf::IntRect((4 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},
	{UDOOR7, sf::IntRect((5 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},
	{UDOOR8, sf::IntRect((5 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{UDOOR9, sf::IntRect((5 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},

	{LDOOR1, sf::IntRect((0 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},
	{LDOOR2, sf::IntRect((0 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{LDOOR3, sf::IntRect((0 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{LDOOR4, sf::IntRect((1 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{LDOOR5, sf::IntRect((1 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{LDOOR6, sf::IntRect((1 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},
	{LDOOR7, sf::IntRect((2 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},
	{LDOOR8, sf::IntRect((2 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{LDOOR9, sf::IntRect((2 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},

	{TORCHa, sf::IntRect((8 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{TORCHb, sf::IntRect((8 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{TORCHc, sf::IntRect((8 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},

	{COLUMN1a, sf::IntRect((6 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{COLUMN1b, sf::IntRect((6 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{COLUMN1c, sf::IntRect((6 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},

	{COLUMN2a, sf::IntRect((7 * spriteSize), (7 * spriteSize), spriteSize, spriteSize)},
	{COLUMN2b, sf::IntRect((7 * spriteSize), (8 * spriteSize), spriteSize, spriteSize)},
	{COLUMN2c, sf::IntRect((7 * spriteSize), (9 * spriteSize), spriteSize, spriteSize)},

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
	cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
	buildSprites();
}

void LevelSystem::buildSprites(bool optimise)
{
	if (!(foreground.loadFromFile("res/img/foreground_tiles.png"))) {
		cerr << "Failed to load spritesheet!" << std::endl;
	}

	_sprites.clear();

	struct tp
	{
		sf::Vector2f p;
		sf::Vector2f s;
		//sf::Color c;
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

	// If tile of the same type are next to each other, we can use one large sprite instead of two.
	  /*if (optimise && nonempty)
	  {
		  vector<tp> tpo;
		  tp last = tps[0];
		  size_t samecount = 0;

		  for (size_t i = 1; i < nonempty; ++i)
		  {
			  // Is this tile compressible with the last?
			  bool same = ((tps[i].p.y == last.p.y) && (tps[i].p.x == last.p.x + (tls.x * (1 + samecount))) && (tps[i].c == last.c));
			  if (same)
			  {
				  ++samecount; // Yes, keep going
			  //	tps[i].c = Color::Green;
			  }
			  else
			  {
				  if (samecount)
				  {
					  last.s.x = (1 + samecount) * tls.x; // Expand tile
				  }
				  // write tile to list
				  tpo.push_back(last);
				  samecount = 0;
				  last = tps[i];
			  }
		  }
		  // catch the last tile
		  if (samecount)
		  {
			  last.s.x = (1 + samecount) * tls.x;
			  tpo.push_back(last);
		  }

		  // No scan down Y, using different algo now that compressible blocks may
		  // not be contiguous
		  const auto xsave = tpo.size();
		  samecount = 0;
		  vector<tp> tpox;
		  for (size_t i = 0; i < tpo.size(); ++i)
		  {
			  last = tpo[i];
			  for (size_t j = i + 1; j < tpo.size(); ++j)
			  {
				  bool same = ((tpo[j].p.x == last.p.x) && (tpo[j].s == last.s) && (tpo[j].p.y == last.p.y + (tls.y * (1 + samecount))) && (tpo[j].c == last.c));
				  if (same)
				  {
					  ++samecount;
					  tpo.erase(tpo.begin() + j);
					  --j;
				  }
			  }
			  if (samecount)
			  {
				  last.s.y = (1 + samecount) * tls.y; // Expand tile
			  }
			  // write tile to list
			  tpox.push_back(last);
			  samecount = 0;
		  }

		  tps.swap(tpox);
	  }*/

	for (auto& t : tps)
	{
		auto s = make_unique<sf::RectangleShape>();
		s->setPosition(t.p);
		s->setSize(t.s);
		s->setTexture(&foreground);
		s->setTextureRect(t.i);
		_sprites.push_back(move(s));
	}

	cout << "Level with " << (_width * _height) << " Tiles, With " << nonempty << " Not Empty, using: " << _sprites.size() << " Sprites\n";
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
	return (Vector2f(p.x, p.y) * _tileSize) + _offset;
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
	cout << "LevelSystem unloading\n";
	_sprites.clear();
	_tiles.reset();
	_width = 0;
	_height = 0;
	_offset = { 0, 0 };
	//TODO unload tex?
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }
