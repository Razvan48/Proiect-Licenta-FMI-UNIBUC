#pragma once

#include <string>
#include <vector>

class BoardManager
{
private:
	BoardManager();
	~BoardManager();
	BoardManager(const BoardManager& other) = delete;
	BoardManager& operator= (const BoardManager& other) = delete;
	BoardManager(const BoardManager&& other) = delete;
	BoardManager& operator= (const BoardManager&& other) = delete;


	std::string piecesConfiguration;

	bool whiteTurn;

public:
	static BoardManager& get();

	void initialize();

	inline std::string getPiecesConfiguration() const { return this->piecesConfiguration; }
	void setPiecesConfiguration(const std::string& piecesConfiguration);

	void applyMove(const std::string& move);
	std::vector<std::string> generateMovesForPiecePosition(const std::string& piecePosition);

	inline bool getWhiteTurn() const { return this->whiteTurn; }
};