#include "system_sound.h"
#include <unordered_map>

// Maps to store all the music and effects
static std::unordered_map<std::string, sf::Music*> music;
static std::unordered_map<std::string, sf::Sound*> effects;
// Booleans that check wheter the music and effect are on or off
static bool audioMusicOn;
static bool audioEffectsOn;
// Music
static sf::Music main_menu_music;
static sf::Music level_1_music;
static sf::Music level_2_music;
static sf::Music level_3_music;
static sf::Music mystic_music;
// Effects buffers
static sf::SoundBuffer button_buffer;
static sf::SoundBuffer player_damage_buffer;
static sf::SoundBuffer player_death_buffer;
static sf::SoundBuffer player_sword1_buffer;
static sf::SoundBuffer player_sword2_buffer;
static sf::SoundBuffer player_sword3_buffer;
static sf::SoundBuffer player_sword4_buffer;
static sf::SoundBuffer boss_death_buffer;
static sf::SoundBuffer boss_hurt_buffer;
static sf::SoundBuffer boss_attack_buffer;
static sf::SoundBuffer boss_roar_buffer;
static sf::SoundBuffer pick_up_buffer;
// Effects
static sf::Sound button_effect;
static sf::Sound player_damage_effect;
static sf::Sound player_death_effect;
static sf::Sound player_sword1_effect;
static sf::Sound player_sword2_effect;
static sf::Sound player_sword3_effect;
static sf::Sound player_sword4_effect;
static sf::Sound boss_death_effect;
static sf::Sound boss_hurt_effect;
static sf::Sound boss_attack_effect;
static sf::Sound boss_roar_effect;
static sf::Sound pick_up_effect;


void Audio::initialise(std::string& musicSetting, std::string& effectsSetting)
{
	// Set music or on off based on the setting stored
	if (!musicSetting.empty() && musicSetting == "Off")
	{
		audioMusicOn = false;
	}
	else
	{
		audioMusicOn = true;
	}
	// Set effects or on off based on the setting stored
	if (!effectsSetting.empty() && effectsSetting == "Off")
	{
		audioEffectsOn = false;
	}
	else
	{
		audioEffectsOn = true;
	}
	// Load all the music for the game
	main_menu_music.openFromFile("res/sounds/main_menu_music.wav");
	level_1_music.openFromFile("res/sounds/level1_music.wav");
	level_2_music.openFromFile("res/sounds/level2_music.wav");
	level_3_music.openFromFile("res/sounds/boss_music.wav");
	mystic_music.openFromFile("res/sounds/mystic_music.wav");
	// Set music as loops
	main_menu_music.setLoop(true);
	level_1_music.setLoop(true);
	level_2_music.setLoop(true);
	level_3_music.setLoop(true);
	mystic_music.setLoop(true);
	// Store the music in the map
	music["main_menu_music"] = &main_menu_music;
	level_3_music.setVolume(50.0f);
	music["level_1_music"] = &level_1_music;
	music["level_2_music"] = &level_2_music;
	music["level_3_music"] = &level_3_music;
	music["mystic_music"] = &mystic_music;
	// Load all the effects for the game
	button_buffer.loadFromFile("res/sounds/button_effect.wav");
	player_damage_buffer.loadFromFile("res/sounds/player_damage.wav");
	player_death_buffer.loadFromFile("res/sounds/player_death.wav");
	player_sword1_buffer.loadFromFile("res/sounds/player_sword.wav");
	player_sword2_buffer.loadFromFile("res/sounds/player_sword2.wav");
	player_sword3_buffer.loadFromFile("res/sounds/player_sword3.wav");
	player_sword4_buffer.loadFromFile("res/sounds/player_sword4.wav");
	boss_death_buffer.loadFromFile("res/sounds/boss_death.wav");
	boss_hurt_buffer.loadFromFile("res/sounds/boss_hurt.wav");
	boss_attack_buffer.loadFromFile("res/sounds/boss_attack.wav");
	boss_roar_buffer.loadFromFile("res/sounds/boss_roar.wav");
	pick_up_buffer.loadFromFile("res/sounds/pick_up.wav");
	// Attach buffer to effects
	button_effect.setBuffer(button_buffer);
	player_damage_effect.setBuffer(player_damage_buffer);
	player_death_effect.setBuffer(player_death_buffer);
	player_sword1_effect.setBuffer(player_sword1_buffer);
	player_sword2_effect.setBuffer(player_sword2_buffer);
	player_sword3_effect.setBuffer(player_sword3_buffer);
	player_sword4_effect.setBuffer(player_sword4_buffer);
	boss_death_effect.setBuffer(boss_death_buffer);
	boss_hurt_effect.setBuffer(boss_hurt_buffer);
	boss_attack_effect.setBuffer(boss_attack_buffer);
	boss_roar_effect.setBuffer(boss_roar_buffer);
	pick_up_effect.setBuffer(pick_up_buffer);
	// Store the effects in the map
	effects["button_effect"] = &button_effect;
	effects["player_damage_effect"] = &player_damage_effect;
	effects["player_death_effect"] = &player_death_effect;
	effects["player_sword_effect"] = &player_sword1_effect;
	effects["player_sword2_effect"] = &player_sword2_effect;
	effects["player_sword3_effect"] = &player_sword3_effect;
	effects["player_sword4_effect"] = &player_sword4_effect;
	effects["boss_death_effect"] = &boss_death_effect;
	effects["boss_hurt_effect"] = &boss_hurt_effect;
	effects["boss_attack_effect"] = &boss_attack_effect;
	effects["boss_roar_effect"] = &boss_roar_effect;
	effects["pick_up_effect"] = &pick_up_effect;
}

void Audio::playMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (audioMusicOn && found != music.end() && found->second->getStatus() != sf::Music::Playing)
	{
		found->second->play();
	}
}
void Audio::stopMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (found != music.end())
	{
		found->second->stop();
	}
}
void Audio::pauseMusic(const std::string& musicName)
{
	auto found = music.find(musicName);
	if (audioMusicOn && found != music.end())
	{
		found->second->pause();
	}
}

void Audio::turnMusicOn() { audioMusicOn = true; }
void Audio::turnMusicOff() { audioMusicOn = false; }
bool Audio::isMusicOn() { return audioMusicOn; }

void Audio::playEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (audioEffectsOn && found != effects.end() && found->second->getStatus() != sf::Sound::Playing)
	{
		found->second->play();
	}
}
void Audio::stopEffect(const std::string& effectName)
{
	auto found = effects.find(effectName);
	if (audioEffectsOn && found != effects.end())
	{
		found->second->stop();
	}
}

void Audio::turnEffectsOn() { audioEffectsOn = true; }
void Audio::turnEffectsOff() { audioEffectsOn = false; }
bool Audio::areEffectsOn() { return audioEffectsOn; }

void Audio::clearAllSounds()
{
	music.clear();
	effects.clear();
}