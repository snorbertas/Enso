#include "Game.hpp"
#include "Physics.h"

Physics::Physics() {

}

Physics::~Physics() {

}

void HandleMonsterPhysics(Game* g) {
	for (int i = 0; i < MAX_MONSTERS; i++) {
		Monster* m = &g->monster[i];
		if (m->type != -1) {
			m->x += m->x_vel;
			m->y += m->y_vel;
			m->y_vel += GRAVITY;

			// Butterflies
			if (m->type == MONSTER_BUTTERFLY) {
				m->y_vel -= GRAVITY;
			}

			if (m->x_vel > MAX_X_VEL) m->x_vel = MAX_X_VEL;
			if (m->x_vel < -MAX_X_VEL) m->x_vel = -MAX_X_VEL;
			if (m->y_vel > MAX_Y_VEL) m->y_vel = MAX_Y_VEL;
			if (m->x_vel > 0) {
				m->x_vel -= RESISTANCE;
			}
			if (m->x_vel < 0) {
				m->x_vel += RESISTANCE;
			}
			if (m->x_vel < RESISTANCE && m->x_vel > -RESISTANCE) m->x_vel = 0;
			if (m->x_vel > 0) m->direction = 0;
			if (m->x_vel < 0) m->direction = 1;
		}
	}
}

void HandleItemPhysics(Game* g) {
	for (int i = 0; i < MAX_ITEMS_DROPPED; i++) {
		if (g->ItemOnMap[i].item.id != -1) {
			g->ItemOnMap[i].y++;
		}
	}
}

void HandlePlayerPhysics(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;

	p->x += p->x_vel;
	p->y += p->y_vel;
	p->x_vel += p->x_acc;
	p->y_vel += p->y_acc;
	p->y_acc = GRAVITY;
	if (g->pl.in_water) {
		p->y_acc = GRAVITY / 2;
		p->x_vel -= (p->x_acc / 2);
		if (g->keys.jump) {
			p->y_vel -= 0.3;
		}
		if (p->y_vel > MAX_Y_VEL / 2) p->y_vel = MAX_Y_VEL / 2;
	}
	if (p->x_vel > MAX_X_VEL) p->x_vel = MAX_X_VEL;
	if (p->x_vel < -MAX_X_VEL) p->x_vel = -MAX_X_VEL;
	if (p->y_vel > MAX_Y_VEL) p->y_vel = MAX_Y_VEL;

	if (p->x < 800) {
		p->x = 800;
	}

	// x resistance
	if (!g->keys.left && !g->keys.right) {
		if (p->x_vel > 0 && !g->keys.left && !g->keys.right) {
			if (p->in_air && p->y_vel < MAX_Y_VEL) {
				p->x_vel -= RESISTANCE * 0.1;
			} else if (p->in_air) {
				p->x_vel -= RESISTANCE * 0.5;
			} else {
				p->x_vel -= RESISTANCE;
			}
		}
		if (p->x_vel < 0 && !g->keys.right && !g->keys.left) {
			if (p->in_air && p->y_vel < MAX_Y_VEL) {
				p->x_vel += RESISTANCE * 0.1;
			} else if (p->in_air) {
				p->x_vel += RESISTANCE * 0.5;
			} else {
				p->x_vel += RESISTANCE;
			}
		}
		if (p->x_vel < RESISTANCE && p->x_vel > -RESISTANCE) p->x_vel = 0;
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
	HandlePlayerPhysics(g);
	HandleMonsterPhysics(g);
	HandleItemPhysics(g);
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
int RegisterPhysicsEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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