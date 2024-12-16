#include "BoardVisualizer.h"

#include "../WindowManager/WindowManager.h"
#include "../Game/Game.h"

#include "../BoardManager/BoardManager.h"

#include "../Entity/TexturableEntity/TexturableEntity.h"

#include "../GameMetadata/GameMetadata.h"

#include "../InputManager/InputManager.h"
#include "../AssetManager/AssetManager.h"

#include "../Client/Client.h"

// INFO: Agent
#include "../GameAgent/GreedyMinMaxAgent/GreedyMinMaxAgent.h"


#include <iostream>

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
				"arialFont",
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
				"arialFont",
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

		if (!GreedyMinMaxAgent::get().getIsRunningTask())
			GreedyMinMaxAgent::get().findBestMove(BoardManager::get().getConfigurationMetadata());



		std::vector<std::pair<char, int>> bestMove = GreedyMinMaxAgent::get().getBestMove();
		if (!bestMove.empty()) // A fost calculat un best move.
		{
			BoardManager::get().addNewConfigurationMetadataInHistory(BoardManager::get().getConfigurationMetadata());
			BoardManager::get().getConfigurationMetadata().initialize(BoardManager::get().applyMoveInternal(BoardManager::get().getConfigurationMetadata(), bestMove));

			// Resetare
			GreedyMinMaxAgent::get().setIsRunningTask(false);
			GreedyMinMaxAgent::get().setBestMove(std::vector<std::pair<char, int>>());

			// Sunet
			AssetManager::get().playSound(this->pieceMoveSoundName, false, true);
			
			// Adaugare Mutare in Istoric
			std::string historyMove = BoardManager::get().convertToExternalMove(bestMove);
			this->addNewMoveInHistory(historyMove.substr((int)historyMove.size() - 4));
		}
	}



	// Logica pentru selectare celule si efectuare mutari
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
						move.push_back(BoardManager::get().getPiecesConfiguration()[(GameMetadata::NUM_TILES_HEIGHT - 1 - this->selectedTileRow) * GameMetadata::NUM_TILES_WIDTH + this->selectedTileColumn]);
						move.push_back((char)('a' + this->selectedTileColumn));
						move.push_back((char)('1' + this->selectedTileRow));
						move.push_back((char)('a' + j));
						move.push_back((char)('1' + i));

						BoardManager::get().addNewConfigurationMetadataInHistory(BoardManager::get().getConfigurationMetadata());
						BoardManager::get().applyMoveExternal(move);
						AssetManager::get().playSound(this->pieceMoveSoundName, false, true);
						this->addNewMoveInHistory(move.substr((int)move.size() - 4)); // Fara caracterul piesei

						this->resetSelectedTiles();
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



	// Ultima Mutare
	if (this->newMoveAtTopOfHistory)
	{
		this->newMoveAtTopOfHistory = false;

		if (!this->movesHistory.empty()) // Poate this->newMoveAtTopOfHistory era pe true din cauza ca s-a dat pop de la Butonul de Undo.
		{
			int row0 = (int)(this->movesHistory.back()[1] - '1');
			int column0 = (int)(this->movesHistory.back()[0] - 'a');
			int row1 = (int)(this->movesHistory.back()[3] - '1');
			int column1 = (int)(this->movesHistory.back()[2] - 'a');

			this->resetSelectedTiles();

			this->boardTiles[row0][column0].setIsSelected(true);
			this->boardTiles[row1][column1].setIsSelected(true);
		}
	}
}

void BoardVisualizer::addNewMoveInHistory(const std::string& move)
{
	this->movesHistory.push_back(move);
	this->newMoveAtTopOfHistory = true;
}

void BoardVisualizer::popLastMoveFromHistory()
{
	if (!this->movesHistory.empty())
	{
		this->movesHistory.pop_back();
		this->newMoveAtTopOfHistory = true;
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



