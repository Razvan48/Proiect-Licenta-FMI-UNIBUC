#pragma once

#include <enet/enet.h>
#include <string>
#include <map>

class Server
{
private:
	enum class Color
	{
		WHITE,
		BLACK,
		NONE,
	};

	struct ClientData
	{
		ENetPeer* peer;
		std::string clientName;
		Server::Color color;
		float lastTimeSentPing;
		float lastTimeReceivedPing;
		bool workingConnection;

		bool hasToSendColor;
		bool hasToSendBoardConfiguration;

		ClientData()
			: peer(nullptr)
			, clientName("")
			, color(Server::Color::NONE)
			, lastTimeSentPing(0.0f)
			, lastTimeReceivedPing(0.0f)
			, workingConnection(false)

			, hasToSendColor(false)
			, hasToSendBoardConfiguration(false)
		{

		}

		void sendMessage(const std::string& messageToSend, bool& failedToSendMessage);
		void sendMessageUnsafe(const std::string& messageToSend);
	};

private:
	Server();
	~Server();
	Server(const Server& other) = delete;
	Server& operator= (const Server& other) = delete;
	Server(const Server&& other) = delete;
	Server& operator= (const Server&& other) = delete;

	const int MAX_NUM_CLIENTS;
	const int NUM_CHANNELS;
	const int TIMEOUT_LIMIT_MS;
	ENetHost* server;
	ENetAddress address;

	const int MINIMUM_PORT;
	const int MAXIMUM_PORT;

	ENetEvent eNetEvent;

	bool succesfullyCreated;
	float lastTimeTriedCreation;
	const float RETRY_CREATION_DELTA_TIME;

	const float TIME_BETWEEN_PINGS;
	const float MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST;

	std::string lastKnownBoardConfiguration;

	std::map<std::string, ClientData> connectedClients;

	// Atentie aici la unicitatea cheii
	inline std::string getClientKey(const ENetAddress& address) const { return std::to_string(address.host) + ":" + std::to_string(address.port); }

	void handleReceivedPacket();
	
public:
	static Server& get();

	void start();
	void update();
	void stop();

	enet_uint16 getPort() const { return this->address.port; }
};