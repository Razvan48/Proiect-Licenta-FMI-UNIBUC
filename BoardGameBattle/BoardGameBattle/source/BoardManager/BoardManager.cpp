#include "BoardManager.h"

#include "../GameMetadata/GameMetadata.h"

#include "../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"

#include "../Game/Game.h"

#include "../Client/Client.h"

#include <iostream>


BoardManager::BoardManager()
	: piecesConfiguration("rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR1111w0000")
	, whiteTurn(true)
{

}

BoardManager::~BoardManager()
{

}

void BoardManager::initialize()
{
	this->piecesConfiguration = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR1111w0000";
	this->whiteTurn = true;
}

void BoardManager::setPiecesConfiguration(const std::string& piecesConfiguration)
{
	this->piecesConfiguration = piecesConfiguration;
	int whiteTurnPos = GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES;
	if (whiteTurnPos < this->piecesConfiguration.size())
		this->whiteTurn = (this->piecesConfiguration[whiteTurnPos] == 'w');
	else
	{
		std::cout << "Error: Could not find whose turn is it inside the Pieces Configuration in BoardManager::setPiecesConfiguration. Set to default to white." << std::endl;
	}
}

BoardManager& BoardManager::get()
{
	static BoardManager instance;
	return instance;
}


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

	if (this->whiteTurn)
		this->piecesConfiguration[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES] = 'w';
	else
		this->piecesConfiguration[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES] = 'b';

	if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::CREATE_GAME)
		CreatedMultiplayerGameVisualInterface::get()->setHasToSendBoardConfiguration(true);
	else if (Game::get().getMultiplayerStatus() == Game::MultiplayerStatus::JOIN_GAME)
		JoinedMultiplayerGameVisualInterface::get()->setHasToSendBoardConfiguration(true);
	// else Singleplayer, nu trebuie sa trimita nimic
}


std::vector<std::string> BoardManager::generateMovesForPiecePosition(const std::string& piecePosition)
{
	std::vector<std::string> moves;

	int column = piecePosition[0] - 'a';
	int row = piecePosition[1] - '1';
	char gamePiece = this->piecesConfiguration[(GameMetadata::NUM_TILES_HEIGHT - 1 - row) * GameMetadata::NUM_TILES_WIDTH + column];

	if (gamePiece == '.')
		return moves;

	if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
	{
		if (Game::get().getColor() == Game::Color::WHITE && ((!('A' <= gamePiece && gamePiece <= 'Z')) || (!this->whiteTurn)))
			return moves;

		if (Game::get().getColor() == Game::Color::BLACK && ((!('a' <= gamePiece && gamePiece <= 'z')) || this->whiteTurn))
			return moves;
	}
	else if (Game::get().getMode() == Game::Mode::MULTIPLAYER)
	{
		if (Client::get().getColor() == "") // Suntem in Multiplayer si nu stim culoarea
			return moves;

		if (Client::get().getColor() == "white" && ((!('A' <= gamePiece && gamePiece <= 'Z')) || (!this->whiteTurn)))
			return moves;

		if (Client::get().getColor() == "black" && ((!('a' <= gamePiece && gamePiece <= 'z')) || this->whiteTurn))
			return moves;
	}
	else // Game::Mode::NONE
	{
		std::cout << "Error: Game Mode not set when generating moves for piece position" << std::endl;
		return moves;
	}

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



