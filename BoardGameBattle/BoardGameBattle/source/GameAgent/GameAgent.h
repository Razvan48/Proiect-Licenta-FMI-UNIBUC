#pragma once

#include "../BoardManager/BoardManager.h"

#include <vector>

#include <mutex>
#include <atomic>

#include <thread>
#include <memory>

class GameAgent
{
private:

protected:
	GameAgent();
	virtual ~GameAgent() = 0;

	bool isFindingBestMove;
	std::mutex isFindingBestMoveMutex;
	std::vector<std::pair<char, int>> bestMove;
	std::mutex bestMoveMutex;
	std::shared_ptr<std::thread> findBestMoveThread;
	std::atomic<bool> isFindBestMoveCancelled;

	bool isEstimating;
	std::mutex isEstimatingMutex;
	float estimation;
	std::mutex estimationMutex;
	std::shared_ptr<std::thread> estimateThread;
	std::atomic<bool> isEstimateCancelled;

	static const float UNREACHABLE_ESTIMATION;

	inline void setEstimation(float estimation)
	{
		this->estimationMutex.lock();
		this->estimation = estimation;
		this->estimationMutex.unlock();
	}

public:
	virtual float evaluateConfiguration(ConfigurationMetadata& configurationMetadata) const = 0;
	virtual void findBestMove(ConfigurationMetadata& configurationMetadata) = 0;
	virtual void reset() = 0;
	virtual void estimateConfiguration(ConfigurationMetadata& configurationMetadata) = 0;
	virtual float getMaxDisplayEstimation() const = 0;

	inline bool getIsFindingBestMove()
	{
		this->isFindingBestMoveMutex.lock();
		bool isFindingBestMove = this->isFindingBestMove;
		this->isFindingBestMoveMutex.unlock();

		return isFindingBestMove;
	}

	inline void setIsFindingBestMove(bool isFindingBestMove)
	{
		this->isFindingBestMoveMutex.lock();
		this->isFindingBestMove = isFindingBestMove;
		this->isFindingBestMoveMutex.unlock();
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

	inline void setIsFindBestMoveCancelled(bool isFindBestMoveCancelled)
	{
		this->isFindBestMoveCancelled.store(isFindBestMoveCancelled);
		if (isFindBestMoveCancelled)
		{
			if (this->findBestMoveThread != nullptr)
			{
				this->findBestMoveThread->join();
				this->findBestMoveThread = nullptr;
			}
		}
	}

	inline bool getIsEstimating()
	{
		this->isEstimatingMutex.lock();
		bool isEstimating = this->isEstimating;
		this->isEstimatingMutex.unlock();

		return isEstimating;
	}

	inline void setIsEstimating(bool isEstimating)
	{
		this->isEstimatingMutex.lock();
		this->isEstimating = isEstimating;
		this->isEstimatingMutex.unlock();
	}

	inline float getEstimation()
	{
		this->estimationMutex.lock();
		float estimation = this->estimation;
		this->estimationMutex.unlock();

		return estimation;
	}

	inline void resetEstimation()
	{
		this->estimationMutex.lock();
		this->estimation = GameAgent::UNREACHABLE_ESTIMATION;
		this->estimationMutex.unlock();
	}

	inline void setIsEstimateCancelled(bool isEstimateCancelled)
	{
		this->isEstimateCancelled.store(isEstimateCancelled);
		if (isEstimateCancelled)
		{
			if (this->estimateThread != nullptr)
			{
				this->estimateThread->join();
				this->estimateThread = nullptr;
			}
		}
	}

	inline float getUnreachableEstimation() const { return GameAgent::UNREACHABLE_ESTIMATION; }
};