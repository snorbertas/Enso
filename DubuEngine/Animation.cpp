#include "Game.hpp"
#include "Animation.h"
#include "ScaledDraw.hpp"

Animation::Animation() {

}

Animation::~Animation() {

}

void RenderTrails(Game* g, ALLEGRO_BITMAP** img_sprite) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Draw all trail animations
	for (int i = 0; i < 48; i++) {
		DrawRotatedImage(g, img_sprite[g->trail[i].sprite_id], g->trail[i].x + x, g->trail[i].y + y, g->trail[i].angle, 0);
		for (int j = 0; j < MAX_PLAYERS; j++) {
			Player* p = &g->Players[j];
			DrawRotatedImage(g, img_sprite[p->trail[i].sprite_id], p->trail[i].x + x, p->trail[i].y + y, p->trail[i].angle, 0);
		}
	}

	// Render experiences
	for (int i = 0; i < MAX_EXPERIENCE; i++) {
		if (g->exp[i].value > -1) {
			//DrawImage(g, img_sprite[139], g->exp[i].x + x, g->exp[i].y + y, 0);
			DrawRotatedImage(g, img_sprite[SPRITE_ANIM_EXP], g->exp[i].x + (BLOCK_SIZE / 2) + x, g->exp[i].y + (BLOCK_SIZE / 2) + y, g->exp[i].degrees, 0);
		}
	}
}

void RenderAnimations(Game* g, ALLEGRO_BITMAP** img_sprite) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render each animation
	for (int i = 0; i < 1000; i++) {
		GFXAnimation* anim = &g->anim[i];
		if (anim->type > -1) {
			if (anim->relative_to_player) {
				DrawImage(g, img_sprite[anim->type + (anim->frame*anim->animated)], anim->p->x + anim->x + x, anim->p->y + anim->y + y, anim->p->direction);
			}
			else {
				DrawImage(g, img_sprite[anim->type + (anim->frame*anim->animated)], anim->x + x, anim->y + y, anim->flip);
			}
		}
	}

	if (g->flag.visible == true) {
		DrawRotatedImage(g, img_sprite[42], g->flag.x + x, g->flag.y + y, g->flag.angle, 0);
		DrawCircle(g, g->flag.x + x, g->flag.y + y, g->flag.r, 1, 208, 149, 208);
		DrawCircle(g, g->flag.x + x, g->flag.y + y, g->flag.r, 5, 163, 73, 164);
	}

	// Temp
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (g->projectiles[i].type == ABILITY_BASIC_SPELL) {
			DrawImage(g, img_sprite[146], g->projectiles[i].x + x, g->projectiles[i].y + y, g->projectiles[i].dir);
		}
	}
}

void HandleAnimations(Game* g) {
	for (int i = 0; i < MAX_ANIMATIONS; i++) {
		if (g->anim[i].type > -1) {
			g->anim[i].frame++;
			if (g->anim[i].frame > g->anim[i].frames) {
				if (g->anim[i].loop > 0) {
					g->anim[i].frame = 0;
					g->anim[i].loop--;
				}
				else {
					g->anim[i].type = -1;
				}
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
	// Animations
	g->animation_tick++;
	if (g->animation_tick == 5) {
		g->animation_tick = 0;
		HandleAnimations(g);
	}
	if (g->pl.colour == 3 && g->pl.y_vel >= 7) {
		g->pl.colour = -1;
	}
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (g->Players[i].colour == 3 && g->Players[i].y_vel >= 7) {
			g->Players[i].colour = -1;
		}
	}
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
int RegisterAnimationEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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