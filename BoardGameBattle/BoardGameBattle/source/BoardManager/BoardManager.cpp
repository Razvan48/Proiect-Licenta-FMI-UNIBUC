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
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->rankBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		for (int j = 0; j < GameMetadata::NUM_TILES_WIDTH; ++j)
			this->rankBitMasks[i] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + j));
	}

	// File Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->fileBitMasks[i] = 0ull;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		for (int j = 0; j < GameMetadata::NUM_TILES_HEIGHT; ++j)
			this->fileBitMasks[i] |= (1ull << (j * GameMetadata::NUM_TILES_WIDTH + column));
	}

	// Top Left Bottom Right Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->topLeftBottomRightDiagonalBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		int minDiff = min(row, column);
		int crtRow = row - minDiff;
		int crtColumn = column - minDiff;

		while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn < GameMetadata::NUM_TILES_WIDTH)
		{
			this->topLeftBottomRightDiagonalBitMasks[i] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
			++crtRow;
			++crtColumn;
		}
	}

	// Top Right Bottom Left Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->topRightBottomLeftDiagonalBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		int minDiff = min(row, GameMetadata::NUM_TILES_WIDTH - 1 - column);
		int crtRow = row - minDiff;
		int crtColumn = column + minDiff;

		while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn >= 0)
		{
			this->topRightBottomLeftDiagonalBitMasks[i] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
			++crtRow;
			--crtColumn;
		}
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

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_WIDTH); ++j) // Masca pentru piesele jucatorului curent
		{
			for (int k = 0; k < (1 << GameMetadata::NUM_TILES_WIDTH); ++k) // Masca pentru piesele adversarului
			{
				this->precalculatedRankAttackZones[i][j][k] = std::make_pair(0ull, 0ull);

				if (j & k)
					continue;
				if ((j & (1 << column)) == 0)
					continue;

				int occColumn0 = -1;
				int occColumn1 = -1;
				for (int crtColumn = column - 1; crtColumn >= 0; --crtColumn)
				{
					if ((1 << crtColumn) & (j | k))
					{
						if (occColumn0 == -1)
							occColumn0 = crtColumn;
						else if (occColumn1 == -1)
						{
							occColumn1 = crtColumn;
							break;
						}
					}
					if (occColumn0 == -1)
						this->precalculatedRankAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				}
				if (occColumn0 != -1 && (k & (1 << occColumn0)))
				{
					this->precalculatedRankAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));

					if (occColumn1 != -1 && (k & (1 << occColumn1)))
						this->precalculatedRankAttackZones[i][j][k].second |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));
				}

				occColumn0 = -1;
				occColumn1 = -1;
				for (int crtColumn = column + 1; crtColumn < GameMetadata::NUM_TILES_WIDTH; ++crtColumn)
				{
					if ((1 << crtColumn) & (j | k))
					{
						if (occColumn0 == -1)
							occColumn0 = crtColumn;
						else if (occColumn1 == -1)
						{
							occColumn1 = crtColumn;
							break;
						}
					}
					if (occColumn0 == -1)
						this->precalculatedRankAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				}
				if (occColumn0 != -1 && (k & (1 << occColumn0)))
				{
					this->precalculatedRankAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occColumn1 != -1 && (k & (1 << occColumn1)))
						this->precalculatedRankAttackZones[i][j][k].second |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));
				}
			}
		}
	}

	// Precalculated File Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_HEIGHT); ++j) // Masca pentru piesele jucatorului curent
		{
			for (int k = 0; k < (1 << GameMetadata::NUM_TILES_HEIGHT); ++k) // Masca pentru piesele adversarului
			{
				this->precalculatedFileAttackZones[i][j][k] = std::make_pair(0ull, 0ull);

				if (j & k)
					continue;
				if ((j & (1 << row)) == 0)
					continue;

				int occRow0 = -1;
				int occRow1 = -1;
				for (int crtRow = row - 1; crtRow >= 0; --crtRow)
				{
					if ((1 << crtRow) & (j | k))
					{
						if (occRow0 == -1)
							occRow0 = crtRow;
						else if (occRow1 == -1)
						{
							occRow1 = crtRow;
							break;
						}
					}
					if (occRow0 == -1)
						this->precalculatedFileAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + column));
				}
				if (occRow0 != -1 && (k & (1 << occRow0)))
				{
					this->precalculatedFileAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
					if (occRow1 != -1 && (k & (1 << occRow1)))
						this->precalculatedFileAttackZones[i][j][k].second |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
				}

				occRow0 = -1;
				occRow1 = -1;
				for (int crtRow = row + 1; crtRow < GameMetadata::NUM_TILES_HEIGHT; ++crtRow)
				{
					if ((1 << crtRow) & (j | k))
					{
						if (occRow0 == -1)
							occRow0 = crtRow;
						else if (occRow1 == -1)
						{
							occRow1 = crtRow;
							break;
						}
					}
					if (occRow0 == -1)
						this->precalculatedFileAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + column));
				}
				if (occRow0 != -1 && (k & (1 << occRow0)))
				{
					this->precalculatedFileAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
					if (occRow1 != -1 && (k & (1 << occRow1)))
						this->precalculatedFileAttackZones[i][j][k].second |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
				}
			}
		}
	}

	// Precalculated Top Left Bottom Right Diagonal Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, GameMetadata::NUM_TILES_WIDTH - 1 - column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			for (int k = 0; k < (1 << diagonalLength); ++k)
			{
				this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k] = std::make_pair(0ull, 0ull);

				if (j & k)
					continue;
				if ((j & (1 << posInDiagonal)) == 0)
					continue;

				int occPos0 = -1;
				int occRow0 = -1;
				int occColumn0 = -1;
				int occPos1 = -1;
				int occRow1 = -1;
				int occColumn1 = -1;
				int crtRow = row - 1;
				int crtColumn = column - 1;
				int crtPosInDiagonal = posInDiagonal - 1;
				while (crtRow >= 0 && crtColumn >= 0)
				{
					if ((1 << crtPosInDiagonal) & (j | k))
					{
						if (occPos0 == -1)
						{
							occPos0 = crtPosInDiagonal;
							occRow0 = crtRow;
							occColumn0 = crtColumn;
						}
						else if (occPos1 == -1)
						{
							occPos1 = crtPosInDiagonal;
							occRow1 = crtRow;
							occColumn1 = crtColumn;
							break;
						}
					}
					if (occPos0 == -1)
						this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					--crtRow;
					--crtColumn;
					--crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
						this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k].second |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
				}

				occPos0 = -1;
				occRow0 = -1;
				occColumn0 = -1;
				occPos1 = -1;
				occRow1 = -1;
				occColumn1 = -1;
				crtRow = row + 1;
				crtColumn = column + 1;
				crtPosInDiagonal = posInDiagonal + 1;
				while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn < GameMetadata::NUM_TILES_WIDTH)
				{
					if ((1 << crtPosInDiagonal) & (j | k))
					{
						if (occPos0 == -1)
						{
							occPos0 = crtPosInDiagonal;
							occRow0 = crtRow;
							occColumn0 = crtColumn;
						}
						else if (occPos1 == -1)
						{
							occPos1 = crtPosInDiagonal;
							occRow1 = crtRow;
							occColumn1 = crtColumn;
							break;
						}
					}
					if (occPos0 == -1)
						this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					++crtRow;
					++crtColumn;
					++crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
						this->precalculatedTopLeftBottomRightDiagonalAttackZones[i][j][k].second |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
				}
			}
		}
	}

	// Precalculated Top Right Bottom Left Diagonal Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, GameMetadata::NUM_TILES_WIDTH - 1 - column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			for (int k = 0; k < (1 << diagonalLength); ++k)
			{
				this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k] = std::make_pair(0ull, 0ull);

				if (j & k)
					continue;
				if ((j & (1 << posInDiagonal)) == 0)
					continue;

				int occPos0 = -1;
				int occRow0 = -1;
				int occColumn0 = -1;
				int occPos1 = -1;
				int occRow1 = -1;
				int occColumn1 = -1;
				int crtRow = row - 1;
				int crtColumn = column + 1;
				int crtPosInDiagonal = posInDiagonal - 1;
				while (crtRow >= 0 && crtColumn < GameMetadata::NUM_TILES_WIDTH)
				{
					if ((1 << crtPosInDiagonal) & (j | k))
					{
						if (occPos0 == -1)
						{
							occPos0 = crtPosInDiagonal;
							occRow0 = crtRow;
							occColumn0 = crtColumn;
						}
						else if (occPos1 == -1)
						{
							occPos1 = crtPosInDiagonal;
							occRow1 = crtRow;
							occColumn1 = crtColumn;
							break;
						}
					}
					if (occPos0 == -1)
						this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					--crtRow;
					++crtColumn;
					--crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
						this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k].second |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
				}

				occPos0 = -1;
				occRow0 = -1;
				occColumn0 = -1;
				occPos1 = -1;
				occRow1 = -1;
				occColumn1 = -1;
				crtRow = row + 1;
				crtColumn = column - 1;
				crtPosInDiagonal = posInDiagonal + 1;
				while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn >= 0)
				{
					if ((1 << crtPosInDiagonal) & (j | k))
					{
						if (occPos0 == -1)
						{
							occPos0 = crtPosInDiagonal;
							occRow0 = crtRow;
							occColumn0 = crtColumn;
						}
						else if (occPos1 == -1)
						{
							occPos1 = crtPosInDiagonal;
							occRow1 = crtRow;
							occColumn1 = crtColumn;
							break;
						}
					}
					if (occPos0 == -1)
						this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					++crtRow;
					--crtColumn;
					++crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
						this->precalculatedTopRightBottomLeftDiagonalAttackZones[i][j][k].second |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
				}
			}
		}
	}
}

BoardManager::~BoardManager()
{

}

unsigned long long BoardManager::extractRank(unsigned long long bitBoard, int pos) const
{
	return (bitBoard & this->rankBitMasks[pos]) >> (pos / GameMetadata::NUM_TILES_WIDTH * GameMetadata::NUM_TILES_WIDTH);
}

unsigned long long BoardManager::extractFile(unsigned long long bitBoard, int pos) const
{
	unsigned long long sol = 0ull;
	bitBoard &= this->fileBitMasks[pos];
	while (bitBoard)
	{
		unsigned long long lsb = (bitBoard & ((~bitBoard) + 1));
		sol |= (1ull << (this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2] / GameMetadata::NUM_TILES_WIDTH));
		bitBoard ^= lsb;
	}
	return sol;
}

unsigned long long BoardManager::extractTopLeftBottomRightDiagonal(unsigned long long bitBoard, int pos) const
{
	unsigned long long sol = 0ull;
	bitBoard &= this->topLeftBottomRightDiagonalBitMasks[pos];
	int firstRowDiag = max(0, pos / GameMetadata::NUM_TILES_WIDTH - pos % GameMetadata::NUM_TILES_WIDTH);
	while (bitBoard)
	{
		unsigned long long lsb = (bitBoard & ((~bitBoard) + 1));
		sol |= (1ull << (pos / GameMetadata::NUM_TILES_WIDTH - firstRowDiag));
		bitBoard ^= lsb;
	}
	return sol;
}

unsigned long long BoardManager::extractTopRightBottomLeftDiagonal(unsigned long long bitBoard, int pos) const
{
	unsigned long long sol = 0ull;
	bitBoard &= this->topRightBottomLeftDiagonalBitMasks[pos];
	int firstRowDiag = max(0, pos / GameMetadata::NUM_TILES_WIDTH - (GameMetadata::NUM_TILES_WIDTH - 1 - pos % GameMetadata::NUM_TILES_WIDTH));
	while (bitBoard)
	{
		unsigned long long lsb = (bitBoard & ((~bitBoard) + 1));
		sol |= (1ull << (pos / GameMetadata::NUM_TILES_WIDTH - firstRowDiag));
		bitBoard ^= lsb;
	}
	return sol;
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



