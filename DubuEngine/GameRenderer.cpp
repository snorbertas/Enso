#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string.h>
#include <iostream>
#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "GameRenderer.hpp"
#include "MiniMap.h"

void RenderGame(Game* g, MoreSprites* csprites, ALLEGRO_FONT** font){
	// Temp fix
	ALLEGRO_BITMAP** img_sprite = csprites->img_sprite;
	ALLEGRO_BITMAP** img_background = csprites->img_background;

	// Render the Backgrounds
	RenderBackgrounds(g, img_background, img_sprite, csprites);

	// Render the thunder weather
	RenderThunder(g);

	// Render bg tiles
	RenderBackgroundPlatforms(g, img_background, img_sprite, csprites);

	// Render Trails
	RenderTrails(g, img_sprite);

	// Render the Player
	RenderCharacter(g, csprites, g->pl.frame, g->pl.x + g->camera.x, g->pl.y + g->camera.y, g->pl.direction,
		g->pl.sprite, g->pl.hair, g->pl.acc, g->pl.outfit,
		g->pl.combat_timer, g->pl.eyes, g->pl.blink);

	// Render other Clients
	RenderPlayers(g, csprites, font);
	
	// Healthbars
	RenderHealthbars(g);

	// NPCs
	RenderNPCs(g, img_sprite, csprites);

	// Items
	RenderItems(g, csprites);

	// Platforms
	RenderPlatforms(g, img_sprite, csprites);

	// Animations
	RenderAnimations(g, img_sprite);

	// Foregrounds
	RenderForegrounds(g, img_sprite, csprites);

	// Rain
	RenderRain(g, img_sprite);

	// Night-time
	RenderNight(g, img_background);

	// Combat Feedback
	RenderCombatFeedback(g, font);

	// Blood
	RenderBlood(g, img_sprite);

	// Minimap
	RenderMiniMap(g);

	// Convo
	RenderConvo(g, img_sprite, font);

	// Editor Events
	RenderEditorEvents(g, font);

	// Editor
	RenderEditor(g, img_sprite, font);
}

void RenderBlood(Game* g, ALLEGRO_BITMAP** img_sprite) {
	if (g->pl.combat_timer >= SecondsToTicks(1.8)) {
		int max_ticks = SecondsToTicks(2.0) - SecondsToTicks(1.8);
		int current_ticks = SecondsToTicks(2.0) - g->pl.combat_timer;
		float opac = 1 - (float)((float)current_ticks / (float)max_ticks);
		std::cout << opac << "\n";
		int bg_width = al_get_bitmap_width(img_sprite[SPRITE_BLOODSCREEN]) - GAME_WIDTH;
		int bg_height = bg_width / 16 * 9;
		DrawScaledImage(g, img_sprite[SPRITE_BLOODSCREEN], 0, 0, -bg_width, -bg_height, 0, opac);
	}
}

void RenderCharacter(Game* g, MoreSprites* csprites, int frame, int x, int y, int flags, int body, int hair, int acc, int outfit, float combat_timer, bool eyes, int blink) { //will complicate it with rgba later (body structure, etc.
	// Offser used for centering special character sprites bigger than the body
	int offset = -72;

	// Calculate Opacity using combat timer
	float opacity = 1;
	if (combat_timer >= SecondsToTicks(1.8)) {
		opacity = 1.0 - (float)(combat_timer - SecondsToTicks(1.8))/SecondsToTicks(0.2);
	}

	// Render the body
	if (body >= 0) {
		DrawImage(g, csprites->img_body[body + frame], x, y, flags, opacity);

		if (eyes) {
			// Eye animation
			int eyes_offset = 0;
			if (frame == 1) eyes_offset += 3;
			if (blink <= 12) {
				DrawImage(g, csprites->img_body[9], x, y + eyes_offset, flags, opacity);
			} else {
				DrawImage(g, csprites->img_body[8], x, y + eyes_offset, flags, opacity);
			}
		}
	}

	// Render the hair
	if (hair >= 0) {
		DrawImage(g, csprites->img_hair[hair + frame], x + offset, y + offset, flags, opacity);
	}

	// Render the accessory
	if (acc >= 0) {
		DrawImage(g, csprites->img_acc[acc + frame], x + offset, y + offset, flags, opacity);
	}

	// Render the outfit
	if (outfit >= 0) {
		DrawImage(g, csprites->img_outfit[outfit + frame], x + offset, y + offset, flags, opacity);
	}
}

void RenderBackgroundPlatforms(Game* g, ALLEGRO_BITMAP** img_background, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites) {
	// Shortened variable names
	Platform* bg = g->lvl->Background;
	int x = g->camera.x;
	int y = g->camera.y;

	// Background platforms
	for (int i = 0; i < MAX_LV_BACKGROUNDS; i++) {
		if (bg[i].type >= 0) {
			if (bg[i].type == LVBG_FENCE) {
				int temp = bg[i].w / BLOCK_SIZE;
				for (int j = 0; j < temp; j++) {
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_FENCE], bg[i].x + (j * BLOCK_SIZE) + x, bg[i].y + y, 0);
				}
			}
			if (bg[i].type == LVBG_WATER) {
				DrawRectangle(g, bg[i].x + x, bg[i].y + y, bg[i].w, bg[i].h, 0, 0, 0, 1);
			}
			if (bg[i].type == LVBG_HOUSE_BEIGE) {
				RenderPlatform(g, img_sprite, csprites, &bg[i],
					SPRITE_TILE_HOUSE_BEIGE_T1, SPRITE_TILE_HOUSE_BEIGE_T2, SPRITE_TILE_HOUSE_BEIGE_T3,
					SPRITE_TILE_HOUSE_BEIGE_L, SPRITE_TILE_HOUSE_BEIGE_R,
					SPRITE_TILE_HOUSE_BEIGE_B1, SPRITE_TILE_HOUSE_BEIGE_B2, SPRITE_TILE_HOUSE_BEIGE_B3,
					224, 209, 175);
			}
			if (bg[i].type == LVBG_HOUSE_DARK) {
				RenderPlatform(g, img_sprite, csprites, &bg[i],
					SPRITE_TILE_HOUSE_DARK_T1, SPRITE_TILE_HOUSE_DARK_T2, SPRITE_TILE_HOUSE_DARK_T3,
					SPRITE_TILE_HOUSE_DARK_L, SPRITE_TILE_HOUSE_DARK_R,
					SPRITE_TILE_HOUSE_DARK_B1, SPRITE_TILE_HOUSE_DARK_B2, SPRITE_TILE_HOUSE_DARK_B3,
					131, 135, 150);
			}
			if (bg[i].type == LVBG_HOUSE_GREY) {
				RenderPlatform(g, img_sprite, csprites, &bg[i],
					SPRITE_TILE_HOUSE_GREY_T1, SPRITE_TILE_HOUSE_GREY_T2, SPRITE_TILE_HOUSE_GREY_T3,
					SPRITE_TILE_HOUSE_GREY_L, SPRITE_TILE_HOUSE_GREY_R,
					SPRITE_TILE_HOUSE_GREY_B1, SPRITE_TILE_HOUSE_GREY_B2, SPRITE_TILE_HOUSE_GREY_B3,
					172, 192, 193);
			}
			if (bg[i].type == LVBG_ROOF_BROWN) {
				RenderPlatform(g, img_sprite, csprites, &bg[i],
					LVBG_ROOF_BROWN, LVBG_ROOF_BROWN + 3);
			}
			if (bg[i].type == LVBG_ROOF_BLUE) {
				RenderPlatform(g, img_sprite, csprites, &bg[i],
					LVBG_ROOF_BLUE, LVBG_ROOF_BLUE + 3);
			}
			if (bg[i].type == LVBG_ROOF_YELLOW) {
				RenderPlatform(g, img_sprite, csprites, &bg[i],
					LVBG_ROOF_YELLOW, LVBG_ROOF_YELLOW + 3);
			}
			if ((bg[i].type >= SPRITE_TILE_HOUSE && bg[i].type <= SPRITE_TILE_ROCK + 9) ||
				(bg[i].type >= SPRITE_TILE_BRICK && bg[i].type <= SPRITE_TILE_BRICK + 5) ||
				(bg[i].type >= SPRITE_TILE_DOOR && bg[i].type <= SPRITE_TILE_MISC + 13)) {
				DrawImage(g, csprites->img_tiles[bg[i].type], bg[i].x + x, bg[i].y - al_get_bitmap_height(csprites->img_tiles[bg[i].type]) + BLOCK_SIZE + y + 1, 0);
				if (g->Editor.edit_mode) {
					DrawRectangle(g, bg[i].x + x, bg[i].y + y, BLOCK_SIZE, BLOCK_SIZE, 0, 50, 0, 0.1);
				}
			}
		}
		if (g->Editor.edit_mode && g->pos_x > 280 && g->Editor.tool_id == 1) {
			// Edit mode block for bg objects
			int hover_x = g->pos_x - g->camera.x;
			int hover_y = g->pos_y - g->camera.y;
			if (collide(hover_x, hover_y, 1, 1, bg[i].x, bg[i].y, bg[i].w, bg[i].h)) {
				DrawOutline(g, bg[i].x + x, bg[i].y + y, bg[i].w, bg[i].h, 255, 255, 255, 2);
			}
		}
	}
}

void RenderBackgrounds(Game* g, ALLEGRO_BITMAP** img_background, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Background offsets
	int x_offset_1 = x * 0.10;
	int x_offset_2 = x * 0.20;
	while (x_offset_1 < -1440) {
		x_offset_1 += 1440;
	}
	while (x_offset_2 < -1440) {
		x_offset_2 += 1440;
	}

	// Calculate bg
	int bg_height = al_get_bitmap_height(img_background[g->lvl->background_0]) - GAME_HEIGHT;
	int bg_width = bg_height * 2;
	// Draw Backgrounds
	DrawScaledImage(g, img_background[g->lvl->background_0], 0, 0, -bg_width, -bg_height, 0);						//Constant
	DrawScaledImage(g, img_background[g->lvl->background_1], 0 + x_offset_1, 0, -bg_width, -bg_height, 0);			//5% distance
	DrawScaledImage(g, img_background[g->lvl->background_1], 0 + 1440 + x_offset_1, 0, -bg_width, -bg_height, 0);	//5% distance
	DrawScaledImage(g, img_background[g->lvl->background_2], 0 + x_offset_2, 0, -bg_width, -bg_height, 0);			//10% distance
	DrawScaledImage(g, img_background[g->lvl->background_2], 0 + 1440 + x_offset_2, 0, -bg_width, -bg_height, 0);	//10% distance

	// Clouds (could make separate function in future)
	for (int i = 0; i < 3; i++) {
		DrawImage(g, img_background[g->cloud[i].type], g->cloud[i].x + x, g->cloud[i].y + y, 0);
	}
}

void RenderPlatform(Game* g, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites, Platform* plt, int t, int m) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Check if big enough platform
	int hblocks = plt->w / BLOCK_SIZE;
	int vblocks = plt->h / BLOCK_SIZE;

	// Draw the top and bottom fill
	for (int i = 0; i < hblocks; i++) {
		DrawImage(g, csprites->img_tiles[t], plt->x + (i * BLOCK_SIZE) + x, plt->y + y, 0);

		// Bottom
		for (int j = 0; j < vblocks; j++) {
			DrawImage(g, csprites->img_tiles[m], plt->x + (i * BLOCK_SIZE ) + x, plt->y + plt->h - (j * BLOCK_SIZE) - BLOCK_SIZE + y, 0);
		}
	}
}

void RenderPlatform(Game* g, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites, Platform* plt, int t1, int t2, int t3, int m1, int m2, int b1, int b2, int b3, int re, int gr, int bl) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Check if big enough platform
	int hblocks = plt->w / BLOCK_SIZE;
	int vblocks = plt->h / BLOCK_SIZE;

	if (hblocks >= 3 && vblocks >= 3) {
		// Draw the plain colour rectangle for platform
		DrawRectangle(g, plt->x + x, plt->y + y, plt->w, plt->h, re, gr, bl);

		// Draw the top and bottom fill
		int temp = (plt->w - BRICK_SIZE) / BRICK_SIZE;
		for (int i = 0; i < temp; i++) {
			DrawImage(g, csprites->img_tiles[t2], plt->x + BRICK_SIZE + (i * BRICK_SIZE) + x, plt->y + y, 0);
			DrawImage(g, csprites->img_tiles[b2], plt->x + BRICK_SIZE + (i * BRICK_SIZE) + x, plt->y + plt->h - BRICK_SIZE + y, 0);
		}

		// Draw the left and right fill
		temp = (plt->h - BRICK_SIZE) / BRICK_SIZE;
		for (int i = 0; i < temp; i++) {
			DrawImage(g, csprites->img_tiles[m1], plt->x + x, plt->y + BRICK_SIZE + (i * BRICK_SIZE) + y, 0);
			DrawImage(g, csprites->img_tiles[m2], plt->x + plt->w - BRICK_SIZE + x, plt->y + BRICK_SIZE + (i * BRICK_SIZE) + y, 0);
		}

		// Draw the corners
		DrawImage(g, csprites->img_tiles[t1], plt->x + x, plt->y + y, 0);
		DrawImage(g, csprites->img_tiles[t3], plt->x + plt->w - BRICK_SIZE + x, plt->y + y, 0);
		DrawImage(g, csprites->img_tiles[b1], plt->x + x, plt->y + plt->h - BRICK_SIZE + y, 0);
		DrawImage(g, csprites->img_tiles[b3], plt->x + plt->w - BRICK_SIZE + x, plt->y + plt->h - BRICK_SIZE + y, 0);
	} else {
		// Red rectangle cuz too small
		DrawRectangle(g, plt->x + x, plt->y + y, plt->w, plt->h, 255, 0, 0);
	}
}

void RenderPlatform(Game* g, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites, Platform* plt, int t1, int t2, int b1, int b2, int re, int gr, int bl) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Draw the plain colour rectangle for platform
	DrawRectangle(g, plt->x + x, plt->y + y, plt->w, plt->h, re, gr, bl);

	// Draw the textures for platform
	int temp = plt->w / (BLOCK_SIZE * 2);
	for (int j = 0; j < temp; j++) {
		if (plt->texture_b) {
			// Bottom textures
			DrawImage(g, csprites->img_tiles[b1], plt->x + ((j * BLOCK_SIZE) * 2) + x, plt->y + plt->h - BLOCK_SIZE + y, 0);
			DrawImage(g, csprites->img_tiles[b2], plt->x + ((j * BLOCK_SIZE) * 2) + BLOCK_SIZE + x, plt->y + plt->h - BLOCK_SIZE + y, 0);
		}
		if (plt->texture_t) {
			// Top textures
			DrawImage(g, csprites->img_tiles[t1], plt->x + ((j * BLOCK_SIZE) * 2) + x, plt->y + y, 0);
			DrawImage(g, csprites->img_tiles[t2], plt->x + ((j * BLOCK_SIZE) * 2) + BLOCK_SIZE + x, plt->y + y, 0);
		}
	}

	// Fill odd number gap
	if ((plt->w / BLOCK_SIZE) % 2 != 0) {
		if (plt->texture_b) DrawImage(g, csprites->img_tiles[b1], plt->x + plt->w - BLOCK_SIZE + x, plt->y + plt->h - BLOCK_SIZE + y, 0);
		if (plt->texture_t) DrawImage(g, csprites->img_tiles[t1], plt->x + plt->w - BLOCK_SIZE + x, plt->y + y, 0);
	}
	temp = plt->h / BLOCK_SIZE;

	// Shading
	for (int j = 0; j < temp; j++) {
		if (plt->texture_l) DrawImage(g, img_sprite[SPRITE_SHADE_L], plt->x + x, plt->y + (j * BLOCK_SIZE) + y, 0);
		if (plt->texture_r) DrawImage(g, img_sprite[SPRITE_SHADE_R], plt->x + plt->w - BLOCK_SIZE + x, plt->y + (j * BLOCK_SIZE) + y, 0);
	}
}

void RenderPlatforms(Game* g, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites) {
	// Shortened variable names
	Platform* plt = g->lvl->Platform;
	int x = g->camera.x;
	int y = g->camera.y;

	// Render all platforms
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if (plt[i].type >= 0) {
			if (plt[i].type == PLATFORM_GRASS) {
				// Grass
				RenderPlatform(g, img_sprite, csprites, &plt[i],
					SPRITE_TILE_GRASS_T1, SPRITE_TILE_GRASS_T2,
					SPRITE_TILE_GRASS_B1, SPRITE_TILE_GRASS_B2,	
					104, 65, 35);
			} else if (plt[i].type == PLATFORM_SAND) {
				// Sand
				RenderPlatform(g, img_sprite, csprites, &plt[i],
					SPRITE_TILE_SAND_T1, SPRITE_TILE_SAND_T2,
					SPRITE_TILE_GRASS_B1, SPRITE_TILE_GRASS_B2,
					104, 65, 35);
			} else if (plt[i].type == PLATFORM_MIST) {
				// Mist
				RenderPlatform(g, img_sprite, csprites, &plt[i],
					SPRITE_TILE_MIST_T1, SPRITE_TILE_MIST_T2,
					SPRITE_TILE_MIST_B1, SPRITE_TILE_MIST_B2,
					166, 168, 171);

			} else if (plt[i].type == PLATFORM_INVIS && g->Editor.edit_mode == true) {
				// Invis
				DrawRectangle(g, plt[i].x + x, plt[i].y + y, plt[i].w, plt[i].h, 0, 0, 0, 0.4);
			}
		}
		// Draw selection outline over the platforms hovered by the cursor
		if (g->Editor.edit_mode && g->pos_x > 280 && g->Editor.tool_id == 1) {
			int hover_x = g->pos_x - g->camera.x;
			int hover_y = g->pos_y - g->camera.y;
			if (collide(hover_x, hover_y, 1, 1, plt[i].x, plt[i].y, plt[i].w, plt[i].h)) {
				DrawOutline(g, plt[i].x + x, plt[i].y + y, plt[i].w, plt[i].h, 255, 255, 255, 2);
			}
		}
	}
}

void RenderForegrounds(Game* g, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites) {
	// Shortened variable names
	Platform* fg = g->lvl->Foreground;
	int x = g->camera.x;
	int y = g->camera.y;

	// Draw all foreground platforms
	for (int i = 0; i < MAX_FOREGROUNDS; i++) {
		if (fg[i].type >= 0) {
			if (fg[i].type == 1) {
				int temp = fg[i].w / BLOCK_SIZE;
				for (int j = 0; j < temp; j++) {
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_FENCE], fg[i].x + (j * BLOCK_SIZE) + x, fg[i].y + y, 0);
				}
			} else if (fg[i].type == 2) {
				DrawRectangle(g, fg[i].x + x, fg[i].y + y, fg[i].w, fg[i].h, 168, 223, 236, 0.1);
				int temp = fg[i].w / (BLOCK_SIZE * 2);
				for (int j = 0; j < temp; j++) {
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_WATER_B1], fg[i].x + ((j * BLOCK_SIZE) * 2) + x, fg[i].y + fg[i].h - BLOCK_SIZE + y, 0);
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_WATER_B2], fg[i].x + ((j * BLOCK_SIZE) * 2) + BLOCK_SIZE + x, fg[i].y + fg[i].h - BLOCK_SIZE + y, 0);
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_WATER_T1], fg[i].x + ((j * BLOCK_SIZE) * 2) + x, fg[i].y + y, 0);
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_WATER_T2], fg[i].x + ((j * BLOCK_SIZE) * 2) + BLOCK_SIZE + x, fg[i].y + y, 0);
				}
				if ((fg[i].w / BLOCK_SIZE) % 2 != 0) {
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_WATER_B1], fg[i].x + fg[i].w - BLOCK_SIZE + x, fg[i].y + fg[i].h - BLOCK_SIZE + y, 0);
					DrawImage(g, csprites->img_tiles[SPRITE_TILE_WATER_T1], fg[i].x + fg[i].w - BLOCK_SIZE + x, fg[i].y + y, 0);
				}
			}
			if (fg[i].type >= 15 && fg[i].type <= 69) {
				if (fg[i].type != 61 && fg[i].type != 63 && fg[i].type != 65) {
					DrawImage(g, csprites->img_tiles[fg[i].type], fg[i].x + x, fg[i].y - al_get_bitmap_height(csprites->img_tiles[fg[i].type]) + BLOCK_SIZE + y + 1, 0);
				} else {
					DrawImage(g, csprites->img_tiles[fg[i].type], fg[i].x + x, fg[i].y - al_get_bitmap_height(csprites->img_tiles[fg[i].type]) + BLOCK_SIZE + 15 + y, 0);
				}
				if (g->Editor.edit_mode) {
					DrawRectangle(g, fg[i].x + x, fg[i].y + y, BLOCK_SIZE, BLOCK_SIZE, 127, 65, 20, 0.1);
				}
			}
		}

		// Draw selection outline over the platforms hovered by the cursor
		if (g->Editor.edit_mode && g->pos_x > 280 && g->Editor.tool_id == 1) {
			int hover_x = g->pos_x - g->camera.x;
			int hover_y = g->pos_y - g->camera.y;
			if (collide(hover_x, hover_y, 1, 1, fg[i].x, fg[i].y, fg[i].w, fg[i].h)) {
				DrawOutline(g, fg[i].x + x, fg[i].y + y, fg[i].w, fg[i].h, 255, 255, 255, 2);
			}
		}
	}
}

void RenderPlayers(Game* g, MoreSprites* csprites, ALLEGRO_FONT** font) {
	// r_x, r_y are the x and y ratios for scaling text sizes
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;

	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render each player and their names
	for (int i = 0; i < MAX_PLAYERS; i++) {
		//Character
		RenderCharacter(g, csprites, g->Players[i].frame,
			g->Players[i].x + x, g->Players[i].y + y,
			g->Players[i].direction,
			g->Players[i].sprite, g->Players[i].hair, g->Players[i].acc, g->Players[i].outfit,
			g->Players[i].combat_timer, g->Players[i].eyes, g->Players[i].blink);
		// RenderCharacter(g, csprites, g->pl.frame,

		//Nametag
		DrawRectangle(g, g->Players[i].x - 25 + x, g->Players[i].y - 25 + y, 100, 20, 0, 0, 0, 0.5);
		DrawText(font[1], 255, 255, 0,
			g->Players[i].x + (g->Players[i].w / 2) + x, g->Players[i].y - 27 + y,
			ALLEGRO_ALIGN_CENTER, "%s (%i)", g->Players[i].name.c_str(), i);
	}
}

void RenderNPCs(Game* g, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render each NPC/Monster
	for (int i = 0; i < MAX_MONSTERS; i++) {
		NPC* npc = &g->monster[i];
		if (npc->type == MONSTER_BUTTERFLY) {
			DrawImage(g, img_sprite[npc->health + npc->frame], npc->x + x, npc->y + y, npc->direction);
		} else if (npc->type >= 0) {
			RenderCharacter(g, csprites, npc->frame, npc->x + x, npc->y + y, npc->direction, npc->sprite, npc->hair, npc->acc, npc->outfit, npc->combat_timer, npc->eyes, npc->blink);
		}
	}
}