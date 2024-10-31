#include "Client.h"

#include <iostream>

Client::Client()
	: MAX_NUM_SERVERS(1), NUM_CHANNELS(1), TIMEOUT_LIMIT_MS(1000)
	, client(NULL), serverAddress(), serverPeer(nullptr), eNetEvent()
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

void Client::start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName)
{
	this->client = enet_host_create(NULL, this->MAX_NUM_SERVERS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite la latimea de banda

	if (client == NULL)
	{
		std::cout << "Error: ENet failed to create client" << std::endl;
	}

	enet_address_set_host(&this->serverAddress, serverIP.c_str());
	this->serverAddress.port = serverPort;
	this->clientName = clientName;

	this->serverPeer = enet_host_connect(this->client, &this->serverAddress, this->NUM_CHANNELS, 0); // 0 = nu trimitem nimic
	if (this->serverPeer == NULL)
	{
		std::cout << "Error: No available peers for initiating an ENet connection (no server available)" << std::endl;
		return;
	}
}

void Client::update()
{
	// TODO:
}

void Client::stop()
{
	enet_host_destroy(this->client);
}

