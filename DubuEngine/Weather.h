#pragma once

#include <allegro5\allegro.h>
#include "DEEvents.h"

// Raindrop structure
struct RainDrop {
	int x = 0;								// x coordinate for raindrop
	int y = 0;								// y coordinate for raindrop
	int fall_speed = 0;						// fall speed for raindrop
};

// Star structure
#define MAX_STARS 30
typedef RainDrop Star;

// Cloud structure (temp)
struct Cloud {
	int type = 6;							// Cloud type (sprite ID)
	float x = 0;							// x coordinate
	float y = 0;							// y coordinate
	double vel = 0.1;						// Horizontal velocity
};


/* =============================== ToggleRain =============================
 *		Toggles rain ON/OFF.
 *
 *		Used when desired.
 */
	void ToggleRain(Game* g,
		bool start);						// If true, start rain. Otherwise stop


/* =============================== HandleRain =============================
 *		Handles rain physics.
 *
 *		Called from HandleGame function.
 */
	void HandleRain(Game* g);

	
/* =============================== RenderNight ============================
 *		Renders a dark layer during night-time
 *
 *		Called from RenderGame function.
 */
	void RenderNight(Game* g,
		ALLEGRO_BITMAP** img_background);


/* ============================== RenderRain ==============================
 *		Renders the rain.
 *
 *		Called from the RenderGame function.
 */
	void RenderRain(Game* g,
		ALLEGRO_BITMAP** img_sprite);

	
/* ============================= RenderThunder ============================
 *		Render the thunder, well actually its lighting, but you get the point.
 *
 *		Called from the RenderGame function.
 */
	void RenderThunder(Game* g);


/* ============================ HanddleThunder ============================
 *		Handle the thunder.
 *
 *		Called when desired.
 */
	void HandleThunder(Game* g,
		ALLEGRO_SAMPLE** sample_sfx);


/* ============================ ToggleThunder ============================
 *		Toggles on/off.
 *
 *		Called when desired.
 */
	void ToggleThunder(Game* g,
		bool on);

int RegisterWeatherEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
