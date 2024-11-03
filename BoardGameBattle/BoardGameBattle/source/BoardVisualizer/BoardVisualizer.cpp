#include "BoardVisualizer.h"

#include "../WindowManager/WindowManager.h"
#include "../Game/Game.h"

#include "../BoardManager/BoardManager.h"

#include "../Entity/TexturableEntity/TexturableEntity.h"

#include <iostream>

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

}

BoardVisualizer::~BoardVisualizer()
{

}

BoardVisualizer& BoardVisualizer::get()
{
	static BoardVisualizer instance;
	return instance;
}

void BoardVisualizer::initialize()
{
	this->boardTiles.clear();
	this->boardCoordinates.clear();

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

	if (Game::get().getColor() == Game::Color::BLACK) // rotire 180 de grade
	{
		for (int i = 0; i < this->NUM_TILES_HEIGHT; ++i)
		{
			for (int j = 0; j < this->NUM_TILES_WIDTH; ++j)
			{
				this->boardTiles[i][j].setRotateAngle(180.0f);
				this->boardTiles[i][j].setPosCenterX(
					(this->NUM_TILES_WIDTH - j) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f
				);
				this->boardTiles[i][j].setPosCenterY(
					(this->NUM_TILES_HEIGHT - i) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f
				);
			}
		}

		for (int j = 0; j < this->NUM_TILES_WIDTH; ++j)
		{
			this->boardCoordinates[j].setRotateAngle(180.0f);
			this->boardCoordinates[j].setPosCenterX(
				(this->NUM_TILES_WIDTH - j) * this->BOARD_TILE_WIDTH + this->BOARD_TILE_WIDTH / 2.0f
			);
		}

		for (int i = 0; i < this->NUM_TILES_HEIGHT; ++i)
		{
			this->boardCoordinates[this->NUM_TILES_WIDTH + i].setRotateAngle(180.0f);
			this->boardCoordinates[this->NUM_TILES_WIDTH + i].setPosCenterY(
				(this->NUM_TILES_HEIGHT - i) * this->BOARD_TILE_HEIGHT + this->BOARD_TILE_HEIGHT / 2.0f
			);
		}
	}
	else if (Game::get().getColor() == Game::Color::NONE) // poate fi eliminat ulterior
	{
		std::cout << "Error: Game Color is NONE when initializing board" << std::endl;
	}
}

void BoardVisualizer::draw()
{
	for (int i = 0; i < this->NUM_TILES_HEIGHT; ++i)
	{
		for (int j = 0; j < this->NUM_TILES_WIDTH; ++j)
		{
			this->boardTiles[i][j].draw();
		}
	}

	for (int i = 0; i < this->boardCoordinates.size(); ++i)
	{
		this->boardCoordinates[i].draw();
	}

	// Desenarea pieselor
	std::string piecesConfiguration = BoardManager::get().getPiecesConfigurationForVisualizing();

	TexturableEntity currentPiece(
		0.0f,
		0.0f,
		this->BOARD_TILE_WIDTH,
		this->BOARD_TILE_HEIGHT,
		0.0f,
		""
	);
	for (int i = 0; i < piecesConfiguration.size(); ++i)
	{
		int currentI = i / this->NUM_TILES_WIDTH;
		int currentJ = i % this->NUM_TILES_WIDTH;
		currentPiece.setPosCenterX(this->boardTiles[currentI][currentJ].getPosCenterX());
		currentPiece.setPosCenterY(this->boardTiles[currentI][currentJ].getPosCenterY());
		currentPiece.setRotateAngle(this->boardTiles[currentI][currentJ].getRotateAngle());

		switch (piecesConfiguration[i])
		{
		case 'K':
			currentPiece.setTextureName(this->whiteKingTextureName);
			break;
		case 'Q':
			currentPiece.setTextureName(this->whiteQueenTextureName);
			break;
		case 'R':
			currentPiece.setTextureName(this->whiteRookTextureName);
			break;
		case 'B':
			currentPiece.setTextureName(this->whiteBishopTextureName);
			break;
		case 'N':
			currentPiece.setTextureName(this->whiteKnightTextureName);
			break;
		case 'P':
			currentPiece.setTextureName(this->whitePawnTextureName);
			break;
		case 'k':
			currentPiece.setTextureName(this->blackKingTextureName);
			break;
		case 'q':
			currentPiece.setTextureName(this->blackQueenTextureName);
			break;
		case 'r':
			currentPiece.setTextureName(this->blackRookTextureName);
			break;
		case 'b':
			currentPiece.setTextureName(this->blackBishopTextureName);
			break;
		case 'n':
			currentPiece.setTextureName(this->blackKnightTextureName);
			break;
		case 'p':
			currentPiece.setTextureName(this->blackPawnTextureName);
			break;
		default:
			std::cout << "Error: Invalid character in pieces configuration received in board visualizer from board manager" << std::endl;
			break;
		}

		currentPiece.draw();
	}
}

void BoardVisualizer::update()
{
	// TODO: selected tiles logic
}



