#include "BoardManager.h"

#include "../GameMetadata/GameMetadata.h"

BoardManager::BoardManager()
{
	this->piecesConfigurationForVisualizing = R"(
									rnbqkbnr
									pppppppp
									........
									........
									........
									........
									PPPPPPPP
									RNBQKBNR
								)";
}

BoardManager::~BoardManager()
{

}

BoardManager& BoardManager::get()
{
	static BoardManager instance;
	return instance;
}

// TODO: COD DOAR DE TEST, VA FI INLOCUIT
void BoardManager::applyMove(const std::string& move) // move e de forma "caracterPiesa + col + linie + col + linie + $ + alta mutare (de exemplu in cazul rocadei sau promovarii de pion)"
{
	for (int i = 0; i < move.size(); i += (GameMetadata::NUM_CHARS_SUBMOVE + 1))
	{
		char gamePiece = move[i];
		int columnStart = move[i + 1] - 'a';
		int rowStart = move[i + 2] - '1';
		int columnEnd = move[i + 3] - 'a';
		int rowEnd = move[i + 4] - '1';

		this->piecesConfigurationForVisualizing[(GameMetadata::NUM_TILES_HEIGHT - 1 - rowStart) * GameMetadata::NUM_TILES_WIDTH + columnStart] = '.';
		this->piecesConfigurationForVisualizing[(GameMetadata::NUM_TILES_HEIGHT - 1 - rowEnd) * GameMetadata::NUM_TILES_WIDTH + columnEnd] = gamePiece;
	}
}

// TODO: COD DOAR DE TEST, VA FI INLOCUIT
std::vector<std::string> BoardManager::generateMovesForPiecePosition(const std::string& piecePosition)
{
	std::vector<std::string> moves;

	int column = piecePosition[0] - 'a';
	int row = piecePosition[1] - '1';
	char gamePiece = this->piecesConfigurationForVisualizing[(GameMetadata::NUM_TILES_HEIGHT - 1 - row) * GameMetadata::NUM_TILES_WIDTH + column];

	if ('a' <= gamePiece && gamePiece <= 'z' && row >= 1) // Piesa Neagra
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row - 1));
		moves.back().push_back('$');
	}
	if ('a' <= gamePiece && gamePiece <= 'z' && row >= 2)
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row - 2));
		moves.back().push_back('$');
	}
	if ('A' <= gamePiece && gamePiece <= 'Z' && row <= 6) // Piesa Alba
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row + 1));
		moves.back().push_back('$');
	}
	if ('A' <= gamePiece && gamePiece <= 'Z' && row <= 5)
	{
		moves.emplace_back();
		moves.back().push_back(gamePiece);
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row));
		moves.back().push_back((char)('a' + column));
		moves.back().push_back((char)('1' + row + 2));
		moves.back().push_back('$');
	}

	return moves;
}



