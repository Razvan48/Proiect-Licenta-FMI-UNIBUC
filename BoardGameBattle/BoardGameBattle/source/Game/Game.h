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
		// etc..
	};
private:
	Game(Game::Status status, bool soundEnabled);
	~Game();
	Game(const Game& other) = delete;
	Game& operator= (const Game& other) = delete;
	Game(const Game&& other) = delete;
	Game& operator= (const Game&& other) = delete;

	Game::Status status;

	bool soundEnabled;

	std::map<Game::Status, VisualInterface> visualInterfaces;

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