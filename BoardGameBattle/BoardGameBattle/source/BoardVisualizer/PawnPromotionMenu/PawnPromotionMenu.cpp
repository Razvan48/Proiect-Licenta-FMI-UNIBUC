#include "PawnPromotionMenu.h"

#include "../BoardVisualizer.h"
#include "../../VisualInterface/SingleplayerGameVisualInterface/SingleplayerGameVisualInterface.h"
#include "../../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"

#include "../../Client/Client.h"

#include <iostream>

PawnPromotionMenu::PawnPromotionMenu()
	: partialConstructedMove("")
{
	// TODO: pozitia si dimensiunea butoanelor
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

	if (this->isPlayerColorWhite())
	{
		this->rookPromotionButton.setTextureName("whiteRookTexture");
		this->knightPromotionButton.setTextureName("whiteKnightTexture");
		this->bishopPromotionButton.setTextureName("whiteBishopTexture");
		this->queenPromotionButton.setTextureName("whiteQueenTexture");
	}
	else
	{
		this->rookPromotionButton.setTextureName("blackRookTexture");
		this->knightPromotionButton.setTextureName("blackKnightTexture");
		this->bishopPromotionButton.setTextureName("blackBishopTexture");
		this->queenPromotionButton.setTextureName("blackQueenTexture");
	}
}

void PawnPromotionMenu::draw()
{
	this->backgroundEntity.draw();
	this->cancelPawnPromotionButton.draw();
	this->rookPromotionButton.draw();
	this->knightPromotionButton.draw();
	this->bishopPromotionButton.draw();
	this->queenPromotionButton.draw();
}

void PawnPromotionMenu::update()
{
	this->backgroundEntity.update();
	this->cancelPawnPromotionButton.update();
	this->rookPromotionButton.update();
	this->knightPromotionButton.update();
	this->bishopPromotionButton.update();
	this->queenPromotionButton.update();

	if (this->cancelPawnPromotionButton.getRecentlyInteractedWith()
		|| this->rookPromotionButton.getRecentlyInteractedWith()
		|| this->knightPromotionButton.getRecentlyInteractedWith()
		|| this->bishopPromotionButton.getRecentlyInteractedWith()
		|| this->queenPromotionButton.getRecentlyInteractedWith())
	{
		if (this->isPlayerColorWhite())
		{
			if (this->rookPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "R";
			else if (this->knightPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "N";
			else if (this->bishopPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "B";
			else if (this->queenPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "Q";
			// else cancel

			BoardVisualizer::get().sendMoveToBoardManager(this->partialConstructedMove);
		}
		else
		{
			if (this->rookPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "r";
			else if (this->knightPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "n";
			else if (this->bishopPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "b";
			else if (this->queenPromotionButton.getRecentlyInteractedWith())
				this->partialConstructedMove += "q";
			// else cancel

			BoardVisualizer::get().sendMoveToBoardManager(this->partialConstructedMove);
		}



		BoardVisualizer::get().setPawnPromotionMenuActive(false);
		if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
			SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::NOT_FINISHED);
		else // if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
		{
			if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
				CreatedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(CreatedMultiplayerGameVisualInterface::FinalMessage::NOT_FINISHED);
			else // if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
				JoinedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(JoinedMultiplayerGameVisualInterface::FinalMessage::NOT_FINISHED);
		}
	}
}

bool PawnPromotionMenu::isPlayerColorWhite() const
{
	if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
		return Game::get().getColor() == Game::Color::WHITE;
	else // if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
	{
		if (Client::get().getColor() == "")
		{
			std::cout << "Error: PawnPromotionMenu: Client color not set" << std::endl;
			return false;
		}
		return Client::get().getColor() == "white";
	}
}


