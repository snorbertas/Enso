#ifndef SERVER_BUILD
#include "Game.hpp"
#endif
#include "Level.h"

Level_a0_8_1::Level_a0_8_1() {

}

Level_a0_8_1::~Level_a0_8_1() {

}

Level_a0_8_2::Level_a0_8_2() {

}

Level_a0_8_2::~Level_a0_8_2() {

}

Level_a0_8_3::Level_a0_8_3() {
	for (int i = 0; i < MAX_PLATFORMS;i++) {
		this->Platform[i] = LPlat();
		this->Background[i] = LPlat();
		this->Foreground[i] = LPlat();
		this->Event[i] = LPlat();
	}
}

Level_a0_8_3::~Level_a0_8_3() {

}

void HandlePlatformCollisions(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;
	Platform* plt = g->lvl->Platform;

	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if (collide(p, &plt[i])) { // Player colliding with the platform
			if (p->y + p->h <= plt[i].y + MAX_Y_VEL) { // On top
				p->y_vel = 0;
				p->y_acc = 0;
				if (p->in_air) {
					p->in_air = false;
					p->x_vel /= 2;
					g->keys.jump = false;
				}
				p->y = plt[i].y - p->h;
			} else if (p->y >= plt[i].y + plt[i].h - MAX_Y_VEL) { // From bellow
				p->y_vel = 1;
				p->y = plt[i].y + plt[i].h + 1;
			} else if (p->x + p->w <= plt[i].x + (plt[i].w / 2)) { // From left
				p->x = plt[i].x - p->w - 1;
				p->x_vel = 0;
			} else if (p->x >= plt[i].x + (plt[i].w / 2)) { // From right
				p->x = plt[i].x + plt[i].w + 1;
				p->x_vel = 0;
			} else { // Glitched inside block
				ShowMessage(g, "Collision Error", "");
			}
		}
		for (int i2 = 0; i2 < MAX_MONSTERS; i2++) {
			Monster* m = &g->monster[i2];
			if (m->type != -1) {
				if (collide(m, &plt[i])) { // Monster colliding with platform
					if (m->y + m->h <= plt[i].y + MAX_Y_VEL) { // On top
						m->y_vel = 0;
						m->y = plt[i].y - m->h;
					} else if (m->y >= plt[i].y + plt[i].h - MAX_Y_VEL) { // From bellow
						m->y_vel = 1;
						m->y = plt[i].y + plt[i].h + 1;
					} else if (m->x + m->w <= plt[i].x + (plt[i].w / 2)) { // From left
						m->x = plt[i].x - m->w - 1;
						m->x_vel = 0;
					} else if (m->x >= plt[i].x + (plt[i].w / 2)) { // From right
						m->x = plt[i].x + plt[i].w + 1;
						m->x_vel = 0;
					} else {
						// Glitched inside block
					}
				}
			}
		}
		for (int j = 0; j < MAX_ITEMS_DROPPED; j++) {
			if (g->ItemOnMap[j].item.id != -1) {
				if (collide(g->ItemOnMap[j].x, g->ItemOnMap[j].y, 34, 34, plt[i].x, plt[i].y, plt[i].w, plt[i].h)) {
					g->ItemOnMap[j].y = plt[i].y - 34;
				}
			}
		}
	}
}

Platform CreatePlatform(int type, int x, int y, int w, int h, bool shade_l, bool shade_r, bool texture_t, bool texture_b) {
	Platform temp;
	temp.type = type;
	temp.x = x;
	temp.y = y;
	temp.w = w;
	temp.h = h;
	temp.texture_l = shade_l;
	temp.texture_r = shade_r;
	temp.texture_t = texture_t;
	temp.texture_b = texture_b;
	return temp;
}

/* ============================= Event Functions ==========================
*		Used according to the event triggered. (Do NOT define in header)
*			- Game ticks
*			- Mouse clicks
*			- Keyboard presses
*/
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	HandlePlatformCollisions(g);
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
int RegisterLevelEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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