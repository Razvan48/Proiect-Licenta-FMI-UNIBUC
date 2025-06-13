#include "BoardVisualizer.h"

#include "../WindowManager/WindowManager.h"
#include "../Game/Game.h"

#include "../BoardManager/BoardManager.h"

#include "../Entity/TexturableEntity/TexturableEntity.h"

#include "../GameMetadata/GameMetadata.h"

#include "../InputManager/InputManager.h"
#include "../AssetManager/AssetManager.h"

#include "../Client/Client.h"

#include "../GameAgent/GameAgentSelector/GameAgentSelector.h" // INFO: Agent


#include "../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/SingleplayerGameVisualInterface/SingleplayerGameVisualInterface.h"

#include "PawnPromotionMenu/PawnPromotionMenu.h" // INFO: Pawn Promotion Menu

#include "../AutomateGameplay/AutomateGameplay.h"


#include <iostream>

#include <string>



BoardVisualizer::BoardVisualizer()
	: whiteBoardTileTextureName("whiteBoardTileTexture")
	, blackBoardTileTextureName("blackBoardTileTexture")
	, selectedBoardTileTextureName("selectedBoardTileTexture")
	, whiteKingTextureName("whiteKingTexture")
	, whiteQueenTextureName("whiteQueenTexture")
	, whiteRookTextureName("whiteRookTexture")
	, whiteBishopTextureName("whiteBishopTexture")
	, whiteKnightTextureName("whiteKnightTexture")
	, whitePawnTextureName("whitePawnTexture")
	, blackKingTextureName("blackKingTexture")
	, blackQueenTextureName("blackQueenTexture")
	, blackRookTextureName("blackRookTexture")
	, blackBishopTextureName("blackBishopTexture")
	, blackKnightTextureName("blackKnightTexture")
	, blackPawnTextureName("blackPawnTexture")
	, BOARD_TILE_WIDTH(1.0f * WindowManager::get().getWindowWidth() / 10.0f)
	, BOARD_TILE_HEIGHT(1.0f * WindowManager::get().getWindowHeight() / 10.0f)
	, selectedTileRow(-1)
	, selectedTileColumn(-1)
	, pieceMoveSoundName("pieceMoveSound")
	, newMoveAtTopOfHistory(false)
	, gameHasEnded(false)
	, pawnPromotionMenuActive(false)
	, estimation(GameAgentSelector::get().getUnreachableEstimation())
	, estimationCalculated(false)
{

}

BoardVisualizer::~BoardVisualizer()
{

}

BoardVisualizer& BoardVisualizer::get()
{
	static BoardVisualizer instance;
	return instance;
}

void BoardVisualizer::initialize()
{
	this->selectedTileRow = -1;
	this->selectedTileColumn = -1;

	this->boardTiles.clear();
	this->boardCoordinates.clear();

	this->boardTiles.resize(GameMetadata::NUM_TILES_HEIGHT);
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
	{
		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
		{
			this->boardTiles[i].push_back(
				BoardTile
				(
					(j + 1) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f,
					(i + 1) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f,
					this->BOARD_TILE_WIDTH,
					this->BOARD_TILE_HEIGHT,
					0.0f,
					(j + i) % 2 ? this->whiteBoardTileTextureName : this->blackBoardTileTextureName,
					false,
					this->selectedBoardTileTextureName
				)
			);
		}
	}
	// a, b, c, d, e, f, g, h
	for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
	{
		this->boardCoordinates.push_back(
			TextEntity
			(
				(j + 1) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f,
				this->BOARD_TILE_HEIGHT / 2.0f,
				this->BOARD_TILE_WIDTH / 2.0f,
				this->BOARD_TILE_HEIGHT / 2.0f,
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"defaultFont",
				std::string(1, (char)('a' + j))
			)
		);

	}
	// 1, 2, 3, 4, 5, 6, 7, 8
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
	{
		this->boardCoordinates.push_back(
			TextEntity
			(
				this->BOARD_TILE_WIDTH / 2.0f,
				(i + 1) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f,
				this->BOARD_TILE_WIDTH / 2.0f,
				this->BOARD_TILE_HEIGHT / 2.0f,
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"defaultFont",
				std::string(1, (char)('1' + i))
			)
		);
	}

	if (Game::get().getColor() == Game::Color::BLACK || (Game::get().getColor() == Game::Color::NONE && Client::get().getColor() == "black")) // rotire 180 de grade
	{
		for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
		{
			for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
			{
				this->boardTiles[i][j].setPosCenterX(
					(GameMetadata::NUM_TILES_WIDTH - j) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f
				);
				this->boardTiles[i][j].setPosCenterY(
					(GameMetadata::NUM_TILES_HEIGHT - i) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f
				);
			}
		}

		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
		{
			this->boardCoordinates[j].setPosCenterX(
				(GameMetadata::NUM_TILES_WIDTH - j) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f
			);
		}

		for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
		{
			this->boardCoordinates[GameMetadata::NUM_TILES_WIDTH + i].setPosCenterY(
				(GameMetadata::NUM_TILES_HEIGHT - i) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f
			);
		}
	}
	else if (Game::get().getColor() == Game::Color::NONE) // poate fi eliminat ulterior
	{
		std::cout << "Error: Game Color is NONE when initializing board" << std::endl;
	}
	else if (Game::get().getColor() != Game::Color::WHITE) // Putea fi si alb.
	{
		std::cout << "Error: Invalid Game Color when initializing board (not even NONE)" << std::endl;
	}

	// Curatam Istoria Mutarilor de la Meciul Anterior
	this->movesHistory.clear();
	this->newMoveAtTopOfHistory = false;

	this->gameHasEnded = false;

	this->pawnPromotionMenuActive = false;


	// INFO: Estimarea este resetata la fiecare inceput de joc.
	this->estimation = GameAgentSelector::get().getUnreachableEstimation();
	this->estimationCalculated = false;


	// Daca Agentul este activ trebuie resetat
	GameAgentSelector::get().setIsFindingBestMove(false);
	GameAgentSelector::get().setBestMove(std::vector<std::pair<char, int>>());
	GameAgentSelector::get().setIsFindBestMoveCancelled(true);

	GameAgentSelector::get().reset(); // INFO: Nu face nimic momentan.

	GameAgentSelector::get().setIsEstimating(false);
	GameAgentSelector::get().resetEstimation();
	GameAgentSelector::get().setIsEstimateCancelled(true);

	// Automate Gameplay
	AutomateGameplay::get().initialize();
}

void BoardVisualizer::draw()
{
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
	{
		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
		{
			this->boardTiles[i][j].draw();
		}
	}

	for (int i = 0; i < this->boardCoordinates.size(); ++i)
	{
		this->boardCoordinates[i].draw();
	}

	// Desenarea pieselor
	std::string piecesConfiguration = BoardManager::get().getPiecesConfiguration();

	TexturableEntity currentPiece(
		0.0f,
		0.0f,
		this->BOARD_TILE_WIDTH,
		this->BOARD_TILE_HEIGHT,
		0.0f,
		""
	);
	for (int i = 0; i < min((int)piecesConfiguration.size(), GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH); ++i)
	{
		int currentI = (GameMetadata::NUM_TILES_HEIGHT - 1) - i / GameMetadata::NUM_TILES_WIDTH;
		int currentJ = i % GameMetadata::NUM_TILES_WIDTH;

		currentPiece.setPosCenterX(this->boardTiles[currentI][currentJ].getPosCenterX());
		currentPiece.setPosCenterY(this->boardTiles[currentI][currentJ].getPosCenterY());
		currentPiece.setRotateAngle(this->boardTiles[currentI][currentJ].getRotateAngle());

		switch (piecesConfiguration[i])
		{
		case '.':
			continue;
			break;
		case 'K':
			currentPiece.setTextureName(this->whiteKingTextureName);
			break;
		case 'Q':
			currentPiece.setTextureName(this->whiteQueenTextureName);
			break;
		case 'R':
			currentPiece.setTextureName(this->whiteRookTextureName);
			break;
		case 'B':
			currentPiece.setTextureName(this->whiteBishopTextureName);
			break;
		case 'N':
			currentPiece.setTextureName(this->whiteKnightTextureName);
			break;
		case 'P':
			currentPiece.setTextureName(this->whitePawnTextureName);
			break;
		case 'k':
			currentPiece.setTextureName(this->blackKingTextureName);
			break;
		case 'q':
			currentPiece.setTextureName(this->blackQueenTextureName);
			break;
		case 'r':
			currentPiece.setTextureName(this->blackRookTextureName);
			break;
		case 'b':
			currentPiece.setTextureName(this->blackBishopTextureName);
			break;
		case 'n':
			currentPiece.setTextureName(this->blackKnightTextureName);
			break;
		case 'p':
			currentPiece.setTextureName(this->blackPawnTextureName);
			break;
		default:
			std::cout << "Error: Invalid character in pieces configuration received in board visualizer from board manager" << std::endl;
			break;
		}

		currentPiece.draw();
	}
}

void BoardVisualizer::update()
{
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
			this->boardTiles[i][j].update();

	for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
		this->boardCoordinates[j].update();

	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
		this->boardCoordinates[GameMetadata::NUM_TILES_WIDTH + i].update();


	if (this->pawnPromotionMenuActive) // INFO: Daca suntem in Pawn Promotion Menu se presupune ca nu se mai intampla nimic din logica de mai jos.
	{
		PawnPromotionMenu::get().draw();
		PawnPromotionMenu::get().update();
		return;
	}


	// Agent
	if (
			Game::get().getMode() == Game::Mode::SINGLEPLAYER
			&&
			(
				(Game::get().getColor() == Game::Color::WHITE && !BoardManager::get().getConfigurationMetadata().whiteTurn)
				||
				(Game::get().getColor() == Game::Color::BLACK && BoardManager::get().getConfigurationMetadata().whiteTurn)
			)
	)
	{
		if (SingleplayerGameVisualInterface::get().get()->getFinalMessage() == SingleplayerGameVisualInterface::FinalMessage::NOT_FINISHED)
		{
			if (!GameAgentSelector::get().getIsFindingBestMove())
				GameAgentSelector::get().findBestMove(BoardManager::get().getConfigurationMetadata());



			std::vector<std::pair<char, int>> bestMove = GameAgentSelector::get().getBestMove();
			if (!bestMove.empty()) // A fost calculat un best move.
			{
				// Adaugare in Istoric pentru BoardManager
				BoardManager::get().addNewConfigurationMetadataInHistory(BoardManager::get().getConfigurationMetadata());
				BoardManager::get().getConfigurationMetadata().initialize(BoardManager::get().applyMoveInternal(BoardManager::get().getConfigurationMetadata(), bestMove, BoardManager::get().getZobristHashingValuesFrequency()));

				// Resetare
				GameAgentSelector::get().setIsFindingBestMove(false);
				GameAgentSelector::get().setBestMove(std::vector<std::pair<char, int>>());
				GameAgentSelector::get().setIsFindBestMoveCancelled(true);

				// Sunet
				AssetManager::get().playSound(this->pieceMoveSoundName, false, true);

				// Adaugare Mutare in Istoric pentru BoardVisualizer
				std::string historyMove = BoardManager::get().convertToExternalMove(bestMove);

				// INFO: Apelul de mai jos trebuie sa ramana ultimul, deoarece se bazeaza pe faptul ca noua configuratie este deja setata (estimarea se bazeaza).
				this->addNewMoveInHistory(historyMove.substr((int)historyMove.size() - 4));

				// Automate Gameplay
				if (AutomateGameplay::get().getConnectionAccepted())
				{
					std::string message = historyMove;
					message.resize(GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY, '$');
					message += BoardManager::get().getPiecesConfigurationAutomateGameplay(); // INFO: E mai mult decat trebuie, d-aia dam resize pe urmatoarea linie.
					message.resize(GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY + GameMetadata::STRING_SIZE_CONFIGURATION_AUTOMATE_GAMEPLAY, '$');
					AutomateGameplay::get().sendMessage(message);
				}
			}
		}
		else
		{
			GameAgentSelector::get().setIsFindingBestMove(false);
			GameAgentSelector::get().setBestMove(std::vector<std::pair<char, int>>());
			GameAgentSelector::get().setIsFindBestMoveCancelled(true);
		}
	}


	// Logica pentru selectare celule si efectuare mutari
	if
		(
			(
				Game::get().getMode() == Game::Mode::SINGLEPLAYER && SingleplayerGameVisualInterface::get().get()->getFinalMessage() == SingleplayerGameVisualInterface::FinalMessage::NOT_FINISHED
			)
				||
			(
				Game::get().getMode() == Game::Mode::MULTIPLAYER && Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME && CreatedMultiplayerGameVisualInterface::get().get()->getFinalMessage() == CreatedMultiplayerGameVisualInterface::FinalMessage::NOT_FINISHED
			)
				||
			(
				Game::get().getMode() == Game::Mode::MULTIPLAYER && Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME && JoinedMultiplayerGameVisualInterface::get().get()->getFinalMessage() == JoinedMultiplayerGameVisualInterface::FinalMessage::NOT_FINISHED
			)
		)
	{
		if (InputManager::get().isLeftMouseButtonReleased())
		{
			if (this->selectedTileRow != -1 && this->selectedTileColumn != -1)
			{
				bool selectedTile = false;

				for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
				{
					for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
					{
						if (this->boardTiles[i][j].isInCompleteMouseCollision() && this->boardTiles[i][j].getIsSelected()
							&& (i != this->selectedTileRow || j != this->selectedTileColumn))
						{
							selectedTile = true;

							std::string move = "";
							move.push_back(BoardManager::get().getPiecesConfiguration()[(GameMetadata::NUM_TILES_HEIGHT - 1 - this->selectedTileRow) * GameMetadata::NUM_TILES_WIDTH + this->selectedTileColumn]); // Caracterul piesei
							move.push_back((char)('a' + this->selectedTileColumn));
							move.push_back((char)('1' + this->selectedTileRow));
							move.push_back((char)('a' + j));
							move.push_back((char)('1' + i));

							if ((move[0] == 'p' || move[0] == 'P') && (move[4] == '1' || move[4] == '8')) // INFO: Daca este o mutare de promovare. (Este pion si a ajuns la capatul tablei.)
							// if ((move[0] == 'p' || move[0] == 'P') && (move[4] == '2' || move[4] == '7')) // INFO: Pentru tabla de n x m in centru.
							{
								this->pawnPromotionMenuActive = true;

								if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
									SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::IN_PAWN_PROMOTION_MENU);
								else // if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
								{
									if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
										CreatedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(CreatedMultiplayerGameVisualInterface::FinalMessage::IN_PAWN_PROMOTION_MENU);
									else // if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
										JoinedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(JoinedMultiplayerGameVisualInterface::FinalMessage::IN_PAWN_PROMOTION_MENU);
								}
								
								PawnPromotionMenu::get().initialize(move);
							}
							else
							{
								this->sendMoveToBoardManager(move);
							}
						}
					}
				}

				if (!selectedTile) // Nu s-a selectat una din celulele ce puteau face o mutare
				{
					int previousSelectedTileRow = this->selectedTileRow;
					int previousSelectedTileColumn = this->selectedTileColumn;
					this->resetSelectedTiles();

					for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
					{
						for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
						{
							if (this->boardTiles[i][j].isInCompleteMouseCollision() && (i != previousSelectedTileRow || j != previousSelectedTileColumn))
							{
								this->boardTiles[i][j].setIsSelected(true);
								this->selectedTileRow = i;
								this->selectedTileColumn = j;

								std::string piecePosition = "";
								piecePosition.push_back((char)('a' + j));
								piecePosition.push_back((char)('1' + i));

								std::vector<std::string> moves = BoardManager::get().generateMovesForPiecePosition(piecePosition);

								for (int k = 0; k < moves.size(); ++k)
								{
									int rowEnd = (int)(moves[k][4] - '1');
									int columnEnd = (int)(moves[k][3] - 'a');

									this->boardTiles[rowEnd][columnEnd].setIsSelected(true);
								}
							}
						}
					}
				}
			}
			else
			{
				this->resetSelectedTiles();

				for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
				{
					for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
					{
						if (this->boardTiles[i][j].isInCompleteMouseCollision())
						{
							this->boardTiles[i][j].setIsSelected(true);
							this->selectedTileRow = i;
							this->selectedTileColumn = j;

							std::string piecePosition = "";
							piecePosition.push_back((char)('a' + j));
							piecePosition.push_back((char)('1' + i));

							std::vector<std::string> moves = BoardManager::get().generateMovesForPiecePosition(piecePosition);

							for (int k = 0; k < moves.size(); ++k)
							{
								int rowEnd = (int)(moves[k][4] - '1');
								int columnEnd = (int)(moves[k][3] - 'a');

								this->boardTiles[rowEnd][columnEnd].setIsSelected(true);
							}
						}
					}
				}
			}
		}

		// Automate Gameplay
		if (Game::get().getMode() == Game::Mode::SINGLEPLAYER &&
				(
					(Game::get().getColor() == Game::Color::WHITE && BoardManager::get().getConfigurationMetadata().whiteTurn)
					||
					(Game::get().getColor() == Game::Color::BLACK && !BoardManager::get().getConfigurationMetadata().whiteTurn)
				)
			)
		{
			if (AutomateGameplay::get().getConnectionAccepted())
			{
				std::string move = AutomateGameplay::get().getMessage(); // INFO: Mutare primita de la server (se presupune ca este corecta)
				if (move != "")
				{
					if (move[(int)move.size() - 1] == '$')
						move.resize(GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY - 1);
					this->sendMoveToBoardManager(move);
				}
			}
		}
	}



	// Ultima Mutare
	if (this->newMoveAtTopOfHistory)
	{
		this->newMoveAtTopOfHistory = false;

		if (!this->movesHistory.empty()) // Poate this->newMoveAtTopOfHistory era pe true din cauza ca s-a dat pop de la Butonul de Undo.
		{
			int row0 = (int)(this->movesHistory.back().first[1] - '1');
			int column0 = (int)(this->movesHistory.back().first[0] - 'a');
			int row1 = (int)(this->movesHistory.back().first[3] - '1');
			int column1 = (int)(this->movesHistory.back().first[2] - 'a');

			this->resetSelectedTiles();

			this->boardTiles[row0][column0].setIsSelected(true);
			this->boardTiles[row1][column1].setIsSelected(true);
		}
	}



	// Jocul s-a terminat
	if (BoardManager::get().isWhiteKingInCheckmate(BoardManager::get().getConfigurationMetadata()) || BoardManager::get().isBlackKingInCheckmate(BoardManager::get().getConfigurationMetadata()))
	{
		if (!this->gameHasEnded)
		{
			this->gameHasEnded = true;

			if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
			{
				if (BoardManager::get().isWhiteKingInCheckmate(BoardManager::get().getConfigurationMetadata()))
				{
					if (Game::get().getColor() == Game::Color::WHITE)
						SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::LOST);
					else
						SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::WON);
				}
				else // if (BoardManager::get().isBlackKingInCheckmate(BoardManager::get().getConfigurationMetadata()))
				{
					if (Game::get().getColor() == Game::Color::BLACK)
						SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::LOST);
					else
						SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::WON);
				}
			}
			else if (Client::get().getColor() != "") // + if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
			{
				SingleplayerGameVisualInterface::FinalMessage finalMessage = SingleplayerGameVisualInterface::FinalMessage::NOT_FINISHED;

				if (BoardManager::get().isWhiteKingInCheckmate(BoardManager::get().getConfigurationMetadata()))
				{
					if (Client::get().getColor() == "white")
						finalMessage = SingleplayerGameVisualInterface::FinalMessage::LOST;
					else
						finalMessage = SingleplayerGameVisualInterface::FinalMessage::WON;
				}
				else // if (BoardManager::get().isBlackKingInCheckmate(BoardManager::get().getConfigurationMetadata()))
				{
					if (Client::get().getColor() == "black")
						finalMessage = SingleplayerGameVisualInterface::FinalMessage::LOST;
					else
						finalMessage = SingleplayerGameVisualInterface::FinalMessage::WON;
				}

				if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
					CreatedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(finalMessage);
				else // if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
					JoinedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(finalMessage);
			}
			else // Culoare egala cu "" sau Game::Mode e NONE
			{
				std::cout << "Error: Invalid Game Mode or Color when game has ended" << std::endl;

				this->gameHasEnded = false;
			}
		}
	}
	else if (BoardManager::get().isWhiteKingInDraw(BoardManager::get().getConfigurationMetadata()) || BoardManager::get().isBlackKingInDraw(BoardManager::get().getConfigurationMetadata()))
	{
		if (!this->gameHasEnded)
		{
			this->gameHasEnded = true;

			if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
				SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::DRAW);
			else // if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
			{
				if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
					CreatedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(CreatedMultiplayerGameVisualInterface::FinalMessage::DRAW);
				else // if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
					JoinedMultiplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(JoinedMultiplayerGameVisualInterface::FinalMessage::DRAW);
			}
		}
	}
	else if (BoardManager::get().isDrawByRepetition(BoardManager::get().getConfigurationMetadata()) && Game::get().getMode() == Game::Mode::SINGLEPLAYER) // INFO: Doar pentru Singleplayer se verifica daca e Draw by Repetition. Implementarea de mai jos este doar pentru Singleplayer.
	{
		if (!this->gameHasEnded)
		{
			this->gameHasEnded = true;

			SingleplayerGameVisualInterface::get().get()->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::DRAW);
		}
	}
	else
	{
		if (this->gameHasEnded)
		{
			this->gameHasEnded = false;

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

	// Estimare
	if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
	{
		SingleplayerGameVisualInterface::get().get()->setEstimationValue(this->estimation);
		SingleplayerGameVisualInterface::get().get()->setEstimationObsolete(!this->estimationCalculated);
	}
	else // if (Game::get().getMode() == Game::Mode::MULTIPLAYER) 
	{
		if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
		{
			CreatedMultiplayerGameVisualInterface::get().get()->setEstimationValue(this->estimation);
			CreatedMultiplayerGameVisualInterface::get().get()->setEstimationObsolete(!this->estimationCalculated);
		}
		else // if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
		{
			JoinedMultiplayerGameVisualInterface::get().get()->setEstimationValue(this->estimation);
			JoinedMultiplayerGameVisualInterface::get().get()->setEstimationObsolete(!this->estimationCalculated);
		}
	}

	if (!this->estimationCalculated)
	{
		if (!GameAgentSelector::get().getIsEstimating())
			GameAgentSelector::get().estimateConfiguration(BoardManager::get().getConfigurationMetadata());

		float estimation = GameAgentSelector::get().getEstimation();
		if (estimation != GameAgentSelector::get().getUnreachableEstimation())
		{
			this->estimation = estimation;
			this->estimationCalculated = true;
		}
	}

	// Automate Gameplay Update
	AutomateGameplay::get().update();
}

void BoardVisualizer::addNewMoveInHistory(const std::string& move)
{
	this->movesHistory.push_back(std::make_pair(move, std::make_pair(this->estimation, this->estimationCalculated)));
	this->newMoveAtTopOfHistory = true;

	// this->estimation = 0.0f; // INFO: Nu facem asta ca sa ramana vechea estimare pe ecran.
	this->estimationCalculated = false;

	// INFO: Daca se adauga o noua mutare in istoric atunci estimarea anterioara nu mai este cea mai recenta.
	GameAgentSelector::get().setIsEstimating(false);
	GameAgentSelector::get().resetEstimation();
	GameAgentSelector::get().setIsEstimateCancelled(true);

	GameAgentSelector::get().estimateConfiguration(BoardManager::get().getConfigurationMetadata());
}

void BoardVisualizer::popLastMoveFromHistory()
{
	if (!this->movesHistory.empty())
	{
		this->estimationCalculated = this->movesHistory.back().second.second;
		if (this->estimationCalculated) // INFO: Facem asta ca sa ramana vechea estimare pe ecran.
			this->estimation = this->movesHistory.back().second.first;

		this->movesHistory.pop_back();
		this->newMoveAtTopOfHistory = true;
	}

	// INFO: Daca se elimina o mutare din istoric atunci estimarea anterioara nu mai este cea mai recenta.
	if (!this->estimationCalculated)
	{
		GameAgentSelector::get().setIsEstimating(false);
		GameAgentSelector::get().resetEstimation();
		GameAgentSelector::get().setIsEstimateCancelled(true);

		GameAgentSelector::get().estimateConfiguration(BoardManager::get().getConfigurationMetadata());
	}
}

void BoardVisualizer::resetSelectedTiles()
{
	this->selectedTileRow = -1;
	this->selectedTileColumn = -1;

	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
			this->boardTiles[i][j].setIsSelected(false);
}

void BoardVisualizer::sendMoveToBoardManager(const std::string& move)
{
	BoardManager::get().addNewConfigurationMetadataInHistory(BoardManager::get().getConfigurationMetadata());
	BoardManager::get().applyMoveExternal(move);
	AssetManager::get().playSound(this->pieceMoveSoundName, false, true);
	std::string moveInHistory = "";
	for (int i = 1; i < 5; ++i)
		moveInHistory.push_back(move[i]);
	// INFO: Mai intai aplicam mutarea pe tabla si apoi adaugam noua configuratie in istoric (estimarea are nevoie ca tabla sa fie actualizata).
	this->addNewMoveInHistory(moveInHistory); // Fara caracterul piesei + Fara ultimul caracter in caz de promovare pion

	this->resetSelectedTiles();
}





