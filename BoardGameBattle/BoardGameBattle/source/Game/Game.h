#pragma once

#include <map>

#include "../VisualInterface/VisualInterface.h"

class Game
{
public:
	enum class Status
	{
		EXITING,
		IN_MAIN_MENU,
		IN_SETTINGS_MENU,
		IN_MODE_MENU,
		IN_COLOR_MENU,
		IN_MULTIPLAYER_MENU,
		IN_INPUT_MENU,
		IN_GAME,
		// etc..
	};
	enum class Mode
	{
		SINGLE_PLAYER,
		MULTIPLAYER,
		NONE,
	};
	enum class Color
	{
		WHITE,
		BLACK,
		NONE,
	};
	enum class MultiplayerStatus
	{
		CREATE_GAME,
		JOIN_GAME,
		NONE,
	};
private:
	Game();
	~Game();
	Game(const Game& other) = delete;
	Game& operator= (const Game& other) = delete;
	Game(const Game&& other) = delete;
	Game& operator= (const Game&& other) = delete;

	Game::Status status;

	bool soundEnabled;

	std::map<Game::Status, VisualInterface> visualInterfaces;

	Game::Mode mode;
	Game::Color color;
	Game::MultiplayerStatus multiplayerStatus;

	void loadResources();
	void run();
	void draw();
	void update();

public:
	static Game& get();
	void start();

	inline void setStatus(const Game::Status& status) { this->status = status; }
	inline void setSoundEnabled(bool soundEnabled) { this->soundEnabled = soundEnabled; }
	inline bool getSoundEnabled() const { return this->soundEnabled; }
};