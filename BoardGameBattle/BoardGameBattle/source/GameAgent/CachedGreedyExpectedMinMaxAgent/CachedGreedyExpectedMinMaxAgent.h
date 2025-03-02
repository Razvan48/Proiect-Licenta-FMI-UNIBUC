#pragma once

#include "../GameAgent.h"

#include "../../BoardManager/BoardManager.h"

#include "../../GameMetadata/GameMetadata.h"

#include <map>

class CachedGreedyExpectedMinMaxAgent : virtual public GameAgent
{
private:
	CachedGreedyExpectedMinMaxAgent();
	virtual ~CachedGreedyExpectedMinMaxAgent();
	CachedGreedyExpectedMinMaxAgent(const CachedGreedyExpectedMinMaxAgent& other) = delete;
	CachedGreedyExpectedMinMaxAgent& operator= (const CachedGreedyExpectedMinMaxAgent& other) = delete;
	CachedGreedyExpectedMinMaxAgent(const CachedGreedyExpectedMinMaxAgent&& other) = delete;
	CachedGreedyExpectedMinMaxAgent& operator= (const CachedGreedyExpectedMinMaxAgent&& other) = delete;

protected:
	static const int MAX_DEPTH;

	static const float UNREACHABLE_INF;
	static const float REACHABLE_INF;

	static const float PAWN_SCORE;
	static const float ROOK_SCORE;
	static const float KNIGHT_SCORE;
	static const float BISHOP_SCORE;
	static const float QUEEN_SCORE;
	static const float KING_SCORE;

	static const float ATTACK_ZONE_SCORE;
	static const float CHECK_SCORE;

	static const float PAWN_POSITION_SCORE_FACTOR;
	static const float ROOK_POSITION_SCORE_FACTOR;
	static const float KNIGHT_POSITION_SCORE_FACTOR;
	static const float BISHOP_POSITION_SCORE_FACTOR;
	static const float QUEEN_POSITION_SCORE_FACTOR;
	static const float KING_POSITION_SCORE_FACTOR;

	static const float WHITE_TURN_SCORE;

	static const float WHITE_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float WHITE_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float WHITE_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float WHITE_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float WHITE_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float WHITE_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	static const float BLACK_PAWN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float BLACK_ROOK_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float BLACK_KNIGHT_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float BLACK_BISHOP_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float BLACK_QUEEN_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];
	static const float BLACK_KING_POSITION_SCORES[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH];

	static const int EXPECTED_NUM_NODES_VISITED;

	static const int MAXIMUM_CACHE_SIZE;

	unsigned long long cacheTime;
	std::map<unsigned long long, unsigned long long> lastTimeAccessedCache;
	std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>> cache;

	void clearCache();
	std::pair<bool, std::pair<float, int>> getEntryFromCache(unsigned long long zobristHashingValue, unsigned long long& cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache);
	void addEntryInCache(unsigned long long zobristHashingValue, float evaluationScore, int depth, unsigned long long& cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache);
	void combineCaches(unsigned long long cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache);

	float minMax(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited, unsigned long long& cacheTime, std::map<unsigned long long, unsigned long long>& lastTimeAccessedCache, std::map<unsigned long long, std::pair<float, std::pair<int, unsigned long long>>>& cache); // INFO: minMax primeste o copie a configuratiei si a map-ului de frecvente

public:
	static CachedGreedyExpectedMinMaxAgent& get();

	virtual float evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const override;
	virtual void findBestMove(ConfigurationMetadata& configurationMetadata) override;
	virtual void reset() override;
};




