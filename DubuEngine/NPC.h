#pragma once
#include "Level.h"
#include "Combat.h"
#include "DEEvents.h"

#define MAX_DB_NPC 100
#define NPC_STATE_IDLE 1
#define NPC_STATE_WALKING 2

class NPC
{
public:
	NPC();
	~NPC();
	bool NPC::OnAFloor(Game* g);

	// NPC Variables
	const char* name = "Mr. Noboddy";		// NPC name
	int combat_timer = 0;					// Combat timer
	int health = 100;						// Current health
	int max_health = 100;					// MAX health
	int level = 0;							// Level for monster
	int frame = 0;							// Animation frame
	int animation = 0;						// Animation timer
	int type = -1;							// Monster type
	float x = 100;							// x coordinate
	float y = 100;							// y coordinate
	int w = 48;								// Monster's width
	int h = 48;								// Monster's height
	float x_vel = 0;						// Horizontal velocity
	float y_vel = 0;						// Vertical velocity
	int direction = 0;						// 1 to flip horizontally
	int timer = 0;							// Timer (for future AI)
	HitBox* hbox;							// Hitboxes pointer belonging to this monster
	Platform zone;							// Zone/area for monster/NPC (spawn event platform)

	int sprite = 0;							// Body Sprite
	int hair = -1;							// Hair sprite
	int acc = -1;							// Accessory sprite
	int outfit = -1;						// Outfit sprite
	bool eyes = false;						// Render eyes
	int blink = -1;							// Blink timer
	int state = NPC_STATE_IDLE;				// NPC state (for AI)
	int target_x = -9999;					// Target x coordinate
	int target_y = -9999;					// Target y coordinate
	bool in_air = false;					// Used for AI to check if it can jump
	bool server_npc = false;				// Wether the npc is controlled by the server or not
};

class NPCDatabase
{
public:
	NPCDatabase();
	~NPCDatabase();
	NPC NPC[MAX_DB_NPC];					// Array of NPCs in the database
};

typedef NPC Monster;


/* ================================ HandleAI ==============================
 *		Handles the artificial intelligence for monsters and NPCs.
 *
 *		Called from HandleGame function.
 */
	void HandleAI(Game* g);

	
/* ========================== HanddleEventNPCSpawns =======================
 *		Checks events on map to spawn NPCs.
 *
 *		Should be called once, when a new map is loaded.
 */
	void HandleEventNPCSpawns(Game* g);
	
	
/* ================================ WipeNPC ===============================
 *		Wipes all NPCs in game.
 *
 *		Called when desired.
 */
	void WipeNPC(Game* g);


/* ================================ SpawnNPC ==============================
 *		Spawns the NPC with specified settings.
 *
 *		Called when desired.
 */
	int SpawnNPC(Game* g,			// Game pointer
		NPC npc,					// NPC object to spawn
		int x, int y,				// x, y coordinates on map to spawn
		Platform* zone=NULL);		// NPC spawn territory


int RegisterNPCEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
