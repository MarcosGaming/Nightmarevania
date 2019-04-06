#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "system_sound.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
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

static bool attackButtonReleased = false;
static bool defendButtonReleased = false;
// LOADING SCREEN

void Loading_update(float dt, const Scene* const scn) 
{
	//  cout << "Eng: Loading Screen\n";
	if (scn->isLoaded()) 
	{
		cout << "Eng: Exiting Loading Screen\n";
		loading = false;
	} else 
	{
		loadingspinner += 220.0f * dt;
		loadingTime += dt;
	}
}
void Loading_render() 
{
	// cout << "Eng: Loading Screen Render\n";
	static CircleShape octagon(80, 8);
	octagon.setOrigin(80, 80);
	octagon.setRotation(loadingspinner);
	octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
	octagon.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
	static Text t("Loading", *Resources::get<sf::Font>("Roboto-Regular.ttf"));
	t.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
	t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f,0.3f));
	Renderer::queue(&t);
	Renderer::queue(&octagon);
}



// SCENE

Scene::~Scene() { UnLoad(); }
// QUESTION: WHAT DOES THIS DO?
void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

void Scene::UnLoad()
{
	ents.list.clear();
	setLoaded(false);
}

void Scene::Update(const double& dt) { 
	ents.update(dt); 
}

void Scene::Render() { 
	ents.render(); 
}

// QUESTION: WHAT DOES THIS DO?
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
// QUESTION: WHAT DOES THIS DO?
void Scene::setLoaded(bool b) 
{
	std::lock_guard<std::mutex> lck(_loaded_mtx);
   _loaded = b;
}



// ENGINE

void Engine::Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn)
{
	RenderWindow window(VideoMode(width, height), gameName);
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);
	Physics::initialise();
	SoundSystem::initialise();
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
			if ((event.type == Event::KeyReleased) && (event.key.code == Keyboard::Space))
			{
				Physics::setCanDoubleJump(true);
			}
			if ((event.type == Event::MouseButtonReleased) && (event.key.code == Mouse::Left))
			{
				attackButtonReleased = true;
			}
			if ((event.type == Event::MouseButtonReleased) && (event.key.code == Mouse::Right))
			{
				defendButtonReleased = true;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
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
		Physics::update(dt);
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
	cout << "Eng: changing scene: " << s << endl;
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr)
	{
		old->UnLoad(); // todo: Unload Async
	}

	if (!s->isLoaded())
	{
		cout << "Eng: Entering Loading Screen\n";
		loadingTime = 0;
		_activeScene->LoadAsync();
		loading = true;
	}
}

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

bool Engine::isAttackButtonReleased() { return attackButtonReleased; }
void Engine::setAttackButtonReleased(bool status) { attackButtonReleased = status; }

bool Engine::isDefendButtonReleased() { return defendButtonReleased; }
void Engine::setDefendButtonReleased(bool status) { defendButtonReleased = status; }

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