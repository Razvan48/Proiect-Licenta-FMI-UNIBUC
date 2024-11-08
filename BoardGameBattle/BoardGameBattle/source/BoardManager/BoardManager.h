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


	std::string piecesConfigurationForVisualizing;

public:
	static BoardManager& get();

	void initialize();
	inline std::string getPiecesConfigurationForVisualizing() const { return this->piecesConfigurationForVisualizing; }
	void applyMove(const std::string& move);
	std::vector<std::string> generateMovesForPiecePosition(const std::string& piecePosition);
};