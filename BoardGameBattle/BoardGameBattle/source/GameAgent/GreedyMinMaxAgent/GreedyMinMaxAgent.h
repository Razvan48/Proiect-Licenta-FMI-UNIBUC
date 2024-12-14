#pragma once

#include "../GameAgent.h"
#include "../../BoardManager/BoardManager.h"

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

public:
	static GreedyMinMaxAgent& get();

	virtual float evaluateConfiguration(const ConfigurationMetadata& configurationMetadata) const override;
	virtual std::vector<std::pair<char, int>> findBestMove(const ConfigurationMetadata& configurationMetadata) const override;
};