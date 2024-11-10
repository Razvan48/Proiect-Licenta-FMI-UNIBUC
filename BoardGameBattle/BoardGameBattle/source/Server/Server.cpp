#include "Server.h"

#include "../RandomGenerator/RandomGenerator.h"
#include "../GlobalClock/GlobalClock.h"

#include <iostream>

#include <set>

Server::Server()
	: MAX_NUM_CLIENTS(2), NUM_CHANNELS(1), TIMEOUT_LIMIT_MS(1000)
	, server(nullptr), MINIMUM_PORT(10000), MAXIMUM_PORT(20000)
	, eNetEvent()
	, succesfullyCreated(false), lastTimeTriedCreation(0.0f), RETRY_CREATION_DELTA_TIME(1.0f)
	, TIME_BETWEEN_PINGS(1.0f), MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST(5.0f)
	, lastKnownBoardConfiguration("")
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
	this->stop();

	this->address.port = RandomGenerator::randomUniformInt(this->MINIMUM_PORT, this->MAXIMUM_PORT);
}

void Server::handleReceivedPacket()
{
	std::string clientKey = this->getClientKey(this->eNetEvent.peer->address);

	if (this->eNetEvent.packet->dataLength == 0)
	{
		std::cout << "Warning: Server received empty packet" << std::endl;
		return;
	}

	std::string receivedMessage((char*)this->eNetEvent.packet->data);
	std::cout << "Received Message: " << receivedMessage << " from " << clientKey << std::endl;

	if (this->connectedClients.size() == this->MAX_NUM_CLIENTS
		&& this->connectedClients.find(clientKey) == this->connectedClients.end())
	{
		std::cout << "Warning: Server reached maximum number of clients and could not allow a recently arrived client to connect" << std::endl;
		return;
	}

	if (this->connectedClients.find(clientKey) == this->connectedClients.end())
	{
		this->connectedClients.insert({
			clientKey, ClientData()
			});
		this->connectedClients.find(clientKey)->second.peer = this->eNetEvent.peer;
	}
	this->connectedClients.find(clientKey)->second.lastTimeReceivedPing = GlobalClock::get().getCurrentTime();

	if (receivedMessage == "white")
	{
		this->connectedClients.find(clientKey)->second.color = Color::WHITE;
	}
	else if (receivedMessage == "black")
	{
		this->connectedClients.find(clientKey)->second.color = Color::BLACK;
	}
	else if (receivedMessage.find("name:") == 0) // Are prefixul "name:"
	{
		this->connectedClients.find(clientKey)->second.clientName = receivedMessage.substr(std::string("name:").size()); // Pornim de la lungimea prefixului
	}
	else if (receivedMessage == "requestColor")
	{
		std::set<Server::Color> availableColors = { Server::Color::WHITE, Server::Color::BLACK };
		for (const auto& connectedClient : this->connectedClients)
		{
			availableColors.erase(connectedClient.second.color);
		}

		std::string sentMessage;
		if (availableColors.size() == 2) // Inca nu s-a setat una dintre culori
		{
			sentMessage = "";
		}
		else
		{
			if ((*availableColors.begin()) == Server::Color::WHITE)
			{
				sentMessage = "white";
			}
			else
			{
				sentMessage = "black";
			}
		}

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->eNetEvent.peer, 0, packet);
		this->connectedClients.find(clientKey)->second.lastTimeSentPing = GlobalClock::get().getCurrentTime();
	}
	else if (receivedMessage == "requestBoardConfiguration")
	{
		ENetPacket* packet = enet_packet_create(this->lastKnownBoardConfiguration.c_str(), this->lastKnownBoardConfiguration.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->eNetEvent.peer, 0, packet);
		this->connectedClients.find(clientKey)->second.lastTimeSentPing = GlobalClock::get().getCurrentTime();
	}
	else if (receivedMessage.find("boardConfiguration:") == 0) // Are prefixul "boardConfiguration:"
	{
		this->lastKnownBoardConfiguration = receivedMessage.substr(std::string("boardConfiguration:").size()); // Pornim de la lungimea prefixului

		for (auto& connectedClient : this->connectedClients)
		{
			if (connectedClient.first == clientKey)
				continue;

			ENetPacket* packet = enet_packet_create(receivedMessage.c_str(), receivedMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(connectedClient.second.peer, 0, packet);
			connectedClient.second.lastTimeSentPing = GlobalClock::get().getCurrentTime();
		}
	}
	else if (receivedMessage == "ping")
	{
		this->connectedClients.find(clientKey)->second.recentlyReceivedPing = true;
	}
	else
	{
		std::cout << "Warning: Server received unrecognized message" << std::endl;
	}

	enet_packet_destroy(this->eNetEvent.packet);
}

void Server::update()
{
	if (!this->succesfullyCreated)
	{
		if (GlobalClock::get().getCurrentTime() - this->lastTimeTriedCreation < this->RETRY_CREATION_DELTA_TIME)
			return;

		this->lastTimeTriedCreation = GlobalClock::get().getCurrentTime();
		this->server = enet_host_create(&this->address, this->MAX_NUM_CLIENTS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite pe latimea de banda

		if (this->server == nullptr)
		{
			std::cout << "Error: ENet server host creation failed" << std::endl;
		}
		else
		{
			this->succesfullyCreated = true;
			std::cout << "Server started on port " << this->address.port << std::endl;
		}

		return;
	}


	// Vedem ce pachete am primit.
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->server, &this->eNetEvent, this->TIMEOUT_LIMIT_MS);
	if (code > 0)
	{
		switch (this->eNetEvent.type)
		{
			/*
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Client connected" << std::endl;
				break;
			*/
			case ENET_EVENT_TYPE_RECEIVE:
				this->handleReceivedPacket();
				break;
			default:
				std::cout << "Warning: Server received unrecognized event type" << std::endl;
				break;
		}
	}
	else if (code < 0)
	{
		std::cout << "Error: Server service failed" << std::endl;
	}

	
	// Vedem daca am pierdut conexiunea cu cineva.
	for (auto& connectedClient : this->connectedClients)
	{
		if (GlobalClock::get().getCurrentTime() - connectedClient.second.lastTimeReceivedPing > this->MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST)
		{
			connectedClient.second.recentlyReceivedPing = false;
		}
	}

	// Apoi trimitem ping-urile catre clienti.
	for (auto& connectedClient : this->connectedClients)
	{
		if (GlobalClock::get().getCurrentTime() - connectedClient.second.lastTimeSentPing < this->TIME_BETWEEN_PINGS)
			continue;

		std::string sentMessage = "ping";

		for (auto& otherConnectedClient : this->connectedClients)
		{
			if (connectedClient.first == otherConnectedClient.first)
				continue;

			sentMessage.push_back('$');
			sentMessage += otherConnectedClient.second.clientName;

			if (GlobalClock::get().getCurrentTime() - otherConnectedClient.second.lastTimeReceivedPing > this->MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST)
				sentMessage.push_back('0');
			else
				sentMessage.push_back('1');
		}
		sentMessage.push_back('$');

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(connectedClient.second.peer, 0, packet);
		connectedClient.second.lastTimeSentPing = GlobalClock::get().getCurrentTime();
	}

	// Pentru debug
	std::cout << "Number of connected clients: " << this->connectedClients.size() << std::endl;
}

void Server::stop()
{
	if (this->server != nullptr)
		enet_host_destroy(this->server);
	this->server = nullptr;



	this->connectedClients.clear();

	this->address.host = ENET_HOST_ANY;
	this->address.port = 0;

	this->succesfullyCreated = false;
	this->lastTimeTriedCreation = 0.0f;

	this->lastKnownBoardConfiguration = "";
}

