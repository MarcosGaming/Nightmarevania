#include "system_renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> drawables;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) { rw = &r; }

void Renderer::shutdown() 
{
	while (!drawables.empty())
	{
		drawables.pop();
	}
}

void Renderer::update(const double&) {}

void Renderer::render() 
{
	if (rw == nullptr) 
	{
		throw("No render window set! ");
	}
	while (!drawables.empty()) 
	{
		rw->draw(*drawables.front());
		drawables.pop();
	}
}

void Renderer::queue(const sf::Drawable* s) { drawables.push(s); }