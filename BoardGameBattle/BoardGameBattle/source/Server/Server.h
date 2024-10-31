#pragma once

#include <enet/enet.h>
#include <map>

class Server
{
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
	
public:
	static Server& get();

	void start();
	void update();
	void stop();

	enet_uint16 getPort() const { return this->address.port; }
};