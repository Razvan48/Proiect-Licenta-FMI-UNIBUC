#include "BoardManager.h"

#include "../GameMetadata/GameMetadata.h"

#include "../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"

#include "../Game/Game.h"

#include "../Client/Client.h"

#include <iostream>


BoardManager::BoardManager()
	: piecesConfiguration("rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR1111w0000")
	, whiteTurn(true)
{
	// Log Power 2
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->logPower2[(1ull << i) % BoardManager::MODULO_LOG_POWER_2] = i;

	// Rank Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
	{
		this->rankBitMasks[i] = 0ull;
		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
			this->rankBitMasks[i] |= (1ull << (i * GameMetadata::NUM_TILES_WIDTH + j));
	}

	// File Bit Masks
	for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
	{
		this->fileBitMasks[j] = 0ull;
		for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT; ++i)
			this->fileBitMasks[j] |= (1ull << (i * GameMetadata::NUM_TILES_WIDTH + j));
	}

	// Precalculated King Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->precalculatedKingAttackZones[i] = 0ull;

		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		if (row > 0)
			this->precalculatedKingAttackZones[i] |= (1ull << ((row - 1) * GameMetadata::NUM_TILES_WIDTH + column));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 1)
			this->precalculatedKingAttackZones[i] |= (1ull << ((row + 1) * GameMetadata::NUM_TILES_WIDTH + column));
		if (column > 0)
			this->precalculatedKingAttackZones[i] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + column - 1));
		if (column < GameMetadata::NUM_TILES_WIDTH - 1)
			this->precalculatedKingAttackZones[i] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + column + 1));
		if (row > 0 && column > 0)
			this->precalculatedKingAttackZones[i] |= (1ull << ((row - 1) * GameMetadata::NUM_TILES_WIDTH + column - 1));
		if (row > 0 && column < GameMetadata::NUM_TILES_WIDTH - 1)
			this->precalculatedKingAttackZones[i] |= (1ull << ((row - 1) * GameMetadata::NUM_TILES_WIDTH + column + 1));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 1 && column > 0)
			this->precalculatedKingAttackZones[i] |= (1ull << ((row + 1) * GameMetadata::NUM_TILES_WIDTH + column - 1));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 1 && column < GameMetadata::NUM_TILES_WIDTH - 1)
			this->precalculatedKingAttackZones[i] |= (1ull << ((row + 1) * GameMetadata::NUM_TILES_WIDTH + column + 1));
	}

	// Precalculated Knight Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->precalculatedKnightAttackZones[i] = 0ull;

		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		// In sens trigonometric
		if (row > 1 && column > 0)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row - 2) * GameMetadata::NUM_TILES_WIDTH + column - 1));
		if (row > 1 && column < GameMetadata::NUM_TILES_WIDTH - 1)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row - 2) * GameMetadata::NUM_TILES_WIDTH + column + 1));
		if (row > 0 && column < GameMetadata::NUM_TILES_WIDTH - 2)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row - 1) * GameMetadata::NUM_TILES_WIDTH + column + 2));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 1 && column < GameMetadata::NUM_TILES_WIDTH - 2)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row + 1) * GameMetadata::NUM_TILES_WIDTH + column + 2));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 2 && column < GameMetadata::NUM_TILES_WIDTH - 1)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row + 2) * GameMetadata::NUM_TILES_WIDTH + column + 1));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 2 && column > 0)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row + 2) * GameMetadata::NUM_TILES_WIDTH + column - 1));
		if (row < GameMetadata::NUM_TILES_HEIGHT - 1 && column > 1)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row + 1) * GameMetadata::NUM_TILES_WIDTH + column - 2));
		if (row > 0 && column > 1)
			this->precalculatedKnightAttackZones[i] |= (1ull << ((row - 1) * GameMetadata::NUM_TILES_WIDTH + column - 2));
	}

	// Precalculated Rank Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_WIDTH); ++j)
		{
			this->precalculatedRankAttackZones[i][j] = std::make_pair(0ull, std::make_pair(std::make_pair(-1, -1), std::make_pair(-1, -1)));

			if (((1ull << column) & j) == 0ull)
				continue;

			int leftColumn0 = -1;
			int leftColumn1 = -1;
			for (int currentLeftColumn = column - 1; currentLeftColumn >= 0; --currentLeftColumn)
			{
				if (((1ull << currentLeftColumn) & j) != 0ull)
				{
					if (leftColumn0 == -1)
						leftColumn0 = row * GameMetadata::NUM_TILES_WIDTH + currentLeftColumn;
					else if (leftColumn1 == -1)
						leftColumn1 = row * GameMetadata::NUM_TILES_WIDTH + currentLeftColumn;
				}
				if (leftColumn0 == -1)
					this->precalculatedRankAttackZones[i][j].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + currentLeftColumn));
			}

			int rightColumn0 = -1;
			int rightColumn1 = -1;
			for (int currentRightColumn = column + 1; currentRightColumn < GameMetadata::NUM_TILES_WIDTH; ++currentRightColumn)
			{
				if (((1ull << currentRightColumn) & j) != 0ull)
				{
					if (rightColumn0 == -1)
						rightColumn0 = row * GameMetadata::NUM_TILES_WIDTH + currentRightColumn;
					else if (rightColumn1 == -1)
						rightColumn1 = row * GameMetadata::NUM_TILES_WIDTH + currentRightColumn;
				}
				if (rightColumn0 == -1)
					this->precalculatedRankAttackZones[i][j].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + currentRightColumn));
			}

			this->precalculatedRankAttackZones[i][j].second.first.first = leftColumn0;
			this->precalculatedRankAttackZones[i][j].second.first.second = leftColumn1;
			this->precalculatedRankAttackZones[i][j].second.second.first = rightColumn0;
			this->precalculatedRankAttackZones[i][j].second.second.second = rightColumn1;
		}
	}

	// Precalculated File Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_HEIGHT); ++j)
		{
			this->precalculatedFileAttackZones[i][j] = std::make_pair(0ull, std::make_pair(std::make_pair(-1, -1), std::make_pair(-1, -1)));

			if (((1ull << row) & j) == 0ull)
				continue;

			int upRow0 = -1;
			int upRow1 = -1;
			for (int currentUpRow = row - 1; currentUpRow >= 0; --currentUpRow)
			{
				if (((1ull << currentUpRow) & j) != 0ull)
				{
					if (upRow0 == -1)
						upRow0 = currentUpRow * GameMetadata::NUM_TILES_WIDTH + column;
					else if (upRow1 == -1)
						upRow1 = currentUpRow * GameMetadata::NUM_TILES_WIDTH + column;
				}
				if (upRow0 == -1)
					this->precalculatedFileAttackZones[i][j].first |= (1ull << (currentUpRow * GameMetadata::NUM_TILES_WIDTH + column));
			}

			int downRown0 = -1;
			int downRow1 = -1;
			for (int currentDownRow = row + 1; currentDownRow < GameMetadata::NUM_TILES_HEIGHT; ++currentDownRow)
			{
				if (((1ull << currentDownRow) & j) != 0ull)
				{
					if (downRown0 == -1)
						downRown0 = currentDownRow * GameMetadata::NUM_TILES_WIDTH + column;
					else if (downRow1 == -1)
						downRow1 = currentDownRow * GameMetadata::NUM_TILES_WIDTH + column;
				}
				if (downRown0 == -1)
					this->precalculatedFileAttackZones[i][j].first |= (1ull << (currentDownRow * GameMetadata::NUM_TILES_WIDTH + column));
			}

			this->precalculatedFileAttackZones[i][j].second.first.first = upRow0;
			this->precalculatedFileAttackZones[i][j].second.first.second = upRow1;
			this->precalculatedFileAttackZones[i][j].second.second.first = downRown0;
			this->precalculatedFileAttackZones[i][j].second.second.second = downRow1;
		}
	}

	// Precalculated Top Left Bottom Right Diagonal Attack Zones
}

BoardManager::~BoardManager()
{

}

void BoardManager::initialize()
{
	this->piecesConfiguration = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR1111w0000";
	this->whiteTurn = true;
}

void BoardManager::setPiecesConfiguration(const std::string& piecesConfiguration)
{
	this->piecesConfiguration = piecesConfiguration;
	int whiteTurnPos = GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES;
	if (whiteTurnPos < this->piecesConfiguration.size())
		this->whiteTurn = (this->piecesConfiguration[whiteTurnPos] == 'w');
	else
	{
		std::cout << "Error: Could not find whose turn is it inside the Pieces Configuration in BoardManager::setPiecesConfiguration. Set to default to white." << std::endl;
	}
}

BoardManager& BoardManager::get()
{
	static BoardManager instance;
	return instance;
}


void BoardManager::applyMove(const std::string& move) // Face presupunerea ca mutarea este legala
{
	for (int i = 0; i < move.size(); i += GameMetadata::NUM_CHARS_SUBMOVE)
	{
		char gamePiece = move[i];
		int columnStart = move[i + 1] - 'a';
		int rowStart = move[i + 2] - '1';
		int columnEnd = move[i + 3] - 'a';
		int rowEnd = move[i + 4] - '1';

		this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - rowStart) * GameMetadata::NUM_TILES_WIDTH + columnStart] = '.';
		this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - rowEnd) * GameMetadata::NUM_TILES_WIDTH + columnEnd] = gamePiece;
	}

	this->whiteTurn = !this->whiteTurn;

	if (this->whiteTurn)
		this->piecesConfiguration[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES] = 'w';
	else
		this->piecesConfiguration[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES] = 'b';

	if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
		CreatedMultiplayerGameVisualInterface::get()->setHasToSendBoardConfiguration(true);
	else if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
		JoinedMultiplayerGameVisualInterface::get()->setHasToSendBoardConfiguration(true);
	// else Singleplayer, nu trebuie sa trimita nimic
}


std::vector<std::string> BoardManager::generateMovesForPiecePosition(const std::string& piecePosition)
{
	std::vector<std::string> moves;

	int column = piecePosition[0] - 'a';
	int row = piecePosition[1] - '1';
	char gamePiece = this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - row) * GameMetadata::NUM_TILES_WIDTH + column];

	if (gamePiece == '.')
		return moves;

	if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
	{
		if (Game::get().getColor() == Game::Color::WHITE && ((!('A' <= gamePiece && gamePiece <= 'Z')) || (!this->whiteTurn)))
			return moves;

		if (Game::get().getColor() == Game::Color::BLACK && ((!('a' <= gamePiece && gamePiece <= 'z')) || this->whiteTurn))
			return moves;
	}
	else if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
	{
		if (Client::get().getColor() == "") // Suntem in Multiplayer si nu stim culoarea
			return moves;

		if (Client::get().getColor() == "white" && ((!('A' <= gamePiece && gamePiece <= 'Z')) || (!this->whiteTurn)))
			return moves;

		if (Client::get().getColor() == "black" && ((!('a' <= gamePiece && gamePiece <= 'z')) || this->whiteTurn))
			return moves;
	}
	else // Game::Mode::NONE
	{
		std::cout << "Error: Game Mode not set when generating moves for piece position" << std::endl;
		return moves;
	}

	if ('A' <= gamePiece && gamePiece <= 'Z' && row <= 6) // Piesa Alba
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row + 1));
	}
	if ('A' <= gamePiece && gamePiece <= 'Z' && row <= 5)
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row + 2));
	}

	if ('a' <= gamePiece && gamePiece <= 'z' && row >= 1) // Piesa Neagra
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row - 1));
	}
	if ('a' <= gamePiece && gamePiece <= 'z' && row >= 2)
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row - 2));
	}

	return moves;
}

void BoardManager::printBitBoard(unsigned long long bitBoard) const
{
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		if ((1ull << i) & bitBoard)
			std::cout << '1';
		else
			std::cout << '0';

		if (i % GameMetadata::NUM_TILES_WIDTH == GameMetadata::NUM_TILES_WIDTH - 1)
			std::cout << std::endl;
	}
}



