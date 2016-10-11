#pragma once
#include "DEEvents.h"

class Physics
{
public:
	Physics();
	~Physics();
};

/* =========================== HandleMonsterPhysics =======================
 *		Handles physics for whole array of game's monsters.
 *
 *		Called from HandleGame function.
 */
	void HandleMonsterPhysics(Game* g);


/* =========================== HandleItemPhysics ==========================
 *		Handles item physics.
 *
 *		Called from HandleGame function.
 */
	void HandleItemPhysics(Game* g);


/* ========================== HandlePlayerPhysics =========================
 *		Handles player's physics.
 *
 *		Called from HandleGame function.
 */
	void HandlePlayerPhysics(Game* g);


int RegisterPhysicsEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
