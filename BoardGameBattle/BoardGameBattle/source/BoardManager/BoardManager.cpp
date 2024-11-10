#include "BoardManager.h"

#include "../GameMetadata/GameMetadata.h"

BoardManager::BoardManager()
	: piecesConfiguration("rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR")
	, whiteTurn(true)
{

}

BoardManager::~BoardManager()
{

}

void BoardManager::initialize()
{
	this->piecesConfiguration = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR";
}

BoardManager& BoardManager::get()
{
	static BoardManager instance;
	return instance;
}

// TODO: COD DOAR DE TEST, VA FI INLOCUIT
void BoardManager::applyMove(const std::string& move) // Face presupunerea ca mutarea este legala
{
	for (int i = 0; i < move.size(); i += GameMetadata::NUM_CHARS_SUBMOVE)
	{
		char gamePiece = move[i];
		int columnStart = move[i + 1] - 'a';
		int rowStart = move[i + 2] - '1';
		int columnEnd = move[i + 3] - 'a';
		int rowEnd = move[i + 4] - '1';

		this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - rowStart) * GameMetadata::NUM_TILES_WIDTH + columnStart] = '.';
		this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - rowEnd) * GameMetadata::NUM_TILES_WIDTH + columnEnd] = gamePiece;
	}

	this->whiteTurn = !this->whiteTurn;
}

// TODO: COD DOAR DE TEST, VA FI INLOCUIT
std::vector<std::string> BoardManager::generateMovesForPiecePosition(const std::string& piecePosition)
{
	std::vector<std::string> moves;

	int column = piecePosition[0] - 'a';
	int row = piecePosition[1] - '1';
	char gamePiece = this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - row) * GameMetadata::NUM_TILES_WIDTH + column];

	if (gamePiece == '.'
		||
		('A' <= gamePiece && gamePiece <= 'Z' && !this->whiteTurn)
		||
		('a' <= gamePiece && gamePiece <= 'z' && this->whiteTurn)
		)
		return moves;

	if ('A' <= gamePiece && gamePiece <= 'Z' && row <= 6) // Piesa Alba
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row + 1));
	}
	if ('A' <= gamePiece && gamePiece <= 'Z' && row <= 5)
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row + 2));
	}

	if ('a' <= gamePiece && gamePiece <= 'z' && row >= 1) // Piesa Neagra
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row - 1));
	}
	if ('a' <= gamePiece && gamePiece <= 'z' && row >= 2)
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row - 2));
	}

	return moves;
}



