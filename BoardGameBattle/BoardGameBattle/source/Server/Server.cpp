#include "Server.h"

#include "../RandomGenerator/RandomGenerator.h"
#include "../GlobalClock/GlobalClock.h"

#include <iostream>

#include <set>

Server::Server()
	: MAX_NUM_CLIENTS(2), NUM_CHANNELS(2), TIME_WAITING_FOR_EVENTS_MS(10)
	, server(nullptr), address(), MINIMUM_PORT(10000), MAXIMUM_PORT(20000)
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

void Server::ClientData::sendMessage(const std::string& messageToSend, bool& failedToSendMessage)
{
	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->peer, 0, packet) == 0)
	{
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
		failedToSendMessage = false;
	}
	else
	{
		failedToSendMessage = true;
		std::cout << "Error: Client failed to send message" << std::endl;
	}
}

void Server::ClientData::sendMessageUnsafe(const std::string& messageToSend)
{
	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->peer, 0, packet) == 0)
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
	else
		std::cout << "Error: Client failed to send message" << std::endl;
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
	std::cout << "Received Message: " << receivedMessage << " from " << clientKey << ", size = " << receivedMessage.size() << std::endl;

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



	if (receivedMessage == "color:white")
	{
		this->connectedClients.find(clientKey)->second.color = Color::WHITE;
	}
	else if (receivedMessage == "color:black")
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

		std::string messageToSend;
		if (availableColors.size() == 2) // Inca nu s-a setat una dintre culori
		{
			messageToSend = "color:";
		}
		else
		{
			if ((*availableColors.begin()) == Server::Color::WHITE)
			{
				this->connectedClients.find(clientKey)->second.color = Color::WHITE;
				messageToSend = "color:white";
			}
			else
			{
				this->connectedClients.find(clientKey)->second.color = Color::BLACK;
				messageToSend = "color:black";
			}
		}

		this->connectedClients.find(clientKey)->second.hasToSendColor = true;
		this->connectedClients.find(clientKey)->second.sendMessage(messageToSend, this->connectedClients.find(clientKey)->second.hasToSendColor);
	}
	else if (receivedMessage == "requestBoardConfiguration")
	{
		std::string messageToSend = "boardConfiguration:" + this->lastKnownBoardConfiguration; // Va fi "" daca inca nu e setata.

		this->connectedClients.find(clientKey)->second.hasToSendBoardConfiguration = true;
		this->connectedClients.find(clientKey)->second.sendMessage(messageToSend, this->connectedClients.find(clientKey)->second.hasToSendBoardConfiguration);
	}
	else if (receivedMessage.find("boardConfiguration:") == 0) // Are prefixul "boardConfiguration:"
	{
		this->lastKnownBoardConfiguration = receivedMessage.substr(std::string("boardConfiguration:").size()); // Pornim de la lungimea prefixului

		for (auto& connectedClient : this->connectedClients)
		{
			if (connectedClient.first == clientKey)
				continue;

			std::string messageToSend = "boardConfiguration:" + this->lastKnownBoardConfiguration;

			connectedClient.second.hasToSendBoardConfiguration = true;
			connectedClient.second.sendMessage(messageToSend, connectedClient.second.hasToSendBoardConfiguration);
		}
	}
	else if (receivedMessage == "ping")
	{
		this->connectedClients.find(clientKey)->second.lastTimeReceivedPing = GlobalClock::get().getCurrentTime();
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
	int code = enet_host_service(this->server, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
	if (code > 0)
	{
		switch (this->eNetEvent.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Client connected" << std::endl;
				break;
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
			connectedClient.second.workingConnection = false;
		}
	}

	// Apoi trimitem ping-urile catre clienti.
	for (auto& connectedClient : this->connectedClients)
	{
		if (GlobalClock::get().getCurrentTime() - connectedClient.second.lastTimeSentPing < this->TIME_BETWEEN_PINGS)
			continue;

		std::string messageToSend = "ping";

		for (auto& otherConnectedClient : this->connectedClients)
		{
			if (connectedClient.first == otherConnectedClient.first)
				continue;

			messageToSend.push_back('$');
			messageToSend += otherConnectedClient.second.clientName;

			if (GlobalClock::get().getCurrentTime() - otherConnectedClient.second.lastTimeReceivedPing > this->MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST)
				messageToSend.push_back('0');
			else
				messageToSend.push_back('1');
		}
		messageToSend.push_back('$');

		connectedClient.second.sendMessageUnsafe(messageToSend);
	}

	// Mai vedem daca ne ramasese ceva de trimis la vreun client ce a esuat.
	for (auto& connectedClient : this->connectedClients)
	{
		if (connectedClient.second.hasToSendColor)
		{
			std::string messageToSend;
			if (connectedClient.second.color == Color::WHITE)
			{
				messageToSend = "color:white";
			}
			else if (connectedClient.second.color == Color::BLACK)
			{
				messageToSend = "color:black";
			}
			else
			{
				messageToSend = "color:";
			}

			connectedClient.second.sendMessage(messageToSend, connectedClient.second.hasToSendColor);
		}

		if (connectedClient.second.hasToSendBoardConfiguration)
		{
			std::string messageToSend = "boardConfiguration:" + this->lastKnownBoardConfiguration;

			connectedClient.second.sendMessage(messageToSend, connectedClient.second.hasToSendBoardConfiguration);
		}
	}


	// Eliminam din structura de date clientii pierduti.
	/*
	for (auto connectedClient = this->connectedClients.begin(); connectedClient != this->connectedClients.end(); )
	{
		if (!connectedClient->second.workingConnection && connectedClient->second.lastTimeReceivedPing != 0.0f)
		{
			enet_peer_disconnect(connectedClient->second.peer, 0);
			connectedClient = this->connectedClients.erase(connectedClient);
		}
		else
			++connectedClient;
	}
	*/
}

void Server::stop()
{
	for (auto& connectedClient : this->connectedClients)
	{
		if (connectedClient.second.peer != nullptr)
			enet_peer_disconnect(connectedClient.second.peer, 0);
	}

	if (this->server != nullptr)
		enet_host_destroy(this->server);
	this->server = nullptr;




	this->address.host = ENET_HOST_ANY;
	this->address.port = 0;

	this->eNetEvent = ENetEvent();

	this->succesfullyCreated = false;
	this->lastTimeTriedCreation = 0.0f;

	this->lastKnownBoardConfiguration = "";

	this->connectedClients.clear();
}

