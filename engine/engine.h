#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

using namespace std;
using namespace sf;

class Scene 
{
public:
	Scene() = default;
	virtual ~Scene();
	virtual void Load() = 0;
	virtual void LoadAsync();
	virtual void UnLoad();
	virtual void Update(const double& dt);
	virtual void Render();
	bool isLoaded() const;
	std::shared_ptr<Entity> makeEntity();

	EntityManager ents;

	//static std::shared_ptr<Entity> player;
	std::shared_ptr<Entity> player;
	std::shared_ptr<sf::Texture> playerAnimations;
	std::shared_ptr<Texture> spriteSheet;
	std::shared_ptr<Texture> combatIcons;

protected:
	void setLoaded(bool);

private:
	mutable bool _loaded;
	mutable std::future<void> _loaded_future;
	mutable std::mutex _loaded_mtx;


	
};

class Engine 
{
public:
	Engine() = delete;
	static void Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn);
	static void ChangeScene(Scene*);
	static sf::RenderWindow& GetWindow();
	static sf::Vector2u getWindowSize();
	static void setVsync(bool b);

	static bool isAttackButtonReleased();
	static void setAttackButtonReleased(bool);

	static bool isDefendButtonReleased();
	static void setDefendButtonReleased(bool);

private:
	static Scene* _activeScene;
	static std::string _gameName;
	static void Update();
	static void Render(sf::RenderWindow& window);

};

namespace timing 
{
	// Return time since Epoc
	long long now();
	// Return time since last() was last called.
	long long last();
}