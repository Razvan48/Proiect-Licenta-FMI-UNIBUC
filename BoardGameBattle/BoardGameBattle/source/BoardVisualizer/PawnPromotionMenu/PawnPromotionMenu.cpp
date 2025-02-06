#include "PawnPromotionMenu.h"

PawnPromotionMenu::PawnPromotionMenu()
	: partialConstructedMove("")
{

}

PawnPromotionMenu::~PawnPromotionMenu()
{

}

PawnPromotionMenu& PawnPromotionMenu::get()
{
	static PawnPromotionMenu instance;
	return instance;
}

void PawnPromotionMenu::initialize(const std::string& move)
{
	this->partialConstructedMove = "";
	this->partialConstructedMove = move;
}

void PawnPromotionMenu::draw()
{

}

void PawnPromotionMenu::update()
{

}