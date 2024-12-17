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
	static const int MAX_DEPTH;

	static const float INF;

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

	float minMax(ConfigurationMetadata configurationMetadata, int depth, float alpha, float beta) const; // INFO: minMax primeste o copie a configuratiei.

public:
	static GreedyMinMaxAgent& get();

	virtual float evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const override;
	virtual void findBestMove(ConfigurationMetadata& configurationMetadata) override;
};




