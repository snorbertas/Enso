#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "Editor.h"

/*	The editor is intentionally trash due to the main purpose it had:
	to stricly serve ME as a tool to create levels/maps. I don't expect
	anyone to understadn this code, nor anyone to judge it in a positive
	way. */

Editor::Editor() {

}

Editor::~Editor() {

}

void HandleEditor(Game* g) {
	if (g->Editor.edit_mode) {
		int hover_x = g->pos_x - g->camera.x;
		int hover_y = g->pos_y - g->camera.y;
		if (g->Editor.target_drag) {
			Platform* pltfrm = g->Editor.plt;
			if (g->Editor.selection_tool_type == 3) {
				pltfrm = g->Editor.bg;
			}
			if (g->Editor.selection_tool_type == 4) {
				pltfrm = g->Editor.fg;
			}
			if (g->Editor.selection_tool_type == 5) {
				pltfrm = g->Editor.ev;
			}
			if (g->Editor.select_type > -1) { // Dragging platforms
				pltfrm->x = hover_x - g->Editor.offset_x;
				pltfrm->y = hover_y - g->Editor.offset_y;
				float temp_xf = (float)pltfrm->x / BLOCK_SIZE;
				int temp_xi = round(pltfrm->x / BLOCK_SIZE);
				if (temp_xf != (float)temp_xi) {
					// Need to snap
					pltfrm->x = (BLOCK_SIZE * temp_xi);
				}
				float temp_yf = (float)pltfrm->y / BLOCK_SIZE;
				int temp_yi = round(pltfrm->y / BLOCK_SIZE);
				if (temp_yf != (float)temp_yi) {
					// Need to snap
					pltfrm->y = (BLOCK_SIZE * temp_yi);
				}
			}
		}
		if (g->Editor.target_resize_w) {
			Platform* pltfrm = g->Editor.plt;
			if (g->Editor.selection_tool_type == 3) {
				pltfrm = g->Editor.bg;
			}
			if (g->Editor.selection_tool_type == 4) {
				pltfrm = g->Editor.fg;
			}
			if (g->Editor.selection_tool_type == 5) {
				pltfrm = g->Editor.ev;
			}
			if (g->Editor.select_type > -1) { // Width resize
				pltfrm->w = (g->Editor.offset_x - pltfrm->x) + hover_x - g->Editor.offset_x;
				if (pltfrm->w < BLOCK_SIZE) pltfrm->w = BLOCK_SIZE;
				float temp_wf = (float)pltfrm->w / BLOCK_SIZE;
				int temp_wi = round(pltfrm->w / BLOCK_SIZE);
				if (temp_wf != (float)temp_wi) {
					// Need to snap
					pltfrm->w = BLOCK_SIZE * temp_wi + BLOCK_SIZE;
				}
			}
		}
		if (g->Editor.target_resize_h) {
			Platform* pltfrm = g->Editor.plt;
			if (g->Editor.selection_tool_type == 3) {
				pltfrm = g->Editor.bg;
			}
			if (g->Editor.selection_tool_type == 4) {
				pltfrm = g->Editor.fg;
			}
			if (g->Editor.selection_tool_type == 5) {
				pltfrm = g->Editor.ev;
			}
			if (g->Editor.select_type > -1) { // Hight resize
				pltfrm->h = (g->Editor.offset_y - pltfrm->y) + hover_y - g->Editor.offset_y;
				if (pltfrm->h < BLOCK_SIZE) pltfrm->h = BLOCK_SIZE;
				float temp_hf = (float)pltfrm->h / BLOCK_SIZE;
				int temp_hi = round(pltfrm->h / BLOCK_SIZE);
				if (temp_hf != (float)temp_hi) {
					// Need to snap
					pltfrm->h = BLOCK_SIZE * temp_hi + BLOCK_SIZE;
				}
			}
		}
	}
}

void RenderEditorEvents(Game* g, ALLEGRO_FONT** font) {
	// r_x, r_y are the x and y ratios for scaling text sizes
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;

	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render each event platform in editor mode
	if (g->Editor.edit_mode) {
		for (int i = 0; i < MAX_EVENTS; i++) {
			if (g->lvl->Event[i].type != -1) {
				Platform* ev = &g->lvl->Event[i];
				DrawRectangle(g, ev->x + x, ev->y + y, ev->w, ev->h, 100, 0, 0, 0.1);
				DrawOutline(g, ev->x + x - 1, ev->y + y - 1, ev->w + 2, ev->h + 2, 0, 0, 0, 2);
				int mx = ev->x + (ev->w / 2);
				int my = ev->y + (ev->h / 2);
				if (ev->type == EE_PLAYER_SPAWN) {
					DrawText(font[2], 255, 255, 255, mx + x, my + y - 12, ALLEGRO_ALIGN_CENTER, "Player Spawn");
				}
				if (ev->type == EE_SKELETON_SPAWN) {
					DrawText(font[2], 255, 255, 255, mx + x, my + y - 12, ALLEGRO_ALIGN_CENTER, "Skeleton Spawn");
				}
				if (ev->type == EE_BUTTERFLY_SPAWN) {
					DrawText(font[2], 255, 255, 255, mx + x, my + y - 12, ALLEGRO_ALIGN_CENTER, "Butterfly Spawn");
				}
				if (ev->type == EE_EXPERIENCE_SPAWN) {
					DrawText(font[2], 255, 255, 255, mx + x, my + y - 12, ALLEGRO_ALIGN_CENTER, "EXP");
				}
				if (ev->type >= 100) {
					DrawText(font[2], 255, 255, 255, mx + x, my + y - 12, ALLEGRO_ALIGN_CENTER, "Event Type: %i", ev->type);
				}
			}
		}
	}
}

void RenderEditor(Game* g, ALLEGRO_BITMAP** img_sprite, ALLEGRO_FONT** font) {
	/*		WARNING!!! The Editor code is very messy and sketchy.
	*		While writing it I was only thinking of the result, using
	*		desparate methods to fix errors. It is meant to be a
	*		temporary tool to help me develop the content for the game.
	*		Functionality > Presentation */

	// r_x, r_y are the x and y ratios for scaling text sizes
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;

	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render all editor-specifics
	if (g->Editor.edit_mode && g->Editor.select_type > -1 && g->Editor.select_id > -1) {
		if (g->Editor.selection_tool_type >= 2) {
			// Choose correct platform type to point
			Platform* pltfrm = g->Editor.plt;
			if (g->Editor.selection_tool_type == 3) {
				pltfrm = g->Editor.bg;
			}
			if (g->Editor.selection_tool_type == 4) {
				pltfrm = g->Editor.fg;
			}
			if (g->Editor.selection_tool_type == 5) {
				pltfrm = g->Editor.ev;
			}

			// Draw outline around selected platform
			DrawOutline(g, pltfrm->x + x, pltfrm->y + y, pltfrm->w, pltfrm->h, 181, 230, 29, 2);

			// Cursor position relative to map
			int hover_x = g->pos_x - g->camera.x;
			int hover_y = g->pos_y - g->camera.y;

			// Horizontal resize
			if (collide(hover_x, hover_y, 1, 1, pltfrm->x + pltfrm->w - 17, pltfrm->y + pltfrm->h / 2 - 6, 17, 13)) {
				DrawImage(g, img_sprite[SPRITE_BUTTON_E_HOR + 1], pltfrm->x + pltfrm->w - 17 + x, pltfrm->y + pltfrm->h / 2 - 6 + y, 0);
			} else {
				DrawImage(g, img_sprite[SPRITE_BUTTON_E_HOR], pltfrm->x + pltfrm->w - 17 + x, pltfrm->y + pltfrm->h / 2 - 6 + y, 0);
			}

			// Vertical resize
			if (collide(hover_x, hover_y, 1, 1, pltfrm->x + pltfrm->w / 2 - 8, pltfrm->y + pltfrm->h - 17, 13, 17)) {
				DrawImage(g, img_sprite[SPRITE_BUTTON_E_VER + 1], pltfrm->x + pltfrm->w / 2 - 8 + x, pltfrm->y + pltfrm->h - 17 + y, 0);
			} else {
				DrawImage(g, img_sprite[SPRITE_BUTTON_E_VER], pltfrm->x + pltfrm->w / 2 - 8 + x, pltfrm->y + pltfrm->h - 17 + y, 0);
			}
		}
	}

	// Render the grid
	if (g->Editor.edit_mode == true && g->Editor.grid == true) {
		DrawRectangle(g, 801 + x, 0, 3, GAME_HEIGHT, 255, 0, 0, 1);
		DrawRectangle(g, 600 + x, (GAME_HEIGHT / 2), 190, 80, 0, 0, 0, 0.8);
		DrawText(font[2], 255, 0, 0, 695 + x, GAME_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Do not allow the player");
		DrawText(font[2], 255, 0, 0, 695 + x, GAME_HEIGHT / 2 + 25, ALLEGRO_ALIGN_CENTRE, "to walk on the left side");
		DrawText(font[2], 255, 0, 0, 695 + x, GAME_HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTRE, "of this line.");

		DrawRectangle(g, 0, g->lvl->ground_level + y, GAME_WIDTH, 3, 0, 0, 255, 1);
		DrawRectangle(g, (GAME_WIDTH/2), g->lvl->ground_level + y + 15, 120, 30, 0, 0, 0, 0.8);
		DrawText(font[2], 0, 0, 255, (GAME_WIDTH / 2) + 60, g->lvl->ground_level + y + 20, ALLEGRO_ALIGN_CENTRE, "Ground level");

		int start_x = 0;
		int start_y = 0;
		int hover_x = -(BLOCK_SIZE * 2) - g->camera.x;
		int hover_y = -(BLOCK_SIZE * 2) - g->camera.y;
		start_x = (int)(hover_x / BLOCK_SIZE) * BLOCK_SIZE;
		start_y = (int)(hover_y / BLOCK_SIZE) * BLOCK_SIZE;
		for (int i = 0; i < 28; i++) {
			DrawOutline(g, start_x + ((i * 2) * BLOCK_SIZE) + x, start_y + y, BLOCK_SIZE, GAME_HEIGHT + (BLOCK_SIZE * 3), 30, 30, 30, 1, 50);
		}
		for (int i = 0; i < 18; i++) {
			DrawOutline(g, start_x + x, start_y + ((i * 2) * BLOCK_SIZE) + y, GAME_WIDTH + (BLOCK_SIZE * 3), BLOCK_SIZE, 30, 30, 30, 1, 50);
		}
	}
	if (g->Editor.edit_mode && g->Editor.loading) {
		DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, 0.5);
	}
}

/* ============================= Event Functions ==========================
*		Used according to the event triggered. (Do NOT define in header)
*			- Game ticks
*			- Mouse clicks
*			- Keyboard presses
*/
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	HandleEditor(g);
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
int RegisterEditorEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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