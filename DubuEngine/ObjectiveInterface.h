#pragma once
#include "DEEvents.h"
#define OBJECTIVE_STATE_IDLE 0
#define OBJECTIVE_STATE_OPENING 1
#define OBJECTIVE_STATE_CLOSING 2


class ObjectiveInterface
{
public:
	ObjectiveInterface();
	~ObjectiveInterface();
	bool open = false;					// Wether the objective interface is openned or not
	int state = OBJECTIVE_STATE_IDLE;	// Objective interface's state
	int goal_id = -1;					// Goal ID for objective
	int x = 0;							// x on screen
	int y = 0;							// y on screen
	int timer = -1;						// Timer for objective interface (used for animation)
	std::string line[3];				// Text information for objective
};

/*Quick terminology:
	- Objective: 1 Interface containing a goal, message, etc.
	- Goal: The ID used to identify/calculate when the objective is completed.
*/

/* ========================== DrawObjectiveDetails ========================
 *		Draws objective-specific arrows, etc.
 *
 *		Called from the RenderInterfaces function.
 */
	void DrawObjectiveDetails(Game* g,
		MoreSprites* sprites);


/* ============================ DestroyObjective ==========================
 *		Destroys and makes the objective avaible for new use.
 *
 *		Called when the objective finishes closing.
 */
	void DestroyObjective(Game* g,			// Game Pointer
		ObjectiveInterface* oi);			// Objective Interface Pointer


/* ============================== HideObjective ===========================
 *		Starts closing the objective.
 *
 *		Called when objective is completed.
 */
	void HideObjective(Game* g,
		ObjectiveInterface* oi);


/* ============================== ShowObjective ===========================
 *		Makes the objective fully visible.
 *
 *		Called when objective finishbes opening.
 */
	void ShowObjective(Game* g,
		ObjectiveInterface* oi);

	
/* =============================== Newbjective ============================
 *		Creates a new objective with the specified goal id.
 *
 *		Called when objective finishes opening and is ready to be idle.
 */
	int NewObjective(Game* g,
		int goal_id,					// Goal ID (look at HandleGoal)
		std::string line1,				// First line of objective
		std::string line2="",			// Second line of objective
		std::string line3="");			// Third line of objective


/* ============================== HandleGoal ==============================
 *		Handles the goal for the specified objective.
 *
 *		Called when desired.
 */
	void HandleGoal(Game* g,
		ObjectiveInterface* oi);


/* ======================== HandleObjectivesInterface =====================
 *		Handles all objectives.
 *
 *		Called every tick.
 */
	void HandleObjectivesInterface(Game* g);

int RegisterObjectiveInterfaceEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
