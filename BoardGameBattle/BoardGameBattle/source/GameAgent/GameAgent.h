#pragma once

#include "../BoardManager/BoardManager.h"

#include <vector>

class GameAgent
{
private:

protected:
	GameAgent();
	virtual ~GameAgent() = 0;

public:
	virtual float evaluateConfiguration(const ConfigurationMetadata& configurationMetadata) const = 0;
	virtual std::vector<std::pair<char, int>> findBestMove(const ConfigurationMetadata& configurationMetadata) const = 0;

};