#include "Client.h"

Client::Client()
{

}

Client::~Client()
{

}

Client& Client::get()
{
	static Client instance;
	return instance;
}

