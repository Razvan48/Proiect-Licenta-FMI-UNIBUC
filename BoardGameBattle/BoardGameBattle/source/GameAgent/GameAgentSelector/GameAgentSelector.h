#pragma once

#include "../GameAgent.h"

// INFO: Cand schimbam agentul, trebuie sa schimbam doar in cpp-ul de la GameAgentSelector.

namespace GameAgentSelector
{
	GameAgent& get();
};