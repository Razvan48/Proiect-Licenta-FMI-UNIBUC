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

	static const int MODULO_LOG_POWER_2 = 67;
	int logPower2[BoardManager::MODULO_LOG_POWER_2];

	unsigned long long rankBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long fileBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long topLeftBottomRightDiagonalBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long topRightBottomLeftDiagonalBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	unsigned long long precalculatedKingAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long precalculatedKnightAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	std::pair<unsigned long long, unsigned long long> precalculatedRankAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH];
	std::pair<unsigned long long, unsigned long long> precalculatedFileAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, unsigned long long> precalculatedTopLeftBottomRightDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, unsigned long long> precalculatedTopRightBottomLeftDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];

	unsigned long long precalculatedEmptyCastleTopLeft;
	unsigned long long precalculatedEmptyCastleTopRight;
	unsigned long long precalculatedEmptyCastleBottomLeft;
	unsigned long long precalculatedEmptyCastleBottomRight;

	unsigned long long precalculatedFullCastleTopLeft;
	unsigned long long precalculatedFullCastleTopRight;
	unsigned long long precalculatedFullCastleBottomLeft;
	unsigned long long precalculatedFullCastleBottomRight;

	std::string piecesConfiguration;

	struct ConfigurationMetadata
	{
		unsigned long long whitePawns;
		unsigned long long whiteRooks;
		unsigned long long whiteKnights;
		unsigned long long whiteBishops;
		unsigned long long whiteQueens;
		unsigned long long whiteKings;

		unsigned long long blackPawns;
		unsigned long long blackRooks;
		unsigned long long blackKnights;
		unsigned long long blackBishops;
		unsigned long long blackQueens;
		unsigned long long blackKings;

		bool whiteTurn;

		int capturableEnPassantPosition;

		bool whiteKingMoved;
		bool whiteRookBottomLeftMoved;
		bool whiteRookBottomRightMoved;

		bool blackKingMoved;
		bool blackRookTopLeftMoved;
		bool blackRookTopRightMoved;

		//

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

		unsigned long long whitePiecesPinnedFromLeft;
		unsigned long long whitePiecesPinnedFromRight;
		unsigned long long whitePiecesPinnedFromTop;
		unsigned long long whitePiecesPinnedFromBottom;

		unsigned long long whitePiecesPinnedFromTopRight;
		unsigned long long whitePiecesPinnedFromBottomLeft;

		unsigned long long whitePiecesPinnedFromTopLeft;
		unsigned long long whitePiecesPinnedFromBottomRight;

		unsigned long long blackPiecesPinnedFromLeft;
		unsigned long long blackPiecesPinnedFromRight;
		unsigned long long blackPiecesPinnedFromTop;
		unsigned long long blackPiecesPinnedFromBottom;

		unsigned long long blackPiecesPinnedFromTopRight;
		unsigned long long blackPiecesPinnedFromBottomLeft;

		unsigned long long blackPiecesPinnedFromTopLeft;
		unsigned long long blackPiecesPinnedFromBottomRight;
	};
	ConfigurationMetadata configurationMetadata;

	unsigned long long extractRank(unsigned long long bitBoard, int pos) const;
	unsigned long long extractFile(unsigned long long bitBoard, int pos) const;
	unsigned long long extractTopLeftBottomRightDiagonal(unsigned long long bitBoard, int pos) const;
	unsigned long long extractTopRightBottomLeftDiagonal(unsigned long long bitBoard, int pos) const;



	void generateWhitePawnAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateWhiteRookAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateWhiteKnightAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateWhiteBishopAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateWhiteQueenAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateWhiteKingAttackZone(ConfigurationMetadata& configurationMetadata);

	void generateBlackPawnAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateBlackRookAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateBlackKnightAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateBlackBishopAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateBlackQueenAttackZone(ConfigurationMetadata& configurationMetadata);
	void generateBlackKingAttackZone(ConfigurationMetadata& configurationMetadata);

	void generateWhiteAttackZones(ConfigurationMetadata& configurationMetadata);
	void generateBlackAttackZones(ConfigurationMetadata& configurationMetadata);

	//

	void generateWhitePawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateWhiteRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateWhiteKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateWhiteBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateWhiteQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateWhiteKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);

	void generateBlackPawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateBlackRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateBlackKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateBlackBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateBlackQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateBlackKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);

	void generateWhiteMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);
	void generateBlackMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::string>& moves);

public:
	static BoardManager& get();

	void initialize();

	inline std::string getPiecesConfiguration() const { return this->piecesConfiguration; }
	void setPiecesConfiguration(const std::string& piecesConfiguration);

	void applyMove(const std::string& move);
	std::vector<std::string> generateMovesForPiecePosition(const std::string& piecePosition);

	inline bool getWhiteTurn() const { return this->configurationMetadata.whiteTurn; }

	void printBitBoard(unsigned long long bitBoard) const;
};

