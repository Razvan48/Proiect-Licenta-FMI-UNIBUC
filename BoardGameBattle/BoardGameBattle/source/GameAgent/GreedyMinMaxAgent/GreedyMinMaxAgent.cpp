#include "GreedyMinMaxAgent.h"

#include "../../GameMetadata/GameMetadata.h"

#include <thread>

#include <future>

#include <iostream>

#include <map>

GreedyMinMaxAgent::GreedyMinMaxAgent()
	: GameAgent()
{

}

GreedyMinMaxAgent::~GreedyMinMaxAgent()
{

}

GreedyMinMaxAgent& GreedyMinMaxAgent::get()
{
	static GreedyMinMaxAgent instance;
	return instance;
}

float GreedyMinMaxAgent::evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const
{
	// INFO: Presupunem ca are deja attack zone-urile calculate corect pentru ambele culori.
	// INFO: evaluateConfiguration nu se ocupa cu starile de Checkmate, acelea sunt gestionate in minMax.

	float evaluationScore = 0.0f;

	// White Pieces

	unsigned long long whitePawns = configurationMetadata.whitePawns;
	while (whitePawns)
	{
		unsigned long long lsb = (whitePawns & ((~whitePawns) + 1));
		evaluationScore = evaluationScore + GreedyMinMaxAgent::PAWN_SCORE + GreedyMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::WHITE_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whitePawns ^= lsb;
	}

	unsigned long long whiteRooks = configurationMetadata.whiteRooks;
	while (whiteRooks)
	{
		unsigned long long lsb = (whiteRooks & ((~whiteRooks) + 1));
		evaluationScore = evaluationScore + GreedyMinMaxAgent::ROOK_SCORE + GreedyMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::WHITE_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteRooks ^= lsb;
	}

	unsigned long long whiteKnights = configurationMetadata.whiteKnights;
	while (whiteKnights)
	{
		unsigned long long lsb = (whiteKnights & ((~whiteKnights) + 1));
		evaluationScore = evaluationScore + GreedyMinMaxAgent::KNIGHT_SCORE + GreedyMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteKnights ^= lsb;
	}

	unsigned long long whiteBishops = configurationMetadata.whiteBishops;
	while (whiteBishops)
	{
		unsigned long long lsb = (whiteBishops & ((~whiteBishops) + 1));
		evaluationScore = evaluationScore + GreedyMinMaxAgent::BISHOP_SCORE + GreedyMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteBishops ^= lsb;
	}

	unsigned long long whiteQueens = configurationMetadata.whiteQueens;
	while (whiteQueens)
	{
		unsigned long long lsb = (whiteQueens & ((~whiteQueens) + 1));
		evaluationScore = evaluationScore + GreedyMinMaxAgent::QUEEN_SCORE + GreedyMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteQueens ^= lsb;
	}

	evaluationScore = evaluationScore + GreedyMinMaxAgent::KING_SCORE + GreedyMinMaxAgent::KING_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::WHITE_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]];

	// Black Pieces

	unsigned long long blackPawns = configurationMetadata.blackPawns;
	while (blackPawns)
	{
		unsigned long long lsb = (blackPawns & ((~blackPawns) + 1));
		evaluationScore = evaluationScore - GreedyMinMaxAgent::PAWN_SCORE - GreedyMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::BLACK_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackPawns ^= lsb;
	}

	unsigned long long blackRooks = configurationMetadata.blackRooks;
	while (blackRooks)
	{
		unsigned long long lsb = (blackRooks & ((~blackRooks) + 1));
		evaluationScore = evaluationScore - GreedyMinMaxAgent::ROOK_SCORE - GreedyMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::BLACK_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackRooks ^= lsb;
	}

	unsigned long long blackKnights = configurationMetadata.blackKnights;
	while (blackKnights)
	{
		unsigned long long lsb = (blackKnights & ((~blackKnights) + 1));
		evaluationScore = evaluationScore - GreedyMinMaxAgent::KNIGHT_SCORE - GreedyMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackKnights ^= lsb;
	}

	unsigned long long blackBishops = configurationMetadata.blackBishops;
	while (blackBishops)
	{
		unsigned long long lsb = (blackBishops & ((~blackBishops) + 1));
		evaluationScore = evaluationScore - GreedyMinMaxAgent::BISHOP_SCORE - GreedyMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackBishops ^= lsb;
	}

	unsigned long long blackQueens = configurationMetadata.blackQueens;
	while (blackQueens)
	{
		unsigned long long lsb = (blackQueens & ((~blackQueens) + 1));
		evaluationScore = evaluationScore - GreedyMinMaxAgent::QUEEN_SCORE - GreedyMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackQueens ^= lsb;
	}

	evaluationScore = evaluationScore - GreedyMinMaxAgent::KING_SCORE - GreedyMinMaxAgent::KING_POSITION_SCORE_FACTOR * GreedyMinMaxAgent::BLACK_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]];

	// Attack Zones

	unsigned long long whiteAttackZones = configurationMetadata.whiteAttackZones;
	while (whiteAttackZones)
	{
		unsigned long long lsb = (whiteAttackZones & ((~whiteAttackZones) + 1));
		evaluationScore += GreedyMinMaxAgent::ATTACK_ZONE_SCORE;
		whiteAttackZones ^= lsb;
	}

	unsigned long long blackAttackZones = configurationMetadata.blackAttackZones;
	while (blackAttackZones)
	{
		unsigned long long lsb = (blackAttackZones & ((~blackAttackZones) + 1));
		evaluationScore -= GreedyMinMaxAgent::ATTACK_ZONE_SCORE;
		blackAttackZones ^= lsb;
	}

	// Checks
	if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
		evaluationScore -= GreedyMinMaxAgent::CHECK_SCORE;
	if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
		evaluationScore += GreedyMinMaxAgent::CHECK_SCORE;

	// White Turn
	if (configurationMetadata.whiteTurn)
		evaluationScore += GreedyMinMaxAgent::WHITE_TURN_SCORE;
	else
		evaluationScore -= GreedyMinMaxAgent::WHITE_TURN_SCORE;

	return evaluationScore;
}

float GreedyMinMaxAgent::minMax(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency) const // INFO: minMax primeste o copie a configuratiei.
{
	if (this->isTaskCancelled.load())
	{
		std::cout << "MinMax Thread was cancelled." << std::endl;
		return 0.0f;
	}

	if (zobristHashingValuesFrequency[configurationMetadata.zobristHashingValue] >= GameMetadata::FREQUENCY_UNTIL_DRAW_REPETITION)
		return 0.0f; // Remiza, aceasta configuratie s-a repetat de FREQUENCY_UNTIL_DRAW_REPETITION ori.

	if (depth == 0)
	{
		BoardManager::get().generateWhiteAttackZones(configurationMetadata);
		BoardManager::get().generateBlackAttackZones(configurationMetadata);
		return this->evaluateConfiguration(configurationMetadata);
	}

	if (configurationMetadata.whiteTurn) // Maximizing
	{
		float maximumScore = -GreedyMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
		BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

		for (int i = 0; i < allWhiteMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			float currentScore = this->minMax(newConfigurationMetadata, depth - 1, alpha, beta, zobristHashingValuesFrequency);
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

		if (maximumScore == -GreedyMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
				maximumScore = -GreedyMinMaxAgent::REACHABLE_INF;
			else
				maximumScore = 0.0f; // Remiza, albul nu poate face mutari, dar nu e nici in sah.
		}

		return maximumScore;
	}
	else // Minimizing
	{
		float minimumScore = GreedyMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
		BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

		for (int i = 0; i < allBlackMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			float currentScore = this->minMax(newConfigurationMetadata, depth - 1, alpha, beta, zobristHashingValuesFrequency);
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

		if (minimumScore == GreedyMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
				minimumScore = GreedyMinMaxAgent::REACHABLE_INF;
			else
				minimumScore = 0.0f; // Remiza, negrul nu poate face mutari, dar nu e nici in sah.
		}

		return minimumScore;
	}
}

void GreedyMinMaxAgent::findBestMove(ConfigurationMetadata& configurationMetadata)
{
	// TODO: multithreading (de inteles future si promises)
	//return this->minMax(configurationMetadata, GreedyMinMaxAgent::MAX_DEPTH, -GreedyMinMaxAgent::INF, GreedyMinMaxAgent::INF).second;
	
	this->setIsRunningTask(true);
	this->setBestMove(std::vector<std::pair<char, int>>());

	this->isTaskCancelled.store(false);

	std::map<unsigned long long int, int> zobristHashingValuesFrequency(BoardManager::get().getZobristHashingValuesFrequency());
	std::thread findBestMoveThread([this, &configurationMetadata, zobristHashingValuesFrequency]() mutable
		{
			std::vector<std::future<float>> futures;
			std::vector<std::thread> threads;

			if (configurationMetadata.whiteTurn)
			{
				float maximumScore = -GreedyMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
				BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

				for (int i = 0; i < allWhiteMoves.size(); ++i)
				{
					std::promise<float> promise;
					futures.push_back(promise.get_future());

					std::map<unsigned long long, int> zobristHashingValuesFrequencyCopy(zobristHashingValuesFrequency);
					threads.push_back(std::thread([this, configurationMetadata, allWhiteMoves, i, promise = std::move(promise), zobristHashingValuesFrequencyCopy]() mutable
						{
							float currentScore = this->minMax(BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], BoardManager::get().getZobristHashingValuesFrequency()), GreedyMinMaxAgent::MAX_DEPTH - 1, -GreedyMinMaxAgent::UNREACHABLE_INF, GreedyMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequencyCopy);
							promise.set_value(currentScore);
						}));
				}

				for (int i = 0; i < threads.size(); ++i)
				{
					threads[i].join();

					float currentScore = futures[i].get();
					if (currentScore > maximumScore)
					{
						maximumScore = currentScore;
						bestMove = allWhiteMoves[i];
					}
				}

				if (!this->isTaskCancelled.load())
					this->setBestMove(bestMove);
			}
			else
			{
				float minimumScore = GreedyMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
				BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

				for (int i = 0; i < allBlackMoves.size(); ++i)
				{
					std::promise<float> promise;
					futures.push_back(promise.get_future());

					std::map<unsigned long long, int> zobristHashingValuesFrequencyCopy(zobristHashingValuesFrequency);
					threads.push_back(std::thread([this, configurationMetadata, allBlackMoves, i, promise = std::move(promise), zobristHashingValuesFrequencyCopy]() mutable
						{
							float currentScore = this->minMax(BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], BoardManager::get().getZobristHashingValuesFrequency()), GreedyMinMaxAgent::MAX_DEPTH - 1, -GreedyMinMaxAgent::UNREACHABLE_INF, GreedyMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequencyCopy);
							promise.set_value(currentScore);
						}));
				}

				for (int i = 0; i < threads.size(); ++i)
				{
					threads[i].join();

					float currentScore = futures[i].get();
					if (currentScore < minimumScore)
					{
						minimumScore = currentScore;
						bestMove = allBlackMoves[i];
					}
				}

				if (!this->isTaskCancelled.load())
					this->setBestMove(bestMove);
			}
		});

	findBestMoveThread.detach();
}

const int GreedyMinMaxAgent::MAX_DEPTH = 5;

const float GreedyMinMaxAgent::UNREACHABLE_INF = 65536.0f;
const float GreedyMinMaxAgent::REACHABLE_INF = GreedyMinMaxAgent::UNREACHABLE_INF - 1.0f;

const float GreedyMinMaxAgent::PAWN_SCORE = 1.0f;
const float GreedyMinMaxAgent::ROOK_SCORE = 5.0f;
const float GreedyMinMaxAgent::KNIGHT_SCORE = 3.0f;
const float GreedyMinMaxAgent::BISHOP_SCORE = 3.15f;
const float GreedyMinMaxAgent::QUEEN_SCORE = 9.0f;
const float GreedyMinMaxAgent::KING_SCORE = 100.0f;

const float GreedyMinMaxAgent::ATTACK_ZONE_SCORE = 0.15f;
const float GreedyMinMaxAgent::CHECK_SCORE = 0.2f;

const float GreedyMinMaxAgent::PAWN_POSITION_SCORE_FACTOR = 0.18f;
const float GreedyMinMaxAgent::ROOK_POSITION_SCORE_FACTOR = 0.18f;
const float GreedyMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR = 0.18f;
const float GreedyMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR = 0.18f;
const float GreedyMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR = 0.18f;
const float GreedyMinMaxAgent::KING_POSITION_SCORE_FACTOR = 0.18f;

const float GreedyMinMaxAgent::WHITE_TURN_SCORE = 0.1f;

const float GreedyMinMaxAgent::WHITE_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::WHITE_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::WHITE_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::BLACK_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::BLACK_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float GreedyMinMaxAgent::BLACK_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

