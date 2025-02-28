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

float GreedyExpectedMinMaxAgent::minMax(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited) const // INFO: minMax primeste o copie a configuratiei.
{
	++numNodesVisited;

	if (this->isTaskCancelled.load())
	{
		std::cout << "MinMax Thread was cancelled." << std::endl;
		return 0.0f;
	}

	if (zobristHashingValuesFrequency[configurationMetadata.zobristHashingValue] >= GameMetadata::FREQUENCY_UNTIL_DRAW_REPETITION)
		return 0.0f; // Remiza, aceasta configuratie s-a repetat de FREQUENCY_UNTIL_DRAW_REPETITION ori.

	if (depth >= GreedyExpectedMinMaxAgent::MAX_DEPTH && expectedNumNodesVisited <= 0) // INFO: Trebuie <= 0 pentru expectedNumNodesVisited
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
			float currentScore = this->minMax(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allWhiteMoves.size() - i));
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
			float currentScore = this->minMax(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allBlackMoves.size() - i));
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

		return minimumScore;
	}
}

void GreedyExpectedMinMaxAgent::findBestMove(ConfigurationMetadata& configurationMetadata)
{
	this->setIsRunningTask(true);
	this->setBestMove(std::vector<std::pair<char, int>>());

	this->isTaskCancelled.store(false);

	std::map<unsigned long long int, int> zobristHashingValuesFrequency(BoardManager::get().getZobristHashingValuesFrequency());
	std::thread findBestMoveThread([this, &configurationMetadata, zobristHashingValuesFrequency]() mutable
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

				for (int i = 0; i < allWhiteMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());

					std::map<unsigned long long, int> zobristHashingValuesFrequencyCopy(zobristHashingValuesFrequency);
					threads.push_back(std::thread([this, configurationMetadata, allWhiteMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequencyCopy, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED / (int)allWhiteMoves.size()]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequencyCopy); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							float currentScore = this->minMax(newConfigurationMetadata, 0, -GreedyExpectedMinMaxAgent::UNREACHABLE_INF, GreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequencyCopy, numNodesVisited, expectedNumNodesVisitedThisThread);
							--zobristHashingValuesFrequencyCopy[newConfigurationMetadata.zobristHashingValue];

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

				if (!this->isTaskCancelled.load())
					this->setBestMove(bestMove);
			}
			else
			{
				float minimumScore = GreedyExpectedMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
				BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

				for (int i = 0; i < allBlackMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());

					std::map<unsigned long long, int> zobristHashingValuesFrequencyCopy(zobristHashingValuesFrequency);
					threads.push_back(std::thread([this, configurationMetadata, allBlackMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequencyCopy, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED / (int)allBlackMoves.size()]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequencyCopy); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							float currentScore = this->minMax(newConfigurationMetadata, 0, -GreedyExpectedMinMaxAgent::UNREACHABLE_INF, GreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequencyCopy, numNodesVisited, expectedNumNodesVisitedThisThread);
							--zobristHashingValuesFrequencyCopy[newConfigurationMetadata.zobristHashingValue];

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

				if (!this->isTaskCancelled.load())
					this->setBestMove(bestMove);
			}

			std::cout << "GreedyExpectedMinMaxAgent: Total Number of Nodes Visited: " << numNodesVisitedTotal << std::endl;
		});

	findBestMoveThread.detach();
}

const int GreedyExpectedMinMaxAgent::MAX_DEPTH = 4;

const float GreedyExpectedMinMaxAgent::UNREACHABLE_INF = 524288.0; // INFO: 2 ^ 19
const float GreedyExpectedMinMaxAgent::REACHABLE_INF = GreedyExpectedMinMaxAgent::UNREACHABLE_INF / 2.0f;

const float GreedyExpectedMinMaxAgent::PAWN_SCORE = 1.0f;
const float GreedyExpectedMinMaxAgent::ROOK_SCORE = 5.0f;
const float GreedyExpectedMinMaxAgent::KNIGHT_SCORE = 3.0f;
const float GreedyExpectedMinMaxAgent::BISHOP_SCORE = 3.15f;
const float GreedyExpectedMinMaxAgent::QUEEN_SCORE = 9.0f;
const float GreedyExpectedMinMaxAgent::KING_SCORE = 100.0f;

const float GreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE = 0.05f;
const float GreedyExpectedMinMaxAgent::CHECK_SCORE = 0.1f;

const float GreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR = 0.1f;
const float GreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR = 0.1f;
const float GreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR = 0.1f;
const float GreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR = 0.1f;
const float GreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR = 0.1f;
const float GreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR = 0.1f;

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

const int GreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED = 17500000;

