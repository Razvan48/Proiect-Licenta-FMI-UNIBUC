#include "GreedyMinMaxAgent.h"

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

float GreedyMinMaxAgent::evaluateConfiguration(const ConfigurationMetadata& configurationMetadata) const
{
	return 0.0f;
}

std::vector<std::pair<char, int>> GreedyMinMaxAgent::findBestMove(const ConfigurationMetadata& configurationMetadata) const
{
	return std::vector<std::pair<char, int>>();
}