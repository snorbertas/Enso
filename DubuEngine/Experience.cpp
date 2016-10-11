#include "Game.hpp"
#include "Experience.h"

Experience::Experience() {

}

Experience::~Experience() {

}

void Experience::Consume(Game* g) {
	g->pl.exp += this->value;
	this->x = -9999;
	this->y = -9999;
	this->value = -1;

	// Check if level-up
	bool done = false;
	while (!done) {
		if (g->pl.exp >= ExpReq(g->pl.level)) {
			// Level-up
			g->pl.exp -= ExpReq(g->pl.level);
			g->pl.level++;
		} else {
			done = true;
		}
	}
}

void HandleExperienceSpawns(Game* g) {
	for (int i = 0; i < MAX_EVENTS; i++) {
		if (g->lvl->Event[i].type == EE_EXPERIENCE_SPAWN) {
			int h_count = g->lvl->Event[i].w / BLOCK_SIZE;
			int v_count = g->lvl->Event[i].h / BLOCK_SIZE;

			for (int v = 0; v < v_count; v++) {
				for (int h = 0; h < h_count; h++) {
					SpawnExperience(g, g->lvl->Event[i].x + (h * BLOCK_SIZE), g->lvl->Event[i].y + (v * BLOCK_SIZE), 1);
				}
			}
		}
	}
}

void DestroyAllExperience(Game* g) {
	for (int i = 0; i < MAX_EXPERIENCE; i++) {
		g->exp[i].x = -9999;
		g->exp[i].y = -9999;
		g->exp[i].value = -1;
	}
}

int SpawnExperience(Game* g, int x, int y, int value) {
	for (int i = 0; i < MAX_EXPERIENCE; i++) {
		if (g->exp[i].value == -1) {
			g->exp[i].x = x;
			g->exp[i].y = y;
			g->exp[i].value = value;
			g->exp[i].degrees = rand() % 360;
			return i;
		}
	}
	return -1;
}

int ExpReq(int current_level) {
	return current_level * 5;
}

/* ============================= Event Functions ==========================
*		Used according to the event triggered. (Do NOT define in header)
*			- Game ticks
*			- Mouse clicks
*			- Keyboard presses
*/
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	// Collision detection
	for (int i = 0; i < MAX_EXPERIENCE; i++) {
		HitBox temp;
		temp.x = g->exp[i].x + 5;
		temp.y = g->exp[i].y + 5;
		temp.w = 22;
		temp.h = 22;
		if (collide(&g->pl, &temp)) {
			g->exp[i].Consume(g);
			al_play_sample(sample_sfx[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}

		// Animation
		g->exp[i].degrees += 4;
		if (g->exp[i].degrees > 360) g->exp[i].degrees = 0;
	}
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
int RegisterExperienceEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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