#include "DEProtocol.hpp"
#include "Game.hpp"
#include "GameHandler.hpp"

void HandleGame(Game* g) {
	for (int i=0; i < MAX_PLAYERS; i++) {
		if (g->SocketUsed[i]) {
			DEPacket p = MakePacket(PACKET_TYPE_TEST, "ping", "ping");
			AddPacketToQueue(&g->players[i], p);
			g->players[i].ReadyToSendPackets = true;
		}
	}
}

int AddPacketToQueue(Player* pl, DEPacket p) {
	for (int i = 0; i < MAX_PACKETS_SERVER; i++) {
		if (pl->PacketQueue[i].packet_type == -1) {
			pl->PacketQueue[i] = p;
			return i;
		}
	}
	return -1;
}