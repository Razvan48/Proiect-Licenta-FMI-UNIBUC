#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#include <string>

class AutomateGameplay
{
private:
	AutomateGameplay();
	~AutomateGameplay();
	AutomateGameplay(const AutomateGameplay& other) = delete;
	AutomateGameplay& operator= (const AutomateGameplay& other) = delete;
	AutomateGameplay(const AutomateGameplay&& other) = delete;
	AutomateGameplay& operator= (const AutomateGameplay&& other) = delete;

	const int PORT;
	const int MAX_NUM_WAITING_CONNECTIONS;

	WSADATA wsaData;

	SOCKET serverSocket;
	sockaddr_in serverAddress;

	SOCKET clientSocket;

	bool connectionAccepted;

public:
	static AutomateGameplay& get();

	void initialize();
	void update();

	void sendMessage(const std::string& message) const;
	std::string getMessage() const;

	inline bool getConnectionAccepted() const { return this->connectionAccepted; }
};

