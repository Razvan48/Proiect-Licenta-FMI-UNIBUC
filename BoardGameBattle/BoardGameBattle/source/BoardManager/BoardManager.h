#pragma once

#include <string>
#include <vector>

#include "../GameMetadata/GameMetadata.h"

class BoardManager
{
private:
	BoardManager();
	~BoardManager();
	BoardManager(const BoardManager& other) = delete;
	BoardManager& operator= (const BoardManager& other) = delete;
	BoardManager(const BoardManager&& other) = delete;
	BoardManager& operator= (const BoardManager&& other) = delete;


	unsigned long long rankBitMasks[GameMetadata::NUM_TILES_HEIGHT];
	unsigned long long fileBitMasks[GameMetadata::NUM_TILES_WIDTH];

	unsigned long long precalculatedKingAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long precalculatedKnightAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];


	std::string piecesConfiguration;

	bool whiteTurn;

	void printBitBoard(unsigned long long bitBoard) const;

	struct ConfigurationMetadata
	{
		unsigned long long whitePawnAttackZone;
		unsigned long long whiteRookAttackZone;
		unsigned long long whiteKnightAttackZone;
		unsigned long long whiteBishopAttackZone;
		unsigned long long whiteQueenAttackZone;
		unsigned long long whiteKingAttackZone;

		unsigned long long blackPawnAttackZone;
		unsigned long long blackRookAttackZone;
		unsigned long long blackKnightAttackZone;
		unsigned long long blackBishopAttackZone;
		unsigned long long blackQueenAttackZone;
		unsigned long long blackKingAttackZone;

		unsigned long long whiteAttackZones;
		unsigned long long blackAttackZones;

		unsigned long long allWhitePieces;
		unsigned long long allBlackPieces;

		unsigned long long allPieces;
		unsigned long long emptyTiles;

		unsigned long long piecesPinnedFromLeft;
		unsigned long long piecesPinnedFromRight;
		unsigned long long piecesPinnedFromTop;
		unsigned long long piecesPinnedFromBottom;

		unsigned long long piecesPinnedFromTopRight;
		unsigned long long piecesPinnedFromBottomLeft;

		unsigned long long piecesPinnedFromTopLeft;
		unsigned long long piecesPinnedFromBottomRight;
	};


public:
	static BoardManager& get();

	void initialize();

	inline std::string getPiecesConfiguration() const { return this->piecesConfiguration; }
	void setPiecesConfiguration(const std::string& piecesConfiguration);

	void applyMove(const std::string& move);
	std::vector<std::string> generateMovesForPiecePosition(const std::string& piecePosition);

	inline bool getWhiteTurn() const { return this->whiteTurn; }
};