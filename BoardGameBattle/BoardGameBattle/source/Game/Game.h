#pragma once

class Game
{
private:
	Game();
	~Game();
	Game(const Game& other) = delete;
	Game& operator= (const Game& other) = delete;
	Game(const Game&& other) = delete;
	Game& operator= (const Game&& other) = delete;

	void run();
	void loadResources();

public:
	static Game& get();
	void start();
	void update();
};