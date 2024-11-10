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
	const int TIMEOUT_LIMIT_MS;

	ENetHost* client;

	ENetAddress serverAddress;

	ENetPeer* serverPeer;

	ENetEvent eNetEvent;


	bool succesfullyConnected;
	float lastTimeTriedConnection;
	const float RETRY_CONNECTION_DELTA_TIME;


	const float TIME_BETWEEN_PINGS;
	const float MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST;


	float lastTimeReceivedPing;
	float lastTimeSentPing;


	std::string opponentName;
	bool serverConnectionOk;
	bool opponentConnectionOk;
	std::string lastKnownBoardConfiguration;

	bool knowsItsOwnColor;
	bool hasSentItsOwnColor;
	bool hasSentItsClientName;
	bool hasRequestedInitialBoardConfiguration;

	std::string clientName;
	std::string color;

	void handleReceivedPacket();

public:
	static Client& get();

	void start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName, const std::string& color);
	void update();
	void stop();

	void sentMessage(const std::string& message);

	inline const std::string& getOpponentName() const { return this->opponentName; }
	inline bool getServerConnectionOk() const { return this->serverConnectionOk; }
	inline bool getOpponentConnectionOk() const { return this->opponentConnectionOk; }
	inline const std::string& getLastKnownBoardConfiguration() const { return this->lastKnownBoardConfiguration; }
};