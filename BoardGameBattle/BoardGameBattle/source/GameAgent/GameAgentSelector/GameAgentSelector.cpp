#include "GameAgentSelector.h"

#include "../GreedyMinMaxAgent/GreedyMinMaxAgent.h"
#include "../GreedyExpectedMinMaxAgent/GreedyExpectedMinMaxAgent.h"

GameAgent& GameAgentSelector::get()
{
	return GreedyExpectedMinMaxAgent::get();
}


