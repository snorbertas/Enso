#pragma once
#define MAX_PACKETS_SERVER 100
#define MAX_PACKETS_CLIENT 50
#define PACKET_TYPE_TEST 1
#define PACKET_TYPE_PING 0
#define PACKET_TYPE_AUTHENTICATE 100
#define PACKET_TYPE_SERVER_MESSAGE 101
#define PACKET_TYPE_CHAT_MESSAGE 102
#define PACKET_TYPE_PLAYER_STATE 103
#define PACKET_TYPE_PLAYER_NAME 104
#define PACKET_TYPE_HIT 105
#define PACKET_TYPE_ACTION 106
#include <iostream>
#include <string.h>

class DEPacket {
public:
/* ============================== DEPacket::add ==============================
 *		Adds an int, float or std::string into the first (1) or second (2)
 *		buffer of information for the packet.
 *
 *		Called when constructing a packet.
 */
	DEPacket add1(int);
	DEPacket add1(float);
	DEPacket add1(std::string);
	DEPacket add2(int);
	DEPacket add2(float);
	DEPacket add2(std::string);

	int packet_type = -1;			// Packet type
	float val1 = 0.0;				// Float value 1
	float val2 = 0.0;				// Float value 2
	char packet_info_1[50] = "";	// First buffer
	char packet_info_2[50] = "";	// Second buffer

	/*	This is a very basic multi-purpose packet structure for this project.
		Each value added into the buffer is separated by a space " ".
		The float values val1 and val2 are mainly used for the longer float values
		for physics such as Player::x_vel and Player::y_vel (Client-sided).
		It is quite obvious that this packet structure is very flawed and is not
		recommended to be used in serious projects. It is a good enough placeholder
		for debugging/testing purposes, but can prove to be a waste of RAM, bandwith
		and performance.
		
		One solution in improving this is by creating 1 basic structure to communicate
		the TYPE of packet. A single 32-bit integer is more than enough for the client
		and server give a little heads-up on what sort of packet to expect next. Example:
		
		##############################################################################

		struct Packet_Type {
			int type;
		}

		struct Packet_Auth {
			char username[50];
			char password[50];
			bool auth;
		}

		struct Packet_PlayerState {
			int x;
			int y;
			float x_vel;
			float y_vel;
		}

		##############################################################################
		
		Enso has never gotten to the point of server/client focus. Thus the server and
		client base is in very poor, early stages. */
};

/* ============================== MakePacket ==============================
 *		Creates a packet with two buffer string values.
 *
 *		Called when creating a packet.
 */
	DEPacket MakePacket(int, const char* info1 = "", const char* info2 = "");