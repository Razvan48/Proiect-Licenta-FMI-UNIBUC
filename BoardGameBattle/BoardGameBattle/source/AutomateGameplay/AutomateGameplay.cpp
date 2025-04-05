#include "AutomateGameplay.h"

#include "../GameMetadata/GameMetadata.h"



#include <iostream>


AutomateGameplay::AutomateGameplay()
	: PORT(10000) // INFO: Portul 10000 este rezervat pentru AutomateGameplay.
	, MAX_NUM_WAITING_CONNECTIONS(1)
	, wsaData()
	, serverSocket(INVALID_SOCKET)
	, serverAddress()
	, clientSocket(INVALID_SOCKET)
	, connectionAccepted(false)
{
	int result = WSAStartup(MAKEWORD(2, 2), &this->wsaData);

	if (result != 0)
	{
		std::cout << "AutomateGameplay: WSAStartup failed: " << result << std::endl;
		return;
	}
}

AutomateGameplay::~AutomateGameplay()
{
	WSACleanup();
}

AutomateGameplay& AutomateGameplay::get()
{
	static AutomateGameplay instance;
	return instance;
}

void AutomateGameplay::initialize()
{
	closesocket(this->clientSocket);
	closesocket(this->serverSocket);

	this->serverSocket = INVALID_SOCKET;
	this->serverAddress = sockaddr_in();
	this->clientSocket = INVALID_SOCKET;

	this->connectionAccepted = false;

	//

	this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // INFO: TCP pentru garantarea livrarii mesajelor.

	this->serverAddress = { 0 };
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->serverAddress.sin_port = htons(this->PORT);

	bind(this->serverSocket, (sockaddr*)&this->serverAddress, sizeof(this->serverAddress));

	listen(this->serverSocket, this->MAX_NUM_WAITING_CONNECTIONS); // INFO: listen() este implicit non-blocking.

	u_long mode = 1;
	ioctlsocket(this->serverSocket, FIONBIO, &mode); // INFO: Non-Blocking pentru accept()
}

void AutomateGameplay::update()
{
	if (!this->connectionAccepted)
	{
		this->clientSocket = accept(this->serverSocket, NULL, NULL);
		if (this->clientSocket != INVALID_SOCKET)
		{
			this->connectionAccepted = true;
			std::cout << "AutomateGameplay: Connection accepted" << std::endl;
		}
	}
}

void AutomateGameplay::sendMessage(const std::string& message) const
{
	if (!this->connectionAccepted)
	{
		std::cout << "AutomateGameplay: Cannot send message because connection is not accepted" << std::endl;
		return;
	}

	int totalBytesSent = 0;
	while (totalBytesSent < (int)message.size())
	{
		int bytesSent = send(this->clientSocket, message.c_str() + totalBytesSent, (int)message.size() - totalBytesSent, 0);
		if (bytesSent == SOCKET_ERROR)
		{
			std::cout << "AutomateGameplay: Error sending message" << std::endl;
			break;
		}
		totalBytesSent += bytesSent;
	}
}

std::string AutomateGameplay::getMessage() const
{
	if (!this->connectionAccepted)
	{
		std::cout << "AutomateGameplay: Cannot get message because connection is not accepted" << std::endl;
		return "";
	}

	std::string message;
	message.resize(GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY);
	int totalBytesReceived = recv(this->clientSocket, &message[0], GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY, 0);

	if (totalBytesReceived <= 0) // INFO: Pot fi si numere negative.
		return "";

	while (totalBytesReceived < GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY)
	{
		int bytesReceived = recv(this->clientSocket, &message[totalBytesReceived], GameMetadata::STRING_SIZE_MOVE_AUTOMATE_GAMEPLAY - totalBytesReceived, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cout << "AutomateGameplay: Error receiving message" << std::endl;
			break;
		}
		totalBytesReceived += bytesReceived;
	}

	std::cout << "AutomateGameplay: Received message: " << message << ", total bytes received: " << totalBytesReceived << std::endl;

	return message;
}

