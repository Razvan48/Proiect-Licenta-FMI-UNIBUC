#include "GameAgent.h"

GameAgent::GameAgent()
	: isFindingBestMove(false)
	, bestMove(std::vector<std::pair<char, int>>())
	, findBestMoveThread(nullptr)
	, isFindBestMoveCancelled(true)

	, isEstimating(false)
	, estimation(GameAgent::UNREACHABLE_ESTIMATION)
	, estimateThread(nullptr)
	, isEstimateCancelled(true)
{

}

GameAgent::~GameAgent()
{
	this->setIsFindingBestMove(false);
	this->setBestMove(std::vector<std::pair<char, int>>());
	this->setIsFindBestMoveCancelled(true);

	this->setIsEstimating(false);
	this->resetEstimation();
	this->setIsEstimateCancelled(true);
}

const float GameAgent::UNREACHABLE_ESTIMATION = 1048576.0f; // 2 ^ 20