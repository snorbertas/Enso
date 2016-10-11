#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Game.hpp"
#include "Weather.h"
#include "ScaledDraw.hpp"

void ToggleThunder(Game* g, bool on) {
	g->lvl->thunder = on;
	g->thunder_timer = SecondsToTicks(10.0);
}

void RenderThunder(Game* g) {
	if (g->lvl->thunder) {
		if (g->thunder_timer <  SecondsToTicks(0.1)) {
			DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 255, 255, 255, 1);
		}
	}
}

void HandleThunder(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	if (g->lvl->thunder) {
		if (g->thunder_timer > -1) {
			g->thunder_timer--;
		} else {
			g->thunder_timer = SecondsToTicks(22.2);
		}
		if(g->thunder_timer == SecondsToTicks(0.1)){
			al_play_sample(sample_sfx[4], 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}


void ToggleRain(Game* g, bool start) {
	if (start == false) {
		g->lvl->rain = false;
		for (int i = 0; i < RAIN_DROPS; i++) {
			g->rdrop[i].fall_speed = 0;
			g->rdrop[i].x = -9999;
			g->rdrop[i].y = -9999;
		}
	}
	else {
		g->lvl->rain = true;
		for (int i = 0; i < RAIN_DROPS; i++) {
			g->rdrop[i].x = 0 + rand() % GAME_WIDTH;
			g->rdrop[i].y = 0 + rand() % GAME_HEIGHT;
			g->rdrop[i].fall_speed = 10 + rand() % 20;
		}
	}
}

void RenderRain(Game* g, ALLEGRO_BITMAP** img_sprite) {
	if (g->lvl->rain) {
		for (int i = 0; i < RAIN_DROPS; i++) {
			DrawImage(g, img_sprite[SPRITE_RAIN], g->rdrop[i].x, g->rdrop[i].y, 0);
		}
	}
}

void HandleRain(Game* g) {
	if (g->lvl->rain) {
		for (int i = 0; i < RAIN_DROPS; i++) {
			g->rdrop[i].y += g->rdrop[i].fall_speed;
			g->rdrop[i].x -= 10;
			if (g->rdrop[i].x < 0) g->rdrop[i].x = GAME_WIDTH + 32;
			if (g->rdrop[i].y > GAME_HEIGHT) g->rdrop[i].y = -32;
		}
	}
}

void RenderNight(Game* g, ALLEGRO_BITMAP** img_background) {
	int x = g->camera.x;
	int y = g->camera.y;
	if (g->lvl->night == true) {
		if (g->flash_light == true) {
			DrawImage(g, img_background[28], g->pl.x + 15 + x - 1600, g->pl.y + 15 + y - 900, 0);
		} else {
			DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, 0.9);
			if (g->lvl->fog) {
				DrawImage(g, img_background[29], g->pl.x + 15 + x - 1600, g->pl.y + 15 + y - 900, 0);
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
	HandleRain(g);
	HandleThunder(g, sample_sfx);

	// Clouds (temp)
	g->cloud[0].x += 0.5;
	g->cloud[1].x += 0.5;
	g->cloud[2].x += 1;
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
int RegisterWeatherEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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