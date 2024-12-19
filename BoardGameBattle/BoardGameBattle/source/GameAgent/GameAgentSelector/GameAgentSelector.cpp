#include "GameAgentSelector.h"

#include "../GreedyMinMaxAgent/GreedyMinMaxAgent.h"

GameAgent& GameAgentSelector::get()
{
	return GreedyMinMaxAgent::get();
}


