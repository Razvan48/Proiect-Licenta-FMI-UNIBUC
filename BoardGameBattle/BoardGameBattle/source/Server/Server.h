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
		float lastTimeReceivedPing;
		float lastTimeSentPing;
		Server::Color color;
		bool recentlyReceivedPing;
		ENetPeer* peer;

		ClientData()
			: lastTimeReceivedPing(0.0f), lastTimeSentPing(0.0f), color(Server::Color::NONE)
			, recentlyReceivedPing(false), peer(nullptr)
		{

		}
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
	ENetAddress address;
	ENetHost* server;

	const int MINIMUM_PORT;
	const int MAXIMUM_PORT;

	ENetEvent eNetEvent;

	bool succesfullyCreated;
	float lastTimeTriedCreation;
	const float RETRY_CREATION_DELTA_TIME;

	const float TIME_BETWEEN_PINGS;
	const float MAXIMUM_TIME_BEFORE_DECLARING_CONNECTION_LOST;

	std::map<std::string, ClientData> connectedClients;

	std::string lastKnownBoardConfiguration;

	// Atentie aici la unicitatea cheii
	inline std::string getClientKey(const ENetAddress& address) const { return std::to_string(address.host); }

	void handleReceivedPacket();
	
public:
	static Server& get();

	void start();
	void update();
	void stop();

	enet_uint16 getPort() const { return this->address.port; }
};