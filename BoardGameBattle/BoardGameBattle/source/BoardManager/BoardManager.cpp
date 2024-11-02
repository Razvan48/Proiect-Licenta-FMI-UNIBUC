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

void BoardManager::update()
{

}



