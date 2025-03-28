#pragma once

#include "../Entity/BoardTile/BoardTile.h"
#include "../Entity/TextEntity/TextEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class BoardVisualizer
{
private:
	BoardVisualizer();
	~BoardVisualizer();
	BoardVisualizer(const BoardVisualizer& other) = delete;
	BoardVisualizer& operator= (const BoardVisualizer& other) = delete;
	BoardVisualizer(const BoardVisualizer&& other) = delete;
	BoardVisualizer& operator= (const BoardVisualizer&& other) = delete;

	std::string whiteBoardTileTextureName;
	std::string blackBoardTileTextureName;
	std::string selectedBoardTileTextureName;

	std::string whiteKingTextureName;
	std::string whiteQueenTextureName;
	std::string whiteRookTextureName;
	std::string whiteBishopTextureName;
	std::string whiteKnightTextureName;
	std::string whitePawnTextureName;

	std::string blackKingTextureName;
	std::string blackQueenTextureName;
	std::string blackRookTextureName;
	std::string blackBishopTextureName;
	std::string blackKnightTextureName;
	std::string blackPawnTextureName;

	std::vector<std::vector<BoardTile>> boardTiles;
	std::vector<TextEntity> boardCoordinates;

	const float BOARD_TILE_WIDTH;
	const float BOARD_TILE_HEIGHT;

	int selectedTileRow;
	int selectedTileColumn;

	std::string pieceMoveSoundName;

	std::vector<std::pair<std::string, std::pair<float, bool>>> movesHistory;
	bool newMoveAtTopOfHistory;

	bool gameHasEnded;

	bool pawnPromotionMenuActive;

	bool estimationCalculated;
	float estimation;

	void resetSelectedTiles();

public:
	static BoardVisualizer& get();

	void initialize();
	void draw();
	void update();

	void addNewMoveInHistory(const std::string& move);
	void popLastMoveFromHistory();
	inline std::string getLastMoveFromHistory() const
	{
		if (this->movesHistory.empty())
			return "";
		return this->movesHistory.back().first;
	}
	inline int getLastMoveFromHistorySize() const { return (int)this->movesHistory.size(); }

	inline bool getPawnPromotionMenuActive() const { return this->pawnPromotionMenuActive; }
	inline void setPawnPromotionMenuActive(bool pawnPromotionMenuActive) { this->pawnPromotionMenuActive = pawnPromotionMenuActive; }

	void sendMoveToBoardManager(const std::string& move);
};