#pragma once

#include <string>

class PawnPromotionMenu
{
private:
	PawnPromotionMenu();
	~PawnPromotionMenu();
	PawnPromotionMenu(const PawnPromotionMenu& other) = delete;
	PawnPromotionMenu& operator= (const PawnPromotionMenu& other) = delete;
	PawnPromotionMenu(const PawnPromotionMenu&& other) = delete;
	PawnPromotionMenu& operator= (const PawnPromotionMenu&& other) = delete;

	std::string partialConstructedMove;

public:

	static PawnPromotionMenu& get();

	void initialize(const std::string& move);
	void draw();
	void update();
};