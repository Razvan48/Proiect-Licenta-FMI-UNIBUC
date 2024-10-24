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

	void loadResources();
	void run();
	void update();

public:
	static Game& get();
	void start();
};