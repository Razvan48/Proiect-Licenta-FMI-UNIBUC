#pragma once

#include "../GameAgent.h"

#include "../../BoardManager/BoardManager.h"

#include "../../GameMetadata/GameMetadata.h"

#include <vector>
#include <map>

struct CellPressure
{
	int whitePressureCount;
	int blackPressureCount;
	bool whiteKingInvolved;
	bool blackKingInvolved;

	CellPressure()
	{
		this->whitePressureCount = 0;
		this->blackPressureCount = 0;
		this->whiteKingInvolved = false;
		this->blackKingInvolved = false;
	}
};

class GreedyExpectedMinMaxAgent : virtual public GameAgent
{
private:
	GreedyExpectedMinMaxAgent();
	virtual ~GreedyExpectedMinMaxAgent();
	GreedyExpectedMinMaxAgent(const GreedyExpectedMinMaxAgent& other) = delete;
	GreedyExpectedMinMaxAgent& operator= (const GreedyExpectedMinMaxAgent& other) = delete;
	GreedyExpectedMinMaxAgent(const GreedyExpectedMinMaxAgent&& other) = delete;
	GreedyExpectedMinMaxAgent& operator= (const GreedyExpectedMinMaxAgent&& other) = delete;

protected:
	static const int MAX_DEPTH_FIND_BEST_MOVE;
	static const int MAX_DEPTH_ESTIMATE_CONFIGURATION;

	static const float UNREACHABLE_INF;
	static const float REACHABLE_INF;

	static const float PAWN_SCORE;
	static const float ROOK_SCORE;
	static const float KNIGHT_SCORE;
	static const float BISHOP_SCORE;
	static const float QUEEN_SCORE;
	static const float KING_SCORE;

	static const float ATTACK_ZONE_SCORE;
	static const float DEFENSE_ZONE_SCORE;
	static const float COVER_ZONE_SCORE;
	static const float CHAIN_CAPTURE_SCORE;
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

	static const int EXPECTED_NUM_NODES_VISITED_FIND_BEST_MOVE;
	static const int EXPECTED_NUM_NODES_VISITED_ESTIMATE_CONFIGURATION;

	static const float MAX_DISPLAY_ESTIMATION;

	std::vector<std::map<unsigned long long, std::pair<int, float>>> alreadyCalculatedNodesMapsFindBestMove;
	std::vector<std::map<unsigned long long, std::pair<int, float>>> alreadyCalculatedNodesMapsEstimateConfiguration;
	void updateAlreadyCalculatedNodes(std::map<unsigned long long, std::pair<int, float>>& alreadyCalculatedNodes, unsigned long long zobristHashingValue, int depth, float value) const;
	float minMaxFindBestMove(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited, std::map<unsigned long long, std::pair<int, float>>& alreadyCalculatedNodes) const; // INFO: minMaxFindBestMove primeste o copie a configuratiei si a map-ului de frecvente.
	float minMaxEstimateConfiguration(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta, std::map<unsigned long long, int>& zobristHashingValuesFrequency, int& numNodesVisited, int expectedNumNodesVisited, std::map<unsigned long long, std::pair<int, float>>& alreadyCalculatedNodes) const; // INFO: minMaxEstimateConfiguration primeste o copie a configuratiei si a map-ului de frecvente.

public:
	static GreedyExpectedMinMaxAgent& get();

	virtual float evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const override;
	virtual void findBestMove(ConfigurationMetadata& configurationMetadata) override;
	virtual void reset() override;
	virtual void estimateConfiguration(ConfigurationMetadata& configurationMetadata) override;
	virtual float getMaxDisplayEstimation() const override;
};




