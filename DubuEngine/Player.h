#pragma once
#include "DEEvents.h"
#include <string.h>

struct Trail { // Temporary (for fun)
	int sprite_id = 5;						// Sprite ID for trail
	float x = -52;							// x coordinate
	float y = -52;							// y coordinate
	int angle = 0;							// Rotation angle
};

class Player
{
public:
	Player();
	~Player();
	Trail* trail;							// Temporary (for fun)
	int meme_dash_timer = -1;				// Temporary (for fun)
	std::string name = "null";				// Player name
	int ability_animation = -1;				// Ability animation ID
	int sprite = 0;							// Player sprite ID
	int hair = -1;							// Player hair sprite ID
	int acc = -1;							// Player accessory sprite ID
	int outfit = -1;						// Player outfit sprite ID
	bool eyes = true;						// Wether player has eyes or not
	bool slash = false;						// Temp AlphaSlash variable for slash variation
	int blink = 0;							// Blink timer
	int last_cast = 0;						// Last cast timer
	int combat_timer = 0;					// Combat timer
	int health = 100;						// Health percentage (temp)
	int energy = 2000;						// Energy
	int max_energy = 2000;					// Maximum Energy
	int bonus_energy = 0;					// Bonus Energy
	int exp = 0;							// Experience
	int level = 1;							// Level
	int w = 48;								// Player width
	int h = 48;								// Player height
	int letgo = 0;							// Wall-slide timer (resistance by input)
	bool in_air = false;					// If true, player is in air
	bool in_water = false;					// If true, player is in water
	int wet = 0;							// Wet timer (prevents walljumps when getting out of water)
	int direction = 1;						// 1 = flip horizontally	
	int frame = 0;							// Frame for player animation
	float x = -1;							// x coordinate
	float y = -1;							// y coordinate
	float x_vel = 0;						// Horizontal velocity
	float x_acc = 0;						// Horizontal acceleration
	float y_vel = 0;						// Vertical velocity
	float y_acc = 0;						// Vertical acceleration
	int animation = 0;						// Animation timer
	int colour = 0;							// (temp)
	HitBox* hbox;							// Hitboxes belonging to player
	int speed = MAX_X_VEL/2;				// Player horizontal speed
};


/* ============================ HandleWallSliding =========================
 *		Handles wall sliding mechanic.
 *
 *		Called from HandleGame function.
 */
	void HandleWallSliding(Game* g);

	
/* =============================== StopPlayer =============================
 *		Handles wall sliding mechanic.
 *
 *		Called from HandleGame function.
 */
	void StopPlayer(Game* g);

int RegisterPlayerEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
