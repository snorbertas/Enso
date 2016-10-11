#pragma once

#include "DEProtocol.hpp"

class Player
{
public:
	Player();
	int id = 0;					// Player ID
	DEPacket* PacketQueue;		// Packet queue
	bool ReadyToSendPackets;	// Set to true when there's a packet ready to be sent by server
};