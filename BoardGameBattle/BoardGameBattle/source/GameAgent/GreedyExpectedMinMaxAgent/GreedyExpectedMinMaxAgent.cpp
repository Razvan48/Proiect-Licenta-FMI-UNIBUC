#include "GreedyExpectedMinMaxAgent.h"

#include "../../GameMetadata/GameMetadata.h"

#include <thread>

#include <future>

#include <iostream>

#include <map>

GreedyExpectedMinMaxAgent::GreedyExpectedMinMaxAgent()
	: GameAgent()
{

}

GreedyExpectedMinMaxAgent::~GreedyExpectedMinMaxAgent()
{

}

GreedyExpectedMinMaxAgent& GreedyExpectedMinMaxAgent::get()
{
	static GreedyExpectedMinMaxAgent instance;
	return instance;
}

float GreedyExpectedMinMaxAgent::evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const
{
	// INFO: Presupunem ca are deja attack zone-urile calculate corect pentru ambele culori.
	// INFO: evaluateConfiguration nu se ocupa cu starile de Checkmate, acelea sunt gestionate in minMax.

	float evaluationScore = 0.0f;

	// White Pieces

	unsigned long long whitePawns = configurationMetadata.whitePawns;
	while (whitePawns)
	{
		unsigned long long lsb = (whitePawns & ((~whitePawns) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::PAWN_SCORE + GreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whitePawns ^= lsb;
	}

	unsigned long long whiteRooks = configurationMetadata.whiteRooks;
	while (whiteRooks)
	{
		unsigned long long lsb = (whiteRooks & ((~whiteRooks) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::ROOK_SCORE + GreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteRooks ^= lsb;
	}

	unsigned long long whiteKnights = configurationMetadata.whiteKnights;
	while (whiteKnights)
	{
		unsigned long long lsb = (whiteKnights & ((~whiteKnights) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::KNIGHT_SCORE + GreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteKnights ^= lsb;
	}

	unsigned long long whiteBishops = configurationMetadata.whiteBishops;
	while (whiteBishops)
	{
		unsigned long long lsb = (whiteBishops & ((~whiteBishops) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::BISHOP_SCORE + GreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteBishops ^= lsb;
	}

	unsigned long long whiteQueens = configurationMetadata.whiteQueens;
	while (whiteQueens)
	{
		unsigned long long lsb = (whiteQueens & ((~whiteQueens) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::QUEEN_SCORE + GreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteQueens ^= lsb;
	}

	evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::KING_SCORE + GreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]];

	// Black Pieces

	unsigned long long blackPawns = configurationMetadata.blackPawns;
	while (blackPawns)
	{
		unsigned long long lsb = (blackPawns & ((~blackPawns) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::PAWN_SCORE - GreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackPawns ^= lsb;
	}

	unsigned long long blackRooks = configurationMetadata.blackRooks;
	while (blackRooks)
	{
		unsigned long long lsb = (blackRooks & ((~blackRooks) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::ROOK_SCORE - GreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackRooks ^= lsb;
	}

	unsigned long long blackKnights = configurationMetadata.blackKnights;
	while (blackKnights)
	{
		unsigned long long lsb = (blackKnights & ((~blackKnights) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::KNIGHT_SCORE - GreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackKnights ^= lsb;
	}

	unsigned long long blackBishops = configurationMetadata.blackBishops;
	while (blackBishops)
	{
		unsigned long long lsb = (blackBishops & ((~blackBishops) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::BISHOP_SCORE - GreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackBishops ^= lsb;
	}

	unsigned long long blackQueens = configurationMetadata.blackQueens;
	while (blackQueens)
	{
		unsigned long long lsb = (blackQueens & ((~blackQueens) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::QUEEN_SCORE - GreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackQueens ^= lsb;
	}

	evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::KING_SCORE - GreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]];

	// Attack Zones

	unsigned long long whiteAttackZones = configurationMetadata.whiteAttackZones;
	while (whiteAttackZones)
	{
		unsigned long long lsb = (whiteAttackZones & ((~whiteAttackZones) + 1));
		evaluationScore += GreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE;
		whiteAttackZones ^= lsb;
	}

	unsigned long long blackAttackZones = configurationMetadata.blackAttackZones;
	while (blackAttackZones)
	{
		unsigned long long lsb = (blackAttackZones & ((~blackAttackZones) + 1));
		evaluationScore -= GreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE;
		blackAttackZones ^= lsb;
	}

	// Checks
	if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
		evaluationScore -= GreedyExpectedMinMaxAgent::CHECK_SCORE;
	if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
		evaluationScore += GreedyExpectedMinMaxAgent::CHECK_SCORE;

	// White Turn
	if (configurationMetadata.whiteTurn)
		evaluationScore += GreedyExpectedMinMaxAgent::WHITE_TURN_SCORE;
	else
		evaluationScore -= GreedyExpectedMinMaxAgent::WHITE_TURN_SCORE;

	return evaluationScore;
}

/*
float GreedyExpectedMinMaxAgent::evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const
{
	// INFO: Presupunem ca are deja attack zone-urile calculate corect pentru ambele culori.
	// INFO: evaluateConfiguration nu se ocupa cu starile de Checkmate, acelea sunt gestionate in minMax.

	float evaluationScore = 0.0f;

	CellPressure cellPressure[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	// White Pieces

	unsigned long long whitePawns = configurationMetadata.whitePawns;
	while (whitePawns)
	{
		unsigned long long lsb = (whitePawns & ((~whitePawns) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::PAWN_SCORE + GreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whitePawns ^= lsb;
	}

	// Atac Stanga Pioni Albi
	unsigned long long whitePawnLeftAttack = (((configurationMetadata.whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal) & (~BoardManager::get().getFileBitMasks(0))) >> (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.whiteKingDefenseZone);
	while (whitePawnLeftAttack)
	{
		unsigned long long lsb = (whitePawnLeftAttack & ((~whitePawnLeftAttack) + 1));
		++cellPressure[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]].whitePressureCount;
		whitePawnLeftAttack ^= lsb;
	}

	// Atac Dreapta Pioni Albi
	unsigned long long whitePawnRightAttack = (((configurationMetadata.whitePawns & (~configurationMetadata.whitePiecesPinnedOnRank) & (~configurationMetadata.whitePiecesPinnedOnFile) & (~configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal) & (~BoardManager::get().getFileBitMasks(GameMetadata::NUM_TILES_WIDTH - 1))) >> (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.whiteKingDefenseZone);
	while (whitePawnRightAttack)
	{
		unsigned long long lsb = (whitePawnRightAttack & ((~whitePawnRightAttack) + 1));
		++cellPressure[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]].whitePressureCount;
		whitePawnRightAttack ^= lsb;
	}

	unsigned long long whiteRooks = configurationMetadata.whiteRooks;
	while (whiteRooks)
	{
		unsigned long long lsb = (whiteRooks & ((~whiteRooks) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::ROOK_SCORE + GreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteRooks ^= lsb;
	}

	// Atac Ture Albe
	whiteRooks = configurationMetadata.whiteRooks;
	while (whiteRooks)
	{
		unsigned long long lsbRook = (whiteRooks & ((~whiteRooks) + 1));

		if (!
			(
				(lsbRook & (configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal))
				||
				(lsbRook & (configurationMetadata.whitePiecesPinnedOnRank & configurationMetadata.whitePiecesPinnedOnFile))
				)
			)
		{
			int posRook = BoardManager::get().logPower2[lsbRook % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long piecesSameRank = BoardManager::get().extractRank(configurationMetadata.allPieces, posRook);
			unsigned long long piecesSameFile = BoardManager::get().extractFile(configurationMetadata.allPieces, posRook);

			unsigned long long rookAttackZone = (BoardManager::get().getPrecalculatedRawLeftAttackZones(posRook, piecesSameRank) | BoardManager::get().getPrecalculatedRawRightAttackZones(posRook, piecesSameRank) | BoardManager::get().getPrecalculatedRawTopAttackZones(posRook, piecesSameFile) | BoardManager::get().getPrecalculatedRawBottomAttackZones(posRook, piecesSameFile));
			rookAttackZone &= configurationMetadata.whiteKingDefenseZone;

			if (lsbRook & configurationMetadata.whitePiecesPinnedOnRank)
				rookAttackZone &= BoardManager::get().getRankBitMasks(posRook);
			if (lsbRook & configurationMetadata.whitePiecesPinnedOnFile)
				rookAttackZone &= BoardManager::get().getFileBitMasks(posRook);

			while (rookAttackZone)
			{
				unsigned long long lsbAttack = (rookAttackZone & ((~rookAttackZone) + 1));

				int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				++cellPressure[posAttack].whitePressureCount;

				rookAttackZone ^= lsbAttack;
			}
		}

		whiteRooks ^= lsbRook;
	}

	unsigned long long whiteKnights = configurationMetadata.whiteKnights;
	while (whiteKnights)
	{
		unsigned long long lsb = (whiteKnights & ((~whiteKnights) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::KNIGHT_SCORE + GreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteKnights ^= lsb;
	}

	// Atac Cai Albi
	whiteKnights = configurationMetadata.whiteKnights;
	while (whiteKnights)
	{
		unsigned long long lsbKnight = (whiteKnights & ((~whiteKnights) + 1));

		if (!(lsbKnight & (configurationMetadata.whitePiecesPinnedOnRank | configurationMetadata.whitePiecesPinnedOnFile | configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal)))
		{
			int posKnight = BoardManager::get().logPower2[lsbKnight % BoardManager::MODULO_LOG_POWER_2];
			unsigned long long knightAttackZone = (BoardManager::get().getPrecalculatedKnightAttackZones(posKnight) & configurationMetadata.whiteKingDefenseZone);

			while (knightAttackZone)
			{
				unsigned long long lsbAttack = (knightAttackZone & ((~knightAttackZone) + 1));

				int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				++cellPressure[posAttack].whitePressureCount;

				knightAttackZone ^= lsbAttack;
			}
		}

		whiteKnights ^= lsbKnight;
	}

	unsigned long long whiteBishops = configurationMetadata.whiteBishops;
	while (whiteBishops)
	{
		unsigned long long lsb = (whiteBishops & ((~whiteBishops) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::BISHOP_SCORE + GreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteBishops ^= lsb;
	}

	// Atac Nebuni Albi
	whiteBishops = configurationMetadata.whiteBishops;
	while (whiteBishops)
	{
		unsigned long long lsbBishop = (whiteBishops & ((~whiteBishops) + 1));

		if (!
			(
				(lsbBishop & (configurationMetadata.whitePiecesPinnedOnRank | configurationMetadata.whitePiecesPinnedOnFile))
				||
				(lsbBishop & (configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal & configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal))
				)
			)
		{
			int posBishop = BoardManager::get().logPower2[lsbBishop % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long piecesSameDiagonal0 = BoardManager::get().extractTopLeftBottomRightDiagonal(configurationMetadata.allPieces, posBishop);
			unsigned long long piecesSameDiagonal1 = BoardManager::get().extractTopRightBottomLeftDiagonal(configurationMetadata.allPieces, posBishop);

			unsigned long long bishopAttackZone = (BoardManager::get().getPrecalculatedRawTopLeftDiagonalAttackZones(posBishop, piecesSameDiagonal0) | BoardManager::get().getPrecalculatedRawBottomRightDiagonalAttackZones(posBishop, piecesSameDiagonal0) | BoardManager::get().getPrecalculatedRawTopRightDiagonalAttackZones(posBishop, piecesSameDiagonal1) | BoardManager::get().getPrecalculatedRawBottomLeftDiagonalAttackZones(posBishop, piecesSameDiagonal1));
			bishopAttackZone &= configurationMetadata.whiteKingDefenseZone;

			if (lsbBishop & configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal)
				bishopAttackZone &= BoardManager::get().getTopLeftBottomRightDiagonalBitMasks(posBishop);
			if (lsbBishop & configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal)
				bishopAttackZone &= BoardManager::get().getTopRightBottomLeftDiagonalBitMasks(posBishop);

			while (bishopAttackZone)
			{
				unsigned long long lsbAttack = (bishopAttackZone & ((~bishopAttackZone) + 1));

				int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				++cellPressure[posAttack].whitePressureCount;

				bishopAttackZone ^= lsbAttack;
			}
		}

		whiteBishops ^= lsbBishop;
	}

	unsigned long long whiteQueens = configurationMetadata.whiteQueens;
	while (whiteQueens)
	{
		unsigned long long lsb = (whiteQueens & ((~whiteQueens) + 1));
		evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::QUEEN_SCORE + GreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteQueens ^= lsb;
	}

	// Atac Regine Albe
	whiteQueens = configurationMetadata.whiteQueens;
	while (whiteQueens)
	{
		unsigned long long lsbQueen = (whiteQueens & ((~whiteQueens) + 1));

		int posQueen = BoardManager::get().logPower2[lsbQueen % BoardManager::MODULO_LOG_POWER_2];

		unsigned long long piecesSameRank = BoardManager::get().extractRank(configurationMetadata.allPieces, posQueen);
		unsigned long long piecesSameFile = BoardManager::get().extractFile(configurationMetadata.allPieces, posQueen);
		unsigned long long piecesSameDiagonal0 = BoardManager::get().extractTopLeftBottomRightDiagonal(configurationMetadata.allPieces, posQueen);
		unsigned long long piecesSameDiagonal1 = BoardManager::get().extractTopRightBottomLeftDiagonal(configurationMetadata.allPieces, posQueen);

		unsigned long long queenAttackZone = (BoardManager::get().getPrecalculatedRawLeftAttackZones(posQueen, piecesSameRank) | BoardManager::get().getPrecalculatedRawRightAttackZones(posQueen, piecesSameRank)
			| BoardManager::get().getPrecalculatedRawTopAttackZones(posQueen, piecesSameFile) | BoardManager::get().getPrecalculatedRawBottomAttackZones(posQueen, piecesSameFile)
			| BoardManager::get().getPrecalculatedRawTopLeftDiagonalAttackZones(posQueen, piecesSameDiagonal0)
			| BoardManager::get().getPrecalculatedRawBottomRightDiagonalAttackZones(posQueen, piecesSameDiagonal0)
			| BoardManager::get().getPrecalculatedRawTopRightDiagonalAttackZones(posQueen, piecesSameDiagonal1)
			| BoardManager::get().getPrecalculatedRawBottomLeftDiagonalAttackZones(posQueen, piecesSameDiagonal1));
		queenAttackZone &= configurationMetadata.whiteKingDefenseZone;

		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnRank)
			queenAttackZone &= BoardManager::get().getRankBitMasks(posQueen);
		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnFile)
			queenAttackZone &= BoardManager::get().getFileBitMasks(posQueen);
		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnTopLeftBottomRightDiagonal)
			queenAttackZone &= BoardManager::get().getTopLeftBottomRightDiagonalBitMasks(posQueen);
		if (lsbQueen & configurationMetadata.whitePiecesPinnedOnTopRightBottomLeftDiagonal)
			queenAttackZone &= BoardManager::get().getTopRightBottomLeftDiagonalBitMasks(posQueen);

		while (queenAttackZone)
		{
			unsigned long long lsbAttack = (queenAttackZone & ((~queenAttackZone) + 1));

			int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

			++cellPressure[posAttack].whitePressureCount;

			queenAttackZone ^= lsbAttack;
		}

		whiteQueens ^= lsbQueen;
	}

	evaluationScore = evaluationScore + GreedyExpectedMinMaxAgent::KING_SCORE + GreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::WHITE_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]];

	// Atac Rege Alb
	unsigned long long lsbWhiteKing = configurationMetadata.whiteKing;
	int posWhiteKing = BoardManager::get().logPower2[lsbWhiteKing % BoardManager::MODULO_LOG_POWER_2];
	unsigned long long whiteKingAttackZone = BoardManager::get().getPrecalculatedKingAttackZones(posWhiteKing);

	while (whiteKingAttackZone)
	{
		unsigned long long lsbAttack = (whiteKingAttackZone & ((~whiteKingAttackZone) + 1));

		int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

		++cellPressure[posAttack].whitePressureCount;
		cellPressure[posAttack].whiteKingInvolved = true;

		whiteKingAttackZone ^= lsbAttack;
	}

	// Black Pieces

	unsigned long long blackPawns = configurationMetadata.blackPawns;
	while (blackPawns)
	{
		unsigned long long lsb = (blackPawns & ((~blackPawns) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::PAWN_SCORE - GreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackPawns ^= lsb;
	}

	// Atac Stanga Pioni Negri
	unsigned long long blackPawnLeftAttack = (((configurationMetadata.blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal) & (~BoardManager::get().getFileBitMasks(0))) << (GameMetadata::NUM_TILES_WIDTH - 1)) & configurationMetadata.blackKingDefenseZone);
	while (blackPawnLeftAttack)
	{
		unsigned long long lsb = (blackPawnLeftAttack & ((~blackPawnLeftAttack) + 1));
		++cellPressure[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]].blackPressureCount;
		blackPawnLeftAttack ^= lsb;
	}

	// Atac Dreapta Pioni Negri
	unsigned long long blackPawnRightAttack = (((configurationMetadata.blackPawns & (~configurationMetadata.blackPiecesPinnedOnRank) & (~configurationMetadata.blackPiecesPinnedOnFile) & (~configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal) & (~BoardManager::get().getFileBitMasks(GameMetadata::NUM_TILES_WIDTH - 1))) << (GameMetadata::NUM_TILES_WIDTH + 1)) & configurationMetadata.blackKingDefenseZone);
	while (blackPawnRightAttack)
	{
		unsigned long long lsb = (blackPawnRightAttack & ((~blackPawnRightAttack) + 1));
		++cellPressure[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]].blackPressureCount;
		blackPawnRightAttack ^= lsb;
	}

	unsigned long long blackRooks = configurationMetadata.blackRooks;
	while (blackRooks)
	{
		unsigned long long lsb = (blackRooks & ((~blackRooks) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::ROOK_SCORE - GreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackRooks ^= lsb;
	}

	// Atac Ture Negre
	blackRooks = configurationMetadata.blackRooks;
	while (blackRooks)
	{
		unsigned long long lsbRook = (blackRooks & ((~blackRooks) + 1));

		if (!
			(
				(lsbRook & (configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal))
				||
				(lsbRook & (configurationMetadata.blackPiecesPinnedOnRank & configurationMetadata.blackPiecesPinnedOnFile))
				)
			)
		{
			int posRook = BoardManager::get().logPower2[lsbRook % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long piecesSameRank = BoardManager::get().extractRank(configurationMetadata.allPieces, posRook);
			unsigned long long piecesSameFile = BoardManager::get().extractFile(configurationMetadata.allPieces, posRook);

			unsigned long long rookAttackZone = (BoardManager::get().getPrecalculatedRawLeftAttackZones(posRook, piecesSameRank) | BoardManager::get().getPrecalculatedRawRightAttackZones(posRook, piecesSameRank) | BoardManager::get().getPrecalculatedRawTopAttackZones(posRook, piecesSameFile) | BoardManager::get().getPrecalculatedRawBottomAttackZones(posRook, piecesSameFile));
			rookAttackZone &= configurationMetadata.blackKingDefenseZone;

			if (lsbRook & configurationMetadata.blackPiecesPinnedOnRank)
				rookAttackZone &= BoardManager::get().getRankBitMasks(posRook);
			if (lsbRook & configurationMetadata.blackPiecesPinnedOnFile)
				rookAttackZone &= BoardManager::get().getFileBitMasks(posRook);

			while (rookAttackZone)
			{
				unsigned long long lsbAttack = (rookAttackZone & ((~rookAttackZone) + 1));

				int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				++cellPressure[posAttack].blackPressureCount;

				rookAttackZone ^= lsbAttack;
			}
		}

		blackRooks ^= lsbRook;
	}

	unsigned long long blackKnights = configurationMetadata.blackKnights;
	while (blackKnights)
	{
		unsigned long long lsb = (blackKnights & ((~blackKnights) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::KNIGHT_SCORE - GreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackKnights ^= lsb;
	}

	// Atac Cai Negri
	blackKnights = configurationMetadata.blackKnights;
	while (blackKnights)
	{
		unsigned long long lsbKnight = (blackKnights & ((~blackKnights) + 1));

		if (!(lsbKnight & (configurationMetadata.blackPiecesPinnedOnRank | configurationMetadata.blackPiecesPinnedOnFile | configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal | configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal)))
		{
			int posKnight = BoardManager::get().logPower2[lsbKnight % BoardManager::MODULO_LOG_POWER_2];
			unsigned long long knightAttackZone = (BoardManager::get().getPrecalculatedKnightAttackZones(posKnight) & configurationMetadata.blackKingDefenseZone);

			while (knightAttackZone)
			{
				unsigned long long lsbAttack = (knightAttackZone & ((~knightAttackZone) + 1));

				int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				++cellPressure[posAttack].blackPressureCount;

				knightAttackZone ^= lsbAttack;
			}
		}

		blackKnights ^= lsbKnight;
	}

	unsigned long long blackBishops = configurationMetadata.blackBishops;
	while (blackBishops)
	{
		unsigned long long lsb = (blackBishops & ((~blackBishops) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::BISHOP_SCORE - GreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackBishops ^= lsb;
	}

	// Atac Nebuni Negri
	blackBishops = configurationMetadata.blackBishops;
	while (blackBishops)
	{
		unsigned long long lsbBishop = (blackBishops & ((~blackBishops) + 1));

		if (!
			(
				(lsbBishop & (configurationMetadata.blackPiecesPinnedOnRank | configurationMetadata.blackPiecesPinnedOnFile))
				||
				(lsbBishop & (configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal & configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal))
				)
			)
		{
			int posBishop = BoardManager::get().logPower2[lsbBishop % BoardManager::MODULO_LOG_POWER_2];

			unsigned long long piecesSameDiagonal0 = BoardManager::get().extractTopLeftBottomRightDiagonal(configurationMetadata.allPieces, posBishop);
			unsigned long long piecesSameDiagonal1 = BoardManager::get().extractTopRightBottomLeftDiagonal(configurationMetadata.allPieces, posBishop);

			unsigned long long bishopAttackZone = (BoardManager::get().getPrecalculatedRawTopLeftDiagonalAttackZones(posBishop, piecesSameDiagonal0) | BoardManager::get().getPrecalculatedRawBottomRightDiagonalAttackZones(posBishop, piecesSameDiagonal0) | BoardManager::get().getPrecalculatedRawTopRightDiagonalAttackZones(posBishop, piecesSameDiagonal1) | BoardManager::get().getPrecalculatedRawBottomLeftDiagonalAttackZones(posBishop, piecesSameDiagonal1));
			bishopAttackZone &= configurationMetadata.blackKingDefenseZone;

			if (lsbBishop & configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal)
				bishopAttackZone &= BoardManager::get().getTopLeftBottomRightDiagonalBitMasks(posBishop);
			if (lsbBishop & configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal)
				bishopAttackZone &= BoardManager::get().getTopRightBottomLeftDiagonalBitMasks(posBishop);

			while (bishopAttackZone)
			{
				unsigned long long lsbAttack = (bishopAttackZone & ((~bishopAttackZone) + 1));

				int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

				++cellPressure[posAttack].blackPressureCount;

				bishopAttackZone ^= lsbAttack;
			}
		}

		blackBishops ^= lsbBishop;
	}

	unsigned long long blackQueens = configurationMetadata.blackQueens;
	while (blackQueens)
	{
		unsigned long long lsb = (blackQueens & ((~blackQueens) + 1));
		evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::QUEEN_SCORE - GreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackQueens ^= lsb;
	}

	// Atac Regine Negre
	blackQueens = configurationMetadata.blackQueens;
	while (blackQueens)
	{
		unsigned long long lsbQueen = (blackQueens & ((~blackQueens) + 1));

		int posQueen = BoardManager::get().logPower2[lsbQueen % BoardManager::MODULO_LOG_POWER_2];

		unsigned long long piecesSameRank = BoardManager::get().extractRank(configurationMetadata.allPieces, posQueen);
		unsigned long long piecesSameFile = BoardManager::get().extractFile(configurationMetadata.allPieces, posQueen);
		unsigned long long piecesSameDiagonal0 = BoardManager::get().extractTopLeftBottomRightDiagonal(configurationMetadata.allPieces, posQueen);
		unsigned long long piecesSameDiagonal1 = BoardManager::get().extractTopRightBottomLeftDiagonal(configurationMetadata.allPieces, posQueen);

		unsigned long long queenAttackZone = (BoardManager::get().getPrecalculatedRawLeftAttackZones(posQueen, piecesSameRank) | BoardManager::get().getPrecalculatedRawRightAttackZones(posQueen, piecesSameRank)
			| BoardManager::get().getPrecalculatedRawTopAttackZones(posQueen, piecesSameFile) | BoardManager::get().getPrecalculatedRawBottomAttackZones(posQueen, piecesSameFile)
			| BoardManager::get().getPrecalculatedRawTopLeftDiagonalAttackZones(posQueen, piecesSameDiagonal0)
			| BoardManager::get().getPrecalculatedRawBottomRightDiagonalAttackZones(posQueen, piecesSameDiagonal0)
			| BoardManager::get().getPrecalculatedRawTopRightDiagonalAttackZones(posQueen, piecesSameDiagonal1)
			| BoardManager::get().getPrecalculatedRawBottomLeftDiagonalAttackZones(posQueen, piecesSameDiagonal1));
		queenAttackZone &= configurationMetadata.blackKingDefenseZone;

		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnRank)
			queenAttackZone &= BoardManager::get().getRankBitMasks(posQueen);
		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnFile)
			queenAttackZone &= BoardManager::get().getFileBitMasks(posQueen);
		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnTopLeftBottomRightDiagonal)
			queenAttackZone &= BoardManager::get().getTopLeftBottomRightDiagonalBitMasks(posQueen);
		if (lsbQueen & configurationMetadata.blackPiecesPinnedOnTopRightBottomLeftDiagonal)
			queenAttackZone &= BoardManager::get().getTopRightBottomLeftDiagonalBitMasks(posQueen);

		while (queenAttackZone)
		{
			unsigned long long lsbAttack = (queenAttackZone & ((~queenAttackZone) + 1));

			int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

			++cellPressure[posAttack].blackPressureCount;

			queenAttackZone ^= lsbAttack;
		}

		blackQueens ^= lsbQueen;
	}

	evaluationScore = evaluationScore - GreedyExpectedMinMaxAgent::KING_SCORE - GreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR * GreedyExpectedMinMaxAgent::BLACK_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]];

	// Atac Rege Negru
	unsigned long long lsbBlackKing = configurationMetadata.blackKing;
	int posBlackKing = BoardManager::get().logPower2[lsbBlackKing % BoardManager::MODULO_LOG_POWER_2];
	unsigned long long blackKingAttackZone = BoardManager::get().getPrecalculatedKingAttackZones(posBlackKing);

	while (blackKingAttackZone)
	{
		unsigned long long lsbAttack = (blackKingAttackZone & ((~blackKingAttackZone) + 1));

		int posAttack = BoardManager::get().logPower2[lsbAttack % BoardManager::MODULO_LOG_POWER_2];

		++cellPressure[posAttack].blackPressureCount;
		cellPressure[posAttack].blackKingInvolved = true;

		blackKingAttackZone ^= lsbAttack;
	}

	// Attack Zones

	unsigned long long whiteAttackZones = configurationMetadata.whiteAttackZones;
	while (whiteAttackZones)
	{
		unsigned long long lsb = (whiteAttackZones & ((~whiteAttackZones) + 1));
		if ((lsb & configurationMetadata.allBlackPieces) != 0ull)
			evaluationScore += GreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE;
		else
			evaluationScore += GreedyExpectedMinMaxAgent::COVER_ZONE_SCORE;
		whiteAttackZones ^= lsb;
	}

	unsigned long long blackAttackZones = configurationMetadata.blackAttackZones;
	while (blackAttackZones)
	{
		unsigned long long lsb = (blackAttackZones & ((~blackAttackZones) + 1));
		if ((lsb & configurationMetadata.allWhitePieces) != 0ull)
			evaluationScore -= GreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE;
		else
			evaluationScore -= GreedyExpectedMinMaxAgent::COVER_ZONE_SCORE;
		blackAttackZones ^= lsb;
	}

	// Defense Zones + Chain Captures

	for (int i = 0; i < GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH; ++i)
	{
		if (((1ull << i) & configurationMetadata.allPieces) == 0ull)
			continue;

		if (((1ull << i) & configurationMetadata.whiteKing) != 0ull)
			continue;
		if (((1ull << i) & configurationMetadata.blackKing) != 0ull)
			continue;

		if (cellPressure[i].whitePressureCount > 0 && cellPressure[i].blackPressureCount == 0 && ((1ull << i) & configurationMetadata.allWhitePieces) != 0ull)
			evaluationScore += GreedyExpectedMinMaxAgent::DEFENSE_ZONE_SCORE;
		else if (cellPressure[i].whitePressureCount == 0 && cellPressure[i].blackPressureCount > 0 && ((1ull << i) & configurationMetadata.allBlackPieces) != 0ull)
			evaluationScore -= GreedyExpectedMinMaxAgent::DEFENSE_ZONE_SCORE;
		else if (cellPressure[i].whitePressureCount > 0 && cellPressure[i].blackPressureCount > 0) // INFO: Stim deja ca celula este ocupata (ori de alb, ori de negru).
		{
			if (cellPressure[i].whitePressureCount - (int)cellPressure[i].whiteKingInvolved > cellPressure[i].blackPressureCount - (int)cellPressure[i].blackKingInvolved)
				evaluationScore += GreedyExpectedMinMaxAgent::CHAIN_CAPTURE_SCORE;
			else if (cellPressure[i].whitePressureCount - (int)cellPressure[i].whiteKingInvolved < cellPressure[i].blackPressureCount - (int)cellPressure[i].blackKingInvolved)
				evaluationScore -= GreedyExpectedMinMaxAgent::CHAIN_CAPTURE_SCORE;
		}
	}

	// Checks
	if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
		evaluationScore -= GreedyExpectedMinMaxAgent::CHECK_SCORE;
	if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
		evaluationScore += GreedyExpectedMinMaxAgent::CHECK_SCORE;

	// White Turn
	if (configurationMetadata.whiteTurn)
		evaluationScore += GreedyExpectedMinMaxAgent::WHITE_TURN_SCORE;
	else
		evaluationScore -= GreedyExpectedMinMaxAgent::WHITE_TURN_SCORE;

	return evaluationScore;
}
*/

void GreedyExpectedMinMaxAgent::updateAlreadyCalculatedNodes(std::map<unsigned long long, std::pair<int, float>>& alreadyCalculatedNodes, unsigned long long zobristHashingValue, int depth, float value) const
{
	auto it = alreadyCalculatedNodes.find(zobristHashingValue);
	if (it != alreadyCalculatedNodes.end())
	{
		if (it->second.first > depth)
			it->second = std::make_pair(depth, value);
	}
	else
		alreadyCalculatedNodes[zobristHashingValue] = std::make_pair(depth, value);
}

float GreedyExpectedMinMaxAgent::minMaxFindBestMove(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited, std::map<unsigned long long, std::pair<int, float>>& alreadyCalculatedNodes) const // INFO: minMaxFindBestMove primeste o copie a configuratiei. 
{
	if (this->isFindBestMoveCancelled.load())
	{
		std::cout << "MinMaxFindBestMove Thread was cancelled." << std::endl;
		return 0.0f;
	}

	++numNodesVisited;

	if (zobristHashingValuesFrequency[configurationMetadata.zobristHashingValue] >= GameMetadata::FREQUENCY_UNTIL_DRAW_REPETITION)
		return 0.0f; // Remiza, aceasta configuratie s-a repetat de FREQUENCY_UNTIL_DRAW_REPETITION ori.

	// INFO: Se verifica daca configuratia a fost deja calculata la o adancime mai mica sau egala.
	auto it = alreadyCalculatedNodes.find(configurationMetadata.zobristHashingValue);
	if (it != alreadyCalculatedNodes.end())
	{
		if (it->second.first <= depth)
			return it->second.second;
	}

	if (depth >= GreedyExpectedMinMaxAgent::MAX_DEPTH_FIND_BEST_MOVE && expectedNumNodesVisited <= 0) // INFO: Trebuie <= 0 pentru expectedNumNodesVisited
	{
		BoardManager::get().generateWhiteAttackZones(configurationMetadata);
		BoardManager::get().generateBlackAttackZones(configurationMetadata);
		return this->evaluateConfiguration(configurationMetadata);
	}

	if (configurationMetadata.whiteTurn) // Maximizing
	{
		float maximumScore = -GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
		BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

		int expectedRemainingNumNodesVisited = expectedNumNodesVisited;
		for (int i = 0; i < allWhiteMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			int numNodesVisitedBefore = numNodesVisited;
			float currentScore = this->minMaxFindBestMove(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allWhiteMoves.size() - i), alreadyCalculatedNodes);
			expectedRemainingNumNodesVisited -= (numNodesVisited - numNodesVisitedBefore);
			--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

			// INFO: Entry-ul in map ramane si daca e pe 0 frecventa. Poate fi util pentru performanta.

			if (currentScore > maximumScore)
			{
				maximumScore = currentScore;
				bestMove = allWhiteMoves[i];

				alpha = std::max(alpha, maximumScore);
				if (alpha >= beta)
					break;
			}
		}

		if (maximumScore == -GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
				maximumScore = -GreedyExpectedMinMaxAgent::REACHABLE_INF + 1.0f * depth; // INFO: Se penalizeaza mai mult din cauza ca este mai adanc in cadrul arborelui MinMax.
			else
				maximumScore = 0.0f; // Remiza, albul nu poate face mutari, dar nu e nici in sah.
		}

		this->updateAlreadyCalculatedNodes(alreadyCalculatedNodes, configurationMetadata.zobristHashingValue, depth, maximumScore);

		return maximumScore;
	}
	else // Minimizing
	{
		float minimumScore = GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
		BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

		int expectedRemainingNumNodesVisited = expectedNumNodesVisited;
		for (int i = 0; i < allBlackMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			int numNodesVisitedBefore = numNodesVisited;
			float currentScore = this->minMaxFindBestMove(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allBlackMoves.size() - i), alreadyCalculatedNodes);
			expectedRemainingNumNodesVisited -= (numNodesVisited - numNodesVisitedBefore);
			--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

			// INFO: Entry-ul in map ramane si daca e pe 0 frecventa. Poate fi util pentru performanta.

			if (currentScore < minimumScore)
			{
				minimumScore = currentScore;
				bestMove = allBlackMoves[i];

				beta = std::min(beta, minimumScore);
				if (alpha >= beta)
					break;
			}
		}

		if (minimumScore == GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
				minimumScore = GreedyExpectedMinMaxAgent::REACHABLE_INF - 1.0f * depth; // INFO: Se penalizeaza mai mult din cauza ca este mai adanc in cadrul arborelui MinMax.
			else
				minimumScore = 0.0f; // Remiza, negrul nu poate face mutari, dar nu e nici in sah.
		}

		this->updateAlreadyCalculatedNodes(alreadyCalculatedNodes, configurationMetadata.zobristHashingValue, depth, minimumScore);

		return minimumScore;
	}
}

float GreedyExpectedMinMaxAgent::minMaxEstimateConfiguration(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited, std::map<unsigned long long, std::pair<int, float>>& alreadyCalculatedNodes) const // INFO: minMaxEstimateConfiguration primeste o copie a configuratiei. 
{
	if (this->isEstimateCancelled.load())
	{
		std::cout << "MinMaxEstimateConfiguration Thread was cancelled." << std::endl;
		return 0.0f;
	}

	++numNodesVisited;

	if (zobristHashingValuesFrequency[configurationMetadata.zobristHashingValue] >= GameMetadata::FREQUENCY_UNTIL_DRAW_REPETITION)
		return 0.0f; // Remiza, aceasta configuratie s-a repetat de FREQUENCY_UNTIL_DRAW_REPETITION ori.

	// INFO: Se verifica daca configuratia a fost deja calculata la o adancime mai mica sau egala.
	auto it = alreadyCalculatedNodes.find(configurationMetadata.zobristHashingValue);
	if (it != alreadyCalculatedNodes.end())
	{
		if (it->second.first <= depth)
			return it->second.second;
	}

	if (depth >= GreedyExpectedMinMaxAgent::MAX_DEPTH_ESTIMATE_CONFIGURATION && expectedNumNodesVisited <= 0) // INFO: Trebuie <= 0 pentru expectedNumNodesVisited
	{
		BoardManager::get().generateWhiteAttackZones(configurationMetadata);
		BoardManager::get().generateBlackAttackZones(configurationMetadata);
		return this->evaluateConfiguration(configurationMetadata);
	}

	if (configurationMetadata.whiteTurn) // Maximizing
	{
		float maximumScore = -GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
		BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

		int expectedRemainingNumNodesVisited = expectedNumNodesVisited;
		for (int i = 0; i < allWhiteMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			int numNodesVisitedBefore = numNodesVisited;
			float currentScore = this->minMaxEstimateConfiguration(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allWhiteMoves.size() - i), alreadyCalculatedNodes);
			expectedRemainingNumNodesVisited -= (numNodesVisited - numNodesVisitedBefore);
			--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

			// INFO: Entry-ul in map ramane si daca e pe 0 frecventa. Poate fi util pentru performanta.

			if (currentScore > maximumScore)
			{
				maximumScore = currentScore;
				bestMove = allWhiteMoves[i];

				alpha = std::max(alpha, maximumScore);
				if (alpha >= beta)
					break;
			}
		}

		if (maximumScore == -GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
				maximumScore = -GreedyExpectedMinMaxAgent::REACHABLE_INF + 1.0f * depth; // INFO: Se penalizeaza mai mult din cauza ca este mai adanc in cadrul arborelui MinMax.
			else
				maximumScore = 0.0f; // Remiza, albul nu poate face mutari, dar nu e nici in sah.
		}

		this->updateAlreadyCalculatedNodes(alreadyCalculatedNodes, configurationMetadata.zobristHashingValue, depth, maximumScore);

		return maximumScore;
	}
	else // Minimizing
	{
		float minimumScore = GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
		BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

		int expectedRemainingNumNodesVisited = expectedNumNodesVisited;
		for (int i = 0; i < allBlackMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			int numNodesVisitedBefore = numNodesVisited;
			float currentScore = this->minMaxEstimateConfiguration(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allBlackMoves.size() - i), alreadyCalculatedNodes);
			expectedRemainingNumNodesVisited -= (numNodesVisited - numNodesVisitedBefore);
			--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

			// INFO: Entry-ul in map ramane si daca e pe 0 frecventa. Poate fi util pentru performanta.

			if (currentScore < minimumScore)
			{
				minimumScore = currentScore;
				bestMove = allBlackMoves[i];

				beta = std::min(beta, minimumScore);
				if (alpha >= beta)
					break;
			}
		}

		if (minimumScore == GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
				minimumScore = GreedyExpectedMinMaxAgent::REACHABLE_INF - 1.0f * depth; // INFO: Se penalizeaza mai mult din cauza ca este mai adanc in cadrul arborelui MinMax.
			else
				minimumScore = 0.0f; // Remiza, negrul nu poate face mutari, dar nu e nici in sah.
		}

		this->updateAlreadyCalculatedNodes(alreadyCalculatedNodes, configurationMetadata.zobristHashingValue, depth, minimumScore);

		return minimumScore;
	}
}

void GreedyExpectedMinMaxAgent::findBestMove(ConfigurationMetadata& configurationMetadata)
{
	this->setIsFindingBestMove(true);
	this->setBestMove(std::vector<std::pair<char, int>>());
	this->setIsFindBestMoveCancelled(false);

	std::map<unsigned long long int, int> zobristHashingValuesFrequency(BoardManager::get().getZobristHashingValuesFrequency());
	this->findBestMoveThread = std::make_shared<std::thread>(std::thread([this, &configurationMetadata, zobristHashingValuesFrequency]() mutable
		{
			std::vector<std::future<float>> scoreFutures;
			std::vector<std::future<int>> numNodesVisitedFutures;
			std::vector<std::thread> threads;

			int numNodesVisitedTotal = 0;

			if (configurationMetadata.whiteTurn)
			{
				float maximumScore = -GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
				BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

				if (allWhiteMoves.size() == 1)
				{
					if (!this->isFindBestMoveCancelled.load())
						this->setBestMove(allWhiteMoves[0]);
					std::cout << "GreedyExpectedMinMaxAgent: FindBestMove: Only one move available." << std::endl;
					return;
				}

				if (this->alreadyCalculatedNodesMapsFindBestMove.size() < allWhiteMoves.size())
					this->alreadyCalculatedNodesMapsFindBestMove.resize(allWhiteMoves.size());

				for (int i = 0; i < allWhiteMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());

					threads.push_back(std::thread([this, configurationMetadata, allWhiteMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequency, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED_FIND_BEST_MOVE / (int)allWhiteMoves.size(), alreadyCalculatedNodes = &(this->alreadyCalculatedNodesMapsFindBestMove[i])]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							alreadyCalculatedNodes->clear();
							float currentScore = this->minMaxFindBestMove(newConfigurationMetadata, 0, -GreedyExpectedMinMaxAgent::UNREACHABLE_INF, GreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequency, numNodesVisited, expectedNumNodesVisitedThisThread, *alreadyCalculatedNodes);
							--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

							// INFO: Entry-ul in map ramane si daca e pe 0 frecventa.

							scorePromise.set_value(currentScore);
							numNodesVisitedPromise.set_value(numNodesVisited);
						}));
				}

				for (int i = 0; i < threads.size(); ++i)
				{
					threads[i].join();

					float currentScore = scoreFutures[i].get();
					if (currentScore > maximumScore)
					{
						maximumScore = currentScore;
						bestMove = allWhiteMoves[i];
					}

					numNodesVisitedTotal += numNodesVisitedFutures[i].get();
				}

				if (maximumScore == -GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
				{
					if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
						maximumScore = -GreedyExpectedMinMaxAgent::REACHABLE_INF;
					else
						maximumScore = 0.0f; // Remiza, albul nu poate face mutari, dar nu e nici in sah.
				}

				if (!this->isFindBestMoveCancelled.load()) // INFO: Se seteaza un bestMove chiar daca nu exista niciun move (exemplu in caz de remiza).
					this->setBestMove(bestMove);
			}
			else
			{
				float minimumScore = GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
				BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

				if (allBlackMoves.size() == 1)
				{
					if (!this->isFindBestMoveCancelled.load())
						this->setBestMove(allBlackMoves[0]);
					std::cout << "GreedyExpectedMinMaxAgent: FindBestMove: Only one move available." << std::endl;
					return;
				}

				if (this->alreadyCalculatedNodesMapsFindBestMove.size() < allBlackMoves.size())
					this->alreadyCalculatedNodesMapsFindBestMove.resize(allBlackMoves.size());

				for (int i = 0; i < allBlackMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());

					threads.push_back(std::thread([this, configurationMetadata, allBlackMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequency, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED_FIND_BEST_MOVE / (int)allBlackMoves.size(), alreadyCalculatedNodes = &(this->alreadyCalculatedNodesMapsFindBestMove[i])]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							alreadyCalculatedNodes->clear();
							float currentScore = this->minMaxFindBestMove(newConfigurationMetadata, 0, -GreedyExpectedMinMaxAgent::UNREACHABLE_INF, GreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequency, numNodesVisited, expectedNumNodesVisitedThisThread, *alreadyCalculatedNodes);
							--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

							// INFO: Entry-ul in map ramane si daca e pe 0 frecventa.

							scorePromise.set_value(currentScore);
							numNodesVisitedPromise.set_value(numNodesVisited);
						}));
				}

				for (int i = 0; i < threads.size(); ++i)
				{
					threads[i].join();

					float currentScore = scoreFutures[i].get();
					if (currentScore < minimumScore)
					{
						minimumScore = currentScore;
						bestMove = allBlackMoves[i];
					}

					numNodesVisitedTotal += numNodesVisitedFutures[i].get();
				}

				if (minimumScore == GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
				{
					if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
						minimumScore = GreedyExpectedMinMaxAgent::REACHABLE_INF;
					else
						minimumScore = 0.0f; // Remiza, negrul nu poate face mutari, dar nu e nici in sah.
				}

				if (!this->isFindBestMoveCancelled.load()) // INFO: Se seteaza un bestMove chiar daca nu exista niciun move (exemplu in caz de remiza).
					this->setBestMove(bestMove);
			}

			std::cout << "GreedyExpectedMinMaxAgent: FindBestMove: Total Number of Nodes Visited: " << numNodesVisitedTotal << std::endl;
		}));
}

void GreedyExpectedMinMaxAgent::reset()
{

}

void GreedyExpectedMinMaxAgent::estimateConfiguration(ConfigurationMetadata& configurationMetadata)
{
	this->setIsEstimating(true);
	this->resetEstimation();
	this->setIsEstimateCancelled(false);

	std::map<unsigned long long int, int> zobristHashingValuesFrequency(BoardManager::get().getZobristHashingValuesFrequency());
	this->estimateThread = std::make_shared<std::thread>(std::thread([this, &configurationMetadata, zobristHashingValuesFrequency]() mutable
		{
			std::vector<std::future<float>> scoreFutures;
			std::vector<std::future<int>> numNodesVisitedFutures;
			std::vector<std::thread> threads;

			int numNodesVisitedTotal = 0;

			if (configurationMetadata.whiteTurn)
			{
				float maximumScore = -GreedyExpectedMinMaxAgent::UNREACHABLE_INF;

				std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
				BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

				if (this->alreadyCalculatedNodesMapsEstimateConfiguration.size() < allWhiteMoves.size())
					this->alreadyCalculatedNodesMapsEstimateConfiguration.resize(allWhiteMoves.size());

				for (int i = 0; i < allWhiteMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());

					threads.push_back(std::thread([this, configurationMetadata, allWhiteMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequency, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED_ESTIMATE_CONFIGURATION / (int)allWhiteMoves.size(), alreadyCalculatedNodes = &(this->alreadyCalculatedNodesMapsEstimateConfiguration[i])]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							alreadyCalculatedNodes->clear();
							float currentScore = this->minMaxEstimateConfiguration(newConfigurationMetadata, 0, -GreedyExpectedMinMaxAgent::UNREACHABLE_INF, GreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequency, numNodesVisited, expectedNumNodesVisitedThisThread, *alreadyCalculatedNodes);
							--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

							// INFO: Entry-ul in map ramane si daca e pe 0 frecventa.

							scorePromise.set_value(currentScore);
							numNodesVisitedPromise.set_value(numNodesVisited);
						}));
				}

				for (int i = 0; i < threads.size(); ++i)
				{
					threads[i].join();

					float currentScore = scoreFutures[i].get();
					maximumScore = std::max(maximumScore, currentScore);

					numNodesVisitedTotal += numNodesVisitedFutures[i].get();
				}

				if (maximumScore == -GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
				{
					if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
						maximumScore = -GreedyExpectedMinMaxAgent::REACHABLE_INF;
					else
						maximumScore = 0.0f; // Remiza, albul nu poate face mutari, dar nu e nici in sah.
				}

				if (!this->isEstimateCancelled.load())
					this->setEstimation(maximumScore);
			}
			else
			{
				float minimumScore = GreedyExpectedMinMaxAgent::UNREACHABLE_INF;

				std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
				BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

				if (this->alreadyCalculatedNodesMapsEstimateConfiguration.size() < allBlackMoves.size())
					this->alreadyCalculatedNodesMapsEstimateConfiguration.resize(allBlackMoves.size());

				for (int i = 0; i < allBlackMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());

					threads.push_back(std::thread([this, configurationMetadata, allBlackMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequency, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED_ESTIMATE_CONFIGURATION / (int)allBlackMoves.size(), alreadyCalculatedNodes = &(this->alreadyCalculatedNodesMapsEstimateConfiguration[i])]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							alreadyCalculatedNodes->clear();
							float currentScore = this->minMaxEstimateConfiguration(newConfigurationMetadata, 0, -GreedyExpectedMinMaxAgent::UNREACHABLE_INF, GreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequency, numNodesVisited, expectedNumNodesVisitedThisThread, *alreadyCalculatedNodes);
							--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

							// INFO: Entry-ul in map ramane si daca e pe 0 frecventa.

							scorePromise.set_value(currentScore);
							numNodesVisitedPromise.set_value(numNodesVisited);
						}));
				}

				for (int i = 0; i < threads.size(); ++i)
				{
					threads[i].join();

					float currentScore = scoreFutures[i].get();
					minimumScore = std::min(minimumScore, currentScore);

					numNodesVisitedTotal += numNodesVisitedFutures[i].get();
				}

				if (minimumScore == GreedyExpectedMinMaxAgent::UNREACHABLE_INF)
				{
					if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
						minimumScore = GreedyExpectedMinMaxAgent::REACHABLE_INF;
					else
						minimumScore = 0.0f; // Remiza, negrul nu poate face mutari, dar nu e nici in sah.
				}

				if (!this->isEstimateCancelled.load())
					this->setEstimation(minimumScore);
			}

			std::cout << "GreedyExpectedMinMaxAgent: EstimateConfiguration: Total Number of Nodes Visited: " << numNodesVisitedTotal << std::endl;
		}));
}

float GreedyExpectedMinMaxAgent::getMaxDisplayEstimation() const { return GreedyExpectedMinMaxAgent::MAX_DISPLAY_ESTIMATION; }

const int GreedyExpectedMinMaxAgent::MAX_DEPTH_FIND_BEST_MOVE = 4;
const int GreedyExpectedMinMaxAgent::MAX_DEPTH_ESTIMATE_CONFIGURATION = 2;

const float GreedyExpectedMinMaxAgent::UNREACHABLE_INF = GreedyExpectedMinMaxAgent::UNREACHABLE_ESTIMATION / 2.0f; // INFO: 2 ^ 20 / 2 = 2^ 19
const float GreedyExpectedMinMaxAgent::REACHABLE_INF = GreedyExpectedMinMaxAgent::UNREACHABLE_INF / 2.0f;

const float GreedyExpectedMinMaxAgent::PAWN_SCORE = 1.0f;
const float GreedyExpectedMinMaxAgent::ROOK_SCORE = 5.0f;
const float GreedyExpectedMinMaxAgent::KNIGHT_SCORE = 3.0f;
const float GreedyExpectedMinMaxAgent::BISHOP_SCORE = 3.15f;
const float GreedyExpectedMinMaxAgent::QUEEN_SCORE = 9.0f;
const float GreedyExpectedMinMaxAgent::KING_SCORE = 100.0f;

const float GreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE = 0.01f;
const float GreedyExpectedMinMaxAgent::DEFENSE_ZONE_SCORE = 0.015f;
const float GreedyExpectedMinMaxAgent::COVER_ZONE_SCORE = 0.035f;
const float GreedyExpectedMinMaxAgent::CHAIN_CAPTURE_SCORE = 0.035f;
const float GreedyExpectedMinMaxAgent::CHECK_SCORE = 0.05f;

const float GreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR = 0.05f;
const float GreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR = 0.05f;
const float GreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR = 0.05f;
const float GreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR = 0.05f;
const float GreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR = 0.05f;
const float GreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR = 0.05f;

const float GreedyExpectedMinMaxAgent::WHITE_TURN_SCORE = 0.1f;

const float GreedyExpectedMinMaxAgent::WHITE_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	4.5f, 5.0f, 5.5f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f,

	3.5f, 4.0f, 4.5f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	2.5f, 3.0f, 3.5f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	2.0f, 2.5f, 3.0f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	1.5f, 2.0f, 2.5f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	0.5f, 1.0f, 1.5f, 1.5f, 1.5f, 1.0f, 0.5f, 0.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

const float GreedyExpectedMinMaxAgent::WHITE_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	5.0f, 5.5f, 6.0f, 6.0f, 6.0f, 5.5f, 5.0f, 4.5f,

	4.0f, 4.5f, 5.0f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	3.0f, 3.5f, 4.0f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	2.5f, 3.0f, 3.5f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	2.0f, 2.5f, 3.0f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	1.5f, 2.0f, 2.5f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	0.5f, 1.0f, 1.5f, 1.5f, 1.5f, 1.0f, 0.5f, 0.0f
};

const float GreedyExpectedMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	4.0f, 4.5f, 5.0f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	3.5f, 4.0f, 4.5f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	3.0f, 3.5f, 4.0f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	2.5f, 3.0f, 3.5f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	2.0f, 2.5f, 3.0f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	1.5f, 2.0f, 2.5f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	0.5f, 1.0f, 1.5f, 1.5f, 1.5f, 1.0f, 0.5f, 0.0f
};

const float GreedyExpectedMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	4.0f, 4.5f, 5.0f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f,

	3.5f, 4.0f, 4.5f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	3.0f, 3.5f, 4.0f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	2.5f, 3.0f, 3.5f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	2.0f, 2.5f, 3.0f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	1.5f, 2.0f, 2.5f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	1.0f, 1.5f, 2.0f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	0.5f, 1.0f, 1.5f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f
};

const float GreedyExpectedMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	4.0f, 4.5f, 5.0f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f,

	3.5f, 4.0f, 4.5f, 5.5f, 5.5f, 4.5f, 4.0f, 3.5f,

	3.0f, 3.5f, 4.0f, 5.0f, 5.0f, 4.0f, 3.5f, 3.0f,

	2.5f, 3.0f, 3.5f, 4.5f, 4.5f, 3.5f, 3.0f, 2.5f,

	2.0f, 2.5f, 3.0f, 4.0f, 4.0f, 3.0f, 2.5f, 2.0f,

	1.5f, 2.0f, 2.5f, 3.5f, 3.5f, 2.5f, 2.0f, 1.5f,

	1.0f, 1.5f, 2.0f, 3.0f, 3.0f, 2.0f, 1.5f, 1.0f,

	0.5f, 1.0f, 1.5f, 2.5f, 2.5f, 1.5f, 1.0f, 0.5f
};

const float GreedyExpectedMinMaxAgent::WHITE_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	4.0f, 4.5f, 5.0f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f,

	3.5f, 4.0f, 4.5f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	3.0f, 3.5f, 4.0f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	2.5f, 3.0f, 3.5f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	2.0f, 2.5f, 3.0f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	1.5f, 2.0f, 2.5f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	1.0f, 1.5f, 2.0f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	0.5f, 1.0f, 1.5f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f
};

const float GreedyExpectedMinMaxAgent::BLACK_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	0.5f, 1.0f, 1.5f, 1.5f, 1.5f, 1.0f, 0.5f, 0.0f,

	1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	1.5f, 2.0f, 2.5f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	2.0f, 2.5f, 3.0f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	2.5f, 3.0f, 3.5f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	3.5f, 4.0f, 4.5f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	4.5f, 5.0f, 5.5f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f
};

const float GreedyExpectedMinMaxAgent::BLACK_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	0.5f, 1.0f, 1.5f, 1.5f, 1.5f, 1.0f, 0.5f, 0.0f,

	1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	1.5f, 2.0f, 2.5f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	2.0f, 2.5f, 3.0f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	2.5f, 3.0f, 3.5f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	3.0f, 3.5f, 4.0f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	4.0f, 4.5f, 5.0f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	5.0f, 5.5f, 6.0f, 6.0f, 6.0f, 5.5f, 5.0f, 4.5f
};

const float GreedyExpectedMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	0.5f, 1.0f, 1.5f, 1.5f, 1.5f, 1.0f, 0.5f, 0.0f,

	1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	1.5f, 2.0f, 2.5f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	2.0f, 2.5f, 3.0f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	2.5f, 3.0f, 3.5f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	3.0f, 3.5f, 4.0f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	3.5f, 4.0f, 4.5f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	4.0f, 4.5f, 5.0f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f
};

const float GreedyExpectedMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	0.5f, 1.0f, 1.5f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	1.0f, 1.5f, 2.0f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	1.5f, 2.0f, 2.5f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	2.0f, 2.5f, 3.0f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	2.5f, 3.0f, 3.5f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	3.0f, 3.5f, 4.0f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	3.5f, 4.0f, 4.5f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	4.0f, 4.5f, 5.0f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f
};

const float GreedyExpectedMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	0.5f, 1.0f, 1.5f, 2.5f, 2.5f, 1.5f, 1.0f, 0.5f,

	1.0f, 1.5f, 2.0f, 3.0f, 3.0f, 2.0f, 1.5f, 1.0f,

	1.5f, 2.0f, 2.5f, 3.5f, 3.5f, 2.5f, 2.0f, 1.5f,

	2.0f, 2.5f, 3.0f, 4.0f, 4.0f, 3.0f, 2.5f, 2.0f,

	2.5f, 3.0f, 3.5f, 4.5f, 4.5f, 3.5f, 3.0f, 2.5f,

	3.0f, 3.5f, 4.0f, 5.0f, 5.0f, 4.0f, 3.5f, 3.0f,

	3.5f, 4.0f, 4.5f, 5.5f, 5.5f, 4.5f, 4.0f, 3.5f,

	4.0f, 4.5f, 5.0f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f
};

const float GreedyExpectedMinMaxAgent::BLACK_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
{
	0.5f, 1.0f, 1.5f, 2.0f, 2.0f, 1.5f, 1.0f, 0.5f,

	1.0f, 1.5f, 2.0f, 2.5f, 2.5f, 2.0f, 1.5f, 1.0f,

	1.5f, 2.0f, 2.5f, 3.0f, 3.0f, 2.5f, 2.0f, 1.5f,

	2.0f, 2.5f, 3.0f, 3.5f, 3.5f, 3.0f, 2.5f, 2.0f,

	2.5f, 3.0f, 3.5f, 4.0f, 4.0f, 3.5f, 3.0f, 2.5f,

	3.0f, 3.5f, 4.0f, 4.5f, 4.5f, 4.0f, 3.5f, 3.0f,

	3.5f, 4.0f, 4.5f, 5.0f, 5.0f, 4.5f, 4.0f, 3.5f,

	4.0f, 4.5f, 5.0f, 5.5f, 5.5f, 5.0f, 4.5f, 4.0f
};

const int GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED_FIND_BEST_MOVE = 25000000;
const int GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED_ESTIMATE_CONFIGURATION = 1000000;

const float GreedyExpectedMinMaxAgent::MAX_DISPLAY_ESTIMATION = GreedyExpectedMinMaxAgent::REACHABLE_INF / 2.0f;

