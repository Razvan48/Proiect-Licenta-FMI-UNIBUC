#pragma once

#include <string>

#include "../../Entity/TexturableEntity/TexturableEntity.h"
#include "../../Entity/Button/CancelPawnPromotionButton/CancelPawnPromotionButton.h"
#include "../../Entity/Button/PawnPromotionButton/PawnPromotionButton.h"

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

	TexturableEntity backgroundEntity;
	CancelPawnPromotionButton cancelPawnPromotionButton;
	PawnPromotionButton rookPromotionButton;
	PawnPromotionButton knightPromotionButton;
	PawnPromotionButton bishopPromotionButton;
	PawnPromotionButton queenPromotionButton;

public:

	static PawnPromotionMenu& get();

	void initialize(const std::string& move);
	void draw();
	void update();

	bool isPlayerColorWhite() const;
};