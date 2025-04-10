#pragma once

#include <string>
#include <enet/enet.h>

class Client
{
private:
	Client();
	~Client();
	Client(const Client& other) = delete;
	Client& operator= (const Client& other) = delete;
	Client(const Client&& other) = delete;
	Client& operator= (const Client&& other) = delete;

	const int MAX_NUM_SERVERS; // Numarul maxim de server-e la care poate fi clientul conectat
	const int NUM_CHANNELS;
	const int TIME_WAITING_FOR_EVENTS_MS;

	ENetPeer* serverPeer;
	ENetHost* client;
	ENetAddress serverAddress;
	ENetEvent eNetEvent;

	bool succesfullyConnected;
	float lastTimeTriedConnection;
	const float RETRY_CONNECTION_DELTA_TIME;


	const float TIME_BETWEEN_PINGS;
	const float MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST;
	float lastTimeReceivedPing;
	float lastTimeSentPing;

	std::string clientName;
	std::string color;
	std::string opponentName;
	std::string lastKnownBoardConfiguration;

	bool workingServerConnection;
	bool workingOpponentConnection;

	bool hasToSendName;
	bool hasToSendColor;
	bool hasToReceiveColor;
	bool hasToSendBoardConfiguration;
	bool hasToReceiveBoardConfiguration;

	float lastTimeRequestedColor;
	const float TIME_BETWEEN_COLOR_REQUESTS;

	float lastTimeRequestedBoardConfiguration;
	const float TIME_BETWEEN_BOARD_CONFIGURATION_REQUESTS;

	void handleReceivedPacket();

public:
	static Client& get();

	void start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName, const std::string& color, const std::string& boardConfiguration);
	void update();
	void stop();

	void sendMessage(const std::string& messageToSend, bool& failedToSendMessage, float& timeWhenMessageSent);
	void sendMessageUnsafe(const std::string& messageToSend, float& timeWhenMessageSent);

	inline const std::string& getColor() const { return this->color; }
	inline const std::string& getOpponentName() const { return this->opponentName; }
	inline const std::string& getLastKnownBoardConfiguration() const { return this->lastKnownBoardConfiguration; }

	inline void setLastKnownBoardConfiguration(const std::string& lastKnownBoardConfiguration) { this->lastKnownBoardConfiguration = lastKnownBoardConfiguration; }

	inline bool getWorkingServerConnection() const { return this->workingServerConnection; }
	inline bool getWorkingOpponentConnection() const { return this->workingOpponentConnection; }
};