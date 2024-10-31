#include "Server.h"

Server::Server()
{

}

Server::~Server()
{

}

Server& Server::get()
{
	static Server instance;
	return instance;
}

