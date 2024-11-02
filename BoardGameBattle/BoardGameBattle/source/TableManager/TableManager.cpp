#include "TableManager.h"

TableManager::TableManager()
{

}

TableManager::~TableManager()
{

}

TableManager& TableManager::get()
{
	static TableManager instance;
	return instance;
}

void TableManager::update()
{

}



