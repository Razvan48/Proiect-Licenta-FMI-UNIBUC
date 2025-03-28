#include "GameAgentSelector.h"

#include "../GreedyExpectedMinMaxAgent/GreedyExpectedMinMaxAgent.h"

GameAgent& GameAgentSelector::get()
{
	return GreedyExpectedMinMaxAgent::get();
}


