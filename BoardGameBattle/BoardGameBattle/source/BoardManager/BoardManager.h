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


	long long rankBitMasks[GameMetadata::NUM_TILES_HEIGHT];
	long long fileBitMasks[GameMetadata::NUM_TILES_WIDTH];

	long long precalculatedKingAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	long long precalculatedKnightAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];


	std::string piecesConfiguration;

	bool whiteTurn;

	void printBitBoard(long long bitBoard) const;

public:
	static BoardManager& get();

	void initialize();

	inline std::string getPiecesConfiguration() const { return this->piecesConfiguration; }
	void setPiecesConfiguration(const std::string& piecesConfiguration);

	void applyMove(const std::string& move);
	std::vector<std::string> generateMovesForPiecePosition(const std::string& piecePosition);

	inline bool getWhiteTurn() const { return this->whiteTurn; }
};