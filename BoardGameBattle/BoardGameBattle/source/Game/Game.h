#pragma once

#include <map>

#include "../VisualInterface/VisualInterface.h"

class Game
{
public:
	enum class Status
	{
		IN_MAIN_MENU,
		IN_GAME,
		// etc..
	};
private:
	Game(Game::Status status);
	~Game();
	Game(const Game& other) = delete;
	Game& operator= (const Game& other) = delete;
	Game(const Game&& other) = delete;
	Game& operator= (const Game&& other) = delete;

	Game::Status status;

	std::map<Game::Status, VisualInterface> visualInterfaces;

	void loadResources();
	void run();
	void draw();
	void update();

public:
	static Game& get();
	void start();

	inline void setStatus(const Game::Status& status) { this->status = status; }
};