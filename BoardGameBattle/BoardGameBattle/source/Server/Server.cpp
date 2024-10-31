#include "Server.h"

#include "../RandomGenerator/RandomGenerator.h"

#include <iostream>

Server::Server()
	: MAX_NUM_CLIENTS(2), NUM_CHANNELS(1), TIMEOUT_LIMIT_MS(1000)
	, host(nullptr), MINIMUM_PORT(10000), MAXIMUM_PORT(20000)
	, eNetEvent()
{
	this->address.host = ENET_HOST_ANY;
	this->address.port = 0;
}

Server::~Server()
{

}

Server& Server::get()
{
	static Server instance;
	return instance;
}

void Server::start()
{
	this->address.port = RandomGenerator::randomUniformInt(this->MINIMUM_PORT, this->MAXIMUM_PORT);

	this->host = enet_host_create(&this->address, this->MAX_NUM_CLIENTS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite pe latimea de banda

	if (this->host == nullptr)
	{
		std::cout << "Error: ENet server host creation failed" << std::endl;
		return;
	}
}

void Server::update()
{
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->host, &this->eNetEvent, this->TIMEOUT_LIMIT_MS);
	if (code > 0)
	{
		switch (this->eNetEvent.type)
		{
			// TODO:
			default:
				std::cout << "Warning: Server received unrecognized event type" << std::endl;
				break;
		}
	}
	else if (code < 0)
	{
		std::cout << "Error: Server service failed" << std::endl;
	}
}

void Server::stop()
{
	enet_host_destroy(this->host);
}

