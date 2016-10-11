#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "MiniMap.h"

MiniMap::MiniMap() {

}

MiniMap::~MiniMap() {

}

void RenderMiniMapTile(Game* g, Platform* plt, int x, int y, int r1, int g1, int  b1, bool cover, int r2, int g2, int b2) {
	// Draw a rectangle representing the platform in minimap
	DrawRectangle(g,
		70 + g->camera.x / 20 + plt->x / 20 + x,
		35 + g->camera.y / 20 + plt->y / 20 + y,
		1 + plt->w / 20,
		1 + plt->h / 20,
		r1, g1, b1);

	// If tile requires a cover, draw the top layer
	if (cover) {
		DrawRectangle(g,
			70 + g->camera.x / 20 + plt->x / 20 + x,
			35 + g->camera.y / 20 + plt->y / 20 + y,
			1 + plt->w / 20,
			1,
			r2, g2, b2);
	}
}

bool ExceedingMiniMap(Game* g, Platform* plt) {
	// Checks if platform is exceeding minimap
	int x = g->Interfaces[INTERFACE_MAP].x + 14;
	int y = g->Interfaces[INTERFACE_MAP].y + 15;
	if ((70 + g->camera.x / 20 + plt->x / 20 + x <= x - 14) || (35 + g->camera.y / 20 + plt->y / 20 + y <= y - 15)) {
		return true;
	}
	else {
		return false;
	}
}

bool WithinMiniMap(Game* g, Platform* plt) {
	// Checks if platform is within the minimap
	int x = g->Interfaces[INTERFACE_MAP].x + 14;
	int y = g->Interfaces[INTERFACE_MAP].y + 15;
	if (collide(70 + g->camera.x / 20 + plt->x / 20 + x,
		35 + g->camera.y / 20 + plt->y / 20 + y,
		1 + plt->w / 20, 1 + plt->h / 20,
		g->Interfaces[INTERFACE_MAP].x, g->Interfaces[INTERFACE_MAP].y, 222, 124)) {
		return true;
	}
	else {
		return false;
	}
}

Platform AdjustPlatformForMiniMap(Game* g, Platform* plt) {
	// Returns a platform with adjusted x/w values so it doesn't exceed the minimap
	int x = g->Interfaces[INTERFACE_MAP].x + 14;
	int y = g->Interfaces[INTERFACE_MAP].y + 15;
	Platform temp = *plt;

	// x
	int x_adjust = x - (70 + (g->camera.x / 20) + (plt->x / 20) + x);
	if (x_adjust > 0) {
		x_adjust *= 20;
		temp.x += x_adjust;
		temp.w -= x_adjust;
	}

	// y
	int y_adjust = y - (35 + (g->camera.y / 20) + (plt->y / 20) + y);
	if (y_adjust > 0) {
		y_adjust *= 20;
		temp.y += y_adjust;
		temp.h -= y_adjust;
	}

	return temp;
}

void RenderMiniMap(Game* g) {
	if (g->Interfaces[INTERFACE_MAP].visible) {
		// Draw the sky
		if (g->lvl->background_0 == 32) {
			DrawRectangle(g, GAME_WIDTH - 220, GAME_HEIGHT - 122, 218, 120, 60, 40, 20);
		} else {
			if (g->lvl->night) {
				DrawRectangle(g, GAME_WIDTH - 220, GAME_HEIGHT - 122, 218, 120, 0, 30, 60);
			} else {
				DrawRectangle(g, GAME_WIDTH - 220, GAME_HEIGHT - 122, 218, 120, 24, 166, 241);
			}
		}

		// x,y
		int x = g->Interfaces[INTERFACE_MAP].x + 14;
		int y = g->Interfaces[INTERFACE_MAP].y + 15;

		// Go trough each platform and render those who fit inside minimap
		for (int i = 0; i < MAX_PLATFORMS; i++) {
			Platform* plt = &g->lvl->Platform[i];
			if (plt->type != 0) {
				bool render = false;
				if (ExceedingMiniMap(g, plt)) {
					if (WithinMiniMap(g, plt)) {
						plt = &AdjustPlatformForMiniMap(g, plt);
						render = true;
					}
				} else {
					render = true;
				}
				if (render) {
					if (plt->type == PLATFORM_GRASS) {
						RenderMiniMapTile(g, plt, x, y, 100, 70, 30, true, 30, 100, 30);
					}
					if (plt->type == PLATFORM_SAND) {
						RenderMiniMapTile(g, plt, x, y, 120, 90, 50, true, 200, 150, 100);
					}
					if (plt->type == PLATFORM_MIST) {
						RenderMiniMapTile(g, plt, x, y, 130, 130, 130, true, 190, 190, 190);
					}
				}
			}
		}

		// Do the same for foregrounds
		for (int i = 0; i < MAX_FOREGROUNDS; i++) {
			Platform* plt = &g->lvl->Foreground[i];
			if (plt->type != 0) {
				bool render = false;
				if (ExceedingMiniMap(g, plt)) {
					if (WithinMiniMap(g, plt)) {
						plt = &AdjustPlatformForMiniMap(g, plt);
						render = true;
					}
				} else {
					render = true;
				}
				if (render) {
					if (plt->type == LVBG_WATER) {
						RenderMiniMapTile(g, plt, x, y, 150, 200, 220, false);
					}
				}
			}
		}
		if(g->lvl->night && g->lvl->fog && !g->flash_light) DrawRectangle(g, GAME_WIDTH - 220, GAME_HEIGHT - 122, 218, 120, 0, 0, 0);

		// Draw the player
		DrawRectangle(g,
			70 + g->camera.x / 20 + g->pl.x / 20 + x - 2,
			35 + g->camera.y / 20 + g->pl.y / 20 + y - 2,
			5, 5, 0, 0, 255);
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
int RegisterMiniMapEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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