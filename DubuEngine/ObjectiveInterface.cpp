#include "Game.hpp"
#include "ObjectiveInterface.h"
#include "ScaledDraw.hpp"

ObjectiveInterface::ObjectiveInterface() {
	for (int i = 0; i < 3; i++) {
		this->line[i] = "";
	}
}

ObjectiveInterface::~ObjectiveInterface() {

}

void DrawObjectiveDetails(Game* g, MoreSprites* sprites) {
	int x = g->camera.x;
	int y = g->camera.y;
	for (int i = 0; i < MAX_OBJECTIVES; i++) {
		ObjectiveInterface* oi = &g->objectives[i];
		if (oi->open) {
			if (oi->goal_id == 1) {
				if (oi->timer > 192 || (oi->timer > 64 && oi->timer < 128)) {
					DrawImage(g, sprites->img_sprite[SPRITE_INTERFACE_ARROW], 2464 + x, y, 0);
				}
			}
		}
	}
}

void DestroyObjective(Game* g, ObjectiveInterface* oi) {
	// Delete
	for (int i = 0; i < 3; i++) {
		oi->line[i] = "";
	}
	oi->open = false;
	oi->x = 0;
	oi->y = 0;
	oi->timer = -1;
	oi->state = OBJECTIVE_STATE_IDLE;
}

void HideObjective(Game* g, ObjectiveInterface* oi) {
	// Start Closing
	oi->state = OBJECTIVE_STATE_CLOSING;
	oi->timer = 250;
	oi->goal_id = -1;
}

void ShowObjective(Game* g, ObjectiveInterface* oi) {
	// Make Idle
	oi->state = OBJECTIVE_STATE_IDLE;
	oi->timer = 100;
	oi->x = 0;
}

int NewObjective(Game* g, int goal_id, std::string line1, std::string line2, std::string line3) {
	// Start Opening
	for (int i = 0; i < MAX_OBJECTIVES; i++) {
		if (!g->objectives[i].open) {
			ObjectiveInterface* oi = &g->objectives[i];
			oi->open = true;
			oi->state = OBJECTIVE_STATE_OPENING;
			oi->goal_id = goal_id;
			oi->timer = 250;
			oi->x = -250;
			// y will be calculated on every handle
			oi->line[0] = line1;
			oi->line[1] = line2;
			oi->line[2] = line3;
			return i;
		}
	}
	std::cout << "Can't show new objective, out of space!\n";
	return -1;
}

void HandleGoal(Game* g, ObjectiveInterface* oi) {
	int goal_id = oi->goal_id;
	switch (goal_id) {
		case 1: // Get inside the Netiki cave
		{
			if (g->story.missions[0].prog >= 40) { // Sigh NEED MAP IDS
				HideObjective(g, oi);
			}
			break;
		}
		case 2: // Complete Level 2
		{
			if (g->monster[0].type == NPC_HOMELESS_MAN) { // Sigh temp, need map IDs
				HideObjective(g, oi);
			}
			break;
		}
		case 3: // Equip first skill
		{
			for (int i = 0; i < 8; i++) {
				if (g->AbilitySlots[0].ability.id == ABILITY_BASIC_SLASH) {
					HideObjective(g, oi);
				}
			}
			break;
		}
		case 4: // Finish level 2
		{
			if (g->story.missions[0].prog >= 42) {
				HideObjective(g, oi);
			}
			break;
		}
	}
}

void HandleObjectivesInterface(Game* g) {
	int y = 0;
	for (int i = 0; i < MAX_OBJECTIVES; i++) {
		ObjectiveInterface* oi = &g->objectives[i];
		if (oi->open) {
			HandleGoal(g, oi);
			oi->y = y;
			y += 55;
			if (oi->timer > 0) oi->timer--;
			if (oi->state == OBJECTIVE_STATE_IDLE) {
				if (oi->timer == 0) oi->timer = 255;
			} else if (oi->state == OBJECTIVE_STATE_CLOSING) {
				oi->x = -250 + oi->timer;
				if (oi->timer == 0) {
					DestroyObjective(g, oi);
				}
			} else if (oi->state == OBJECTIVE_STATE_OPENING) {
				oi->x = -oi->timer;
				if (oi->timer == 0) ShowObjective(g, oi);
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
	HandleObjectivesInterface(g);
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
int RegisterObjectiveInterfaceEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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