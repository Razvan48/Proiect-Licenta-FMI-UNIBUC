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
	std::string clientName;

	ENetPeer* serverPeer;

	ENetEvent eNetEvent;

public:
	static Client& get();

	void start(const std::string& serverIP, enet_uint16 serverPort, const std::string& clientName, const std::string& color);
	void update();
	void stop();

};