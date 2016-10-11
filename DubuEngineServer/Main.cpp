#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <fstream>
#include <sstream>
#define recv_s(Socket,DataPtr,Size,Other) recv( (Socket), (char *)(DataPtr), (Size), (Other))
#define send_s(Socket,DataPtr,Size,Other) send( (Socket), (char *)(DataPtr), (Size), (Other)) 
#include <winsock2.h>
#include <string>
#include <process.h>
#include <allegro5/allegro.h>
#include "Game.hpp"
#include "GameHandler.hpp"
#include "Main.hpp"
#include "Player.hpp"
#include "DEProtocol.hpp"

int main()
{
	LoadSettings();
	if (InitServer() == false) {
		return 0; // Can't initiate
	}

	Game g = Game();
	_beginthreadex(0, 0, GameThread, &g, 0, 0);

	while (!g.ready) { // Wait until game finishes initiliazing/loading
		Sleep(1000);
	}

	std::cout << "Listening for connections on port " << port << "\n";
	while (!g.done) {
		if (NewPlayerID(&g) >= 0) { // If server is not full
			client = accept(Socket, 0, 0);

			if (client == INVALID_SOCKET) {
				std::cout << "invalid client socket\n";
				continue;
			}
			if (NewPlayerID(&g) >= 0) {
				_beginthreadex(0, 0, ClientThread, &g, 0, 0);
			}
			else {
				std::cout << "New connection refused: SERVER FULL\n";
			}
		}
	}

	WSACleanup();
	return 0;
}

bool InitServer() {
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		std::cout << "WSA Initialization failed!\r\n";
		WSACleanup();
		return false;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed.\r\n";
		WSACleanup();
		return false;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed.\r\n";
		WSACleanup();
		return false;
	}

	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = INADDR_ANY;
	serverInf.sin_port = htons(25565);

	if (bind(Socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
	{
		std::cout << "Unable to bind socket!\r\n";
		WSACleanup();
		return false;
	}

	int iResult = listen(Socket, SOMAXCONN);

	if (iResult)
	{
		printf("Can't listen %u", GetLastError());
		return false;
	}
	return true;
}

unsigned int __stdcall GameThread(void *data)
{
	std::cout << "Initializing Game...\n";
	Game* g = (Game*)data;

	if (!al_init()) {
		std::cout << "Couldn't initiate Allegro Timer\n";
		return -1;
	}
	timer = al_create_timer(1.0 / 1.0);
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	g->ready = true;

	while (!g->done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			g->ready = true;
			HandleGame(g);
		}
	}
	return 0;
}

int NewPlayerID(Game* g) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (g->SocketUsed[i] == false) return i;
	}
	return -1;
}

unsigned int __stdcall ClientThread(void *data)
{
	Game* g = (Game*)data;
	int pID = NewPlayerID(g);
	g->ClientSocket[pID] = client;
	g->SocketUsed[pID] = true;
	SOCKET Client = g->ClientSocket[pID];

	std::cout << "Client connected (" << pID << ") " << GetIP(Client) << "\n";

	// Disable nagle
	int flag = 1;
	setsockopt(Client,	// Socket affected
		IPPROTO_TCP,	// Set option at TCP level
		TCP_NODELAY,    // Name of option
		(char *)&flag,  // The cast is historical cruft
		sizeof(int));   // Length of option value

	u_long iMode = 1;
	ioctlsocket(Client, FIONBIO, &iMode);

	DEPacket msg = MakePacket(-1, "", "");
	while (true)
	{
		Sleep(1);
		int nReadBytes = recv_s(Client, &msg, sizeof(msg), 0);
		if (nReadBytes == SOCKET_ERROR) {
			// Nothing yet
		}
		else {
			if (msg.packet_type == PACKET_TYPE_AUTHENTICATE) {
				if ((std::string)msg.packet_info_2 == "pass") {
					AddPacketToQueue(&g->players[pID], MakePacket(PACKET_TYPE_AUTHENTICATE, "", ""));
					for (int i = 0; i < MAX_PLAYERS; i++) {
						if (g->SocketUsed[i] && i != pID) {
							DEPacket p = MakePacket(PACKET_TYPE_PLAYER_NAME, std::to_string(pID).c_str(), msg.packet_info_1);
							AddPacketToQueue(&g->players[i], p);
							p = MakePacket(PACKET_TYPE_CHAT_MESSAGE, "__SYSTEM__", ((std::string)msg.packet_info_1 + " has joined the room.").c_str());
							AddPacketToQueue(&g->players[i], p);
							g->players[i].ReadyToSendPackets = true;
						}
					}
				}
				else {
					// Invalid username/pass
					g->SocketUsed[pID] = false;
					std::cout << "Client disconnected! (" << pID << ") \n";
					shutdown(Client, SD_SEND); // Shutdown our socket
					closesocket(Client); // Close our socket entirely

					for (int i = 0; i < MAX_PLAYERS; i++) {
						if (g->SocketUsed[i] && i != pID) {
							DEPacket p;
							p = MakePacket(PACKET_TYPE_PLAYER_STATE, "", "");
							p.add2(-9999);
							p.add2(-9999);
							p.add2(0);
							p.add2(100);
							p.val1 = 0;
							p.val2 = 0;
							AddPacketToQueue(&g->players[i], p);
							g->players[i].ReadyToSendPackets = true;
						}
					}
					return 0;
				}
			}
			else
				if (msg.packet_type == PACKET_TYPE_CHAT_MESSAGE) {
					std::cout << msg.packet_info_1 << "[" << pID << "]: " << msg.packet_info_2 << "\n";
					for (int i = 0; i < MAX_PLAYERS; i++) {
						if (g->SocketUsed[i]) {
							DEPacket p = MakePacket(PACKET_TYPE_CHAT_MESSAGE, msg.packet_info_1, msg.packet_info_2);
							AddPacketToQueue(&g->players[i], p);
						}
					}
				}
				else
					if (msg.packet_type == PACKET_TYPE_PLAYER_STATE) {
						for (int i = 0; i < MAX_PLAYERS; i++) {
							if (g->SocketUsed[i] && i != pID) {
								DEPacket p = MakePacket(PACKET_TYPE_PLAYER_STATE, std::to_string(pID).c_str(), msg.packet_info_2);
								p.val1 = msg.val1;
								p.val2 = msg.val2;
								AddPacketToQueue(&g->players[i], p);
								g->players[i].ReadyToSendPackets = true;
							}
						}
					}
					else
						if (msg.packet_type == PACKET_TYPE_PLAYER_NAME) {
							for (int i = 0; i < MAX_PLAYERS; i++) {
								if (g->SocketUsed[i] && i != pID) {
									DEPacket p = MakePacket(PACKET_TYPE_PLAYER_NAME, std::to_string(pID).c_str(), msg.packet_info_2);
									AddPacketToQueue(&g->players[i], p);
									g->players[i].ReadyToSendPackets = true;
								}
							}
						}
						else
							if (msg.packet_type == PACKET_TYPE_HIT) {
								int i = atoi(msg.packet_info_1);
								if (g->SocketUsed[i]) {
									DEPacket p = MakePacket(PACKET_TYPE_HIT, "", msg.packet_info_2);
									AddPacketToQueue(&g->players[i], p);
									g->players[i].ReadyToSendPackets = true;
								}
							}
							else
								if (msg.packet_type == PACKET_TYPE_ACTION) {
									for (int i = 0; i < MAX_PLAYERS; i++) {
										if (g->SocketUsed[i] && i != pID) {
											DEPacket p = MakePacket(PACKET_TYPE_ACTION, std::to_string(pID).c_str(), msg.packet_info_2);
											p.val1 = msg.val1;
											AddPacketToQueue(&g->players[i], p);
											g->players[i].ReadyToSendPackets = true;
										}
									}
								}
								else
									if (msg.packet_type == PACKET_TYPE_PING) {
										AddPacketToQueue(&g->players[pID], msg);
										g->players[pID].ReadyToSendPackets = true;
									}
		}

		if (g->players[pID].ReadyToSendPackets) {
			g->players[pID].ReadyToSendPackets = false;
			for (int i = 0; i < MAX_PACKETS_SERVER; i++) {
				if (g->players[pID].PacketQueue[i].packet_type != -1) {
					int nSendBytes = send_s(Client, &g->players[pID].PacketQueue[i], sizeof(g->players[pID].PacketQueue[i]), 0);
					if (nSendBytes == SOCKET_ERROR)
					{
						g->SocketUsed[pID] = false;
						std::cout << "Client disconnected! (" << pID << ") \n";
						shutdown(Client, SD_SEND); // Shutdown our socket		
						closesocket(Client); // Close our socket entirely

						for (int i = 0; i < MAX_PLAYERS; i++) {
							if (g->SocketUsed[i] && i != pID) {
								DEPacket p;
								p = MakePacket(PACKET_TYPE_PLAYER_STATE, "", "");
								p.add2(-9999);
								p.add2(-9999);
								p.add2(0);
								p.add2(100);
								p.val1 = 0;
								p.val2 = 0;
								AddPacketToQueue(&g->players[i], p);
								g->players[i].ReadyToSendPackets = true;
							}
						}
						return 0;
					}
					g->players[pID].PacketQueue[i].packet_type = -1;
				}
			}
		}
	}
	return 0;
}

char* GetIP(SOCKET Client) {
	SOCKADDR_IN client_info = { 0 };
	int addrsize = sizeof(client_info);
	getpeername(Client, (sockaddr*)&client_info, &addrsize);
	return inet_ntoa(client_info.sin_addr);
}

void LoadSettings() {
	std::cout << "Loading settings...\n";
	std::ifstream file("config.ini");
	file >> port;
}