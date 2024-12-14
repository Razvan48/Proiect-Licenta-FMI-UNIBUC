#pragma once

#include "../GameAgent.h"
#include "../../BoardManager/BoardManager.h"

#include "../../GameMetadata/GameMetadata.h"

class GreedyMinMaxAgent : virtual public GameAgent
{
private:
	GreedyMinMaxAgent();
	virtual ~GreedyMinMaxAgent();
	GreedyMinMaxAgent(const GreedyMinMaxAgent& other) = delete;
	GreedyMinMaxAgent& operator= (const GreedyMinMaxAgent& other) = delete;
	GreedyMinMaxAgent(const GreedyMinMaxAgent&& other) = delete;
	GreedyMinMaxAgent& operator= (const GreedyMinMaxAgent&& other) = delete;

protected:
	static const int MAX_DEPTH = 3;

	static const float INF;

	static const float PAWN_SCORE;
	static const float ROOK_SCORE;
	static const float KNIGHT_SCORE;
	static const float BISHOP_SCORE;
	static const float QUEEN_SCORE;
	static const float KING_SCORE;

	static const float ATTACK_ZONE_SCORE;
	static const float CHECK_SCORE;

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

	std::vector<std::pair<char, int>> minMax(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta) const; // INFO: minMax primeste o copie a configuratiei.

public:
	static GreedyMinMaxAgent& get();

	virtual float evaluateConfiguration(const ConfigurationMetadata& configurationMetadata) const override;
	virtual std::vector<std::pair<char, int>> findBestMove(const ConfigurationMetadata& configurationMetadata) const override;
};

const float GreedyMinMaxAgent::INF = 65536.0f;

const float GreedyMinMaxAgent::PAWN_SCORE = 1.0f;
const float GreedyMinMaxAgent::ROOK_SCORE = 5.0f;
const float GreedyMinMaxAgent::KNIGHT_SCORE = 3.0f;
const float GreedyMinMaxAgent::BISHOP_SCORE = 3.0f;
const float GreedyMinMaxAgent::QUEEN_SCORE = 9.0f;
const float GreedyMinMaxAgent::KING_SCORE = 100.0f;

const float GreedyMinMaxAgent::ATTACK_ZONE_SCORE = 0.1f;
const float GreedyMinMaxAgent::CHECK_SCORE = 0.2f;





