#include "system_resolution.h"
#include "engine.h"

// Booleans to keep track of which screen settigns are set
static bool fullScreen;
static bool borderless;
static bool resolution1920x1080;
static bool resolution1600x900;
static bool resolution1280x720;
static bool resolution1024x576;
// Functions to change screen resolution
static void SetWindowView(int width, int height);
static void ChangeResolution(int, int, bool);
// Icon for the window
static sf::Image Icon;

void Resolution::initialise(std::string& resolutionSetting, std::string& fullScreenSetting, std::string& borderlessSetting)
{
	// Load Icon
	Icon.loadFromFile("res/img/Icon.png");
	// Turn fullscreen on or not based on setting saved
	if (!fullScreenSetting.empty() && fullScreenSetting == "On")
	{
		turnFullScreenOn();
		return;
	}
	// Set borderless based on setting saved
	if (!borderlessSetting.empty() && borderlessSetting == "On")
	{
		borderless = true;
	}
	else
	{
		borderless = false;
	}
	// Set resolution based on setting saved
	if (!resolutionSetting.empty() && resolutionSetting == "1920x1080")
	{
		changeTo1920x1080();
	}
	else if (!resolutionSetting.empty() && resolutionSetting == "1600x900")
	{
		changeTo1600x900();
	}
	else if (!resolutionSetting.empty() && resolutionSetting == "1024x576")
	{
		changeTo1024x576();
	}
	else
	{
		changeTo1280x720();
	}
}

bool Resolution::isFullScreenOn() { return fullScreen; }
bool Resolution::isBorderlessOn() { return borderless; }
bool Resolution::isResolution1920x1080On() { return resolution1920x1080; }
bool Resolution::isResolution1600x900On() { return resolution1600x900; }
bool Resolution::isResolution1280x720On() { return resolution1280x720; }
bool Resolution::isResolution1024x576On() { return resolution1024x576; }

void Resolution::changeTo1920x1080()
{
	resolution1920x1080 = true;
	resolution1600x900 = false;
	resolution1280x720 = false;
	resolution1024x576 = false;
	fullScreen = false;
	ChangeResolution(1920, 1080, borderless);
}
void Resolution::changeTo1600x900()
{
	resolution1920x1080 = false;
	resolution1600x900 = true;
	resolution1280x720 = false;
	resolution1024x576 = false;
	fullScreen = false;
	ChangeResolution(1600, 900, borderless);
}
void Resolution::changeTo1280x720()
{
	resolution1920x1080 = false;
	resolution1600x900 = false;
	resolution1280x720 = true;
	resolution1024x576 = false;
	fullScreen = false;
	ChangeResolution(1280, 720, borderless);
}
void Resolution::changeTo1024x576()
{
	resolution1920x1080 = false;
	resolution1600x900 = false;
	resolution1280x720 = false;
	resolution1024x576 = true;
	fullScreen = false;
	ChangeResolution(1024, 576, borderless);
}

void Resolution::turnFullScreenOn()
{
	resolution1920x1080 = false;
	resolution1600x900 = false;
	resolution1280x720 = false;
	resolution1024x576 = false;
	borderless = true;
	fullScreen = true;
	int x = sf::VideoMode::getDesktopMode().width;
	int y = sf::VideoMode::getDesktopMode().height;
	Engine::GetWindow().create(sf::VideoMode(x, y), "Nightmarevania", sf::Style::Fullscreen);
	SetWindowView(x, y);
}
void Resolution::turnFullScreenOff()
{
	borderless = false;
	fullScreen = false;
	int x = sf::VideoMode::getDesktopMode().width;
	int y = sf::VideoMode::getDesktopMode().height;
	Engine::GetWindow().create(sf::VideoMode(x, y), "Nightmarevania", sf::Style::Close);
	SetWindowView(x, y);
}

void Resolution::turnBorderlessOn()
{
	borderless = true;
	int previousX = Engine::GetWindow().getSize().x;
	int previousY = Engine::GetWindow().getSize().y;
	Engine::GetWindow().create(sf::VideoMode(previousX, previousY), "Nightmarevania", sf::Style::None);
	SetWindowView(previousX, previousY);
}
void Resolution::turnBorderlessOff()
{
	borderless = false;
	fullScreen = false;
	int previousX = Engine::GetWindow().getSize().x;
	int previousY = Engine::GetWindow().getSize().y;
	Engine::GetWindow().create(sf::VideoMode(previousX, previousY), "Nightmarevania", sf::Style::Close);
	SetWindowView(previousX, previousY);
}

// Create FloatRect to fits Game into Screen while preserving aspect
static sf::FloatRect CalculateViewport(const sf::Vector2u& screensize, const sf::Vector2u& gamesize)
{
	const sf::Vector2f screensf((float)screensize.x, (float)screensize.y);
	const sf::Vector2f gamesf((float)gamesize.x, (float)gamesize.y);
	const float gameAspect = gamesf.x / gamesf.y;
	const float screenAspect = screensf.x / screensf.y;
	// Final size.x of game viewport in pixels
	float scaledWidth;
	// Final size.y of game viewport in pixels
	float scaledHeight;
	// False = scale to screen.x, True = screen.y
	bool scaleSide = false;

	//Work out which way to scale
	if (gamesize.x > gamesize.y)
	{
		// Game is wider than tall, can we use full width?
		if (screensf.y < (screensf.x / gameAspect)) {
			// No, not high enough to fit game height
			scaleSide = true;
		}
		else {
			// Yes, use all width available
			scaleSide = false;
		}
	}
	else
	{
		// Game is Square or Taller than Wide, can we use full height?
		if (screensf.x < (screensf.y * gameAspect))
		{
			// No, screensize not wide enough to fit game width
			scaleSide = false;
		}
		else
		{
			// Yes, use all height available
			scaleSide = true;
		}
	}

	if (scaleSide)
	{
		// Use max screen height
		scaledHeight = screensf.y;
		scaledWidth = floor(scaledHeight * gameAspect);
	}
	else
	{
		// Use max screen width
		scaledWidth = screensf.x;
		scaledHeight = floor(scaledWidth / gameAspect);
	}

	// Calculate as percent of screen
	const float widthPercent = (scaledWidth / screensf.x);
	const float heightPercent = (scaledHeight / screensf.y);

	return sf::FloatRect(0, 0, widthPercent, heightPercent);
}
// Sets window view
static void SetWindowView(int width, int height)
{
	const sf::Vector2u screensize(width, height);
	const sf::Vector2u gamesize(GAMEX, GAMEY);
	// Set View as normal
	Engine::GetWindow().setSize(screensize);
	sf::FloatRect visibleArea(0.f, 0.f, (float)gamesize.x, (float)gamesize.y);
	auto v = sf::View(visibleArea);
	// Figure out how to scale and maintain aspect;
	auto viewport = CalculateViewport(screensize, gamesize);
	// Optionally Center it
	bool centered = true;
	if (centered)
	{
		viewport.left = (1.0f - viewport.width) * 0.5f;
		viewport.top = (1.0f - viewport.height) * 0.5f;
	}
	v.setViewport(viewport);
	Engine::GetWindow().setView(v);
	Engine::GetWindow().setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
}
// Creates new window with the new settings
static void ChangeResolution(int width, int height, bool borderless)
{
	if (borderless)
	{
		Engine::GetWindow().create(sf::VideoMode(width, height), "Nightmarevania", sf::Style::None);
		SetWindowView(width, height);
	}
	else
	{
		Engine::GetWindow().create(sf::VideoMode(width, height), "Nightmarevania", sf::Style::Close);
		SetWindowView(width, height);
	}
}