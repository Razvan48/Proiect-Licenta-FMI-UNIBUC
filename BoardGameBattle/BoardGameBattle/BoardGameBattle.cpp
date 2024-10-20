#include <iostream>
#include <enet/enet.h>

const int MAX_CLIENTS = 32;
const int NUM_CHANNELS = 1;
const int TIMEOUT_LIMIT_MS = 1000;

unsigned int clientIDGenerator = 0;

int main()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "An error occurred while initializing ENet!\n";
		return 1;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 7777;

	ENetHost* server;
	server = enet_host_create(&address, MAX_CLIENTS, NUM_CHANNELS, 0, 0); // 0, 0 means no bandwidth limits
	if (server == nullptr)
	{
		std::cerr << "An error occurred while trying to create an ENet server host!\n";
		return 1;
	}

	ENetEvent eNetEvent;
	while (enet_host_service(server, &eNetEvent, TIMEOUT_LIMIT_MS) >= 0)
	{
		switch (eNetEvent.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			std::cout << "A new client connected from " << eNetEvent.peer->address.host << ":" << eNetEvent.peer->address.port << ".\n";
			std::cout << "New client ID: " << clientIDGenerator << "\n";
			ENetPacket* packet;
			packet = enet_packet_create(&clientIDGenerator, sizeof(unsigned int), ENET_PACKET_FLAG_RELIABLE);
			++clientIDGenerator;
			enet_peer_send(eNetEvent.peer, 0, packet);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			// broadcast the received packet to all connected clients except the client who sent it
			for (size_t i = 0; i < server->peerCount; ++i)
			{
				ENetPeer* peer = &server->peers[i];
				if (peer->state == ENET_PEER_STATE_CONNECTED && peer != eNetEvent.peer)
				{
					ENetPacket* packet = enet_packet_create(eNetEvent.packet->data, eNetEvent.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(peer, 0, packet);
				}
			}
			enet_packet_destroy(eNetEvent.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << eNetEvent.peer->address.host << ":" << eNetEvent.peer->address.port << " with data " << eNetEvent.peer->data << " disconnected.\n";
			eNetEvent.peer->data = nullptr;
			break;
		default:
			break;
		}
	}

	enet_host_destroy(server);
	return 0;
}
