#include "Client.h"

#include "../GlobalClock/GlobalClock.h"

#include <iostream>

Client::Client()
	: MAX_NUM_SERVERS(1), NUM_CHANNELS(1), TIME_WAITING_FOR_EVENTS_MS(10)
	, serverPeer(nullptr), client(NULL), serverAddress(), eNetEvent()
	, succesfullyConnected(false)
	, lastTimeTriedConnection(0.0f)
	, RETRY_CONNECTION_DELTA_TIME(1.0f)
	, TIME_BETWEEN_PINGS(1.0f)
	, MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST(5.0f)
	, lastTimeReceivedPing(0.0f)
	, lastTimeSentPing(0.0f)
	, clientName("")
	, color("")
	, opponentName("")
	, lastKnownBoardConfiguration("")
	, workingServerConnection(false)
	, workingOpponentConnection(false)
	, hasToSendName(false)
	, hasToSendColor(false)
	, hasToReceiveColor(false)
	, hasToSendBoardConfiguration(false)
	, hasToReceiveBoardConfiguration(false)
	, lastTimeRequestedColor(0.0f)
	, TIME_BETWEEN_COLOR_REQUESTS(1.0f)
	, lastTimeRequestedBoardConfiguration(0.0f)
	, TIME_BETWEEN_BOARD_CONFIGURATION_REQUESTS(1.0f)
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

void Client::start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName, const std::string& color, const std::string& boardConfiguration)
{
	this->stop();

	this->clientName = clientName;
	this->color = color;
	this->lastKnownBoardConfiguration = boardConfiguration;

	// Client Name
	this->hasToSendName = true;

	// Color
	if (!this->color.empty())
		this->hasToSendColor = true;
	else
		this->hasToReceiveColor = true;

	// BoardConfiguration
	if (!this->lastKnownBoardConfiguration.empty())
		this->hasToSendBoardConfiguration = true;
	else
		this->hasToReceiveBoardConfiguration = true;



	this->client = enet_host_create(NULL, this->MAX_NUM_SERVERS, this->NUM_CHANNELS, 0, 0); // 0, 0 inseamna fara limite la latimea de banda
	if (client == NULL)
	{
		std::cout << "Error: ENet failed to create client" << std::endl;
	}

	enet_address_set_host(&this->serverAddress, serverIP.c_str());
	this->serverAddress.port = serverPort;

	std::cout << "Client initialized with: " << serverIP << ' ' << this->serverAddress.port << ' ' << this->clientName << ' ' << this->color << std::endl;
}

void Client::sendMessage(const std::string& messageToSend, bool& failedToSendMessage, float& timeWhenMessageSent)
{
	if (!this->succesfullyConnected)
	{
		std::cout << "Error: Client cannot send message because it is not connected to server" << std::endl;
		return;
	}

	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->serverPeer, 0, packet) == 0)
	{
		timeWhenMessageSent = GlobalClock::get().getCurrentTime();
		failedToSendMessage = false;

		std::cout << "Client sent message: " << messageToSend << std::endl;
	}
	else
	{
		failedToSendMessage = true;
		std::cout << "Error: Client failed to send message" << std::endl;
	}
}

void Client::sendMessageUnsafe(const std::string& messageToSend, float& timeWhenMessageSent)
{
	if (!this->succesfullyConnected)
	{
		std::cout << "Error: Client cannot send message because it is not connected to server" << std::endl;
		return;
	}

	ENetPacket* packet = enet_packet_create(messageToSend.c_str(), messageToSend.size() + 1, ENET_PACKET_FLAG_RELIABLE);

	// 0 daca a avut succes
	if (enet_peer_send(this->serverPeer, 0, packet) == 0)
	{
		timeWhenMessageSent = GlobalClock::get().getCurrentTime();

		std::cout << "Client sent message: " << messageToSend << std::endl;
	}
	else
		std::cout << "Error: Client failed to send message" << std::endl;
}

void Client::handleReceivedPacket()
{
	std::cout << "In handleReceivedPacket in Client named " << this->clientName << " with color " << this->color << std::endl;

	this->lastTimeReceivedPing = GlobalClock::get().getCurrentTime(); // Ar putea fi mutat mai jos de if-ul ce urmeaza.

	if (this->eNetEvent.packet->dataLength == 0)
	{
		std::cout << "Warning: Client received empty packet" << std::endl;
		return;
	}

	std::string receivedMessage((char*)this->eNetEvent.packet->data);
	std::cout << "Received Message: " << receivedMessage << " from server, size=" << receivedMessage.size() << std::endl;

	if (receivedMessage.find("color:") == 0)
	{
		if (receivedMessage == "color:")
			this->hasToReceiveColor = true;
		else
		{
			this->color = receivedMessage.substr(std::string("color:").size());

			// this->hasToReceiveColor = false; // nu ar trebui sa fie nevoie, e deja pus pe false din metoda update()
		}
	}
	else if (receivedMessage.find("boardConfiguration:") == 0)
	{
		if (receivedMessage == "boardConfiguration:")
			this->hasToReceiveBoardConfiguration = true;
		else
		{
			this->lastKnownBoardConfiguration = receivedMessage.substr(std::string("boardConfiguration:").size());

			// this->hasToReceiveBoardConfiguration = false; // nu ar trebui sa fie nevoie, e deja pus pe false din metoda update()
		}
	}
	else if (receivedMessage.find("ping") == 0) // "ping" este prefix pentru mesaj. Aici aflam si daca am pierdut conexiunea doar cu celalalt oponent.
	{
		int pos = std::string("ping").size();
		std::string currentSubstring = "";


		this->workingOpponentConnection = false;


		for (int i = pos; i < receivedMessage.size(); ++i)
		{
			if (receivedMessage[i] == '$')
			{
				if (!currentSubstring.empty())
				{
					if (currentSubstring.back() == '1')
						this->workingOpponentConnection = true;
					else
						this->workingOpponentConnection = false;

					this->opponentName = currentSubstring.substr(0, (int)currentSubstring.size() - 1);
				}

				currentSubstring = "";
				continue;
			}

			currentSubstring.push_back(receivedMessage[i]);
		}
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

	// Trimitem ce informatii vitale stim deja catre server.
	if (this->hasToSendName)
	{
		this->sendMessage("name:" + this->clientName, this->hasToSendName, this->lastTimeSentPing);
	}
	if (this->hasToSendColor)
	{
		this->sendMessage("color:" + this->color, this->hasToSendColor, this->lastTimeSentPing);
	}
	if (this->hasToSendBoardConfiguration)
	{
		this->sendMessage("boardConfiguration:" + this->lastKnownBoardConfiguration, this->hasToSendBoardConfiguration, this->lastTimeSentPing);
	}

	// Cerem ce informatii vitale avem nevoie pentru a incepe interactiunea.
	if (this->hasToReceiveColor 
		&& GlobalClock::get().getCurrentTime() - this->lastTimeRequestedColor > this->TIME_BETWEEN_COLOR_REQUESTS)
	{
		this->sendMessage("requestColor", this->hasToReceiveColor, this->lastTimeSentPing);
		this->lastTimeRequestedColor = GlobalClock::get().getCurrentTime();
	}
	if (this->hasToReceiveBoardConfiguration
		&& GlobalClock::get().getCurrentTime() - this->lastTimeRequestedBoardConfiguration > this->TIME_BETWEEN_BOARD_CONFIGURATION_REQUESTS)
	{
		this->sendMessage("requestBoardConfiguration", this->hasToReceiveBoardConfiguration, this->lastTimeSentPing);
		this->lastTimeRequestedBoardConfiguration = GlobalClock::get().getCurrentTime();
	}

	// Vedem ce pachete am primit.
	// code = 0 inseamna ca nu a fost niciun eveniment
	int code = enet_host_service(this->client, &this->eNetEvent, this->TIME_WAITING_FOR_EVENTS_MS);
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
		this->workingServerConnection = false;
		this->workingOpponentConnection = false;
	}
	else
	{
		this->workingServerConnection = true;
	}

	// Apoi trimitem ping-ul catre server.
	if (GlobalClock::get().getCurrentTime() - this->lastTimeSentPing > this->TIME_BETWEEN_PINGS)
	{
		std::string messageToSend = "ping";

		this->sendMessageUnsafe(messageToSend, this->lastTimeSentPing);
	}

	// Daca am pierdut conexiunea cu oponentul atunci nu ii mai afisam numele.
	if (!this->workingOpponentConnection)
		this->opponentName = "";
}

void Client::stop()
{
	if (this->serverPeer != nullptr)
		enet_peer_disconnect(this->serverPeer, 0);
	//if (this->client != nullptr)
	//	enet_host_flush(this->client);
	if (this->client != nullptr)
		enet_host_destroy(this->client);

	this->serverPeer = nullptr;
	this->client = nullptr;

	this->serverAddress = ENetAddress();
	this->eNetEvent = ENetEvent();


	this->succesfullyConnected = false;
	this->lastTimeTriedConnection = 0.0f;

	this->lastTimeReceivedPing = 0.0f;
	this->lastTimeSentPing = 0.0f;

	this->clientName = "";
	this->color = "";
	this->opponentName = "";
	this->lastKnownBoardConfiguration = "";

	this->workingServerConnection = false;
	this->workingOpponentConnection = false;

	this->hasToSendName = false;
	this->hasToSendColor = false;
	this->hasToReceiveColor = false;
	this->hasToSendBoardConfiguration = false;
	this->hasToReceiveBoardConfiguration = false;

	this->lastTimeRequestedColor = 0.0f;
	this->lastTimeRequestedBoardConfiguration = 0.0f;
}

