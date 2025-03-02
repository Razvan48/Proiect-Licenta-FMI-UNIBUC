#include "CachedGreedyExpectedMinMaxAgent.h"

#include "../../GameMetadata/GameMetadata.h"

#include <thread>

#include <future>

#include <iostream>

#include <map>

CachedGreedyExpectedMinMaxAgent::CachedGreedyExpectedMinMaxAgent()
	: GameAgent()
	, cacheTime(0ull)
{

}

CachedGreedyExpectedMinMaxAgent::~CachedGreedyExpectedMinMaxAgent()
{

}

CachedGreedyExpectedMinMaxAgent& CachedGreedyExpectedMinMaxAgent::get()
{
	static CachedGreedyExpectedMinMaxAgent instance;
	return instance;
}

float CachedGreedyExpectedMinMaxAgent::evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const
{
	// INFO: Presupunem ca are deja attack zone-urile calculate corect pentru ambele culori.
	// INFO: evaluateConfiguration nu se ocupa cu starile de Checkmate, acelea sunt gestionate in minMax.

	float evaluationScore = 0.0f;

	// White Pieces

	unsigned long long whitePawns = configurationMetadata.whitePawns;
	while (whitePawns)
	{
		unsigned long long lsb = (whitePawns & ((~whitePawns) + 1));
		evaluationScore = evaluationScore + CachedGreedyExpectedMinMaxAgent::PAWN_SCORE + CachedGreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::WHITE_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whitePawns ^= lsb;
	}

	unsigned long long whiteRooks = configurationMetadata.whiteRooks;
	while (whiteRooks)
	{
		unsigned long long lsb = (whiteRooks & ((~whiteRooks) + 1));
		evaluationScore = evaluationScore + CachedGreedyExpectedMinMaxAgent::ROOK_SCORE + CachedGreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::WHITE_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteRooks ^= lsb;
	}

	unsigned long long whiteKnights = configurationMetadata.whiteKnights;
	while (whiteKnights)
	{
		unsigned long long lsb = (whiteKnights & ((~whiteKnights) + 1));
		evaluationScore = evaluationScore + CachedGreedyExpectedMinMaxAgent::KNIGHT_SCORE + CachedGreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteKnights ^= lsb;
	}

	unsigned long long whiteBishops = configurationMetadata.whiteBishops;
	while (whiteBishops)
	{
		unsigned long long lsb = (whiteBishops & ((~whiteBishops) + 1));
		evaluationScore = evaluationScore + CachedGreedyExpectedMinMaxAgent::BISHOP_SCORE + CachedGreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteBishops ^= lsb;
	}

	unsigned long long whiteQueens = configurationMetadata.whiteQueens;
	while (whiteQueens)
	{
		unsigned long long lsb = (whiteQueens & ((~whiteQueens) + 1));
		evaluationScore = evaluationScore + CachedGreedyExpectedMinMaxAgent::QUEEN_SCORE + CachedGreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		whiteQueens ^= lsb;
	}

	evaluationScore = evaluationScore + CachedGreedyExpectedMinMaxAgent::KING_SCORE + CachedGreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::WHITE_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.whiteKing % BoardManager::MODULO_LOG_POWER_2]];

	// Black Pieces

	unsigned long long blackPawns = configurationMetadata.blackPawns;
	while (blackPawns)
	{
		unsigned long long lsb = (blackPawns & ((~blackPawns) + 1));
		evaluationScore = evaluationScore - CachedGreedyExpectedMinMaxAgent::PAWN_SCORE - CachedGreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::BLACK_PAWN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackPawns ^= lsb;
	}

	unsigned long long blackRooks = configurationMetadata.blackRooks;
	while (blackRooks)
	{
		unsigned long long lsb = (blackRooks & ((~blackRooks) + 1));
		evaluationScore = evaluationScore - CachedGreedyExpectedMinMaxAgent::ROOK_SCORE - CachedGreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::BLACK_ROOK_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackRooks ^= lsb;
	}

	unsigned long long blackKnights = configurationMetadata.blackKnights;
	while (blackKnights)
	{
		unsigned long long lsb = (blackKnights & ((~blackKnights) + 1));
		evaluationScore = evaluationScore - CachedGreedyExpectedMinMaxAgent::KNIGHT_SCORE - CachedGreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackKnights ^= lsb;
	}

	unsigned long long blackBishops = configurationMetadata.blackBishops;
	while (blackBishops)
	{
		unsigned long long lsb = (blackBishops & ((~blackBishops) + 1));
		evaluationScore = evaluationScore - CachedGreedyExpectedMinMaxAgent::BISHOP_SCORE - CachedGreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackBishops ^= lsb;
	}

	unsigned long long blackQueens = configurationMetadata.blackQueens;
	while (blackQueens)
	{
		unsigned long long lsb = (blackQueens & ((~blackQueens) + 1));
		evaluationScore = evaluationScore - CachedGreedyExpectedMinMaxAgent::QUEEN_SCORE - CachedGreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[BoardManager::get().logPower2[lsb % BoardManager::MODULO_LOG_POWER_2]];
		blackQueens ^= lsb;
	}

	evaluationScore = evaluationScore - CachedGreedyExpectedMinMaxAgent::KING_SCORE - CachedGreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR * CachedGreedyExpectedMinMaxAgent::BLACK_KING_POSITION_SCORES[BoardManager::get().logPower2[configurationMetadata.blackKing % BoardManager::MODULO_LOG_POWER_2]];

	// Attack Zones

	unsigned long long whiteAttackZones = configurationMetadata.whiteAttackZones;
	while (whiteAttackZones)
	{
		unsigned long long lsb = (whiteAttackZones & ((~whiteAttackZones) + 1));
		evaluationScore += CachedGreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE;
		whiteAttackZones ^= lsb;
	}

	unsigned long long blackAttackZones = configurationMetadata.blackAttackZones;
	while (blackAttackZones)
	{
		unsigned long long lsb = (blackAttackZones & ((~blackAttackZones) + 1));
		evaluationScore -= CachedGreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE;
		blackAttackZones ^= lsb;
	}

	// Checks
	if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
		evaluationScore -= CachedGreedyExpectedMinMaxAgent::CHECK_SCORE;
	if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
		evaluationScore += CachedGreedyExpectedMinMaxAgent::CHECK_SCORE;

	// White Turn
	if (configurationMetadata.whiteTurn)
		evaluationScore += CachedGreedyExpectedMinMaxAgent::WHITE_TURN_SCORE;
	else
		evaluationScore -= CachedGreedyExpectedMinMaxAgent::WHITE_TURN_SCORE;

	return evaluationScore;
}

void CachedGreedyExpectedMinMaxAgent::clearCache()
{
	// INFO: Nu ar fi nevoie de lock/unlock daca nu apelam aceasta metoda in alt thread.
	this->cacheTime = 0ull;
	this->lastTimeAccessedCache.clear();
	this->cache.clear();
}

std::pair<bool, std::pair<float, int>> CachedGreedyExpectedMinMaxAgent::getEntryFromCache(unsigned long long zobristHashingValue, unsigned long long& cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache)
{
	auto it = cache.find(zobristHashingValue);
	if (it != cache.end())
	{
		lastTimeAccessedCache.erase(it->second.second.second);
		++cacheTime;
		lastTimeAccessedCache[cacheTime] = zobristHashingValue;
		it->second.second.second = cacheTime;

		return std::make_pair(true, std::make_pair(it->second.first, it->second.second.first));
	}

	return std::make_pair(false, std::make_pair(0.0f, 0));
}

void CachedGreedyExpectedMinMaxAgent::addEntryInCache(unsigned long long zobristHashingValue, float evaluationScore, int depth, unsigned long long& cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache)
{
	auto it = cache.find(zobristHashingValue);
	if (it != cache.end())
	{
		if (depth < it->second.second.first)
		{
			lastTimeAccessedCache.erase(it->second.second.second);
			++cacheTime;
			lastTimeAccessedCache[cacheTime] = zobristHashingValue;
			it->second.first = evaluationScore;
			it->second.second.first = depth;
			it->second.second.second = cacheTime;
		}
		else
		{
			lastTimeAccessedCache.erase(it->second.second.second);
			++cacheTime;
			lastTimeAccessedCache[cacheTime] = zobristHashingValue;
			it->second.second.second = cacheTime;
		}
	}
	else
	{
		if (cache.size() >= CachedGreedyExpectedMinMaxAgent::MAXIMUM_CACHE_SIZE)
		{
			auto it = lastTimeAccessedCache.begin();
			cache.erase(it->second);
			lastTimeAccessedCache.erase(it);
		}

		++cacheTime;
		lastTimeAccessedCache[cacheTime] = zobristHashingValue;
		cache[zobristHashingValue] = std::make_pair(evaluationScore, std::make_pair(depth, cacheTime));
	}
}

void CachedGreedyExpectedMinMaxAgent::combineCaches(unsigned long long cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache)
{
	this->cacheTime = std::max(this->cacheTime, cacheTime);

	for (auto itOutside = cache.begin(); itOutside != cache.end(); ++itOutside)
	{
		auto itInside = this->cache.find(itOutside->first);
		if (itInside == this->cache.end())
		{
			if (this->cache.size() >= CachedGreedyExpectedMinMaxAgent::MAXIMUM_CACHE_SIZE)
			{
				auto it = this->lastTimeAccessedCache.begin();
				this->cache.erase(it->second);
				this->lastTimeAccessedCache.erase(it);
			}

			this->lastTimeAccessedCache[lastTimeAccessedCache[itOutside->first]] = itOutside->first;
			this->cache[itOutside->first] = itOutside->second;
		}
		else
		{
			// if (itOutside->second.second.first < itInside->second.second.first) // INFO: Aceasta conditie ar trebui sa fie mereu adevarata.
			// {
				this->lastTimeAccessedCache.erase(itInside->second.second.second);
				this->lastTimeAccessedCache[lastTimeAccessedCache[itOutside->first]] = itOutside->first;
				itInside->second.first = itOutside->second.first;
				itInside->second.second.first = itOutside->second.second.first;
				itInside->second.second.second = lastTimeAccessedCache[itOutside->first];
			// }
			/*
			else
			{
				this->lastTimeAccessedCache.erase(lastTimeAccessedCache[itOutside->first]);
				this->lastTimeAccessedCache[lastTimeAccessedCache[itOutside->first]] = itOutside->first;
			}
			*/
		}
	}
}

float CachedGreedyExpectedMinMaxAgent::minMax(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited, unsigned long long& cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache) // INFO: minMax primeste o copie a configuratiei.
{
	++numNodesVisited;

	if (this->isTaskCancelled.load())
	{
		std::cout << "MinMax Thread was cancelled." << std::endl;
		return 0.0f;
	}

	if (zobristHashingValuesFrequency[configurationMetadata.zobristHashingValue] >= GameMetadata::FREQUENCY_UNTIL_DRAW_REPETITION)
		return 0.0f; // Remiza, aceasta configuratie s-a repetat de FREQUENCY_UNTIL_DRAW_REPETITION ori.

	// Cache
	std::pair<bool, std::pair<float, int>> entryFromCache = this->getEntryFromCache(configurationMetadata.zobristHashingValue, cacheTime, lastTimeAccessedCache, cache);
	if (entryFromCache.first && entryFromCache.second.second <= depth)
		return entryFromCache.second.first;

	if (depth >= CachedGreedyExpectedMinMaxAgent::MAX_DEPTH && expectedNumNodesVisited <= 0) // INFO: Trebuie <= 0 pentru expectedNumNodesVisited
	{
		BoardManager::get().generateWhiteAttackZones(configurationMetadata);
		BoardManager::get().generateBlackAttackZones(configurationMetadata);
		float evaluationScore = this->evaluateConfiguration(configurationMetadata);

		// Cache
		this->addEntryInCache(configurationMetadata.zobristHashingValue, evaluationScore, depth, cacheTime, lastTimeAccessedCache, cache);

		return evaluationScore;
	}

	if (configurationMetadata.whiteTurn) // Maximizing
	{
		float maximumScore = -CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
		BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

		int expectedRemainingNumNodesVisited = expectedNumNodesVisited;
		for (int i = 0; i < allWhiteMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			int numNodesVisitedBefore = numNodesVisited;
			float currentScore = this->minMax(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allWhiteMoves.size() - i), cacheTime, lastTimeAccessedCache, cache);
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

		if (maximumScore == -CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isWhiteKingInCheck(configurationMetadata))
				maximumScore = -CachedGreedyExpectedMinMaxAgent::REACHABLE_INF + 1.0f * depth; // INFO: Se penalizeaza mai mult din cauza ca este mai adanc in cadrul arborelui MinMax.
			else
				maximumScore = 0.0f; // Remiza, albul nu poate face mutari, dar nu e nici in sah.
		}

		// Cache
		this->addEntryInCache(configurationMetadata.zobristHashingValue, maximumScore, depth, cacheTime, lastTimeAccessedCache, cache);

		return maximumScore;
	}
	else // Minimizing
	{
		float minimumScore = CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF;
		std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

		std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
		BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

		int expectedRemainingNumNodesVisited = expectedNumNodesVisited;
		for (int i = 0; i < allBlackMoves.size(); ++i)
		{
			ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
			int numNodesVisitedBefore = numNodesVisited;
			float currentScore = this->minMax(newConfigurationMetadata, depth + 1, alpha, beta, zobristHashingValuesFrequency, numNodesVisited, expectedRemainingNumNodesVisited / ((int)allBlackMoves.size() - i), cacheTime, lastTimeAccessedCache, cache);
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

		if (minimumScore == CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF)
		{
			if (BoardManager::get().isBlackKingInCheck(configurationMetadata))
				minimumScore = CachedGreedyExpectedMinMaxAgent::REACHABLE_INF - 1.0f * depth; // INFO: Se penalizeaza mai mult din cauza ca este mai adanc in cadrul arborelui MinMax.
			else
				minimumScore = 0.0f; // Remiza, negrul nu poate face mutari, dar nu e nici in sah.
		}

		// Cache
		this->addEntryInCache(configurationMetadata.zobristHashingValue, minimumScore, depth, cacheTime, lastTimeAccessedCache, cache);

		return minimumScore;
	}
}

void CachedGreedyExpectedMinMaxAgent::findBestMove(ConfigurationMetadata& configurationMetadata)
{
	this->setIsRunningTask(true);
	this->setBestMove(std::vector<std::pair<char, int>>());

	this->isTaskCancelled.store(false);

	std::map<unsigned long long int, int> zobristHashingValuesFrequency(BoardManager::get().getZobristHashingValuesFrequency());
	std::thread findBestMoveThread([this, &configurationMetadata, zobristHashingValuesFrequency]() mutable
		{
			std::vector<std::future<float>> scoreFutures;
			std::vector<std::future<int>> numNodesVisitedFutures;

			std::vector<std::future<unsigned long long>> cacheTimeFutures;
			std::vector<std::future<std::map<unsigned long long, unsigned long long>>> lastTimeAccessedCacheFutures;
			std::vector<std::future<std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>>> cacheFutures;

			std::vector<std::thread> threads;

			int numNodesVisitedTotal = 0;

			if (configurationMetadata.whiteTurn)
			{
				float maximumScore = -CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allWhiteMoves;
				BoardManager::get().generateWhiteMoves(configurationMetadata, allWhiteMoves);

				for (int i = 0; i < allWhiteMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());
					std::promise<unsigned long long> cacheTimePromise;
					cacheTimeFutures.push_back(cacheTimePromise.get_future());
					std::promise<std::map<unsigned long long, unsigned long long>> lastTimeAccessedCachePromise;
					lastTimeAccessedCacheFutures.push_back(lastTimeAccessedCachePromise.get_future());
					std::promise<std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>> cachePromise;
					cacheFutures.push_back(cachePromise.get_future());

					threads.push_back(std::thread([this, configurationMetadata, allWhiteMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequency, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = CachedGreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED / (int)allWhiteMoves.size(), cacheTimePromise = std::move(cacheTimePromise), lastTimeAccessedCachePromise = std::move(lastTimeAccessedCachePromise), cachePromise = std::move(cachePromise)]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allWhiteMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							unsigned long long cacheTime = this->cacheTime;
							std::map<unsigned long long, unsigned long long> lastTimeAccessedCache(this->lastTimeAccessedCache);
							std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>> cache(this->cache);
							float currentScore = this->minMax(newConfigurationMetadata, 0, -CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF, CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequency, numNodesVisited, expectedNumNodesVisitedThisThread, cacheTime, lastTimeAccessedCache, cache);
							--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

							// INFO: Entry-ul in map ramane si daca e pe 0 frecventa.

							scorePromise.set_value(currentScore);
							numNodesVisitedPromise.set_value(numNodesVisited);

							cacheTimePromise.set_value(cacheTime);
							lastTimeAccessedCachePromise.set_value(lastTimeAccessedCache);
							cachePromise.set_value(cache);
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

					// Cache
					std::map<unsigned long long, unsigned long long> lastTimeAccessedCache(lastTimeAccessedCacheFutures[i].get());
					std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>> cache(cacheFutures[i].get());
					this->combineCaches(cacheTimeFutures[i].get(), lastTimeAccessedCache, cache);
				}

				if (!this->isTaskCancelled.load())
					this->setBestMove(bestMove);
			}
			else
			{
				float minimumScore = CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF;
				std::vector<std::pair<char, int>> bestMove = std::vector<std::pair<char, int>>();

				std::vector<std::vector<std::pair<char, int>>> allBlackMoves;
				BoardManager::get().generateBlackMoves(configurationMetadata, allBlackMoves);

				for (int i = 0; i < allBlackMoves.size(); ++i)
				{
					std::promise<float> scorePromise;
					scoreFutures.push_back(scorePromise.get_future());
					std::promise<int> numNodesVisitedPromise;
					numNodesVisitedFutures.push_back(numNodesVisitedPromise.get_future());
					std::promise<unsigned long long> cacheTimePromise;
					cacheTimeFutures.push_back(cacheTimePromise.get_future());
					std::promise<std::map<unsigned long long, unsigned long long>> lastTimeAccessedCachePromise;
					lastTimeAccessedCacheFutures.push_back(lastTimeAccessedCachePromise.get_future());
					std::promise<std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>> cachePromise;
					cacheFutures.push_back(cachePromise.get_future());

					threads.push_back(std::thread([this, configurationMetadata, allBlackMoves, i, scorePromise = std::move(scorePromise), zobristHashingValuesFrequency, numNodesVisitedPromise = std::move(numNodesVisitedPromise), expectedNumNodesVisitedThisThread = CachedGreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED / (int)allBlackMoves.size(), cacheTimePromise = std::move(cacheTimePromise), lastTimeAccessedCachePromise = std::move(lastTimeAccessedCachePromise), cachePromise = std::move(cachePromise)]() mutable
						{
							ConfigurationMetadata newConfigurationMetadata = BoardManager::get().applyMoveInternal(configurationMetadata, allBlackMoves[i], zobristHashingValuesFrequency); // INFO: Se incrementeaza in apel frecventa.
							int numNodesVisited = 0;
							unsigned long long cacheTime = this->cacheTime;
							std::map<unsigned long long, unsigned long long> lastTimeAccessedCache(this->lastTimeAccessedCache);
							std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>> cache(this->cache);
							float currentScore = this->minMax(newConfigurationMetadata, 0, -CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF, CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF, zobristHashingValuesFrequency, numNodesVisited, expectedNumNodesVisitedThisThread, cacheTime, lastTimeAccessedCache, cache);
							--zobristHashingValuesFrequency[newConfigurationMetadata.zobristHashingValue];

							// INFO: Entry-ul in map ramane si daca e pe 0 frecventa.

							scorePromise.set_value(currentScore);
							numNodesVisitedPromise.set_value(numNodesVisited);

							cacheTimePromise.set_value(cacheTime);
							lastTimeAccessedCachePromise.set_value(lastTimeAccessedCache);
							cachePromise.set_value(cache);
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

					// Cache
					std::map<unsigned long long, unsigned long long> lastTimeAccessedCache(lastTimeAccessedCacheFutures[i].get());
					std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>> cache(cacheFutures[i].get());
					this->combineCaches(cacheTimeFutures[i].get(), lastTimeAccessedCache, cache);
				}

				if (!this->isTaskCancelled.load())
					this->setBestMove(bestMove);
			}

			std::cout << "CachedGreedyExpectedMinMaxAgent: Total Number of Nodes Visited: " << numNodesVisitedTotal << std::endl;
		});

	findBestMoveThread.detach();
}

void CachedGreedyExpectedMinMaxAgent::reset()
{
	this->clearCache();
}

const int CachedGreedyExpectedMinMaxAgent::MAX_DEPTH = 4;

const float CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF = 524288.0; // INFO: 2 ^ 19
const float CachedGreedyExpectedMinMaxAgent::REACHABLE_INF = CachedGreedyExpectedMinMaxAgent::UNREACHABLE_INF / 2.0f;

const float CachedGreedyExpectedMinMaxAgent::PAWN_SCORE = 1.0f;
const float CachedGreedyExpectedMinMaxAgent::ROOK_SCORE = 5.0f;
const float CachedGreedyExpectedMinMaxAgent::KNIGHT_SCORE = 3.0f;
const float CachedGreedyExpectedMinMaxAgent::BISHOP_SCORE = 3.15f;
const float CachedGreedyExpectedMinMaxAgent::QUEEN_SCORE = 9.0f;
const float CachedGreedyExpectedMinMaxAgent::KING_SCORE = 100.0f;

const float CachedGreedyExpectedMinMaxAgent::ATTACK_ZONE_SCORE = 0.05f;
const float CachedGreedyExpectedMinMaxAgent::CHECK_SCORE = 0.1f;

const float CachedGreedyExpectedMinMaxAgent::PAWN_POSITION_SCORE_FACTOR = 0.1f;
const float CachedGreedyExpectedMinMaxAgent::ROOK_POSITION_SCORE_FACTOR = 0.1f;
const float CachedGreedyExpectedMinMaxAgent::KNIGHT_POSITION_SCORE_FACTOR = 0.1f;
const float CachedGreedyExpectedMinMaxAgent::BISHOP_POSITION_SCORE_FACTOR = 0.1f;
const float CachedGreedyExpectedMinMaxAgent::QUEEN_POSITION_SCORE_FACTOR = 0.1f;
const float CachedGreedyExpectedMinMaxAgent::KING_POSITION_SCORE_FACTOR = 0.1f;

const float CachedGreedyExpectedMinMaxAgent::WHITE_TURN_SCORE = 0.1f;

const float CachedGreedyExpectedMinMaxAgent::WHITE_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::WHITE_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::WHITE_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::WHITE_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::WHITE_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::WHITE_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::BLACK_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::BLACK_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::BLACK_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::BLACK_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::BLACK_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const float CachedGreedyExpectedMinMaxAgent::BLACK_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH] =
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

const int CachedGreedyExpectedMinMaxAgent::EXPECTED_NUM_NODES_VISITED = 20000000;

const int CachedGreedyExpectedMinMaxAgent::MAXIMUM_CACHE_SIZE = 100000;

