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
	std::string boardTileSelectedTextureName;

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

	const int NUM_TILES_WIDTH;
	const int NUM_TILES_HEIGHT;

	const float BOARD_TILE_WIDTH;
	const float BOARD_TILE_HEIGHT;

public:
	static BoardVisualizer& get();

	void draw();
	void update();
};