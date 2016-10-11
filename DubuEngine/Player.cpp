#include "Game.hpp"
#include "Player.h"

Player::Player() {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		this->trail = new Trail[50];
		for (int j = 0; j < 50; j++) {
			this->trail[i].angle = rand() % 360;
		}
	}
}

Player::~Player() {

}

void StopPlayer(Game* g) {
	g->keys.left = false;
	g->keys.right = false;
	g->pl.x_vel = 0;
	g->pl.x_acc = 0;
}

void HandleWallSliding(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;

	if (p->y_vel > 0 && OnAWall(g)) {
		p->in_air = false;
		p->y_vel = WALL_SLIDE;
	}
}

void HandleRecovery(Game* g) {
	if (g->pl.energy < (g->pl.max_energy + g->pl.bonus_energy)) {
		g->pl.energy += 5;
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
	HandleWallSliding(g);
	HandleRecovery(g);
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
int RegisterPlayerEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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