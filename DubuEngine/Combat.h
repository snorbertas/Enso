#pragma once
#include "DEEvents.h"
#include <allegro5/allegro_font.h>

struct Projectile;

// Hitbox structure
struct HitBox {
	int type = -1;							// Hitbox type ID
	int x = 0;								// x coordinate for hitbox
	int y = 0;								// y coordinate for hitbox
	int w = 0;								// Hitbox width
	int h = 0;								// Hitbox height
	Projectile* projectile = NULL;
};

// Projectile structure
struct Projectile {
	int type = -1;							// Hitbox type ID
	int x = 0;								// x coordinate for hitbox
	int y = 0;								// y coordinate for hitbox
	int dir = -1;							// Direction
	int timer = -1;							// Timer
	HitBox* hitbox = NULL;
};

// Combat feedback structure
struct CbFeedback {
	int value = 0;							// Value (negative for heal)
	int expire = 0;							// Expire timer
	int x = 0;								// x coordinate
	int y = 0;								// y coordinate
};

class Combat
{
public:
	Combat();
	~Combat();
};
 

/* =========================== RenderHealthbars ===========================
*		Renders the healthbars.
*
*		Called from the RenderGame function.
*/
	void RenderHealthbars(Game* g);


/* ========================== HandleCombatTimers ==========================
 *		Handles combat timers for all players.
 *
 *		Called from HandleGame function.
 */
	void HandleCombatTimers(Game* g);


/* ========================== HandleGameHitboxes ==========================
 *		Jandles hitboxes collisions.
 *
 *		Called from HandleGame function.
 */
	void HandleGameHitboxes(Game* g,
	ALLEGRO_SAMPLE** sample_sfx);


/* ============================ HandleDeathCheck ==========================
 *		Handles the death checker for local player.
 *
 *		Called from HandleGame function.
 */
	void HandleDeathCheck(Game* g);


/* ========================= RenderCombatFeedback =========================
 *		Renders the combat feedback, damage/heals, etc.
 *
 *		Called from the RenderGame function.
 */
	void RenderCombatFeedback(Game* g,
		ALLEGRO_FONT** font);


int RegisterCombatEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
