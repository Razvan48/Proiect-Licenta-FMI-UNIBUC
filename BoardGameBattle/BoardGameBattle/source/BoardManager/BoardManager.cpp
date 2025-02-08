#include "BoardManager.h"

#include "../GameMetadata/GameMetadata.h"

#include "../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"

#include "../Game/Game.h"

#include "../Client/Client.h"

#include <iostream>

BoardManager::BoardManager()
	: configurationMetadata("rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR100000000")
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

	// Precalculated Nearest Pieces On Rank
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_WIDTH); ++j)
		{
			this->precalculatedNearestPiecesOnRank[i][j] = std::make_pair(0ull, 0ull);

			if ((j & (1 << column)) == 0)
				continue;

			for (int k = column - 1; k >= 0; --k)
			{
				if (j & (1 << k))
				{
					this->precalculatedNearestPiecesOnRank[i][j].first = (1ull << (row * GameMetadata::NUM_TILES_WIDTH + k));
					break;
				}
			}
			for (int k = column + 1; k < GameMetadata::NUM_TILES_WIDTH; ++k)
			{
				if (j & (1 << k))
				{
					this->precalculatedNearestPiecesOnRank[i][j].second = (1ull << (row * GameMetadata::NUM_TILES_WIDTH + k));
					break;
				}
			}
		}
	}

	// Precalculated Nearest Pieces On Top Left Bottom Right Diagonal
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, GameMetadata::NUM_TILES_WIDTH - 1 - column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[i][j] = std::make_pair(0ull, 0ull);

			if ((j & (1 << posInDiagonal)) == 0)
				continue;

			int crtRow = row - 1;
			int crtColumn = column - 1;
			int crtPosInDiagonal = posInDiagonal - 1;
			while (crtRow >= 0 && crtColumn >= 0)
			{
				if (j & (1 << crtPosInDiagonal))
				{
					this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[i][j].first = (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
					break;
				}
				--crtRow;
				--crtColumn;
				--crtPosInDiagonal;
			}

			crtRow = row + 1;
			crtColumn = column + 1;
			crtPosInDiagonal = posInDiagonal + 1;
			while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn < GameMetadata::NUM_TILES_WIDTH)
			{
				if (j & (1 << crtPosInDiagonal))
				{
					this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[i][j].second = (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
					break;
				}
				++crtRow;
				++crtColumn;
				++crtPosInDiagonal;
			}
		}
	}

	// Precalculated Nearest Pieces On Top Right Bottom Left Diagonal
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, GameMetadata::NUM_TILES_WIDTH - 1 - column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[i][j] = std::make_pair(0ull, 0ull);

			if ((j & (1 << posInDiagonal)) == 0)
				continue;

			int crtRow = row - 1;
			int crtColumn = column + 1;
			int crtPosInDiagonal = posInDiagonal - 1;
			while (crtRow >= 0 && crtColumn < GameMetadata::NUM_TILES_WIDTH)
			{
				if (j & (1 << crtPosInDiagonal))
				{
					this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[i][j].first = (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
					break;
				}
				--crtRow;
				++crtColumn;
				--crtPosInDiagonal;
			}

			crtRow = row + 1;
			crtColumn = column - 1;
			crtPosInDiagonal = posInDiagonal + 1;
			while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn >= 0)
			{
				if (j & (1 << crtPosInDiagonal))
				{
					this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[i][j].second = (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
					break;
				}
				++crtRow;
				--crtColumn;
				++crtPosInDiagonal;
			}
		}
	}

	// Precalculated Raw Left Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_WIDTH); ++j)
		{
			this->precalculatedRawLeftAttackZones[i][j] = 0ull;

			if ((j & (1 << column)) == 0)
				continue;

			for (int k = column - 1; k >= 0; --k)
			{
				this->precalculatedRawLeftAttackZones[i][j] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + k));
				if (j & (1 << k))
					break;
			}
		}
	}

	// Precalculated Raw Right Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_WIDTH); ++j)
		{
			this->precalculatedRawRightAttackZones[i][j] = 0ull;

			if ((j & (1 << column)) == 0)
				continue;

			for (int k = column + 1; k < GameMetadata::NUM_TILES_WIDTH; ++k)
			{
				this->precalculatedRawRightAttackZones[i][j] |= (1ull << (row * GameMetadata::NUM_TILES_WIDTH + k));
				if (j & (1 << k))
					break;
			}
		}
	}

	// Precalculated Raw Top Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_HEIGHT); ++j)
		{
			this->precalculatedRawTopAttackZones[i][j] = 0ull;

			if ((j & (1 << row)) == 0)
				continue;

			for (int k = row - 1; k >= 0; --k)
			{
				this->precalculatedRawTopAttackZones[i][j] |= (1ull << (k * GameMetadata::NUM_TILES_WIDTH + column));
				if (j & (1 << k))
					break;
			}
		}
	}

	// Precalculated Raw Bottom Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;

		for (int j = 0; j < (1 << GameMetadata::NUM_TILES_HEIGHT); ++j)
		{
			this->precalculatedRawBottomAttackZones[i][j] = 0ull;

			if ((j & (1 << row)) == 0)
				continue;

			for (int k = row + 1; k < GameMetadata::NUM_TILES_HEIGHT; ++k)
			{
				this->precalculatedRawBottomAttackZones[i][j] |= (1ull << (k * GameMetadata::NUM_TILES_WIDTH + column));
				if (j & (1 << k))
					break;
			}
		}
	}

	// Precalculated Raw Top Left Diagonal Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, GameMetadata::NUM_TILES_WIDTH - 1 - column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			this->precalculatedRawTopLeftDiagonalAttackZones[i][j] = 0ull;

			if ((j & (1 << posInDiagonal)) == 0)
				continue;

			int crtRow = row - 1;
			int crtColumn = column - 1;
			int crtPosInDiagonal = posInDiagonal - 1;
			while (crtRow >= 0 && crtColumn >= 0)
			{
				this->precalculatedRawTopLeftDiagonalAttackZones[i][j] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				if (j & (1 << crtPosInDiagonal))
					break;
				--crtRow;
				--crtColumn;
				--crtPosInDiagonal;
			}
		}
	}

	// Precalculated Raw Top Right Diagonal Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, GameMetadata::NUM_TILES_WIDTH - 1 - column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			this->precalculatedRawTopRightDiagonalAttackZones[i][j] = 0ull;

			if ((j & (1 << posInDiagonal)) == 0)
				continue;

			int crtRow = row - 1;
			int crtColumn = column + 1;
			int crtPosInDiagonal = posInDiagonal - 1;
			while (crtRow >= 0 && crtColumn < GameMetadata::NUM_TILES_WIDTH)
			{
				this->precalculatedRawTopRightDiagonalAttackZones[i][j] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				if (j & (1 << crtPosInDiagonal))
					break;
				--crtRow;
				++crtColumn;
				--crtPosInDiagonal;
			}
		}
	}

	// Precalculated Raw Bottom Left Diagonal Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, GameMetadata::NUM_TILES_WIDTH - 1 - column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			this->precalculatedRawBottomLeftDiagonalAttackZones[i][j] = 0ull;

			if ((j & (1 << posInDiagonal)) == 0)
				continue;

			int crtRow = row + 1;
			int crtColumn = column - 1;
			int crtPosInDiagonal = posInDiagonal + 1;
			while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn >= 0)
			{
				this->precalculatedRawBottomLeftDiagonalAttackZones[i][j] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				if (j & (1 << crtPosInDiagonal))
					break;
				++crtRow;
				--crtColumn;
				++crtPosInDiagonal;
			}
		}
	}

	// Precalculated Raw Bottom Right Diagonal Attack Zones
	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		int row = i / GameMetadata::NUM_TILES_WIDTH;
		int column = i % GameMetadata::NUM_TILES_WIDTH;
		int posInDiagonal = min(row, column);
		int diagonalLength = posInDiagonal + 1 + min(GameMetadata::NUM_TILES_HEIGHT - 1 - row, GameMetadata::NUM_TILES_WIDTH - 1 - column);

		for (int j = 0; j < (1 << diagonalLength); ++j)
		{
			this->precalculatedRawBottomRightDiagonalAttackZones[i][j] = 0ull;

			if ((j & (1 << posInDiagonal)) == 0)
				continue;

			int crtRow = row + 1;
			int crtColumn = column + 1;
			int crtPosInDiagonal = posInDiagonal + 1;
			while (crtRow < GameMetadata::NUM_TILES_HEIGHT && crtColumn < GameMetadata::NUM_TILES_WIDTH)
			{
				this->precalculatedRawBottomRightDiagonalAttackZones[i][j] |= (1ull << (crtRow * GameMetadata::NUM_TILES_WIDTH + crtColumn));
				if (j & (1 << crtPosInDiagonal))
					break;
				++crtRow;
				++crtColumn;
				++crtPosInDiagonal;
			}
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

	this->whiteKingPos = 60;
	this->whiteRookBottomLeftPos = 56;
	this->whiteRookBottomRightPos = 63;

	this->blackKingPos = 4;
	this->blackRookTopLeftPos = 0;
	this->blackRookTopRightPos = 7;
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
	this->configurationMetadata.initialize("rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR100000000");

	// Curatam Istoricul de la Meciul Anterior
	this->configurationMetadataHistory.clear();
}

BoardManager& BoardManager::get()
{
	static BoardManager instance;
	return instance;
}

std::string BoardManager::getPiecesConfiguration() const
{
	std::string piecesConfiguration = "";

	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		if (this->configurationMetadata.whitePawns & (1ull << i))
			piecesConfiguration.push_back('P');
		else if (this->configurationMetadata.whiteRooks & (1ull << i))
			piecesConfiguration.push_back('R');
		else if (this->configurationMetadata.whiteKnights & (1ull << i))
			piecesConfiguration.push_back('N');
		else if (this->configurationMetadata.whiteBishops & (1ull << i))
			piecesConfiguration.push_back('B');
		else if (this->configurationMetadata.whiteQueens & (1ull << i))
			piecesConfiguration.push_back('Q');
		else if (this->configurationMetadata.whiteKing & (1ull << i))
			piecesConfiguration.push_back('K');
		else if (this->configurationMetadata.blackPawns & (1ull << i))
			piecesConfiguration.push_back('p');
		else if (this->configurationMetadata.blackRooks & (1ull << i))
			piecesConfiguration.push_back('r');
		else if (this->configurationMetadata.blackKnights & (1ull << i))
			piecesConfiguration.push_back('n');
		else if (this->configurationMetadata.blackBishops & (1ull << i))
			piecesConfiguration.push_back('b');
		else if (this->configurationMetadata.blackQueens & (1ull << i))
			piecesConfiguration.push_back('q');
		else if (this->configurationMetadata.blackKing & (1ull << i))
			piecesConfiguration.push_back('k');
		else
			piecesConfiguration.push_back('.');
	}

	piecesConfiguration.push_back(('0' + (int)this->configurationMetadata.whiteTurn));

	if (this->configurationMetadata.capturableEnPassantPosition <= 9)
	{
		piecesConfiguration.push_back('0');
		piecesConfiguration.push_back('0' + this->configurationMetadata.capturableEnPassantPosition);
	}
	else
	{
		piecesConfiguration.push_back('0' + this->configurationMetadata.capturableEnPassantPosition / 10);
		piecesConfiguration.push_back('0' + this->configurationMetadata.capturableEnPassantPosition % 10);
	}

	piecesConfiguration.push_back('0' + this->configurationMetadata.whiteKingMoved);
	piecesConfiguration.push_back('0' + this->configurationMetadata.whiteRookBottomLeftMoved);
	piecesConfiguration.push_back('0' + this->configurationMetadata.whiteRookBottomRightMoved);

	piecesConfiguration.push_back('0' + this->configurationMetadata.blackKingMoved);
	piecesConfiguration.push_back('0' + this->configurationMetadata.blackRookTopLeftMoved);
	piecesConfiguration.push_back('0' + this->configurationMetadata.blackRookTopRightMoved);

	return piecesConfiguration;
}

std::string BoardManager::convertToExternalMove(const std::vector<std::pair<char, int>>& internalMove) const
{
	if (internalMove.empty())
	{
		std::cout << "Error : Conversion to external move failed, internalMove is empty" << std::endl;
		return "";
	}
	if (internalMove.size() < 2)
	{
		std::cout << "Error : Conversion to external move failed, internalMove has less than 2 elements" << std::endl;
		return "";
	}

	std::string externalMove = "";
	externalMove.push_back(internalMove[0].first);
	for (int i = 0; i < 2; ++i)
	{
		int row = internalMove[i].second / GameMetadata::NUM_TILES_WIDTH;
		int column = internalMove[i].second % GameMetadata::NUM_TILES_WIDTH;

		externalMove.push_back((char)('a' + column));
		externalMove.push_back((char)('1' + GameMetadata::NUM_TILES_HEIGHT - 1 - row));
	}

	return externalMove;
}

ConfigurationMetadata BoardManager::applyMoveInternal(const ConfigurationMetadata& configurationMetadata, const std::vector<std::pair<char, int>>& internalMove)
{
	// INFO: Face presupunerea ca mutarea e legala (si apartine culorii care trebuie sa mute) (din motive de optimizare)

	ConfigurationMetadata newConfigurationMetadata(configurationMetadata);

	newConfigurationMetadata.capturableEnPassantPosition = 0; // INFO: 0 inseamna ca nu exista o astfel de pozitie (teoretic 0 exista pe tabla, practic nu se poate face un enpassant in 0, deci e ok)

	for (int i = 0; i < internalMove.size(); ++i)
	{
		if (internalMove[i].first == 'P')
		{
			if (internalMove[i].second / GameMetadata::NUM_TILES_WIDTH == 4 && i >= 1 && internalMove[i - 1].first == 'P' && internalMove[i - 1].second / GameMetadata::NUM_TILES_WIDTH == 6)
				newConfigurationMetadata.capturableEnPassantPosition = internalMove[i].second;

			newConfigurationMetadata.whitePawns ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'R')
		{
			if (!newConfigurationMetadata.whiteRookBottomLeftMoved && internalMove[i].second == this->whiteRookBottomLeftPos)
				newConfigurationMetadata.whiteRookBottomLeftMoved = true;
			else if (!newConfigurationMetadata.whiteRookBottomRightMoved && internalMove[i].second == this->whiteRookBottomRightPos)
				newConfigurationMetadata.whiteRookBottomRightMoved = true;

			newConfigurationMetadata.whiteRooks ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'N')
		{
			newConfigurationMetadata.whiteKnights ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'B')
		{
			newConfigurationMetadata.whiteBishops ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'Q')
		{
			newConfigurationMetadata.whiteQueens ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'K')
		{
			newConfigurationMetadata.whiteKingMoved = true; // INFO: e simplificat, dar merge

			newConfigurationMetadata.whiteKing ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'p')
		{
			if (internalMove[i].second / GameMetadata::NUM_TILES_WIDTH == 3 && i >= 1 && internalMove[i - 1].first == 'p' && internalMove[i - 1].second / GameMetadata::NUM_TILES_WIDTH == 1)
				newConfigurationMetadata.capturableEnPassantPosition = internalMove[i].second;

			newConfigurationMetadata.blackPawns ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'r')
		{
			if (!newConfigurationMetadata.blackRookTopLeftMoved && internalMove[i].second == this->blackRookTopLeftPos)
				newConfigurationMetadata.blackRookTopLeftMoved = true;
			else if (!newConfigurationMetadata.blackRookTopRightMoved && internalMove[i].second == this->blackRookTopRightPos)
				newConfigurationMetadata.blackRookTopRightMoved = true;

			newConfigurationMetadata.blackRooks ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'n')
		{
			newConfigurationMetadata.blackKnights ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'b')
		{
			newConfigurationMetadata.blackBishops ^= (1ull << internalMove[i].second);
		}
		else if (internalMove[i].first == 'q')
		{
			newConfigurationMetadata.blackQueens ^= (1ull << internalMove[i].second);
		}
		else // Black King (k)
		{
			newConfigurationMetadata.blackKingMoved = true; // INFO: e simplificat, dar merge

			newConfigurationMetadata.blackKing ^= (1ull << internalMove[i].second);
		}
	}

	if (newConfigurationMetadata.whiteTurn)
	{
		newConfigurationMetadata.allWhitePieces = newConfigurationMetadata.whitePawns | newConfigurationMetadata.whiteRooks | newConfigurationMetadata.whiteKnights | newConfigurationMetadata.whiteBishops | newConfigurationMetadata.whiteQueens | newConfigurationMetadata.whiteKing;

		// INFO: Se elimina piesele capturate

		newConfigurationMetadata.blackPawns ^= (newConfigurationMetadata.blackPawns & newConfigurationMetadata.allWhitePieces);
		newConfigurationMetadata.blackRooks ^= (newConfigurationMetadata.blackRooks & newConfigurationMetadata.allWhitePieces);
		newConfigurationMetadata.blackKnights ^= (newConfigurationMetadata.blackKnights & newConfigurationMetadata.allWhitePieces);
		newConfigurationMetadata.blackBishops ^= (newConfigurationMetadata.blackBishops & newConfigurationMetadata.allWhitePieces);
		newConfigurationMetadata.blackQueens ^= (newConfigurationMetadata.blackQueens & newConfigurationMetadata.allWhitePieces);
		newConfigurationMetadata.blackKing ^= (newConfigurationMetadata.blackKing & newConfigurationMetadata.allWhitePieces);

		newConfigurationMetadata.allBlackPieces = newConfigurationMetadata.blackPawns | newConfigurationMetadata.blackRooks | newConfigurationMetadata.blackKnights | newConfigurationMetadata.blackBishops | newConfigurationMetadata.blackQueens | newConfigurationMetadata.blackKing;
	}
	else
	{
		newConfigurationMetadata.allBlackPieces = newConfigurationMetadata.blackPawns | newConfigurationMetadata.blackRooks | newConfigurationMetadata.blackKnights | newConfigurationMetadata.blackBishops | newConfigurationMetadata.blackQueens | newConfigurationMetadata.blackKing;

		// INFO: Se elimina piesele capturate

		newConfigurationMetadata.whitePawns ^= (newConfigurationMetadata.whitePawns & newConfigurationMetadata.allBlackPieces);
		newConfigurationMetadata.whiteRooks ^= (newConfigurationMetadata.whiteRooks & newConfigurationMetadata.allBlackPieces);
		newConfigurationMetadata.whiteKnights ^= (newConfigurationMetadata.whiteKnights & newConfigurationMetadata.allBlackPieces);
		newConfigurationMetadata.whiteBishops ^= (newConfigurationMetadata.whiteBishops & newConfigurationMetadata.allBlackPieces);
		newConfigurationMetadata.whiteQueens ^= (newConfigurationMetadata.whiteQueens & newConfigurationMetadata.allBlackPieces);
		newConfigurationMetadata.whiteKing ^= (newConfigurationMetadata.whiteKing & newConfigurationMetadata.allBlackPieces);

		newConfigurationMetadata.allWhitePieces = newConfigurationMetadata.whitePawns | newConfigurationMetadata.whiteRooks | newConfigurationMetadata.whiteKnights | newConfigurationMetadata.whiteBishops | newConfigurationMetadata.whiteQueens | newConfigurationMetadata.whiteKing;
	}


	newConfigurationMetadata.allPieces = newConfigurationMetadata.allWhitePieces | newConfigurationMetadata.allBlackPieces;

	newConfigurationMetadata.emptyTiles = (~newConfigurationMetadata.allPieces);

	// 

	newConfigurationMetadata.whiteTurn = !newConfigurationMetadata.whiteTurn;

	return newConfigurationMetadata;
}

std::vector<std::pair<char, int>> BoardManager::convertToInternalMove(const ConfigurationMetadata& configurationMetadata, const std::string& externalMove) const
{
	// INFO: Ne permitem sa facem metoda aceasta mai ineficienta, deoarece bottleneck-ul in acest caz este interactiunea umana.

	if (externalMove.empty())
	{
		std::cout << "Error : Conversion to internal move failed, externalMove is empty" << std::endl;
		return std::vector<std::pair<char, int>>();
	}
	if (externalMove.size() < 5)
	{
		std::cout << "Error : Conversion to internal move failed, externalMove has less than 5 characters" << std::endl;
		return std::vector<std::pair<char, int>>();
	}

	std::vector<std::pair<char, int>> internalMove;

	char piece = externalMove[0];
	int pos0 = (GameMetadata::NUM_TILES_HEIGHT - 1 - (externalMove[2] - '1')) * GameMetadata::NUM_TILES_WIDTH + (externalMove[1] - 'a');
	int pos1 = (GameMetadata::NUM_TILES_HEIGHT - 1 - (externalMove[4] - '1')) * GameMetadata::NUM_TILES_WIDTH + (externalMove[3] - 'a');

	internalMove.emplace_back(piece, pos0);
	internalMove.emplace_back(piece, pos1);

	// En Passant
	if (piece == 'P' && pos0 % GameMetadata::NUM_TILES_WIDTH != pos1 % GameMetadata::NUM_TILES_WIDTH && configurationMetadata.capturableEnPassantPosition == pos1 + GameMetadata::NUM_TILES_WIDTH)
	{
		internalMove.emplace_back('p', configurationMetadata.capturableEnPassantPosition);
	}
	else if (piece == 'p' && pos0 % GameMetadata::NUM_TILES_WIDTH != pos1 % GameMetadata::NUM_TILES_WIDTH && configurationMetadata.capturableEnPassantPosition == pos1 - GameMetadata::NUM_TILES_WIDTH)
	{
		internalMove.emplace_back('P', configurationMetadata.capturableEnPassantPosition);
	}

	// Rocadele
	if (piece == 'K' && pos0 == this->whiteKingPos)
	{
		if (pos1 == this->whiteKingPos - 2)
		{
			internalMove.emplace_back('R', this->whiteRookBottomLeftPos);
			internalMove.emplace_back('R', this->whiteKingPos - 1);
		}
		else if (pos1 == this->whiteKingPos + 2)
		{
			internalMove.emplace_back('R', this->whiteRookBottomRightPos);
			internalMove.emplace_back('R', this->whiteKingPos + 1);
		}
	}
	else if (piece == 'k' && pos0 == this->blackKingPos)
	{
		if (pos1 == this->blackKingPos - 2)
		{
			internalMove.emplace_back('r', this->blackRookTopLeftPos);
			internalMove.emplace_back('r', this->blackKingPos - 1);
		}
		else if (pos1 == this->blackKingPos + 2)
		{
			internalMove.emplace_back('r', this->blackRookTopRightPos);
			internalMove.emplace_back('r', this->blackKingPos + 1);
		}
	}

	// Promovare Pion
	if (piece == 'P' && pos1 / GameMetadata::NUM_TILES_WIDTH == 0)
	{
		internalMove.pop_back();

		if (externalMove.size() < 6)
		{
			std::cout << "Info : Conversion to internal move failed, externalMove is a pawn promotion but does not specify the promotion piece, using Queen as default" << std::endl;
			internalMove.emplace_back('Q', pos1);
		}
		else
			internalMove.emplace_back(externalMove[5], pos1);
	}
	else if (piece == 'p' && pos1 / GameMetadata::NUM_TILES_WIDTH == GameMetadata::NUM_TILES_HEIGHT - 1)
	{
		internalMove.pop_back();

		if (externalMove.size() < 6)
		{
			std::cout << "Info : Conversion to internal move failed, externalMove is a pawn promotion but does not specify the promotion piece, using Queen as default" << std::endl;
			internalMove.emplace_back('q', pos1);
		}
		else
			internalMove.emplace_back(externalMove[5], pos1);
	}

	return internalMove;
}

void BoardManager::applyMoveExternal(const std::string& externalMove)
{
	this->configurationMetadata.initialize(this->applyMoveInternal(this->configurationMetadata, this->convertToInternalMove(this->configurationMetadata, externalMove)));

	//

	if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
		CreatedMultiplayerGameVisualInterface::get()->setHasToSendBoardConfiguration(true);
	else if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
		JoinedMultiplayerGameVisualInterface::get()->setHasToSendBoardConfiguration(true);
	// else Singleplayer, nu trebuie sa trimita nimic
}

std::vector<std::string> BoardManager::generateMovesForPiecePosition(const std::string& piecePosition)
{
	// INFO: piecePosition are exact 2 char-uri.

	if (Game::get().getMode() == Game::Mode::NONE)
	{
		std::cout << "Error: Game Mode not set when generating moves for piece position" << std::endl;
		return std::vector<std::string>();
	}
	else if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
	{
		if (Game::get().getColor() == Game::Color::WHITE && !this->configurationMetadata.whiteTurn)
			return std::vector<std::string>();
		else if (Game::get().getColor() == Game::Color::BLACK && this->configurationMetadata.whiteTurn)
			return std::vector<std::string>();
	}
	else if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
	{
		if (Client::get().getColor() == "")
			return std::vector<std::string>();
		else if (Client::get().getColor() == "white" && !this->configurationMetadata.whiteTurn)
			return std::vector<std::string>();
		else if (Client::get().getColor() == "black" && this->configurationMetadata.whiteTurn)
			return std::vector<std::string>();
	}

	int row = GameMetadata::NUM_TILES_HEIGHT - 1 - (piecePosition[1] - '1');
	int column = piecePosition[0] - 'a';
	std::string piecesConfiguration = this->getPiecesConfiguration();
	char gamePiece = piecesConfiguration[row * GameMetadata::NUM_TILES_WIDTH + column];

	if (gamePiece == '.')
		return std::vector<std::string>();



	std::vector<std::vector<std::pair<char, int>>> internalMoves;
	if (this->configurationMetadata.whiteTurn)
		this->generateWhiteMoves(this->configurationMetadata, internalMoves);
	else
		this->generateBlackMoves(this->configurationMetadata, internalMoves);


	std::vector<std::string> externalMoves;
	for (int i = 0; i < internalMoves.size(); ++i)
	{
		std::string externalMove = this->convertToExternalMove(internalMoves[i]);

		if (externalMove[1] == piecePosition[0] && externalMove[2] == piecePosition[1])
			externalMoves.push_back(externalMove);
	}

	return externalMoves;
}

// White Pieces Attack Zones Generation

void BoardManager::generateWhitePawnAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.whitePawnAttackZone = 0ull;
	unsigned long long whitePawns = (configurationMetadata.whitePawns & (~this->rankBitMasks[0]));

	// Atac Stanga
	unsigned long long leftAttackZone = ((whitePawns & (~this->fileBitMasks[0])) >> (GameMetadata::NUM_TILES_WIDTH + 1));
	configurationMetadata.whitePawnAttackZone |= leftAttackZone;
	if (leftAttackZone & configurationMetadata.blackKing)
	{
		configurationMetadata.blackKingDefenseZone &= (configurationMetadata.blackKing << (GameMetadata::NUM_TILES_WIDTH + 1));
	}

	// Atac Dreapta
	unsigned long long rightAttackZone = ((whitePawns & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) >> (GameMetadata::NUM_TILES_WIDTH - 1));
	configurationMetadata.whitePawnAttackZone |= rightAttackZone;
	if (rightAttackZone & configurationMetadata.blackKing)
	{
		configurationMetadata.blackKingDefenseZone &= (configurationMetadata.blackKing << (GameMetadata::NUM_TILES_WIDTH - 1));
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

		configurationMetadata.whiteRookAttackZone |= (this->precalculatedRawLeftAttackZones[pos][whitePiecesSameRank | blackPiecesSameRank] | this->precalculatedRawRightAttackZones[pos][whitePiecesSameRank | blackPiecesSameRank]);

		// Top-Bottom
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteRookAttackZone |= (this->precalculatedRawTopAttackZones[pos][whitePiecesSameFile | blackPiecesSameFile] | this->precalculatedRawBottomAttackZones[pos][whitePiecesSameFile | blackPiecesSameFile]);
		//


		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnRank |= this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first;
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnRank |= this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].second.first;
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnFile |= this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first;
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnFile |= this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].second.first;

		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteRookAttackZone |= this->precalculatedRawLeftAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameRank | blackPiecesSameRank];
		}
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteRookAttackZone |= this->precalculatedRawRightAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameRank | blackPiecesSameRank];
		}
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteRookAttackZone |= this->precalculatedRawTopAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameFile | blackPiecesSameFile];
		}
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteRookAttackZone |= this->precalculatedRawBottomAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameFile | blackPiecesSameFile];
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
			configurationMetadata.blackKingDefenseZone &= lsb;
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

		configurationMetadata.whiteBishopAttackZone |= (this->precalculatedRawTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0] | this->precalculatedRawBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0]);

		// Top Right Bottom Left Diagonal
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteBishopAttackZone |= (this->precalculatedRawTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1] | this->precalculatedRawBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1]);
		//


		if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first;
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].second.first;
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first;
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.second & configurationMetadata.blackKing)
			this->configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].second.first;

		if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteBishopAttackZone |= this->precalculatedRawTopLeftDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0];
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteBishopAttackZone |= this->precalculatedRawBottomRightDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0];
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteBishopAttackZone |= this->precalculatedRawTopRightDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1];
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteBishopAttackZone |= this->precalculatedRawBottomLeftDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1];
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

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedRawLeftAttackZones[pos][whitePiecesSameRank | blackPiecesSameRank] | this->precalculatedRawRightAttackZones[pos][whitePiecesSameRank | blackPiecesSameRank]);

		// Top-Bottom
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedRawTopAttackZones[pos][whitePiecesSameFile | blackPiecesSameFile] | this->precalculatedRawBottomAttackZones[pos][whitePiecesSameFile | blackPiecesSameFile]);

		// Top Left Bottom Right Diagonal
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedRawTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0] | this->precalculatedRawBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0]);

		// Top Right Bottom Left Diagonal
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);

		configurationMetadata.whiteQueenAttackZone |= (this->precalculatedRawTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1] | this->precalculatedRawBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1]);
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

		if (this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawLeftAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameRank | blackPiecesSameRank];
		}
		else if (this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][whitePiecesSameRank][blackPiecesSameRank].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawRightAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameRank | blackPiecesSameRank];
		}
		else if (this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawTopAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameFile | blackPiecesSameFile];
		}
		else if (this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][whitePiecesSameFile][blackPiecesSameFile].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawBottomAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameFile | blackPiecesSameFile];
		}
		else if (this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawTopLeftDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0];
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawBottomRightDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal0 | blackPiecesSameDiagonal0];
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawTopRightDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1];
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first & configurationMetadata.blackKing)
		{
			configurationMetadata.blackKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first) & (~configurationMetadata.blackKing));

			configurationMetadata.whiteQueenAttackZone |= this->precalculatedRawBottomLeftDiagonalAttackZones[this->logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]][whitePiecesSameDiagonal1 | blackPiecesSameDiagonal1];
		}

		whiteQueens ^= lsb;
	}
}

void BoardManager::generateWhiteKingAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.whiteKingAttackZone = 0ull;

	unsigned long long lsb = configurationMetadata.whiteKing;

	configurationMetadata.whiteKingAttackZone |= this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
	if (this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]] & configurationMetadata.blackKing)
	{
		configurationMetadata.blackKingDefenseZone &= lsb;
	}
}

// Black Pieces Attack Zones Generation

void BoardManager::generateBlackPawnAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.blackPawnAttackZone = 0ull;
	unsigned long long blackPawns = (configurationMetadata.blackPawns & (~this->rankBitMasks[7 * GameMetadata::NUM_TILES_WIDTH]));

	// Atac Stanga
	unsigned long long leftAttackZone = ((blackPawns & (~this->fileBitMasks[0])) << (GameMetadata::NUM_TILES_WIDTH + 1));
	configurationMetadata.blackPawnAttackZone |= leftAttackZone;
	if (leftAttackZone & configurationMetadata.whiteKing)
	{
		configurationMetadata.whiteKingDefenseZone &= (configurationMetadata.whiteKing >> (GameMetadata::NUM_TILES_WIDTH + 1));
	}

	// Atac Dreapta
	unsigned long long rightAttackZone = ((blackPawns & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) << (GameMetadata::NUM_TILES_WIDTH - 1));
	configurationMetadata.blackPawnAttackZone |= rightAttackZone;
	if (rightAttackZone & configurationMetadata.whiteKing)
	{
		configurationMetadata.whiteKingDefenseZone &= (configurationMetadata.whiteKing >> (GameMetadata::NUM_TILES_WIDTH - 1));
	}

	// Atac En Passant (nu trebuie) (zona de atac adaugata de en passant nu este niciodata utila)
}

void BoardManager::generateBlackRookAttackZone(ConfigurationMetadata& configurationMetadata)
{
	// Nu tinem cont de pins aici pentru culoarea curenta. Acum generam pin-urile pentru culoarea opusa, pentru care generam mutarile.
	// Nu initializam pin-urile cu 0ull, pentru ca le share-uim intre sliding pieces (rooks, bishops, queens).

	configurationMetadata.blackRookAttackZone = 0ull;
	unsigned long long blackRooks = configurationMetadata.blackRooks;

	while (blackRooks)
	{
		unsigned long long lsb = (blackRooks & ((~blackRooks) + 1));
		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		// Left-Right
		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackRookAttackZone |= (this->precalculatedRawLeftAttackZones[pos][blackPiecesSameRank | whitePiecesSameRank] | this->precalculatedRawRightAttackZones[pos][blackPiecesSameRank | whitePiecesSameRank]);

		// Top-Bottom
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackRookAttackZone |= (this->precalculatedRawTopAttackZones[pos][blackPiecesSameFile | whitePiecesSameFile] | this->precalculatedRawBottomAttackZones[pos][blackPiecesSameFile | whitePiecesSameFile]);
		//


		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnRank |= this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first;
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnRank |= this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].second.first;
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnFile |= this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first;
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnFile |= this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].second.first;

		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackRookAttackZone |= this->precalculatedRawLeftAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameRank | whitePiecesSameRank];
		}
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackRookAttackZone |= this->precalculatedRawRightAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameRank | whitePiecesSameRank];
		}
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackRookAttackZone |= this->precalculatedRawTopAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameFile | whitePiecesSameFile];
		}
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackRookAttackZone |= this->precalculatedRawBottomAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameFile | whitePiecesSameFile];
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
			configurationMetadata.whiteKingDefenseZone &= lsb;
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

		configurationMetadata.blackBishopAttackZone |= (this->precalculatedRawTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0] | this->precalculatedRawBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0]);

		// Top Right Bottom Left Diagonal
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackBishopAttackZone |= (this->precalculatedRawTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1] | this->precalculatedRawBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1]);
		//


		if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first;
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal |= this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].second.first;
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first;
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.second & configurationMetadata.whiteKing)
			this->configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal |= this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].second.first;

		if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackBishopAttackZone |= this->precalculatedRawTopLeftDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0];
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackBishopAttackZone |= this->precalculatedRawBottomRightDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0];
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackBishopAttackZone |= this->precalculatedRawTopRightDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1];
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackBishopAttackZone |= this->precalculatedRawBottomLeftDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1];
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

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedRawLeftAttackZones[pos][blackPiecesSameRank | whitePiecesSameRank] | this->precalculatedRawRightAttackZones[pos][blackPiecesSameRank | whitePiecesSameRank]);

		// Top-Bottom
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedRawTopAttackZones[pos][blackPiecesSameFile | whitePiecesSameFile] | this->precalculatedRawBottomAttackZones[pos][blackPiecesSameFile | whitePiecesSameFile]);

		// Top Left Bottom Right Diagonal
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedRawTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0] | this->precalculatedRawBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0]);

		// Top Right Bottom Left Diagonal
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, pos);
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, pos);

		configurationMetadata.blackQueenAttackZone |= (this->precalculatedRawTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1] | this->precalculatedRawBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1]);
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

		if (this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedLeftAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawLeftAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameRank | whitePiecesSameRank];
		}
		else if (this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedRightAttackZones[pos][blackPiecesSameRank][whitePiecesSameRank].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawRightAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameRank | whitePiecesSameRank];
		}
		else if (this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawTopAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameFile | whitePiecesSameFile];
		}
		else if (this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomAttackZones[pos][blackPiecesSameFile][whitePiecesSameFile].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawBottomAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameFile | whitePiecesSameFile];
		}
		else if (this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawTopLeftDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0];
		}
		else if (this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomRightDiagonalAttackZones[pos][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawBottomRightDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal0 | whitePiecesSameDiagonal0];
		}
		else if (this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedTopRightDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawTopRightDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1];
		}
		else if (this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first & configurationMetadata.whiteKing)
		{
			configurationMetadata.whiteKingDefenseZone &= ((lsb | this->precalculatedBottomLeftDiagonalAttackZones[pos][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first) & (~configurationMetadata.whiteKing));

			configurationMetadata.blackQueenAttackZone |= this->precalculatedRawBottomLeftDiagonalAttackZones[this->logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]][blackPiecesSameDiagonal1 | whitePiecesSameDiagonal1];
		}

		blackQueens ^= lsb;
	}
}

void BoardManager::generateBlackKingAttackZone(ConfigurationMetadata& configurationMetadata)
{
	configurationMetadata.blackKingAttackZone = 0ull;

	unsigned long long lsb = configurationMetadata.blackKing;

	configurationMetadata.blackKingAttackZone |= this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
	if (this->precalculatedKingAttackZones[this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]] & configurationMetadata.whiteKing)
	{
		configurationMetadata.whiteKingDefenseZone &= lsb;
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

void BoardManager::generateWhitePawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.whiteKingDefenseZone == 0ull)
		return;

	unsigned long long whitePawns = configurationMetadata.whitePawns;

	// Avansare Simpla (fara promovare)
	unsigned long long singleAdvance = (((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & (~this->rankBitMasks[0]) & (~this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH])) >> GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces) & configurationMetadata.whiteKingDefenseZone);
	while (singleAdvance)
	{
		unsigned long long lsb = (singleAdvance & ((~singleAdvance) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('P', pos));

		singleAdvance ^= lsb;
	}

	// Avansare Dubla
	unsigned long long doubleAdvance = (((((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH]) >> GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces)) >> GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces) & configurationMetadata.whiteKingDefenseZone);
	while (doubleAdvance)
	{
		unsigned long long lsb = (doubleAdvance & ((~doubleAdvance) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + 2 * GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('P', pos));

		doubleAdvance ^= lsb;
	}

	// Atac Stanga (fara promovare)
	unsigned long long leftAttack = (((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & (~this->rankBitMasks[0]) & (~this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH]) & (~this->fileBitMasks[0])) >> (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.allBlackPieces & configurationMetadata.whiteKingDefenseZone);
	while (leftAttack)
	{
		unsigned long long lsb = (leftAttack & ((~leftAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('P', pos));

		leftAttack ^= lsb;
	}

	// Atac Dreapta (fara promovare)
	unsigned long long rightAttack = (((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & (~this->rankBitMasks[0]) & (~this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH]) & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) >> (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.allBlackPieces & configurationMetadata.whiteKingDefenseZone);
	while (rightAttack)
	{
		unsigned long long lsb = (rightAttack & ((~rightAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('P', pos));

		rightAttack ^= lsb;
	}

	// Promovare Avansare Simpla
	unsigned long long promotionSingleAdvance = (((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH]) >> GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces) & configurationMetadata.whiteKingDefenseZone);
	while (promotionSingleAdvance)
	{
		unsigned long long lsb = (promotionSingleAdvance & ((~promotionSingleAdvance) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('R', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('N', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('B', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('Q', pos));

		promotionSingleAdvance ^= lsb;
	}

	// Promovare Atac Stanga
	unsigned long long promotionLeftAttack = (((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH] & (~this->fileBitMasks[0])) >> (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.allBlackPieces & configurationMetadata.whiteKingDefenseZone);
	while (promotionLeftAttack)
	{
		unsigned long long lsb = (promotionLeftAttack & ((~promotionLeftAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('R', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('N', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('B', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('Q', pos));

		promotionLeftAttack ^= lsb;
	}

	// Promovare Atac Dreapta
	unsigned long long promotionRightAttack = (((whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH] & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) >> (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.allBlackPieces & configurationMetadata.whiteKingDefenseZone);
	while (promotionRightAttack)
	{
		unsigned long long lsb = (promotionRightAttack & ((~promotionRightAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('R', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('N', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('B', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', pos + GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('Q', pos));

		promotionRightAttack ^= lsb;
	}

	// En Passant // INFO: En Passant da return prematur.
	if (configurationMetadata.capturableEnPassantPosition == 0) // INFO: Daca nu exista o pozitie de capturare en passant, atunci capturableEnPassantPosition este 0.
		return;

	unsigned long long enPassantRankPieces = this->extractRank(configurationMetadata.allPieces, configurationMetadata.capturableEnPassantPosition);
	unsigned long long enPassantDiagonalPieces0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allPieces, configurationMetadata.capturableEnPassantPosition - 1);
	unsigned long long enPassantDiagonalPieces1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allPieces, configurationMetadata.capturableEnPassantPosition + 1);

	unsigned long long whitePawnEnPassantRight = ((((1ull << configurationMetadata.capturableEnPassantPosition) >> 1) & whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[configurationMetadata.capturableEnPassantPosition]) & (configurationMetadata.whiteKingDefenseZone << (GameMetadata::NUM_TILES_WIDTH - 1)));
	if (whitePawnEnPassantRight &&
			(
				!
				(
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].first & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].second & (configurationMetadata.blackRooks | configurationMetadata.blackQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].second & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].first & (configurationMetadata.blackRooks | configurationMetadata.blackQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces0].first & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces0].second & (configurationMetadata.blackBishops | configurationMetadata.blackQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces0].second & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces0].first & (configurationMetadata.blackBishops | configurationMetadata.blackQueens))
					)
				)
			)
		)
	{
		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', configurationMetadata.capturableEnPassantPosition - 1));
		moves.back().emplace_back(std::make_pair('P', configurationMetadata.capturableEnPassantPosition - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('p', configurationMetadata.capturableEnPassantPosition));
	}

	unsigned long long whitePawnEnPassantLeft = ((((1ull << configurationMetadata.capturableEnPassantPosition) << 1) & whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & this->rankBitMasks[configurationMetadata.capturableEnPassantPosition]) & (configurationMetadata.whiteKingDefenseZone << (GameMetadata::NUM_TILES_WIDTH + 1)));
	if (whitePawnEnPassantLeft &&
			(
				!
				(
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].first & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].second & (configurationMetadata.blackRooks | configurationMetadata.blackQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].second & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].first & (configurationMetadata.blackRooks | configurationMetadata.blackQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces1].first & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces1].second & (configurationMetadata.blackBishops | configurationMetadata.blackQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces1].second & configurationMetadata.whiteKing)
						&&
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces1].first & (configurationMetadata.blackBishops | configurationMetadata.blackQueens))
					)
				)
			)
		)
	{
		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('P', configurationMetadata.capturableEnPassantPosition + 1));
		moves.back().emplace_back(std::make_pair('P', configurationMetadata.capturableEnPassantPosition - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('p', configurationMetadata.capturableEnPassantPosition));
	}
}

void BoardManager::generateWhiteRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.whiteKingDefenseZone == 0ull)
		return;

	unsigned long long whiteRooks = configurationMetadata.whiteRooks;

	while (whiteRooks)
	{
		unsigned long long lsbRook = (whiteRooks & ((~whiteRooks) + 1));

		if (!
			(
				(lsbRook & (configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal))
				||
				(lsbRook & (configurationMetadata.whitePiecesPinnedOnRank & configurationMetadata.whitePiecesPinnedOnFile))
				)
			)
		{
			int posRook = this->logPower2[lsbRook % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, posRook);
			unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, posRook);

			unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, posRook);
			unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, posRook);

			unsigned long long rookAttackZone = (this->precalculatedLeftAttackZones[posRook][whitePiecesSameRank][blackPiecesSameRank].first | this->precalculatedRightAttackZones[posRook][whitePiecesSameRank][blackPiecesSameRank].first | this->precalculatedTopAttackZones[posRook][whitePiecesSameFile][blackPiecesSameFile].first | this->precalculatedBottomAttackZones[posRook][whitePiecesSameFile][blackPiecesSameFile].first);
			rookAttackZone &= configurationMetadata.whiteKingDefenseZone;

			if (lsbRook & configurationMetadata.whitePiecesPinnedOnRank)
				rookAttackZone &= this->rankBitMasks[posRook];
			if (lsbRook & configurationMetadata.whitePiecesPinnedOnFile)
				rookAttackZone &= this->fileBitMasks[posRook];

			while (rookAttackZone)
			{
				unsigned long long lsbAttack = (rookAttackZone & ((~rookAttackZone) + 1));

				int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				moves.emplace_back();
				moves.back().emplace_back(std::make_pair('R', posRook));
				moves.back().emplace_back(std::make_pair('R', posAttack));

				rookAttackZone ^= lsbAttack;
			}
		}

		whiteRooks ^= lsbRook;
	}
}

void BoardManager::generateWhiteKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.whiteKingDefenseZone == 0ull)
		return;

	unsigned long long whiteKnights = configurationMetadata.whiteKnights;

	while (whiteKnights)
	{
		unsigned long long lsbKnight = (whiteKnights & ((~whiteKnights) + 1));

		if (!(lsbKnight & (configurationMetadata.whitePiecesPinnedOnRank | configurationMetadata.whitePiecesPinnedOnFile | configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal)))
		{
			int posKnight = this->logPower2[lsbKnight % BoardManager::MODULO_LOG_POWER_2];
			unsigned long long knightAttackZone = (this->precalculatedKnightAttackZones[posKnight] & (~configurationMetadata.allWhitePieces) & configurationMetadata.whiteKingDefenseZone);

			while (knightAttackZone)
			{
				unsigned long long lsbAttack = (knightAttackZone & ((~knightAttackZone) + 1));

				int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				moves.emplace_back();
				moves.back().emplace_back(std::make_pair('N', posKnight));
				moves.back().emplace_back(std::make_pair('N', posAttack));

				knightAttackZone ^= lsbAttack;
			}
		}

		whiteKnights ^= lsbKnight;
	}
}

void BoardManager::generateWhiteBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.whiteKingDefenseZone == 0ull)
		return;

	unsigned long long whiteBishops = configurationMetadata.whiteBishops;

	while (whiteBishops)
	{
		unsigned long long lsbBishop = (whiteBishops & ((~whiteBishops) + 1));

		if (!
				(
					(lsbBishop & (configurationMetadata.whitePiecesPinnedOnRank | configurationMetadata.whitePiecesPinnedOnFile))
					||
					(lsbBishop & (configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal & configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal))
				)
		)
		{
			int posBishop = this->logPower2[lsbBishop % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, posBishop);
			unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, posBishop);

			unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, posBishop);
			unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, posBishop);

			unsigned long long bishopAttackZone = (this->precalculatedTopLeftDiagonalAttackZones[posBishop][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[posBishop][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first | this->precalculatedTopRightDiagonalAttackZones[posBishop][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[posBishop][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first);
			bishopAttackZone &= configurationMetadata.whiteKingDefenseZone;

			if (lsbBishop & configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal)
				bishopAttackZone &= this->topLeftBottomRightDiagonalBitMasks[posBishop];
			if (lsbBishop & configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal)
				bishopAttackZone &= this->topRightBottomLeftDiagonalBitMasks[posBishop];

			while (bishopAttackZone)
			{
				unsigned long long lsbAttack = (bishopAttackZone & ((~bishopAttackZone) + 1));

				int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				moves.emplace_back();
				moves.back().emplace_back(std::make_pair('B', posBishop));
				moves.back().emplace_back(std::make_pair('B', posAttack));

				bishopAttackZone ^= lsbAttack;
			}
		}

		whiteBishops ^= lsbBishop;
	}
}

void BoardManager::generateWhiteQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.whiteKingDefenseZone == 0ull)
		return;

	unsigned long long whiteQueens = configurationMetadata.whiteQueens;

	while (whiteQueens)
	{
		unsigned long long lsbQueen = (whiteQueens & ((~whiteQueens) + 1));

		int posQueen = this->logPower2[lsbQueen % BoardManager::MODULO_LOG_POWER_2];

		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, posQueen);
		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, posQueen);

		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, posQueen);
		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, posQueen);

		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, posQueen);
		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, posQueen);

		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, posQueen);
		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, posQueen);

		unsigned long long queenAttackZone = (this->precalculatedLeftAttackZones[posQueen][whitePiecesSameRank][blackPiecesSameRank].first | this->precalculatedRightAttackZones[posQueen][whitePiecesSameRank][blackPiecesSameRank].first
			| this->precalculatedTopAttackZones[posQueen][whitePiecesSameFile][blackPiecesSameFile].first | this->precalculatedBottomAttackZones[posQueen][whitePiecesSameFile][blackPiecesSameFile].first
			| this->precalculatedTopLeftDiagonalAttackZones[posQueen][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[posQueen][whitePiecesSameDiagonal0][blackPiecesSameDiagonal0].first
			| this->precalculatedTopRightDiagonalAttackZones[posQueen][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[posQueen][whitePiecesSameDiagonal1][blackPiecesSameDiagonal1].first);
		queenAttackZone &= configurationMetadata.whiteKingDefenseZone;

		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnRank)
			queenAttackZone &= this->rankBitMasks[posQueen];
		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnFile)
			queenAttackZone &= this->fileBitMasks[posQueen];
		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal)
			queenAttackZone &= this->topLeftBottomRightDiagonalBitMasks[posQueen];
		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal)
			queenAttackZone &= this->topRightBottomLeftDiagonalBitMasks[posQueen];

		while (queenAttackZone)
		{
			unsigned long long lsbAttack = (queenAttackZone & ((~queenAttackZone) + 1));

			int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

			moves.emplace_back();
			moves.back().emplace_back(std::make_pair('Q', posQueen));
			moves.back().emplace_back(std::make_pair('Q', posAttack));

			queenAttackZone ^= lsbAttack;
		}

		whiteQueens ^= lsbQueen;
	}
}

void BoardManager::generateWhiteKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	unsigned long long lsbKing = configurationMetadata.whiteKing;
	int posKing = this->logPower2[lsbKing % BoardManager::MODULO_LOG_POWER_2];
	unsigned long long kingAttackZone = (this->precalculatedKingAttackZones[posKing] & (~configurationMetadata.blackAttackZones) & (~configurationMetadata.allWhitePieces));

	while (kingAttackZone)
	{
		unsigned long long lsbAttack = (kingAttackZone & ((~kingAttackZone) + 1));

		int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('K', posKing));
		moves.back().emplace_back(std::make_pair('K', posAttack));

		kingAttackZone ^= lsbAttack;
	}

	// Castling
	if (!configurationMetadata.whiteKingMoved)
	{
		if (!configurationMetadata.whiteRookBottomLeftMoved && (configurationMetadata.blackAttackZones & this->precalculatedFullCastleBottomLeft) == 0ull && (configurationMetadata.allPieces & this->precalculatedEmptyCastleBottomLeft) == 0ull)
		{
			moves.emplace_back();
			moves.back().emplace_back(std::make_pair('K', posKing));
			moves.back().emplace_back(std::make_pair('K', posKing - 2));
			moves.back().emplace_back(std::make_pair('R', posKing - 4));
			moves.back().emplace_back(std::make_pair('R', posKing - 1));
		}

		if (!configurationMetadata.whiteRookBottomRightMoved && (configurationMetadata.blackAttackZones & this->precalculatedFullCastleBottomRight) == 0ull && (configurationMetadata.allPieces & this->precalculatedEmptyCastleBottomRight) == 0ull)
		{
			moves.emplace_back();
			moves.back().emplace_back(std::make_pair('K', posKing));
			moves.back().emplace_back(std::make_pair('K', posKing + 2));
			moves.back().emplace_back(std::make_pair('R', posKing + 3));
			moves.back().emplace_back(std::make_pair('R', posKing + 1));
		}
	}
}

// Black Pieces Moves Generation

void BoardManager::generateBlackPawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.blackKingDefenseZone == 0ull)
		return;

	unsigned long long blackPawns = configurationMetadata.blackPawns;

	// Avansare Simpla (fara promovare)
	unsigned long long singleAdvance = (((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & (~this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH]) & (~this->rankBitMasks[7 * GameMetadata::NUM_TILES_WIDTH])) << GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces) & configurationMetadata.blackKingDefenseZone);
	while (singleAdvance)
	{
		unsigned long long lsb = (singleAdvance & ((~singleAdvance) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('p', pos));

		singleAdvance ^= lsb;
	}

	// Avansare Dubla
	unsigned long long doubleAdvance = (((((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[GameMetadata::NUM_TILES_WIDTH]) << GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces)) << GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces) & configurationMetadata.blackKingDefenseZone);
	while (doubleAdvance)
	{
		unsigned long long lsb = (doubleAdvance & ((~doubleAdvance) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - 2 * GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('p', pos));

		doubleAdvance ^= lsb;
	}

	// Atac Stanga (fara promovare)
	unsigned long long leftAttack = (((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & (~this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH]) & (~this->rankBitMasks[7 * GameMetadata::NUM_TILES_WIDTH]) & (~this->fileBitMasks[0])) << (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.allWhitePieces & configurationMetadata.blackKingDefenseZone);
	while (leftAttack)
	{
		unsigned long long lsb = (leftAttack & ((~leftAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('p', pos));

		leftAttack ^= lsb;
	}

	// Atac Dreapta (fara promovare)
	unsigned long long rightAttack = (((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & (~this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH]) & (~this->rankBitMasks[7 * GameMetadata::NUM_TILES_WIDTH]) & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) << (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.allWhitePieces & configurationMetadata.blackKingDefenseZone);
	while (rightAttack)
	{
		unsigned long long lsb = (rightAttack & ((~rightAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('p', pos));

		rightAttack ^= lsb;
	}

	// Promovare Avansare Simpla
	unsigned long long promotionSingleAdvance = (((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH]) << GameMetadata::NUM_TILES_WIDTH) & (~configurationMetadata.allPieces) & configurationMetadata.blackKingDefenseZone);
	while (promotionSingleAdvance)
	{
		unsigned long long lsb = (promotionSingleAdvance & ((~promotionSingleAdvance) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('r', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('n', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('b', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('q', pos));

		promotionSingleAdvance ^= lsb;
	}

	// Promovare Atac Stanga
	unsigned long long promotionLeftAttack = (((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH] & (~this->fileBitMasks[0])) << (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.allWhitePieces & configurationMetadata.blackKingDefenseZone);
	while (promotionLeftAttack)
	{
		unsigned long long lsb = (promotionLeftAttack & ((~promotionLeftAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('r', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('n', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('b', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH + 1));
		moves.back().emplace_back(std::make_pair('q', pos));

		promotionLeftAttack ^= lsb;
	}

	// Promovare Atac Dreapta
	unsigned long long promotionRightAttack = (((blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[6 * GameMetadata::NUM_TILES_WIDTH] & (~this->fileBitMasks[GameMetadata::NUM_TILES_WIDTH - 1])) << (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.allWhitePieces & configurationMetadata.blackKingDefenseZone);
	while (promotionRightAttack)
	{
		unsigned long long lsb = (promotionRightAttack & ((~promotionRightAttack) + 1));

		int pos = this->logPower2[lsb % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('r', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('n', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('b', pos));

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', pos - GameMetadata::NUM_TILES_WIDTH - 1));
		moves.back().emplace_back(std::make_pair('q', pos));

		promotionRightAttack ^= lsb;
	}

	// En Passant // INFO: En Passant da return prematur.
	if (configurationMetadata.capturableEnPassantPosition == 0) // INFO: Daca nu exista o pozitie de capturare en passant, atunci capturableEnPassantPosition este 0.
		return;

	unsigned long long enPassantRankPieces = this->extractRank(configurationMetadata.allPieces, configurationMetadata.capturableEnPassantPosition);
	unsigned long long enPassantDiagonalPieces0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allPieces, configurationMetadata.capturableEnPassantPosition + 1);
	unsigned long long enPassantDiagonalPieces1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allPieces, configurationMetadata.capturableEnPassantPosition - 1);

	unsigned long long blackPawnEnPassantRight = ((((1ull << configurationMetadata.capturableEnPassantPosition) << 1) & blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & this->rankBitMasks[configurationMetadata.capturableEnPassantPosition]) & (configurationMetadata.blackKingDefenseZone >> (GameMetadata::NUM_TILES_WIDTH - 1)));
	if (blackPawnEnPassantRight &&
			(
				!
				(
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].first & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].second & (configurationMetadata.whiteRooks | configurationMetadata.whiteQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].second & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH + 1))].first & (configurationMetadata.whiteRooks | configurationMetadata.whiteQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces0].first & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces0].second & (configurationMetadata.whiteBishops | configurationMetadata.whiteQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces0].second & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[configurationMetadata.capturableEnPassantPosition + 1][enPassantDiagonalPieces0].first & (configurationMetadata.whiteBishops | configurationMetadata.whiteQueens))
					)
				)
			)
		)
	{
		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', configurationMetadata.capturableEnPassantPosition + 1));
		moves.back().emplace_back(std::make_pair('p', configurationMetadata.capturableEnPassantPosition + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('P', configurationMetadata.capturableEnPassantPosition));
	}

	unsigned long long blackPawnEnPassantLeft = ((((1ull << configurationMetadata.capturableEnPassantPosition) >> 1) & blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & this->rankBitMasks[configurationMetadata.capturableEnPassantPosition]) & (configurationMetadata.blackKingDefenseZone >> (GameMetadata::NUM_TILES_WIDTH + 1)));
	if (blackPawnEnPassantLeft &&
			(
				!
				(
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].first & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].second & (configurationMetadata.whiteRooks | configurationMetadata.whiteQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].second & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnRank[configurationMetadata.capturableEnPassantPosition][enPassantRankPieces ^ (1ull << (configurationMetadata.capturableEnPassantPosition % GameMetadata::NUM_TILES_WIDTH - 1))].first & (configurationMetadata.whiteRooks | configurationMetadata.whiteQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces1].first & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces1].second & (configurationMetadata.whiteBishops | configurationMetadata.whiteQueens))
					)
					||
					(
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces1].second & configurationMetadata.blackKing)
						&&
						(this->precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[configurationMetadata.capturableEnPassantPosition - 1][enPassantDiagonalPieces1].first & (configurationMetadata.whiteBishops | configurationMetadata.whiteQueens))
					)
				)
			)
		)
	{
		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('p', configurationMetadata.capturableEnPassantPosition - 1));
		moves.back().emplace_back(std::make_pair('p', configurationMetadata.capturableEnPassantPosition + GameMetadata::NUM_TILES_WIDTH));
		moves.back().emplace_back(std::make_pair('P', configurationMetadata.capturableEnPassantPosition));
	}
}

void BoardManager::generateBlackRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.blackKingDefenseZone == 0ull)
		return;

	unsigned long long blackRooks = configurationMetadata.blackRooks;

	while (blackRooks)
	{
		unsigned long long lsbRook = (blackRooks & ((~blackRooks) + 1));

		if (!
				(
					(lsbRook & (configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal))
					||
					(lsbRook & (configurationMetadata.blackPiecesPinnedOnRank & configurationMetadata.blackPiecesPinnedOnFile))
				)
		)
		{
			int posRook = this->logPower2[lsbRook % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, posRook);
			unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, posRook);

			unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, posRook);
			unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, posRook);

			unsigned long long rookAttackZone = (this->precalculatedLeftAttackZones[posRook][blackPiecesSameRank][whitePiecesSameRank].first | this->precalculatedRightAttackZones[posRook][blackPiecesSameRank][whitePiecesSameRank].first | this->precalculatedTopAttackZones[posRook][blackPiecesSameFile][whitePiecesSameFile].first | this->precalculatedBottomAttackZones[posRook][blackPiecesSameFile][whitePiecesSameFile].first);
			rookAttackZone &= configurationMetadata.blackKingDefenseZone;

			if (lsbRook & configurationMetadata.blackPiecesPinnedOnRank)
				rookAttackZone &= this->rankBitMasks[posRook];
			if (lsbRook & configurationMetadata.blackPiecesPinnedOnFile)
				rookAttackZone &= this->fileBitMasks[posRook];

			while (rookAttackZone)
			{
				unsigned long long lsbAttack = (rookAttackZone & ((~rookAttackZone) + 1));

				int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				moves.emplace_back();
				moves.back().emplace_back(std::make_pair('r', posRook));
				moves.back().emplace_back(std::make_pair('r', posAttack));

				rookAttackZone ^= lsbAttack;
			}
		}

		blackRooks ^= lsbRook;
	}
}

void BoardManager::generateBlackKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.blackKingDefenseZone == 0ull)
		return;

	unsigned long long blackKnights = configurationMetadata.blackKnights;

	while (blackKnights)
	{
		unsigned long long lsbKnight = (blackKnights & ((~blackKnights) + 1));

		if (!(lsbKnight & (configurationMetadata.blackPiecesPinnedOnRank | configurationMetadata.blackPiecesPinnedOnFile | configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal)))
		{
			int posKnight = this->logPower2[lsbKnight % BoardManager::MODULO_LOG_POWER_2];
			unsigned long long knightAttackZone = (this->precalculatedKnightAttackZones[posKnight] & (~configurationMetadata.allBlackPieces) & configurationMetadata.blackKingDefenseZone);

			while (knightAttackZone)
			{
				unsigned long long lsbAttack = (knightAttackZone & ((~knightAttackZone) + 1));

				int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				moves.emplace_back();
				moves.back().emplace_back(std::make_pair('n', posKnight));
				moves.back().emplace_back(std::make_pair('n', posAttack));

				knightAttackZone ^= lsbAttack;
			}
		}

		blackKnights ^= lsbKnight;
	}
}

void BoardManager::generateBlackBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.blackKingDefenseZone == 0ull)
		return;

	unsigned long long blackBishops = configurationMetadata.blackBishops;

	while (blackBishops)
	{
		unsigned long long lsbBishop = (blackBishops & ((~blackBishops) + 1));

		if (!
			(
				(lsbBishop & (configurationMetadata.blackPiecesPinnedOnRank | configurationMetadata.blackPiecesPinnedOnFile))
				||
				(lsbBishop & (configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal & configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal))
				)
			)
		{
			int posBishop = this->logPower2[lsbBishop % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, posBishop);
			unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, posBishop);

			unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, posBishop);
			unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, posBishop);

			unsigned long long bishopAttackZone = (this->precalculatedTopLeftDiagonalAttackZones[posBishop][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[posBishop][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first | this->precalculatedTopRightDiagonalAttackZones[posBishop][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[posBishop][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first);
			bishopAttackZone &= configurationMetadata.blackKingDefenseZone;

			if (lsbBishop & configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal)
				bishopAttackZone &= this->topLeftBottomRightDiagonalBitMasks[posBishop];
			if (lsbBishop & configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal)
				bishopAttackZone &= this->topRightBottomLeftDiagonalBitMasks[posBishop];

			while (bishopAttackZone)
			{
				unsigned long long lsbAttack = (bishopAttackZone & ((~bishopAttackZone) + 1));

				int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				moves.emplace_back();
				moves.back().emplace_back(std::make_pair('b', posBishop));
				moves.back().emplace_back(std::make_pair('b', posAttack));

				bishopAttackZone ^= lsbAttack;
			}
		}

		blackBishops ^= lsbBishop;
	}
}

void BoardManager::generateBlackQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	if (configurationMetadata.blackKingDefenseZone == 0ull)
		return;

	unsigned long long blackQueens = configurationMetadata.blackQueens;

	while (blackQueens)
	{
		unsigned long long lsbQueen = (blackQueens & ((~blackQueens) + 1));

		int posQueen = this->logPower2[lsbQueen % BoardManager::MODULO_LOG_POWER_2];

		unsigned long long blackPiecesSameRank = this->extractRank(configurationMetadata.allBlackPieces, posQueen);
		unsigned long long whitePiecesSameRank = this->extractRank(configurationMetadata.allWhitePieces, posQueen);

		unsigned long long blackPiecesSameFile = this->extractFile(configurationMetadata.allBlackPieces, posQueen);
		unsigned long long whitePiecesSameFile = this->extractFile(configurationMetadata.allWhitePieces, posQueen);

		unsigned long long blackPiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allBlackPieces, posQueen);
		unsigned long long whitePiecesSameDiagonal0 = this->extractTopLeftBottomRightDiagonal(configurationMetadata.allWhitePieces, posQueen);

		unsigned long long blackPiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allBlackPieces, posQueen);
		unsigned long long whitePiecesSameDiagonal1 = this->extractTopRightBottomLeftDiagonal(configurationMetadata.allWhitePieces, posQueen);

		unsigned long long queenAttackZone = (this->precalculatedLeftAttackZones[posQueen][blackPiecesSameRank][whitePiecesSameRank].first | this->precalculatedRightAttackZones[posQueen][blackPiecesSameRank][whitePiecesSameRank].first
			| this->precalculatedTopAttackZones[posQueen][blackPiecesSameFile][whitePiecesSameFile].first | this->precalculatedBottomAttackZones[posQueen][blackPiecesSameFile][whitePiecesSameFile].first
			| this->precalculatedTopLeftDiagonalAttackZones[posQueen][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first | this->precalculatedBottomRightDiagonalAttackZones[posQueen][blackPiecesSameDiagonal0][whitePiecesSameDiagonal0].first
			| this->precalculatedTopRightDiagonalAttackZones[posQueen][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first | this->precalculatedBottomLeftDiagonalAttackZones[posQueen][blackPiecesSameDiagonal1][whitePiecesSameDiagonal1].first);
		queenAttackZone &= configurationMetadata.blackKingDefenseZone;

		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnRank)
			queenAttackZone &= this->rankBitMasks[posQueen];
		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnFile)
			queenAttackZone &= this->fileBitMasks[posQueen];
		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal)
			queenAttackZone &= this->topLeftBottomRightDiagonalBitMasks[posQueen];
		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal)
			queenAttackZone &= this->topRightBottomLeftDiagonalBitMasks[posQueen];

		while (queenAttackZone)
		{
			unsigned long long lsbAttack = (queenAttackZone & ((~queenAttackZone) + 1));

			int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

			moves.emplace_back();
			moves.back().emplace_back(std::make_pair('q', posQueen));
			moves.back().emplace_back(std::make_pair('q', posAttack));

			queenAttackZone ^= lsbAttack;
		}

		blackQueens ^= lsbQueen;
	}
}

void BoardManager::generateBlackKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	unsigned long long lsbKing = configurationMetadata.blackKing;
	int posKing = this->logPower2[lsbKing % BoardManager::MODULO_LOG_POWER_2];
	unsigned long long kingAttackZone = (this->precalculatedKingAttackZones[posKing] & (~configurationMetadata.whiteAttackZones) & (~configurationMetadata.allBlackPieces));

	while (kingAttackZone)
	{
		unsigned long long lsbAttack = (kingAttackZone & ((~kingAttackZone) + 1));

		int posAttack = this->logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

		moves.emplace_back();
		moves.back().emplace_back(std::make_pair('k', posKing));
		moves.back().emplace_back(std::make_pair('k', posAttack));

		kingAttackZone ^= lsbAttack;
	}

	// Castling
	if (!configurationMetadata.blackKingMoved)
	{
		if (!configurationMetadata.blackRookTopLeftMoved && (configurationMetadata.whiteAttackZones & this->precalculatedFullCastleTopLeft) == 0ull && (configurationMetadata.allPieces & this->precalculatedEmptyCastleTopLeft) == 0ull)
		{
			moves.emplace_back();
			moves.back().emplace_back(std::make_pair('k', posKing));
			moves.back().emplace_back(std::make_pair('k', posKing - 2));
			moves.back().emplace_back(std::make_pair('r', posKing - 4));
			moves.back().emplace_back(std::make_pair('r', posKing - 1));
		}
		if (!configurationMetadata.blackRookTopRightMoved && (configurationMetadata.whiteAttackZones & this->precalculatedFullCastleTopRight) == 0ull && (configurationMetadata.allPieces & this->precalculatedEmptyCastleTopRight) == 0ull)
		{
			moves.emplace_back();
			moves.back().emplace_back(std::make_pair('k', posKing));
			moves.back().emplace_back(std::make_pair('k', posKing + 2));
			moves.back().emplace_back(std::make_pair('r', posKing + 3));
			moves.back().emplace_back(std::make_pair('r', posKing + 1));
		}
	}
}

// All Pieces Moves Generation

void BoardManager::generateWhiteMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	configurationMetadata.whitePiecesPinnedOnRank = 0ull;
	configurationMetadata.whitePiecesPinnedOnFile = 0ull;
	configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal = 0ull;
	configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal = 0ull;

	configurationMetadata.whiteKingDefenseZone = (~(0ull));

	generateBlackAttackZones(configurationMetadata);

	// INFO: Sunt in alta ordine decat cea obisnuita pentru a face practic un move ordering pentru MinMax.
	generateWhiteQueenMoves(configurationMetadata, moves);
	generateWhiteRookMoves(configurationMetadata, moves);
	generateWhiteBishopMoves(configurationMetadata, moves);
	generateWhiteKnightMoves(configurationMetadata, moves);
	generateWhitePawnMoves(configurationMetadata, moves);
	generateWhiteKingMoves(configurationMetadata, moves);
}

void BoardManager::generateBlackMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves)
{
	configurationMetadata.blackPiecesPinnedOnRank = 0ull;
	configurationMetadata.blackPiecesPinnedOnFile = 0ull;
	configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal = 0ull;
	configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal = 0ull;

	configurationMetadata.blackKingDefenseZone = (~(0ull));

	generateWhiteAttackZones(configurationMetadata);

	// INFO: Sunt in alta ordine decat cea obisnuita pentru a face practic un move ordering pentru MinMax.
	generateBlackQueenMoves(configurationMetadata, moves);
	generateBlackRookMoves(configurationMetadata, moves);
	generateBlackBishopMoves(configurationMetadata, moves);
	generateBlackKnightMoves(configurationMetadata, moves);
	generateBlackPawnMoves(configurationMetadata, moves);
	generateBlackKingMoves(configurationMetadata, moves);
}

// Checkmates

bool BoardManager::isWhiteKingInCheckmate(ConfigurationMetadata& configurationMetadata)
{
	std::vector<std::vector<std::pair<char, int>>> moves;
	this->generateWhiteMoves(configurationMetadata, moves);

	return moves.empty() && this->isWhiteKingInCheck(configurationMetadata);
}

bool BoardManager::isBlackKingInCheckmate(ConfigurationMetadata& configurationMetadata)
{
	std::vector<std::vector<std::pair<char, int>>> moves;
	this->generateBlackMoves(configurationMetadata, moves);

	return moves.empty() && this->isBlackKingInCheck(configurationMetadata);
}

// Draws

bool BoardManager::isWhiteKingInDraw(ConfigurationMetadata& configurationMetadata)
{
	std::vector<std::vector<std::pair<char, int>>> moves;
	this->generateWhiteMoves(configurationMetadata, moves);

	return moves.empty() && !this->isWhiteKingInCheck(configurationMetadata);
}

bool BoardManager::isBlackKingInDraw(ConfigurationMetadata& configurationMetadata)
{
	std::vector<std::vector<std::pair<char, int>>> moves;
	this->generateBlackMoves(configurationMetadata, moves);

	return moves.empty() && !this->isBlackKingInCheck(configurationMetadata);
}

//

int BoardManager::getGeneratedWhiteMovesCount(ConfigurationMetadata& configurationMetadata)
{
	std::vector<std::vector<std::pair<char, int>>> moves;
	this->generateWhiteMoves(configurationMetadata, moves);

	return (int)moves.size();
}

int BoardManager::getGeneratedBlackMovesCount(ConfigurationMetadata& configurationMetadata)
{
	std::vector<std::vector<std::pair<char, int>>> moves;
	this->generateBlackMoves(configurationMetadata, moves);

	return (int)moves.size();
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

	std::cout << std::endl;
}

ConfigurationMetadata::ConfigurationMetadata(const std::string& configurationString)
{
	this->whitePawns = 0ull;
	this->whiteRooks = 0ull;
	this->whiteKnights = 0ull;
	this->whiteBishops = 0ull;
	this->whiteQueens = 0ull;
	this->whiteKing = 0ull;

	this->blackPawns = 0ull;
	this->blackRooks = 0ull;
	this->blackKnights = 0ull;
	this->blackBishops = 0ull;
	this->blackQueens = 0ull;
	this->blackKing = 0ull;

	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		if (configurationString[i] == '.')
			continue;

		if (configurationString[i] == 'P')
			this->whitePawns |= (1ull << i);
		else if (configurationString[i] == 'R')
			this->whiteRooks |= (1ull << i);
		else if (configurationString[i] == 'N')
			this->whiteKnights |= (1ull << i);
		else if (configurationString[i] == 'B')
			this->whiteBishops |= (1ull << i);
		else if (configurationString[i] == 'Q')
			this->whiteQueens |= (1ull << i);
		else if (configurationString[i] == 'K')
			this->whiteKing |= (1ull << i);
		else if (configurationString[i] == 'p')
			this->blackPawns |= (1ull << i);
		else if (configurationString[i] == 'r')
			this->blackRooks |= (1ull << i);
		else if (configurationString[i] == 'n')
			this->blackKnights |= (1ull << i);
		else if (configurationString[i] == 'b')
			this->blackBishops |= (1ull << i);
		else if (configurationString[i] == 'q')
			this->blackQueens |= (1ull << i);
		else if (configurationString[i] == 'k')
			this->blackKing |= (1ull << i);
	}

	this->whiteTurn = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] - '0');

	this->capturableEnPassantPosition = (configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 1] - '0') * 10 + (configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 2] - '0');

	this->whiteKingMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 3] - '0');
	this->whiteRookBottomLeftMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 4] - '0');
	this->whiteRookBottomRightMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 5] - '0');

	this->blackKingMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 6] - '0');
	this->blackRookTopLeftMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 7] - '0');
	this->blackRookTopRightMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 8] - '0');

	//

	this->allWhitePieces = this->whitePawns | this->whiteRooks | this->whiteKnights | this->whiteBishops | this->whiteQueens | this->whiteKing;

	this->allBlackPieces = this->blackPawns | this->blackRooks | this->blackKnights | this->blackBishops | this->blackQueens | this->blackKing;

	this->allPieces = this->allWhitePieces | this->allBlackPieces;

	this->emptyTiles = (~this->allPieces);
}

ConfigurationMetadata::ConfigurationMetadata(const ConfigurationMetadata& configurationMetadata)
	: whitePawns(configurationMetadata.whitePawns)
	, whiteRooks(configurationMetadata.whiteRooks)
	, whiteKnights(configurationMetadata.whiteKnights)
	, whiteBishops(configurationMetadata.whiteBishops)
	, whiteQueens(configurationMetadata.whiteQueens)
	, whiteKing(configurationMetadata.whiteKing)

	, blackPawns(configurationMetadata.blackPawns)
	, blackRooks(configurationMetadata.blackRooks)
	, blackKnights(configurationMetadata.blackKnights)
	, blackBishops(configurationMetadata.blackBishops)
	, blackQueens(configurationMetadata.blackQueens)
	, blackKing(configurationMetadata.blackKing)

	, whiteTurn(configurationMetadata.whiteTurn)

	, capturableEnPassantPosition(configurationMetadata.capturableEnPassantPosition)

	, whiteKingMoved(configurationMetadata.whiteKingMoved)
	, whiteRookBottomLeftMoved(configurationMetadata.whiteRookBottomLeftMoved)
	, whiteRookBottomRightMoved(configurationMetadata.whiteRookBottomRightMoved)

	, blackKingMoved(configurationMetadata.blackKingMoved)
	, blackRookTopLeftMoved(configurationMetadata.blackRookTopLeftMoved)
	, blackRookTopRightMoved(configurationMetadata.blackRookTopRightMoved)
{
	this->allWhitePieces = this->whitePawns | this->whiteRooks | this->whiteKnights | this->whiteBishops | this->whiteQueens | this->whiteKing;

	this->allBlackPieces = this->blackPawns | this->blackRooks | this->blackKnights | this->blackBishops | this->blackQueens | this->blackKing;

	this->allPieces = this->allWhitePieces | this->allBlackPieces;

	this->emptyTiles = (~this->allPieces);
}

void ConfigurationMetadata::initialize(const std::string& configurationString)
{
	this->whitePawns = 0ull;
	this->whiteRooks = 0ull;
	this->whiteKnights = 0ull;
	this->whiteBishops = 0ull;
	this->whiteQueens = 0ull;
	this->whiteKing = 0ull;

	this->blackPawns = 0ull;
	this->blackRooks = 0ull;
	this->blackKnights = 0ull;
	this->blackBishops = 0ull;
	this->blackQueens = 0ull;
	this->blackKing = 0ull;

	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		if (configurationString[i] == '.')
			continue;

		if (configurationString[i] == 'P')
			this->whitePawns |= (1ull << i);
		else if (configurationString[i] == 'R')
			this->whiteRooks |= (1ull << i);
		else if (configurationString[i] == 'N')
			this->whiteKnights |= (1ull << i);
		else if (configurationString[i] == 'B')
			this->whiteBishops |= (1ull << i);
		else if (configurationString[i] == 'Q')
			this->whiteQueens |= (1ull << i);
		else if (configurationString[i] == 'K')
			this->whiteKing |= (1ull << i);
		else if (configurationString[i] == 'p')
			this->blackPawns |= (1ull << i);
		else if (configurationString[i] == 'r')
			this->blackRooks |= (1ull << i);
		else if (configurationString[i] == 'n')
			this->blackKnights |= (1ull << i);
		else if (configurationString[i] == 'b')
			this->blackBishops |= (1ull << i);
		else if (configurationString[i] == 'q')
			this->blackQueens |= (1ull << i);
		else if (configurationString[i] == 'k')
			this->blackKing |= (1ull << i);
	}

	this->whiteTurn = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] - '0');

	this->capturableEnPassantPosition = (configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 1] - '0') * 10 + (configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 2] - '0');

	this->whiteKingMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 3] - '0');
	this->whiteRookBottomLeftMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 4] - '0');
	this->whiteRookBottomRightMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 5] - '0');

	this->blackKingMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 6] - '0');
	this->blackRookTopLeftMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 7] - '0');
	this->blackRookTopRightMoved = (bool)(configurationString[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + 8] - '0');

	//

	this->allWhitePieces = this->whitePawns | this->whiteRooks | this->whiteKnights | this->whiteBishops | this->whiteQueens | this->whiteKing;

	this->allBlackPieces = this->blackPawns | this->blackRooks | this->blackKnights | this->blackBishops | this->blackQueens | this->blackKing;

	this->allPieces = this->allWhitePieces | this->allBlackPieces;

	this->emptyTiles = (~this->allPieces);
}

void ConfigurationMetadata::initialize(const ConfigurationMetadata& configurationMetadata)
{
	this->whitePawns = configurationMetadata.whitePawns;
	this->whiteRooks = configurationMetadata.whiteRooks;
	this->whiteKnights = configurationMetadata.whiteKnights;
	this->whiteBishops = configurationMetadata.whiteBishops;
	this->whiteQueens = configurationMetadata.whiteQueens;
	this->whiteKing = configurationMetadata.whiteKing;

	this->blackPawns = configurationMetadata.blackPawns;
	this->blackRooks = configurationMetadata.blackRooks;
	this->blackKnights = configurationMetadata.blackKnights;
	this->blackBishops = configurationMetadata.blackBishops;
	this->blackQueens = configurationMetadata.blackQueens;
	this->blackKing = configurationMetadata.blackKing;

	this->whiteTurn = configurationMetadata.whiteTurn;

	this->capturableEnPassantPosition = configurationMetadata.capturableEnPassantPosition;

	this->whiteKingMoved = configurationMetadata.whiteKingMoved;
	this->whiteRookBottomLeftMoved = configurationMetadata.whiteRookBottomLeftMoved;
	this->whiteRookBottomRightMoved = configurationMetadata.whiteRookBottomRightMoved;

	this->blackKingMoved = configurationMetadata.blackKingMoved;
	this->blackRookTopLeftMoved = configurationMetadata.blackRookTopLeftMoved;
	this->blackRookTopRightMoved = configurationMetadata.blackRookTopRightMoved;



	this->allWhitePieces = this->whitePawns | this->whiteRooks | this->whiteKnights | this->whiteBishops | this->whiteQueens | this->whiteKing;

	this->allBlackPieces = this->blackPawns | this->blackRooks | this->blackKnights | this->blackBishops | this->blackQueens | this->blackKing;

	this->allPieces = this->allWhitePieces | this->allBlackPieces;

	this->emptyTiles = (~this->allPieces);
}




