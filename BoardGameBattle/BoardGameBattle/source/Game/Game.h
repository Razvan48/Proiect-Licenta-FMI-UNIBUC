#pragma once

#include <string>
#include <map>
#include <memory>

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
		IN_SINGLEPLAYER_COLOR_MENU,
		IN_MULTIPLAYER_COLOR_MENU,
		IN_MULTIPLAYER_MENU,
		IN_INPUT_CREATE_MENU,
		IN_INPUT_JOIN_MENU,
		IN_SINGLEPLAYER_GAME,
		IN_CREATED_MULTIPLAYER_GAME,
		IN_JOINED_MULTIPLAYER_GAME,

		NONE, // Doar la Undo Move Button e folosit.
		// etc..
	};
	enum class Mode
	{
		SINGLEPLAYER,
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

	std::map<Game::Status, std::shared_ptr<VisualInterface>> visualInterfaces;

	Game::Mode mode;
	Game::Color color;
	Game::MultiplayerStatus multiplayerStatus;

	const int EXIT_TIME_MS;

	void loadResources();
	void run();
	void draw();
	void update();

	void initializeENet();

	void printGameStatuses();

public:
	static Game& get();
	void start();

	inline Game::Status getStatus() const { return this->status; }
	inline Game::Mode getMode() const { return this->mode; }
	inline Game::Color getColor() const { return this->color; }
	inline Game::MultiplayerStatus getMultiplayerStatus() const { return this->multiplayerStatus; }

	void setStatus(const Game::Status& status);
	inline void setMode(const Game::Mode& mode) { this->mode = mode; }
	inline void setColor(const Game::Color& color) { this->color = color; }
	inline void setMultiplayerStatus(const Game::MultiplayerStatus& multiplayerStatus) { this->multiplayerStatus = multiplayerStatus; }
};