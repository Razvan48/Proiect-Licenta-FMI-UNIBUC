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
	// INFO: Presupunem ca are deja attack zone-urile calculate corect pentru ambele culori.

	float evaluation = 0.0f;

	unsigned long long whitePawns = configurationMetadata.whitePawns;
	while (whitePawns)
	{
		unsigned long long lsb = (whitePawns & ((~whitePawns) + 1));

		evaluation += GreedyMinMaxAgent::PAWN_SCORE;

		whitePawns ^= lsb;
	}
}

std::vector<std::pair<char, int>> GreedyMinMaxAgent::minMax(ConfigurationMetadata configurationMetadata, int depth) const // INFO: minMax primeste o copie a configuratiei.
{
	// TODO:
	return std::vector<std::pair<char, int>>();
}

std::vector<std::pair<char, int>> GreedyMinMaxAgent::findBestMove(const ConfigurationMetadata& configurationMetadata) const
{
	// TODO:
	return std::vector<std::pair<char, int>>();
}