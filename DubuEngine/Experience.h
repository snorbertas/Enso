#pragma once
#include "DEEvents.h"

#define MAX_EXPERIENCE 500

class Experience
{
public:
	Experience();
	~Experience();
	int x = -9999;						// x coordinate for experience
	int y = -9999;						// y coordinate for experience
	int value = -1;						// Experience value when collected
	int degrees = 0;					// Experience rotation 0 - 360
	void Experience::Consume(Game* g);	// Consumes the experience
};

/* ========================= HandleExperienceSpawns =====================
 *		Handles all experience spawn points.
 *
 *		Called when desired.
 */
	void HandleExperienceSpawns(Game* g);

/* ========================== DestroyAllExperience ======================
 *		Clears the map of all spawned experience.
 *
 *		Called when desired.
 */
	void DestroyAllExperience(Game* g);


/* ============================= SpawnExperience ========================
 *		Spawns an experience on the map.
 *
 *		Called when desired.
 */
	int SpawnExperience(Game* g, int x, int y, int value);
	
	
/* ================================= ExpReq =============================
 *		Returns expereience required to level-up based on current level.
 *
 *		Called when desired.
 */
	int ExpReq(int current_level);


int RegisterExperienceEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
