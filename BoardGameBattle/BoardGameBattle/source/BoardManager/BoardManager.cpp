#include "BoardManager.h"

#include "../GameMetadata/GameMetadata.h"

#include "../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"

#include "../Game/Game.h"

#include "../Client/Client.h"

#include <iostream>

BoardManager::BoardManager()
	: piecesConfiguration("rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR1111w0000")
{
	// TODO: move this from here
	this->configurationMetadata.whiteTurn = true;


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

	// Left Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->leftBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < column; ++j)
			this->leftBitMasks[i] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + j));
	}

	// Right Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->rightBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = column + 1; j < GameMetadata::NUM_TILES_WIDTH; ++j)
			this->rightBitMasks[i] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + j));
	}

	// Top Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->topBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < row; ++j)
			this->topBitMasks[i] |= (1ull << (j * GameMetadata::NUM_TILES_WIDTH + column));
	}

	// Bottom Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->bottomBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = row + 1; j < GameMetadata::NUM_TILES_HEIGHT; ++j)
			this->bottomBitMasks[i] |= (1ull << (j * GameMetadata::NUM_TILES_WIDTH + column));
	}

	// Top Left Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->topLeftDiagonalBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		int crtRow = row - 1;
		int crtColumn = column - 1;

		while (crtRow >= 0 && crtColumn >= 0)
		{
			this->topLeftDiagonalBitMasks[i] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
			--crtRow;
			--crtColumn;
		}
	}

	// Top Right Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->topRightDiagonalBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		int crtRow = row - 1;
		int crtColumn = column + 1;

		while (crtRow >= 0 && crtColumn < GameMetadata::NUM_TILES_WIDTH)
		{
			this->topRightDiagonalBitMasks[i] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
			--crtRow;
			++crtColumn;
		}
	}

	// Bottom Left Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->bottomLeftDiagonalBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		int crtRow = row + 1;
		int crtColumn = column - 1;

		while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn >= 0)
		{
			this->bottomLeftDiagonalBitMasks[i] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
			++crtRow;
			--crtColumn;
		}
	}

	// Bottom Right Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		this->bottomRightDiagonalBitMasks[i] = 0ull;
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		int crtRow = row + 1;
		int crtColumn = column + 1;

		while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn < GameMetadata::NUM_TILES_WIDTH)
		{
			this->bottomRightDiagonalBitMasks[i] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
			++crtRow;
			++crtColumn;
		}
	}

	// Expanded Left Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedLeftBitMasks[i] = (this->leftBitMasks[i] | (1ull << i));

	// Expanded Right Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedRightBitMasks[i] = (this->rightBitMasks[i] | (1ull << i));

	// Expanded Top Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedTopBitMasks[i] = (this->topBitMasks[i] | (1ull << i));

	// Expanded Bottom Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedBottomBitMasks[i] = (this->bottomBitMasks[i] | (1ull << i));

	// Expanded Top Left Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedTopLeftDiagonalBitMasks[i] = (this->topLeftDiagonalBitMasks[i] | (1ull << i));

	// Expanded Top Right Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedTopRightDiagonalBitMasks[i] = (this->topRightDiagonalBitMasks[i] | (1ull << i));

	// Expanded Bottom Left Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedBottomLeftDiagonalBitMasks[i] = (this->bottomLeftDiagonalBitMasks[i] | (1ull << i));

	// Expanded Bottom Right Diagonal Bit Masks
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
		this->expandedBottomRightDiagonalBitMasks[i] = (this->bottomRightDiagonalBitMasks[i] | (1ull << i));

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

	// Precalculated Left-Right Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_WIDTH); ++j) // Masca pentru piesele jucatorului curent
		{
			for (int k = 0; k < (1 << GameMetadata::NUM_TILES_WIDTH); ++k) // Masca pentru piesele adversarului
			{
				this->precalculatedLeftAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));
				this->precalculatedRightAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));

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
						this->precalculatedLeftAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				}
				if (occColumn0 != -1 && (k & (1 << occColumn0)))
				{
					this->precalculatedLeftAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));

					if (occColumn1 != -1 && (k & (1 << occColumn1)))
					{
						this->precalculatedLeftAttackZones[i][j][k].second.first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));
						this->precalculatedLeftAttackZones[i][j][k].second.second |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn1));
					}
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
						this->precalculatedRightAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				}
				if (occColumn0 != -1 && (k & (1 << occColumn0)))
				{
					this->precalculatedRightAttackZones[i][j][k].first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occColumn1 != -1 && (k & (1 << occColumn1)))
					{
						this->precalculatedRightAttackZones[i][j][k].second.first |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn0));
						this->precalculatedRightAttackZones[i][j][k].second.second |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + occColumn1));
					}
				}
			}
		}
	}

	// Precalculated Top-Bottom Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_HEIGHT); ++j) // Masca pentru piesele jucatorului curent
		{
			for (int k = 0; k < (1 << GameMetadata::NUM_TILES_HEIGHT); ++k) // Masca pentru piesele adversarului
			{
				this->precalculatedTopAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));
				this->precalculatedBottomAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));

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
						this->precalculatedTopAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + column));
				}
				if (occRow0 != -1 && (k & (1 << occRow0)))
				{
					this->precalculatedTopAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
					if (occRow1 != -1 && (k & (1 << occRow1)))
					{
						this->precalculatedTopAttackZones[i][j][k].second.first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
						this->precalculatedTopAttackZones[i][j][k].second.second |= (1ull << (occRow1 * GameMetadata::NUM_TILES_WIDTH + column));
					}
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
						this->precalculatedBottomAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + column));
				}
				if (occRow0 != -1 && (k & (1 << occRow0)))
				{
					this->precalculatedBottomAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
					if (occRow1 != -1 && (k & (1 << occRow1)))
					{
						this->precalculatedBottomAttackZones[i][j][k].second.first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + column));
						this->precalculatedBottomAttackZones[i][j][k].second.second |= (1ull << (occRow1 * GameMetadata::NUM_TILES_WIDTH + column));
					}
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
				this->precalculatedTopLeftDiagonalAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));
				this->precalculatedBottomRightDiagonalAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));

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
						this->precalculatedTopLeftDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					--crtRow;
					--crtColumn;
					--crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedTopLeftDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
					{
						this->precalculatedTopLeftDiagonalAttackZones[i][j][k].second.first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
						this->precalculatedTopLeftDiagonalAttackZones[i][j][k].second.second |= (1ull << (occRow1 * GameMetadata::NUM_TILES_WIDTH + occColumn1));
					}
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
						this->precalculatedBottomRightDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					++crtRow;
					++crtColumn;
					++crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedBottomRightDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
					{
						this->precalculatedBottomRightDiagonalAttackZones[i][j][k].second.first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
						this->precalculatedBottomRightDiagonalAttackZones[i][j][k].second.second |= (1ull << (occRow1 * GameMetadata::NUM_TILES_WIDTH + occColumn1));
					}
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
				this->precalculatedTopRightDiagonalAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));
				this->precalculatedBottomLeftDiagonalAttackZones[i][j][k] = std::make_pair(0ull, std::make_pair(0ull, 0ull));

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
						this->precalculatedTopRightDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					--crtRow;
					++crtColumn;
					--crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedTopRightDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
					{
						this->precalculatedTopRightDiagonalAttackZones[i][j][k].second.first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
						this->precalculatedTopRightDiagonalAttackZones[i][j][k].second.second |= (1ull << (occRow1 * GameMetadata::NUM_TILES_WIDTH + occColumn1));
					}
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
						this->precalculatedBottomLeftDiagonalAttackZones[i][j][k].first |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));

					++crtRow;
					--crtColumn;
					++crtPosInDiagonal;
				}
				if (occPos0 != -1 && (k & (1 << occPos0)))
				{
					this->precalculatedBottomLeftDiagonalAttackZones[i][j][k].first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
					if (occPos1 != -1 && (k & (1 << occPos1)))
					{
						this->precalculatedBottomLeftDiagonalAttackZones[i][j][k].second.first |= (1ull << (occRow0 * GameMetadata::NUM_TILES_WIDTH + occColumn0));
						this->precalculatedBottomLeftDiagonalAttackZones[i][j][k].second.second |= (1ull << (occRow1 * GameMetadata::NUM_TILES_WIDTH + occColumn1));
					}
				}
			}
		}
	}


	this->precalculatedEmptyCastleTopLeft = ((1ull << 1) | (1ull << 2) | (1ull << 3));
	this->precalculatedEmptyCastleTopRight = ((1ull << 5) | (1ull << 6));
	this->precalculatedEmptyCastleBottomLeft = ((1ull << 57) | (1ull << 58) | (1ull << 59));
	this->precalculatedEmptyCastleBottomRight = ((1ull << 61) | (1ull << 62));

	this->precalculatedFullCastleTopLeft = ((1ull << 0) | (1ull << 1) | (1ull << 2) | (1ull << 3) | (1ull << 4));
	this->precalculatedFullCastleTopRight = ((1ull << 4) | (1ull << 5) | (1ull << 6) | (1ull << 7));
	this->precalculatedFullCastleBottomLeft = ((1ull << 56) | (1ull << 57) | (1ull << 58) | (1ull << 59) | (1ull << 60));
	this->precalculatedFullCastleBottomRight = ((1ull << 60) | (1ull << 61) | (1ull << 62) | (1ull << 63));
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
		sol |= (1ull << (this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2] / GameMetadata::NUM_TILES_WIDTH - firstRowDiag));
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
		sol |= (1ull << (this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2] / GameMetadata::NUM_TILES_WIDTH - firstRowDiag));
		bitBoard ^= lsb;
	}
	return sol;
}

void BoardManager::initialize()
{
	this->piecesConfiguration = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR1111w0000";

	// TODO: move this
	this->configurationMetadata.whiteTurn = true;
}

void BoardManager::setPiecesConfiguration(const std::string& piecesConfiguration)
{
	this->piecesConfiguration = piecesConfiguration;
	int whiteTurnPos = GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES;
	if (whiteTurnPos < this->piecesConfiguration.size())
		this->configurationMetadata.whiteTurn = (this->piecesConfiguration[whiteTurnPos] == 'w');
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

	this->configurationMetadata.whiteTurn = !this->configurationMetadata.whiteTurn;

	if (this->configurationMetadata.whiteTurn)
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
		if (Game::get().getColor() == Game::Color::WHITE && ((!('A' <= gamePiece && gamePiece <= 'Z')) || (!this->configurationMetadata.whiteTurn)))
			return moves;

		if (Game::get().getColor() == Game::Color::BLACK && ((!('a' <= gamePiece && gamePiece <= 'z')) || this->configurationMetadata.whiteTurn))
			return moves;
	}
	else if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
	{
		if (Client::get().getColor() == "") // Suntem in Multiplayer si nu stim culoarea
			return moves;

		if (Client::get().getColor() == "white" && ((!('A' <= gamePiece && gamePiece <= 'Z')) || (!this->configurationMetadata.whiteTurn)))
			return moves;

		if (Client::get().getColor() == "black" && ((!('a' <= gamePiece && gamePiece <= 'z')) || this->configurationMetadata.whiteTurn))
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

// White Pieces Attack Zones Generation

void BoardManager::generateWhitePawnAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.whitePawnAttackZone = 0ull;
	unsigned long long validWhitePawns = (configurationMetadata.whitePawns & (~this->rankBitMasks[0]));

	// Atac Stanga
	unsigned long long leftAttackZone = (((validWhitePawns & (~this->fileBitMasks[0])) >> (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.allBlackPieces);
	configurationMetadata.whitePawnAttackZone |= leftAttackZone;
	if (leftAttackZone & configurationMetadata.blackKing)
	{
		++configurationMetadata.numPiecesAttackingBlackKing;
		configurationMetadata.blackKingDefenseZone |= (configurationMetadata.blackKing << (GameMetadata::NUM_TILES_WIDTH + 1));
	}

	// Atac Dreapta
	unsigned long long rightAttackZone = (((validWhitePawns & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) >> (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.allBlackPieces);
	configurationMetadata.whitePawnAttackZone |= rightAttackZone;
	if (rightAttackZone & configurationMetadata.blackKing)
	{
		++configurationMetadata.numPiecesAttackingBlackKing;
		configurationMetadata.blackKingDefenseZone |= (configurationMetadata.blackKing << (GameMetadata::NUM_TILES_WIDTH - 1));
	}

	// Atac En Passant (nu trebuie) (zona de atac adaugata de en passant nu este niciodata utila)
}

void BoardManager::generateWhiteRookAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.whiteRookAttackZone = 0ull;
	unsigned long long whiteRooks = configurationMetadata.whiteRooks;

	while (whiteRooks)
	{
		unsigned long long lsb = (whiteRooks & ((~whiteRooks) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Left-Right
		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteRookAttackZone |= (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first | this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first);

		// Top-Bottom
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteRookAttackZone |= (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first | this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first);
		//


		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnRank |= this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first;
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnRank |= this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first;
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnFile |= this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first;
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnFile |= this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first;

		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));
		}

		whiteRooks ^= lsb;
	}
}

void BoardManager::generateWhiteKnightAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.whiteKnightAttackZone = 0ull;
	unsigned long long whiteKnights = configurationMetadata.whiteKnights;

	while (whiteKnights)
	{
		unsigned long long lsb = (whiteKnights & ((~whiteKnights) + 1));

		configurationMetadata.whiteKnightAttackZone |= this->precalculatedKnightAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		if (this->precalculatedKnightAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]] & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone |= lsb;
		}

		whiteKnights ^= lsb;
	}
}

void BoardManager::generateWhiteBishopAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.whiteBishopAttackZone = 0ull;
	unsigned long long whiteBishops = configurationMetadata.whiteBishops;

	while (whiteBishops)
	{
		unsigned long long lsb = (whiteBishops & ((~whiteBishops) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Top Left Bottom Right Diagonal
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteBishopAttackZone |= (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first);

		// Top Right Bottom Left Diagonal
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteBishopAttackZone |= (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first);
		//


		if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first;
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first;
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first;
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first;

		if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));
		}

		whiteBishops ^= lsb;
	}
}

void BoardManager::generateWhiteQueenAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.whiteQueenAttackZone = 0ull;
	unsigned long long whiteQueens = configurationMetadata.whiteQueens;

	while (whiteQueens)
	{
		unsigned long long lsb = (whiteQueens & ((~whiteQueens) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Left-Right
		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first | this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first);

		// Top-Bottom
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first | this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first);

		// Top Left Bottom Right Diagonal
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first);

		// Top Right Bottom Left Diagonal
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first);
		//


		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnRank |= this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first;
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnRank |= this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first;
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnFile |= this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first;
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnFile |= this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first;
		else if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first;
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first;
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first;
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first;

		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));
		}

		whiteQueens ^= lsb;
	}
}

void BoardManager::generateWhiteKingAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.whiteKingAttackZone = 0ull;
	unsigned long long whiteKings = configurationMetadata.whiteKing;

	while (whiteKings)
	{
		unsigned long long lsb = (whiteKings & ((~whiteKings) + 1));

		configurationMetadata.whiteKingAttackZone |= this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		if (this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]] & configurationMetadata.blackKing)
		{
			++configurationMetadata.numPiecesAttackingBlackKing;
			configurationMetadata.blackKingDefenseZone |= lsb;
		}

		whiteKings ^= lsb;
	}
}

// Black Pieces Attack Zones Generation

void BoardManager::generateBlackPawnAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.blackPawnAttackZone = 0ull;
	unsigned long long validBlackPawns = (configurationMetadata.blackPawns & (~this->rankBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH - 1]));

	// Atac Stanga
	unsigned long long leftAttackZone = (((validBlackPawns & (~this->fileBitMasks[0])) << (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.allWhitePieces);
	configurationMetadata.blackPawnAttackZone |= leftAttackZone;
	if (leftAttackZone & configurationMetadata.whiteKing)
	{
		++configurationMetadata.numPiecesAttackingWhiteKing;
		configurationMetadata.whiteKingDefenseZone |= (configurationMetadata.whiteKing >> (GameMetadata::NUM_TILES_WIDTH + 1));
	}

	// Atac Dreapta
	unsigned long long rightAttackZone = (((validBlackPawns & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) << (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.allWhitePieces);
	configurationMetadata.blackPawnAttackZone |= rightAttackZone;
	if (rightAttackZone & configurationMetadata.whiteKing)
	{
		++configurationMetadata.numPiecesAttackingWhiteKing;
		configurationMetadata.whiteKingDefenseZone |= (configurationMetadata.whiteKing >> (GameMetadata::NUM_TILES_WIDTH - 1));
	}

	// Atac En Passant (nu trebuie) (zona de atac adaugata de en passant nu este niciodata utila)
}

void BoardManager::generateBlackRookAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.blackAttackZones = 0ull;
	unsigned long long blackRooks = configurationMetadata.blackRooks;

	while (blackRooks)
	{
		unsigned long long lsb = (blackRooks & ((~blackRooks) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Left-Right
		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackRookAttackZone |= (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first | this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first);

		// Top-Bottom
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackRookAttackZone |= (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first | this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first);
		//


		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnRank |= this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first;
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnRank |= this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first;
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnFile |= this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first;
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnFile |= this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first;

		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));
		}

		blackRooks ^= lsb;
	}
}

void BoardManager::generateBlackKnightAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.blackKnightAttackZone = 0ull;
	unsigned long long blackKnights = configurationMetadata.blackKnights;

	while (blackKnights)
	{
		unsigned long long lsb = (blackKnights & ((~blackKnights) + 1));

		configurationMetadata.blackKnightAttackZone |= this->precalculatedKnightAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		if (this->precalculatedKnightAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]] & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone |= lsb;
		}

		blackKnights ^= lsb;
	}
}

void BoardManager::generateBlackBishopAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.blackBishopAttackZone = 0ull;
	unsigned long long blackBishops = configurationMetadata.blackBishops;

	while (blackBishops)
	{
		unsigned long long lsb = (blackBishops & ((~blackBishops) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Top Left Bottom Right Diagonal
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackBishopAttackZone |= (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first);

		// Top Right Bottom Left Diagonal
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackBishopAttackZone |= (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first);
		//


		if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first;
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first;
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first;
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first;

		if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));
		}

		blackBishops ^= lsb;
	}
}

void BoardManager::generateBlackQueenAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.blackQueenAttackZone = 0ull;
	unsigned long long blackQueens = configurationMetadata.blackQueens;

	while (blackQueens)
	{
		unsigned long long lsb = (blackQueens & ((~blackQueens) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Left-Right
		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first | this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first);

		// Top-Bottom
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first | this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first);

		// Top Left Bottom Right Diagonal
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first);

		// Top Right Bottom Left Diagonal
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first);
		//


		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnRank |= this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first;
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnRank |= this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first;
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnFile |= this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first;
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnFile |= this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first;
		else if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first;
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first;
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first;
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first;

		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));
		}

		blackQueens ^= lsb;
	}
}

void BoardManager::generateBlackKingAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.blackKingAttackZone = 0ull;
	unsigned long long blackKings = configurationMetadata.blackKing;

	while (blackKings)
	{
		unsigned long long lsb = (blackKings & ((~blackKings) + 1));

		configurationMetadata.blackKingAttackZone |= this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		if (this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]] & configurationMetadata.whiteKing)
		{
			++configurationMetadata.numPiecesAttackingWhiteKing;
			configurationMetadata.whiteKingDefenseZone |= lsb;
		}

		blackKings ^= lsb;
	}
}

// Attack Zones Generation

void BoardManager::generateWhiteAttackZones(ConfigurationMetadata& configurationMetadata)
{
	generateWhitePawnAttackZone(configurationMetadata);
	generateWhiteRookAttackZone(configurationMetadata);
	generateWhiteKnightAttackZone(configurationMetadata);
	generateWhiteBishopAttackZone(configurationMetadata);
	generateWhiteQueenAttackZone(configurationMetadata);
	generateWhiteKingAttackZone(configurationMetadata);

	configurationMetadata.whiteAttackZones =
		configurationMetadata.whitePawnAttackZone |
		configurationMetadata.whiteRookAttackZone |
		configurationMetadata.whiteKnightAttackZone |
		configurationMetadata.whiteBishopAttackZone |
		configurationMetadata.whiteQueenAttackZone |
		configurationMetadata.whiteKingAttackZone;
}

void BoardManager::generateBlackAttackZones(ConfigurationMetadata& configurationMetadata)
{
	generateBlackPawnAttackZone(configurationMetadata);
	generateBlackRookAttackZone(configurationMetadata);
	generateBlackKnightAttackZone(configurationMetadata);
	generateBlackBishopAttackZone(configurationMetadata);
	generateBlackQueenAttackZone(configurationMetadata);
	generateBlackKingAttackZone(configurationMetadata);

	configurationMetadata.blackAttackZones =
		configurationMetadata.blackPawnAttackZone |
		configurationMetadata.blackRookAttackZone |
		configurationMetadata.blackKnightAttackZone |
		configurationMetadata.blackBishopAttackZone |
		configurationMetadata.blackQueenAttackZone |
		configurationMetadata.blackKingAttackZone;
}

// White Pieces Moves Generation

void BoardManager::generateWhitePawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateWhiteRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateWhiteKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateWhiteBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateWhiteQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateWhiteKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

// Black Pieces Moves Generation

void BoardManager::generateBlackPawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateBlackRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateBlackKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateBlackBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateBlackQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

void BoardManager::generateBlackKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	// TODO:
}

// All Pieces Moves Generation

void BoardManager::generateWhiteMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	configurationMetadata.whitePiecesPinnedOnRank = 0ull;
	configurationMetadata.whitePiecesPinnedOnFile = 0ull;
	configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal = 0ull;
	configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal = 0ull;

	configurationMetadata.numPiecesAttackingWhiteKing = 0;
	configurationMetadata.whiteKingDefenseZone = (~(0ull));


	generateBlackAttackZones(configurationMetadata);

	generateWhitePawnMoves(configurationMetadata, moves);
	generateWhiteRookMoves(configurationMetadata, moves);
	generateWhiteKnightMoves(configurationMetadata, moves);
	generateWhiteBishopMoves(configurationMetadata, moves);
	generateWhiteQueenMoves(configurationMetadata, moves);
	generateWhiteKingMoves(configurationMetadata, moves);
}

void BoardManager::generateBlackMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves)
{
	configurationMetadata.blackPiecesPinnedOnRank = 0ull;
	configurationMetadata.blackPiecesPinnedOnFile = 0ull;
	configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal = 0ull;
	configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal = 0ull;

	configurationMetadata.numPiecesAttackingBlackKing = 0;
	configurationMetadata.blackKingDefenseZone = (~(0ull));

	generateWhiteAttackZones(configurationMetadata);

	generateBlackPawnMoves(configurationMetadata, moves);
	generateBlackRookMoves(configurationMetadata, moves);
	generateBlackKnightMoves(configurationMetadata, moves);
	generateBlackBishopMoves(configurationMetadata, moves);
	generateBlackQueenMoves(configurationMetadata, moves);
	generateBlackKingMoves(configurationMetadata, moves);
}

//

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



