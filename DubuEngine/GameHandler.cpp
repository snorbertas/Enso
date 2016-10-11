#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Level.h"
#include "Game.hpp"
#include "GameHandler.hpp"
#include "NPC.h"

void NewGame(Game* g){
	// Under construction
	std::cout << "NewGame" << "\n";
	g->gameover = false;
}

// Temp meme
void HandleMemeDash(Game* g) {
	g->pl.meme_dash_timer--;
	if (g->pl.colour == 4) {
		if (g->pl.meme_dash_timer <= 0) {
			g->pl.colour = -1;
		} else {
			g->pl.x_vel = MAX_X_VEL * 3;
			if (g->pl.direction == 1) g->pl.x_vel = -MAX_X_VEL * 3;
			g->pl.colour = 4;
		}
	}
	if (g->pl.meme_dash_timer < -1) g->pl.meme_dash_timer = -1;

	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player* p = &g->Players[i];
		p->meme_dash_timer--;
		if (p->colour == 4) {
			if (p->meme_dash_timer <= 0) {
				p->colour = -1;
			} else {
				p->x_vel = MAX_X_VEL * 3;
				if (p->direction == 1) p->x_vel = -MAX_X_VEL * 3;
				p->colour = 4;
			}
		}
		if (p->meme_dash_timer < -1) p->meme_dash_timer = -1;
	}
}

void HandleGame(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Shortened pointers
	Player* p = &g->pl;

	// Packets
	HandleGamePackets(g);

	// Temp meme
	HandleMemeDash(g);

	// Input
	HandleGameInput(g, sample_sfx);

	// Animation handle
	HandleGameMisc(g);

	// Water
	HandleWater(g);

	// Flag (temp)
	g->flag.angle += 6;
	if (g->flag.angle >= 360) g->flag.angle = 0;
	if (!collide(p->x + p->w / 2, p->y + p->h / 2, p->w / 2, g->flag.x, g->flag.y, g->flag.r)) {
		g->flag.visible = false;
	}
}

int HandleWater(Game* g) {
	// Wet
	if (g->pl.wet > 0) g->pl.wet--;
	for (int i = 0; i < MAX_FOREGROUNDS; i++) {
		if (g->lvl->Foreground[i].type == 2) {
			if (collide(&g->pl, &g->lvl->Foreground[i])) {
				if (!g->pl.in_water) {
					StartAnimation(g, 95, g->pl.x - (g->pl.w / 2), g->pl.y + g->pl.y_vel, 0, true, 0);
				}
				g->pl.colour = 2;
				g->pl.in_water = true;
				g->pl.wet = TPS;
				return 1;
			}
		}
	}
	if (g->pl.in_water) {
		StartAnimation(g, 95, g->pl.x - (g->pl.w / 2), g->pl.y + g->pl.y_vel, 0, true, 0);
	}
	if(g->pl.colour == 2) g->pl.colour = -1;
	g->pl.in_water = false;
	return 0;
}

void HandleGamePackets(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;

	// Temporary client code
	if (g->connected) {
		g->packet_debug++;
		if (g->packet_debug < 0) g->packet_debug = 0;
		if (g->packet_debug >= 2) {
			g->packet_debug = 0;
			DEPacket pmov;
			pmov = MakePacket(PACKET_TYPE_PLAYER_STATE, "", "");
			pmov.add2(p->x);
			pmov.add2(p->y);
			pmov.add2(p->direction);
			pmov.add2(p->health);
			pmov.add2(p->sprite);
			pmov.add2(p->outfit);
			pmov.add2(p->hair);
			pmov.add2(p->acc);
			pmov.add2(p->frame);
			pmov.val1 = p->x_vel;
			pmov.val2 = p->y_vel;
			AddPacketToQueue(g, pmov);
			g->ReadyToSendPackets = true;
		}
		for (int i = 0; i < MAX_PLAYERS; i++) {
			g->Players[i].x += g->Players[i].x_vel;
			g->Players[i].y += g->Players[i].y_vel;
		}

		// Ping
		g->ticks_since_last_ping++;
		if (g->ticks_since_last_ping >= TPS) {
			g->ticks_since_last_ping = 0;
			// Increase ping by 1000 so we know player is lagging
			if (g->ticks_since_last_ping - al_get_time() > g->ping * 2) {
				g->ping += 1000;
			}
			// Send a ping
			DEPacket ping = MakePacket(PACKET_TYPE_PING, "", "");
			AddPacketToQueue(g, ping);
			g->ReadyToSendPackets = true;
		}
	}
}

void HandleGameInput(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Shortened pointers
	Player* p = &g->pl;

	if (g->keys.left && p->last_cast == 0) { // Left
		if (p->last_cast == 0 && p->combat_timer < SecondsToTicks(4.5)) p->direction = 1;

		if (p->x_vel > 0 && p->y_vel == 0) p->x_vel = 0; // Disable if dont want instant-stop
		if (OnAWall(g) && !OnAFloor(g)) {
			p->letgo++;
			if (p->letgo >= 25) {
				p->x_acc = -X_ACC;
				p->letgo = 0;
			} else {
				p->x_acc = 0;
				if(p->direction == 1) p->x_vel = 0;
			}
		} else {
			if (-p->speed < p->x_vel) {
				if (p->y_vel == 0) {
					p->x_acc = -X_ACC;
				}
				else {
					p->x_acc = -X_ACC / 2;
				}
			} else {
				p->x_acc = 0;
			}
		}
	} else if (g->keys.right && p->last_cast == 0) { // Right
		if (p->last_cast == 0 && p->combat_timer < SecondsToTicks(4.5)) p->direction = 0;

		if (p->x_vel < 0 && p->y_vel == 0) p->x_vel = 0; // Disable if dont want instant-stop
		if (OnAWall(g) && !OnAFloor(g)) {
			p->letgo++;
			if (p->letgo >= 25) {
				p->x_acc = X_ACC;
				p->letgo = 0;
			} else {
				p->x_acc = 0;
			}
		} else {
			if (p->speed > p->x_vel) {
				if (p->y_vel == 0) {
					p->x_acc = X_ACC;
				} else {
					p->x_acc = X_ACC / 2;
				}
			} else {
				p->x_acc = 0;
			}
		}
	}

	// Down
	if (g->keys.down) {
		p->x_vel /= 1.2;
	}
}

void HandleGameMisc(Game* g) { // Temporary
	// Shortened pointers
	Player* p = &g->pl;

	// Last cast
	p->last_cast--;
	if (p->last_cast < 0) p->last_cast = 0;

	// Combat Feedback
	for (int i = 0; i < MAX_COMBAT_FEEDBACK; i++) {
		if (g->cfeedback[i].expire > 0) {
			g->cfeedback[i].expire--;
			g->cfeedback[i].y -= 0.5;
		}
	}

	// Player animations
	if (p->last_cast != 0) {
		p->frame = p->ability_animation;
	} else if (p->y_vel == 0) { // y stationary
		p->animation++;
		if (p->animation == 40) p->animation = 0;
		if (p->animation > 14) {
			p->frame = 0;
		} else {
			p->frame = 1;
		}
	} else if (p->y_vel > 0) { // Falling
		p->frame = 3;
	} else if (p->y_vel < 0) { // Jumping
		p->frame = 2;
	}

	// Player blinking
	p->blink--;
	if (p->blink <= 0) p->blink = 500;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		g->Players[i].blink--;
		if (g->Players[i].blink <= 0) g->Players[i].blink = 500;

	}

	// Trail temp
	for (int i = 0; i < 49; i++) {
		g->trail[i] = g->trail[i + 1];
		g->trail[i].angle += 3;
		g->trail[i].y -= 0.5;
		if (g->trail[i].angle > 360) g->trail[i].angle = 0;
		for (int j = 0; j < MAX_PLAYERS; j++) {
			Player* mp = &g->Players[j];
			mp->trail[i] = mp->trail[i + 1];
			mp->trail[i].angle += 3;
			mp->trail[i].y -= 0.5;
			if (mp->trail[i].angle > 360) mp->trail[i].angle = 0;
		}
	}
	g->trail[49].x = p->x + 26;
	g->trail[49].y = p->y + 34;
	g->trail[49].angle += 3;
	if (g->trail[49].angle > 360) g->trail[49].angle = 0;
	if (p->colour == 1) {
		g->trail[49].sprite_id = 0;
	} else if (p->colour == 2) {
		g->trail[49].sprite_id = 1;
	} else if (p->colour == 3) {
		g->trail[49].sprite_id = 2;
	} else if (p->colour == 4) {
		g->trail[49].sprite_id = 3;
	} else {
		g->trail[49].sprite_id = NO_SPRITE;
	}

	for (int j = 0; j < MAX_PLAYERS; j++) {
		Player* mp = &g->Players[j];
		mp->trail[49].x = mp->x + 26;
		mp->trail[49].y = mp->y + 34;
		mp->trail[49].angle += 3;
		if (g->trail[49].angle > 360) mp->trail[49].angle = 0;
		if (mp->colour == 1) {
			mp->trail[49].sprite_id = 0;
		} else if (mp->colour == 2) {
			mp->trail[49].sprite_id = 1;
		} else if (mp->colour == 3) {
			mp->trail[49].sprite_id = 2;
		} else if (mp->colour == 4) {
			mp->trail[49].sprite_id = 3;
		} else {
			mp->trail[49].sprite_id = NO_SPRITE;
		}
	}
}