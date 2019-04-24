#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "system_sound.h"
#include "system_resolution.h"
#include "system_controller.h"
#include "system_saving.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <stdexcept>
#include <memory>

using namespace sf;
using namespace std;

Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

float frametimes[256] = {};
uint8_t ftc = 0;

// LOADING SCREEN

void Loading_update(float dt, const Scene* const scn) 
{
	if (scn->isLoaded()) 
	{
		loading = false;
	} 
	else 
	{
		loadingTime += dt;
	}
}
void Loading_render() 
{
	static Text t("Loading...", *Resources::get<sf::Font>("Gothic.ttf"));
	t.setFillColor(Color(255,255,255,(Uint8)min(255.f,40.f*loadingTime)));
	t.setPosition(Vector2f((GAMEX*0.40f), GAMEY*0.5f));
	t.setCharacterSize(60.0f);
	t.setOutlineColor(Color::White);

	Renderer::queue(&t);
}



// SCENE

Scene::~Scene() { UnLoad(); }

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

void Scene::UnLoad()
{
	pausedEnts.list.clear();
	ents.list.clear();
	setLoaded(false);
}

void Scene::Update(const double& dt) 
{ 
	if (_paused)
	{
		pausedEnts.update(dt);
	}
	else
	{
		ents.update(dt);
	} 
}

void Scene::Render() 
{ 
	ents.render(); 
	if (_paused)
	{
		pausedEnts.render();
	}
}


bool Scene::isLoaded() const
{
	std::lock_guard<std::mutex> lck(_loaded_mtx);
	// Are we already loading asynchronously? Yes and has finished
	if (_loaded_future.valid() && _loaded_future.wait_for(chrono::seconds(0)) == future_status::ready)
	{
		_loaded_future.get();
		_loaded = true;
	}
	return _loaded;
}

std::shared_ptr<Entity> Scene::makeEntity() 
{
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

std::shared_ptr<Entity> Scene::makePausedEntity()
{
	auto e = make_shared<Entity>(this);
	pausedEnts.list.push_back(e);
	return std::move(e);
}

void Scene::setPause(bool pause) { _paused = pause; }

bool Scene::isPaused() const { return _paused; }

void Scene::setLoaded(bool b) 
{
	std::lock_guard<std::mutex> lck(_loaded_mtx);
   _loaded = b;
}



// ENGINE

void Engine::Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn)
{
	RenderWindow window(VideoMode(width, height), gameName, sf::Style::Close);
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);
	Physics::initialise();
	Saving::initialise();
	Audio::initialise(*Saving::getMusicSettingSaved(), *Saving::getEffectSettingSaved());
	Resolution::initialise(*Saving::getResolutionSettingSaved(), *Saving::getFullscreenSettingSaved(), *Saving::getBorderlessSettingSaved());
	Controller::initialise(*Saving::getKeysButtonsSaved(), *Saving::getControllerButtonsSaved());
	ChangeScene(scn);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			// The controller will process the events relevant to it
			if ((event.type == Event::KeyReleased) || (event.type == Event::MouseButtonReleased) || (event.type == Event::JoystickButtonReleased))
			{
				Controller::processEvent(event);
			}
		}
		//window.clear();
		window.clear(sf::Color(7,7,7)); //slightly adjust the background to match the background of the tiles
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr)
	{
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	Saving::saveSettingsInFile();
	window.close();
	Physics::shutdown();
	// Render::shutdown();
}

void Engine::Update()
{
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		if (ftc % 60 == 0)
		{
			double davg = 0;
			for (const auto t : frametimes)
			{
				davg += t;
			}
			davg = 1.0 / (davg / 255.0);
			_window->setTitle(avg + toStrDecPt(2, davg));
		}
	}

	if (loading)
	{
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr)
	{
		if (!_activeScene->isPaused())
		{
			Physics::update(dt);
		}
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window)
{
	if (loading)
	{
		Loading_render();
	}
	else if (_activeScene != nullptr)
	{
		_activeScene->Render();
	}

	Renderer::render();
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s)
{
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr)
	{
		old->UnLoad();
	}

	if (!s->isLoaded())
	{
		loadingTime = 0;
		_activeScene->LoadAsync();
		loading = true;
	}
}

Scene* Engine::getActiveScene() {
	return Engine::_activeScene;
}

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

// TIMING

namespace timing 
{
	// Return time since Epoc
	long long now() 
	{
		return std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}
	// Return time since last() was last called.
	long long last() 
	{
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
}