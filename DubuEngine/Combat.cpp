#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "Combat.h"

Combat::Combat() {

}

Combat::~Combat() {

}

void RenderHealthbars(Game* g) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Player
	if (g->pl.combat_timer > 0) {
		DrawRectangle(g, g->pl.x - 11 + x, g->pl.y - 8 + y, 74, 7, 0, 0, 0);
		DrawRectangle(g, g->pl.x - 10 + x, g->pl.y - 7 + y, 72, 5, 255, 0, 0);
		DrawRectangle(g, g->pl.x - 10 + x, g->pl.y - 7 + y, (int)(72 * (float)((float)g->pl.health / 100.00)), 5, 0, 255, 0);
	}

	// Clients
	for (int i = 0; i < MAX_PLAYERS; i++) {
		Player* pt = &g->Players[i];
		if (pt->combat_timer > 0) {
			DrawRectangle(g, pt->x - 11 + x, pt->y - 8 + y, 74, 7, 0, 0, 0);
			DrawRectangle(g, pt->x - 10 + x, pt->y - 7 + y, 72, 5, 255, 0, 0);
			DrawRectangle(g, pt->x - 10 + x, pt->y - 7 + y, (int)(72 * (float)((float)pt->health / 100.00)), 5, 0, 255, 0);
		}
	}

	// NPCs
	for (int i = 0; i < MAX_MONSTERS; i++) {
		Monster* m = &g->monster[i];
		if (m->combat_timer > 0) {
			DrawRectangle(g, m->x - 11 + x, m->y - 8 + y, 74, 7, 0, 0, 0);
			DrawRectangle(g, m->x - 10 + x, m->y - 7 + y, 72, 5, 255, 0, 0);
			DrawRectangle(g, m->x - 10 + x, m->y - 7 + y, (int)(72 * (float)((float)m->health / (float)m->max_health)), 5, 0, 255, 0);
		}
	}
}

void RenderCombatFeedback(Game* g, ALLEGRO_FONT** font) {
	// r_x, r_y are the x and y ratios for scaling text sizes
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;

	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render all combat feedbacks (damage, heals, etc.)
	for (int i = 0; i < MAX_COMBAT_FEEDBACK; i++) {
		if (g->cfeedback[i].expire > 0) {
			DrawText(font[0], 255, 10, 10, g->cfeedback[i].x + x, g->cfeedback[i].y + y, ALLEGRO_ALIGN_CENTER, "%i", g->cfeedback[i].value);
		}
	}
}


void HandleCombatTimers(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;

	if (p->combat_timer > 0) p->combat_timer--;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (g->Players[i].combat_timer > 0) g->Players[i].combat_timer--;
	}
}

void HandleGameHitboxes(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Shortened pointers
	Player* p = &g->pl;

	for (int i = 0; i < MAX_PLAYER_HITBOXES; i++) {
		if (g->pl.hbox[i].type == 0) { //normal attack?
			if (g->connected) {
				for (int i2 = 0; i2 < MAX_PLAYERS; i2++) {
					Player* pt = &g->Players[i2];
					if (collide(pt, &p->hbox[i])) {
						pt->combat_timer = SecondsToTicks(2.0);
						int dmg = 5 + rand() % 10;
						int direction = 1;
						if (p->x < pt->x) direction = 0;
						pt->health -= dmg;
						if (pt->health < 0) pt->health = 0;
						CombatFeedback(g, dmg, pt->x + (pt->w / 2), pt->y + (pt->h / 2));
						DEPacket hit_packet = MakePacket(PACKET_TYPE_HIT, std::to_string(i2).c_str(), "");
						hit_packet.add2(direction);
						hit_packet.add2(dmg);
						AddPacketToQueue(g, hit_packet);
						g->ReadyToSendPackets = true;
					}
				}
			}
			for (int i2 = 0; i2 < MAX_MONSTERS; i2++) {
				Monster* m = &g->monster[i2];
				if (m->type == NPC_SLIME || m->type == NPC_BAT) {
					if (collide(m, &p->hbox[i]) && m->health > 0) {
						m->combat_timer = SecondsToTicks(2.0);
						m->x_vel = 5;
						if (p->direction == 1) m->x_vel = -5;
						m->y_vel = -5;
						m->health -= 5;
						CombatFeedback(g, 5 + rand() % 2, m->x + (m->w / 2), m->y + (m->h / 2));
						if (m->health <= 0) {
							// We can drop some loot here
						}
					}
				}
			}
			g->pl.hbox[i].type = -1;
		}
		if (g->pl.hbox[i].type == 1) {
			if (g->connected) {
				for (int i2 = 0; i2 < MAX_PLAYERS; i2++) {
					Player* pt = &g->Players[i2];
					if (collide(pt, &p->hbox[i])) {
						pt->combat_timer = SecondsToTicks(2.0);
						int dmg = 5 + rand() % 10;
						int direction = 1;
						if (p->x < pt->x) direction = 0;
						pt->health -= dmg;
						if (pt->health < 0) pt->health = 0;
						CombatFeedback(g, dmg, pt->x + (pt->w / 2), pt->y + (pt->h / 2));
						DEPacket hit_packet = MakePacket(PACKET_TYPE_HIT, std::to_string(i2).c_str(), "");
						hit_packet.add2(direction);
						hit_packet.add2(dmg);
						AddPacketToQueue(g, hit_packet);
						g->ReadyToSendPackets = true;
						g->pl.hbox[i].type = -1;
						if (g->pl.hbox[i].projectile != NULL) {
							g->pl.hbox[i].projectile->type = -1;
						}
					}
				}
			}
			for (int i2 = 0; i2 < MAX_MONSTERS; i2++) {
				Monster* m = &g->monster[i2];
				if (m->type == NPC_SLIME || m->type == NPC_BAT) {
					if (collide(m, &p->hbox[i]) && m->health > 0) {
						m->combat_timer = SecondsToTicks(2.0);
						m->x_vel = 5;
						if (p->direction == 1) m->x_vel = -5;
						m->y_vel = -5;
						m->health -= 5;
						CombatFeedback(g, 5 + rand() % 2, m->x + (m->w / 2), m->y + (m->h / 2));
						g->pl.hbox[i].type = -1;
						if (g->pl.hbox[i].projectile != NULL) {
							g->pl.hbox[i].projectile->type = -1;
						}
						if (m->health <= 0) {
							// We can drop some loot here
						}
					}
				}
			}
		}
	}
}

void HandleProjectiles(Game* g) {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (g->projectiles[i].type == ABILITY_BASIC_SPELL) {
			int x_vel = 5;
			if (g->projectiles[i].dir == 1) x_vel = -5;
			g->projectiles[i].timer++;
			int y_vel = (g->projectiles[i].timer / 20);
			g->projectiles[i].x += x_vel;
			g->projectiles[i].y += y_vel;
			if (g->projectiles[i].hitbox != NULL) {
				g->projectiles[i].hitbox->x = g->projectiles[i].x;
				g->projectiles[i].hitbox->y = g->projectiles[i].y;
			}
			if (g->projectiles[i].timer > SecondsToTicks(1.0)) {
				g->projectiles[i].type = -1;
				if (g->projectiles[i].hitbox != NULL) {
					g->projectiles[i].hitbox->type = -1;
				}
			}
		}
	}
}

void HandleDeathCheck(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;

	if (p->health <= 0) {
		p->health = 100;
		bool found_spawn = false;
		for (int j = 0; j < MAX_EVENTS; j++) {
			if (g->lvl->Event[j].type == EE_PLAYER_SPAWN) {
				g->pl.x = g->lvl->Event[j].x + rand() % g->lvl->Event[j].w;
				g->pl.y = g->lvl->Event[j].y + rand() % g->lvl->Event[j].h;
				j = MAX_EVENTS;
				found_spawn = true;
			}
		}
	}
}

/* ============================= Event Functions ==========================
 *		Used according to the event triggered. (Do NOT define in header)
 *			- Game ticks
 *			- Mouse clicks
 *			- Keyboard presses
 */
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	HandleCombatTimers(g);
	HandleGameHitboxes(g, sample_sfx);
	HandleDeathCheck(g);
	HandleProjectiles(g);
}

static void Click(Game* g, int button, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Click
}

static void Press(Game* g, int kid, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Press
}


/* =========================== Register Function ==========================
*		The register function.
*		Registers the event functions to be triggered by the main loop.
*		May change the name and the event types to match the class purpose.
*
*		Call it once with the rest of init functions on main().
*/
int RegisterCombatEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
	for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
		if (t[i] == NULL && c[i] == NULL && p[i] == NULL) {
			t[i] = (TimerEventFunction)&Tick;		// Remove if not needed
			c[i] = (MouseEventFunction)&Click;		// Remove if not needed
			p[i] = (KeyboardEventFunction)&Press;	// Remove if not needed
			return i;
		}
	}
	return -1; // No slots
}