#include "scene_ending.h"
#include "../game.h"
#include "../components/cmp_text.h"
#include <system_renderer.h>
#include <system_sound.h>
#include <system_resolution.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> ending_dialogue;
static shared_ptr<Entity> the_end_dialogue;

void EndingScene::Load()
{
	// Ending dialogue
	{
		ending_dialogue = makeEntity();
		// Dialogue text component
		auto text = ending_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("Mysterious Voice: Thank you for your efforts defeating Erebus, however Serah,\nI am afraid that your journey... has just begun.");
		text->setFunction([&]() {text->endingDialogue(); });
	}
	// The end text
	{
		the_end_dialogue = makeEntity();
		the_end_dialogue->addTag("TheEnd");
		the_end_dialogue->setAlive(false);
		// Dialogue text component
		auto text = the_end_dialogue->addComponent<DialogueBoxComponent>();
		text->setCompleteText("The End ?");
		text->setFunction([&]() {text->theEndDialogue(); });
	}
	setLoaded(true);
}

void EndingScene::Update(const double& dt)
{
	Scene::Update(dt);
	Audio::playMusic("main_menu_music");
}

void EndingScene::Render()
{
	Scene::Render();
}

void EndingScene::UnLoad()
{
	Scene::UnLoad();
}