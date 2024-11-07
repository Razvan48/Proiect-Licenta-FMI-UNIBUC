#include "BoardManager.h"

BoardManager::BoardManager()
{

}

BoardManager::~BoardManager()
{

}

BoardManager& BoardManager::get()
{
	static BoardManager instance;
	return instance;
}

std::string BoardManager::getPiecesConfigurationForVisualizing() const
{
	// TODO: de implementat, momentan e asa pentru testing
	return R"(
			rnbqkbnr
			pppppppp
			........
			........
			........
			........
			PPPPPPPP
			RNBQKBNR
			)";
}



