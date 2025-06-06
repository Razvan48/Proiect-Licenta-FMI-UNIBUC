#pragma once

#include <string>
#include <vector>
#include <map>

#include "../GameMetadata/GameMetadata.h"

struct ConfigurationMetadata
{
	unsigned long long whitePawns;
	unsigned long long whiteRooks;
	unsigned long long whiteKnights;
	unsigned long long whiteBishops;
	unsigned long long whiteQueens;
	unsigned long long whiteKing;

	unsigned long long blackPawns;
	unsigned long long blackRooks;
	unsigned long long blackKnights;
	unsigned long long blackBishops;
	unsigned long long blackQueens;
	unsigned long long blackKing;

	bool whiteTurn;

	int capturableEnPassantPosition;

	bool whiteKingMoved;
	bool whiteRookBottomLeftMoved;
	bool whiteRookBottomRightMoved;

	bool blackKingMoved;
	bool blackRookTopLeftMoved;
	bool blackRookTopRightMoved;

	//

	unsigned long long allWhitePieces;
	unsigned long long allBlackPieces;

	unsigned long long allPieces;
	unsigned long long emptyTiles;

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

	unsigned long long whitePiecesPinnedOnRank;
	unsigned long long whitePiecesPinnedOnFile;
	unsigned long long whitePiecesPinnedOnTopLeftBottomRightDiagonal;
	unsigned long long whitePiecesPinnedOnTopRightBottomLeftDiagonal;

	unsigned long long blackPiecesPinnedOnRank;
	unsigned long long blackPiecesPinnedOnFile;
	unsigned long long blackPiecesPinnedOnTopLeftBottomRightDiagonal;
	unsigned long long blackPiecesPinnedOnTopRightBottomLeftDiagonal;

	unsigned long long whiteKingDefenseZone;
	unsigned long long blackKingDefenseZone;

	//

	unsigned long long zobristHashingValue;

	//

	ConfigurationMetadata(const std::string& configurationString);
	ConfigurationMetadata(const ConfigurationMetadata& configurationMetadata);

	ConfigurationMetadata() = default; // Doar in constructorul BoardManager-ului trebuie folosit, exista doar ca sa nu loopeze la infinit acel constructor.

	void initialize(const std::string& configurationString);
	void initialize(const ConfigurationMetadata& configurationMetadata);
};

class BoardManager
{
private:
	BoardManager();
	~BoardManager();
	BoardManager(const BoardManager& other) = delete;
	BoardManager& operator= (const BoardManager& other) = delete;
	BoardManager(const BoardManager&& other) = delete;
	BoardManager& operator= (const BoardManager&& other) = delete;

public:
	static const int MODULO_LOG_POWER_2 = 67;
	int logPower2[BoardManager::MODULO_LOG_POWER_2];

private:
	unsigned long long rankBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long fileBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long topLeftBottomRightDiagonalBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long topRightBottomLeftDiagonalBitMasks[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

public: // INFO: Functiile de mai jos pot fi folosite in evaluarea Greedy a agentilor.
	inline unsigned long long getRankBitMasks(int pos) const { return this->rankBitMasks[pos]; }
	inline unsigned long long getFileBitMasks(int pos) const { return this->fileBitMasks[pos]; }
	inline unsigned long long getTopLeftBottomRightDiagonalBitMasks(int pos) const { return this->topLeftBottomRightDiagonalBitMasks[pos]; }
	inline unsigned long long getTopRightBottomLeftDiagonalBitMasks(int pos) const { return this->topRightBottomLeftDiagonalBitMasks[pos]; }

private:
	std::pair<unsigned long long, unsigned long long> precalculatedNearestPiecesOnRank[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH];
	std::pair<unsigned long long, unsigned long long> precalculatedNearestPiecesOnTopLeftBottomRightDiagonal[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, unsigned long long> precalculatedNearestPiecesOnTopRightBottomLeftDiagonal[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];

	unsigned long long precalculatedRawLeftAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH];
	unsigned long long precalculatedRawRightAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH];
	unsigned long long precalculatedRawTopAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];
	unsigned long long precalculatedRawBottomAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];
	unsigned long long precalculatedRawTopLeftDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];
	unsigned long long precalculatedRawTopRightDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];
	unsigned long long precalculatedRawBottomLeftDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];
	unsigned long long precalculatedRawBottomRightDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT];

	unsigned long long precalculatedKingAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long precalculatedKnightAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedLeftAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedRightAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_WIDTH];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedTopAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedBottomAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedTopLeftDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedTopRightDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedBottomLeftDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];
	std::pair<unsigned long long, std::pair<unsigned long long, unsigned long long>> precalculatedBottomRightDiagonalAttackZones[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH][1 << GameMetadata::NUM_TILES_HEIGHT][1 << GameMetadata::NUM_TILES_HEIGHT];

	unsigned long long precalculatedEmptyCastleTopLeft;
	unsigned long long precalculatedEmptyCastleTopRight;
	unsigned long long precalculatedEmptyCastleBottomLeft;
	unsigned long long precalculatedEmptyCastleBottomRight;

	unsigned long long precalculatedFullCastleTopLeft;
	unsigned long long precalculatedFullCastleTopRight;
	unsigned long long precalculatedFullCastleBottomLeft;
	unsigned long long precalculatedFullCastleBottomRight;

	unsigned long long precalculatedUsefulCastleTopLeft;
	unsigned long long precalculatedUsefulCastleTopRight;
	unsigned long long precalculatedUsefulCastleBottomLeft;
	unsigned long long precalculatedUsefulCastleBottomRight;

	int whiteKingPos;
	int whiteRookBottomLeftPos;
	int whiteRookBottomRightPos;

	int blackKingPos;
	int blackRookTopLeftPos;
	int blackRookTopRightPos;

	// Zobrist Hashing
	
	unsigned long long zobristHashingWhitePawn[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingWhiteRook[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingWhiteKnight[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingWhiteBishop[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingWhiteQueen[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingWhiteKing[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	unsigned long long zobristHashingBlackPawn[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingBlackRook[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingBlackKnight[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingBlackBishop[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingBlackQueen[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	unsigned long long zobristHashingBlackKing[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	unsigned long long zobristHashingWhiteTurn;

	unsigned long long zobristHashingCapturableEnPassant[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	unsigned long long zobristHashingWhiteKingMoved;
	unsigned long long zobristHashingWhiteRookBottomLeftMoved;
	unsigned long long zobristHashingWhiteRookBottomRightMoved;

	unsigned long long zobristHashingBlackKingMoved;
	unsigned long long zobristHashingBlackRookTopLeftMoved;
	unsigned long long zobristHashingBlackRookTopRightMoved;

	void generateZobristHashing();

	std::map<unsigned long long, int> zobristHashingValuesFrequency;

public:
	void calculateZobristHashingValue(ConfigurationMetadata& configurationMetadata) const;

private:

	//

	ConfigurationMetadata configurationMetadata;

	std::vector<ConfigurationMetadata> configurationMetadataHistory;

public: // INFO: Functiile de mai jos pot fi folosite in evaluarea Greedy a agentilor.
	unsigned long long extractRank(unsigned long long bitBoard, int pos) const;
	unsigned long long extractFile(unsigned long long bitBoard, int pos) const;
	unsigned long long extractTopLeftBottomRightDiagonal(unsigned long long bitBoard, int pos) const;
	unsigned long long extractTopRightBottomLeftDiagonal(unsigned long long bitBoard, int pos) const;

	inline unsigned long long getPrecalculatedRawLeftAttackZones(int pos, unsigned long long rankBitMask) const { return this->precalculatedRawLeftAttackZones[pos][rankBitMask]; }
	inline unsigned long long getPrecalculatedRawRightAttackZones(int pos, unsigned long long rankBitMask) const { return this->precalculatedRawRightAttackZones[pos][rankBitMask]; }
	inline unsigned long long getPrecalculatedRawTopAttackZones(int pos, unsigned long long fileBitMask) const { return this->precalculatedRawTopAttackZones[pos][fileBitMask]; }
	inline unsigned long long getPrecalculatedRawBottomAttackZones(int pos, unsigned long long fileBitMask) const { return this->precalculatedRawBottomAttackZones[pos][fileBitMask]; }
	inline unsigned long long getPrecalculatedRawTopLeftDiagonalAttackZones(int pos, unsigned long long topLeftBottomRightDiagonalBitMask) const { return this->precalculatedRawTopLeftDiagonalAttackZones[pos][topLeftBottomRightDiagonalBitMask]; }
	inline unsigned long long getPrecalculatedRawTopRightDiagonalAttackZones(int pos, unsigned long long topRightBottomLeftDiagonalBitMask) const { return this->precalculatedRawTopRightDiagonalAttackZones[pos][topRightBottomLeftDiagonalBitMask]; }
	inline unsigned long long getPrecalculatedRawBottomLeftDiagonalAttackZones(int pos, unsigned long long topRightBottomLeftDiagonalBitMask) const { return this->precalculatedRawBottomLeftDiagonalAttackZones[pos][topRightBottomLeftDiagonalBitMask]; }
	inline unsigned long long getPrecalculatedRawBottomRightDiagonalAttackZones(int pos, unsigned long long topLeftBottomRightDiagonalBitMask) const { return this->precalculatedRawBottomRightDiagonalAttackZones[pos][topLeftBottomRightDiagonalBitMask]; }

	inline unsigned long long getPrecalculatedKingAttackZones(int pos) const { return this->precalculatedKingAttackZones[pos]; }
	inline unsigned long long getPrecalculatedKnightAttackZones(int pos) const { return this->precalculatedKnightAttackZones[pos]; }



private:
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

public:
	void generateWhiteAttackZones(ConfigurationMetadata& configurationMetadata);
	void generateBlackAttackZones(ConfigurationMetadata& configurationMetadata);

	//
private:
	void generateWhitePawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateWhiteRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateWhiteKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateWhiteBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateWhiteQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateWhiteKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);

	void generateBlackPawnMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateBlackRookMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateBlackKnightMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateBlackBishopMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateBlackQueenMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateBlackKingMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);

public:
	void filterMoves(std::vector<std::vector<std::pair<char, int>>>& moves) const;
	void generateWhiteMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);
	void generateBlackMoves(ConfigurationMetadata& configurationMetadata, std::vector<std::vector<std::pair<char, int>>>& moves);

	//
public:
	// INFO: Functiile de mai jos fac presupunerea ca attack (pentru ambele culori) si defense zone-urile sunt deja calculate.
	inline bool isWhiteKingInCheck(const ConfigurationMetadata& configurationMetadata) const { return (configurationMetadata.whiteKing & configurationMetadata.blackAttackZones); }
	inline bool isBlackKingInCheck(const ConfigurationMetadata& configurationMetadata) const { return (configurationMetadata.blackKing & configurationMetadata.whiteAttackZones); }

	bool isWhiteKingInCheckmate(ConfigurationMetadata& configurationMetadata);
	bool isBlackKingInCheckmate(ConfigurationMetadata& configurationMetadata);

	bool isWhiteKingInDraw(ConfigurationMetadata& configurationMetadata);
	bool isBlackKingInDraw(ConfigurationMetadata& configurationMetadata);

public: // INFO: Board Visualizer are nevoie pentru conversii.
	std::string convertToExternalMove(const std::vector<std::pair<char, int>>& internalMove) const;

public: // INFO: Trebuie sa fie publica pentru a putea fi apelata din GameAgent.
	ConfigurationMetadata& getConfigurationMetadata() { return this->configurationMetadata; }
	ConfigurationMetadata applyMoveInternal(const ConfigurationMetadata& configurationMetadata, const std::vector<std::pair<char, int>>& internalMove, std::map<unsigned long long, int>& zobristHashingValuesFrequency) const;

public:
	static BoardManager& get();

	void initialize();

	std::string getPiecesConfiguration() const;
	std::string getPiecesConfigurationAutomateGameplay() const;

	std::vector<std::pair<char, int>> convertToInternalMove(const std::string& externalMove) const;
	void applyMoveExternal(const std::string& externalMove);

	inline bool getWhiteTurn() const { return this->configurationMetadata.whiteTurn; }

	std::vector<std::string> generateMovesForPiecePosition(const std::string& piecePosition);

	void setPiecesConfiguration(const std::string& piecesConfiguration);

	inline void addNewConfigurationMetadataInHistory(const ConfigurationMetadata& configurationMetadata) { this->configurationMetadataHistory.push_back(configurationMetadata); } // INFO: Atentie ca nu populeaza map-ul de frecvente de hashing. Stiva de istoric nu contine niciodata configuratia curenta.
	void popLastConfigurationMetadataFromHistory();

	int getGeneratedWhiteMovesCount(ConfigurationMetadata& configurationMetadata);
	int getGeneratedBlackMovesCount(ConfigurationMetadata& configurationMetadata);

	inline bool isDrawByRepetition(ConfigurationMetadata& configurationMetadata) { return this->zobristHashingValuesFrequency[configurationMetadata.zobristHashingValue] >= GameMetadata::FREQUENCY_UNTIL_DRAW_REPETITION; }
	inline std::map<unsigned long long, int>& getZobristHashingValuesFrequency() { return this->zobristHashingValuesFrequency; }

	void printBitBoard(unsigned long long bitBoard) const;
};

