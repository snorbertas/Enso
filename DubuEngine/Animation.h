#pragma once
#include "DEEvents.h"
#include "Player.h"

// Flag structure (temp)
struct Flag {
	int x = -100;							// x coordinate
	int y = -100;							// y coordinate
	int angle = 0;							// Rotation angle
	int r = 0;								// Flag radius
	bool visible = false;					// If true, it's visible
};

// GFX Animation structure
struct GFXAnimation {
	int animated = 1;						// 1 = Animated, 0 = Still image
	int type = -1;							// Animation type (sprite ID)
	float x = 0;							// x coordinate
	float y = 0;							// y coordinate
	int flip = 0;							// 1 = Flip horizontally
	int frame = 0;							// Animation frame
	bool relative_to_player = false;		// If true, animation's coordinates are relative to a player
	int frames = 0;							// Max frames for animation
	int loop = 0;							// Number of loops for animation
	Player* p;								// Player pointer relative to
};

// Trail structure (temp?)
struct TrailAnim {
	int sprite_id = 5;						// Sprite ID for trail
	float x = -52;							// x coordinate
	float y = -52;							// y coordinate
	int angle = 0;							// Rotation angle
};

class Animation
{
public:
	Animation();
	~Animation();
};


/* ============================= RenderTrails =============================
 *		Renders the trail animations.
 *		Currently only used for water trail.
 *
 *		Called from the RenderGame function.
 */
void RenderTrails(Game* g,
	ALLEGRO_BITMAP** img_sprite);


/* =========================== RenderAnimations ===========================
 *		Renders the animations.
 *
 *		Called from the RenderGame function.
 */
void RenderAnimations(Game* g,
	ALLEGRO_BITMAP** img_sprite);

int RegisterAnimationEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
