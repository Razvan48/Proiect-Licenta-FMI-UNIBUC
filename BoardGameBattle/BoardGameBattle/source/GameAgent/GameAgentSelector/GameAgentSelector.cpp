#include "GameAgentSelector.h"

#include "../GreedyExpectedMinMaxAgent/GreedyExpectedMinMaxAgent.h"
#include "../CachedGreedyExpectedMinMaxAgent/CachedGreedyExpectedMinMaxAgent.h"

GameAgent& GameAgentSelector::get()
{
	return CachedGreedyExpectedMinMaxAgent::get();
}


