#pragma once

#include "../BoardManager/BoardManager.h"

#include <vector>

#include <mutex>
#include <atomic>

class GameAgent
{
private:

protected:
	GameAgent();
	virtual ~GameAgent() = 0;

	bool isRunningTask;
	std::mutex isRunningTaskMutex;

	std::vector<std::pair<char, int>> bestMove;
	std::mutex bestMoveMutex;

public:
	std::atomic<bool> isTaskCancelled;

public:
	virtual float evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const = 0;
	virtual void findBestMove(ConfigurationMetadata& configurationMetadata) = 0;

	inline bool getIsRunningTask()
	{
		this->isRunningTaskMutex.lock();
		bool isRunningTask = this->isRunningTask;
		this->isRunningTaskMutex.unlock();

		return isRunningTask;
	}

	inline void setIsRunningTask(bool isRunningTask)
	{
		this->isRunningTaskMutex.lock();
		this->isRunningTask = isRunningTask;
		this->isRunningTaskMutex.unlock();
	}

	inline std::vector<std::pair<char, int>> getBestMove()
	{
		this->bestMoveMutex.lock();
		std::vector<std::pair<char, int>> bestMove = this->bestMove;
		this->bestMoveMutex.unlock();

		return bestMove;
	}

	inline void setBestMove(const std::vector<std::pair<char, int>>& bestMove)
	{
		this->bestMoveMutex.lock();
		this->bestMove = bestMove;
		this->bestMoveMutex.unlock();
	}
};