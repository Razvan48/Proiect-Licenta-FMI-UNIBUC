#include "BoardVisualizer.h"

#include "../WindowManager/WindowManager.h"
#include "../Game/Game.h"

BoardVisualizer::BoardVisualizer()
	: whiteBoardTileTextureName("whiteBoardTileTexture")
	, blackBoardTileTextureName("blackBoardTileTexture")
	, boardTileSelectedTextureName("boardTileSelectedTexture")
	, whiteKingTextureName("whiteKingTexture")
	, whiteQueenTextureName("whiteQueenTexture")
	, whiteRookTextureName("whiteRookTexture")
	, whiteBishopTextureName("whiteBishopTexture")
	, whiteKnightTextureName("whiteKnightTexture")
	, whitePawnTextureName("whitePawnTexture")
	, blackKingTextureName("blackKingTexture")
	, blackQueenTextureName("blackQueenTexture")
	, blackRookTextureName("blackRookTexture")
	, blackBishopTextureName("blackBishopTexture")
	, blackKnightTextureName("blackKnightTexture")
	, blackPawnTextureName("blackPawnTexture")
	, NUM_TILES_WIDTH(8)
	, NUM_TILES_HEIGHT(8)
	, BOARD_TILE_WIDTH(1.0f * WindowManager::get().getWindowWidth() / 10.0f)
	, BOARD_TILE_HEIGHT(1.0f * WindowManager::get().getWindowHeight() / 10.0f)
{
	this->boardTiles.resize(this->NUM_TILES_HEIGHT);
	for (int i = 0; i < this->NUM_TILES_HEIGHT; ++i)
	{
		for (int j = 0; j < this->NUM_TILES_WIDTH; ++j)
		{
			this->boardTiles[i].push_back(
				BoardTile
				(
					(j + 1) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f,
					(i + 1) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f,
					this->BOARD_TILE_WIDTH,
					this->BOARD_TILE_HEIGHT,
					0.0f,
					(j + i) % 2 ? this->blackBoardTileTextureName : this->whiteBoardTileTextureName
				)
			);
		}
	}

	// a, b, c, d, e, f, g, h
	for (int j = 0; j < this->NUM_TILES_WIDTH; ++j)
	{
		this->boardCoordinates.push_back(
			TextEntity
			(
				(j + 1) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f,
				this->BOARD_TILE_HEIGHT / 2.0f,
				this->BOARD_TILE_WIDTH,
				this->BOARD_TILE_HEIGHT,
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				std::to_string((char)('a' + j))
			)
		);

	}

	// 1, 2, 3, 4, 5, 6, 7, 8
	for (int i = 0; i < this->NUM_TILES_HEIGHT; ++i)
	{
		this->boardCoordinates.push_back(
			TextEntity
			(
				this->BOARD_TILE_WIDTH / 2.0f,
				(i + 1) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f,
				this->BOARD_TILE_WIDTH,
				this->BOARD_TILE_HEIGHT,
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				std::to_string((char)('1' + i))
			)
		);
	}
}

BoardVisualizer::~BoardVisualizer()
{

}

BoardVisualizer& BoardVisualizer::get()
{
	static BoardVisualizer instance;
	return instance;
}

void BoardVisualizer::draw()
{
	// TODO:
}

void BoardVisualizer::update()
{
	// TODO:
}

