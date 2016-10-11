#include "Game.hpp"
#include "RewardInterface.h"

RewardInterface::RewardInterface() {

}

RewardInterface::~RewardInterface() {

}

void DestroyReward(Game* g, RewardInterface* ri) {
	ri->state = REWARD_STATE_CLOSED;

	// Sort
	int free_slot = -1;
	for (int i = 0; i < MAX_REWARDS; i++) {
		if (g->rewards[i].state == REWARD_STATE_OPEN) {
			// Try put it to the bottom
			if (free_slot != -1) {
				g->rewards[free_slot] = g->rewards[i];
				g->rewards[i].state = REWARD_STATE_CLOSED;
				// Recalculate next free slot
				free_slot = -1;
				for (int j = 0; j < MAX_REWARDS; j++) {
					if (g->rewards[j].state == REWARD_STATE_CLOSED) {
						free_slot = j;
						j = MAX_REWARDS;
					}
				}
			}
		} else {
			if (free_slot == -1) free_slot = i;
		}
	}
}

int NewReward(Game* g, std::string msg, int re, int gr, int bl) {
	for (int i = 0; i < MAX_REWARDS; i++) {
		if (g->rewards[i].state == REWARD_STATE_CLOSED) {
			g->rewards[i].r = re;
			g->rewards[i].g = gr;
			g->rewards[i].b = bl;
			g->rewards[i].msg = msg;
			g->rewards[i].timer = SecondsToTicks(5.0);
			g->rewards[i].state = REWARD_STATE_OPEN;
			return i;
		}
	}
	return -1;
}

void HandleRewardInterface(Game* g) {
	// Sort
	for (int i = 0; i < MAX_REWARDS; i++) {
		if (g->rewards[i].state == REWARD_STATE_OPEN) {
			g->rewards[i].timer--;
			if (g->rewards[i].timer <= 0) {
				DestroyReward(g, &g->rewards[i]);
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
	HandleRewardInterface(g);
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
int RegisterRewardInterfaceEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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