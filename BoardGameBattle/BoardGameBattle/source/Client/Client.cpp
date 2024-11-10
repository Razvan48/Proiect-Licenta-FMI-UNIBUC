#include "Client.h"

#include "../GlobalClock/GlobalClock.h"

#include <iostream>

Client::Client()
	: MAX_NUM_SERVERS(1), NUM_CHANNELS(1), TIMEOUT_LIMIT_MS(1000)
	, client(NULL), serverAddress(), serverPeer(nullptr), eNetEvent()
	, succesfullyConnected(false)
	, lastTimeTriedConnection(0.0f)
	, RETRY_CONNECTION_DELTA_TIME(1.0f)
	, TIME_BETWEEN_PINGS(1.0f)
	, MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST(5.0f)
	, lastTimeReceivedPing(0.0f)
	, lastTimeSentPing(0.0f)
	, opponentName("")
	, serverConnectionOk(false)
	, opponentConnectionOk(false)
	, lastKnownBoardConfiguration("")
	, knowsItsOwnColor(false)
	, hasSentItsOwnColor(false)
	, hasSentItsClientName(false)
	, hasRequestedInitialBoardConfiguration(false)
	, clientName("")
	, color("")
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

void Client::start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName, const std::string& color)
{
	this->stop();

	this->clientName = clientName;
	this->color = color;

	if (!color.empty())
	{
		this->knowsItsOwnColor = true;
	}

	this->client = enet_host_create(NULL, this->MAX_NUM_SERVERS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite la latimea de banda

	if (client == NULL)
	{
		std::cout << "Error: ENet failed to create client" << std::endl;
	}

	enet_address_set_host(&this->serverAddress, serverIP.c_str());
	this->serverAddress.port = serverPort;

	std::cout << "Client initialized with: " << this->clientName << ' ' << this->color << std::endl;
}

void Client::sentMessage(const std::string& message)
{
	if (!this->succesfullyConnected)
	{
		std::cout << "Warning: Client cannot send message because it is not connected to server" << std::endl;
		return;
	}

	ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(this->serverPeer, 0, packet);
	this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
}

void Client::handleReceivedPacket()
{
	this->lastTimeReceivedPing = GlobalClock::get().getCurrentTime();

	if (this->eNetEvent.packet->dataLength == 0)
	{
		std::cout << "Warning: Client received empty packet" << std::endl;
		return;
	}

	std::string receivedMessage((char*)this->eNetEvent.packet->data);
	std::cout << "Received Message: " << receivedMessage << " from server" << std::endl;

	if (receivedMessage == "white" || receivedMessage == "black")
	{
		this->color = receivedMessage;
		this->hasSentItsOwnColor = true;
	}
	else if (receivedMessage.find("ping") == 0) // "ping" este prefix pentru mesaj. Aici aflam si daca am pierdut conexiunea doar cu celalalt oponent.
	{
		int pos = std::string("ping").size();
		std::string currentSubstring = "";
		for (int i = pos; i < receivedMessage.size(); ++i)
		{
			if (receivedMessage[i] == '$')
			{
				if (!currentSubstring.empty())
				{
					if (currentSubstring.back() == '1')
						this->opponentConnectionOk = true;
					else
						this->opponentConnectionOk = false;

					this->opponentName = currentSubstring.substr(0, (int)currentSubstring.size() - 1);
				}

				currentSubstring = "";
				continue;
			}

			currentSubstring.push_back(receivedMessage[i]);
		}
	}
	else if (receivedMessage.find("boardConfiguration:") == 0) // "boardConfiguration" este prefix pentru mesaj.
	{
		this->lastKnownBoardConfiguration = receivedMessage.substr(std::string("boardConfiguration:").size()); // Pornim de la lungimea prefixului.
	}
	else
	{
		std::cout << "Warning: Client received unrecognized message" << std::endl;
	}

	enet_packet_destroy(this->eNetEvent.packet);
}

void Client::update()
{
	if (!this->succesfullyConnected)
	{
		if (GlobalClock::get().getCurrentTime() - this->lastTimeTriedConnection < this->RETRY_CONNECTION_DELTA_TIME)
			return;

		this->serverPeer = enet_host_connect(this->client, &this->serverAddress, this->NUM_CHANNELS, 0); // 0 = nu trimitem nimic
		if (this->serverPeer == NULL)
		{
			std::cout << "Error: No available peers for initiating an ENet connection (no server available)" << std::endl;
		}
		else
		{
			this->succesfullyConnected = true;
			std::cout << "Client connected to server" << std::endl;
		}

		return;
	}

	// Rezolvam problemele de baza (clientName, color si initialBoardConfiguration)
	if (!this->hasSentItsClientName)
	{
		std::string sentMessage = "name:" + this->clientName;

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->serverPeer, 0, packet);
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();

		this->hasSentItsClientName = true;
	}

	if (this->knowsItsOwnColor && !this->hasSentItsOwnColor)
	{
		std::string sentMessage = this->color;

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->serverPeer, 0, packet);
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();

		this->hasSentItsOwnColor = true;
	}
	else if (!this->knowsItsOwnColor)
	{
		std::string sentMessage = "requestColor";

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->serverPeer, 0, packet);
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
	}

	if (!this->hasRequestedInitialBoardConfiguration)
	{
		std::string sentMessage = "requestBoardConfiguration";

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->serverPeer, 0, packet);
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();

		this->hasRequestedInitialBoardConfiguration = true;
	}

	// Vedem ce pachete am primit.
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->client, &this->eNetEvent, this->TIMEOUT_LIMIT_MS);
	if (code > 0)
	{
		switch (this->eNetEvent.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			this->handleReceivedPacket();
			break;
		default:
			std::cout << "Warning: Client received unrecognized event type" << std::endl;
			break;
		}
	}
	else if (code < 0)
	{
		std::cout << "Error: Client service failed" << std::endl;
	}


	// Vedem daca am pierdut conexiunea cu serverul.
	if (GlobalClock::get().getCurrentTime() - this->lastTimeReceivedPing > this->MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST)
	{
		this->serverConnectionOk = false;
		this->opponentConnectionOk = false;
	}

	// Apoi trimitem ping-ul catre server.
	if (GlobalClock::get().getCurrentTime() - this->lastTimeSentPing > this->TIME_BETWEEN_PINGS)
	{
		std::string sentMessage = "ping";

		ENetPacket* packet = enet_packet_create(sentMessage.c_str(), sentMessage.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(this->serverPeer, 0, packet);
		this->lastTimeSentPing = GlobalClock::get().getCurrentTime();
	}
}

void Client::stop()
{
	if (this->client != nullptr)
		enet_host_flush(this->client);
	if (this->serverPeer != nullptr)
		enet_peer_disconnect(this->serverPeer, 0);
	if (this->client != nullptr)
		enet_host_destroy(this->client);

	this->client = nullptr;
	this->serverPeer = nullptr;


	this->serverAddress.host = 0;
	this->serverAddress.port = 0;


	this->succesfullyConnected = false;
	this->lastTimeTriedConnection = 0.0f;

	this->lastTimeReceivedPing = 0.0f;
	this->lastTimeSentPing = 0.0f;


	this->opponentName = "";
	this->serverConnectionOk = false;
	this->opponentConnectionOk = false;
	this->lastKnownBoardConfiguration = "";

	this->knowsItsOwnColor = false;
	this->hasSentItsOwnColor = false;
	this->hasSentItsClientName = false;
	this->hasRequestedInitialBoardConfiguration = false;

	this->clientName = "";
	this->color = "";
}

