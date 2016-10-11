#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "MenuRenderer.hpp"
#include "GameRenderer.hpp"

void RenderMenu(Game* g, MoreSprites* char_sprites, ALLEGRO_FONT** font){
	// Temp fix
	ALLEGRO_BITMAP** img_sprite = char_sprites->img_sprite;
	ALLEGRO_BITMAP** img_background = char_sprites->img_background;

	// Virtual cursor position
	if (g->menu_x > g->pos_x) {
		g->menu_x -= 5;
		if (g->menu_x < g->pos_x) g->menu_x = g->pos_x;
	} else if (g->menu_x < g->pos_x) {
		g->menu_x += 5;
		if (g->menu_x > g->pos_x) g->menu_x = g->pos_x;
	}
	if (g->menu_y > g->pos_y) {
		g->menu_y -= 5;
		if (g->menu_y < g->pos_y) g->menu_y = g->pos_y;
	} else if (g->menu_y < g->pos_y) {
		g->menu_y += 5;
		if (g->menu_y > g->pos_y) g->menu_y = g->pos_y;
	}

	// Local time
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	int hour = timeinfo->tm_hour;
	int minute = timeinfo->tm_min;
	float night_bg_opacity = 1;
	float night_dark_opacity = 0.9;
	if (hour >= 19 && hour < 20) {
		night_bg_opacity = ((float)minute / (float)60);
		night_dark_opacity *= ((float)minute / (float)60);
	} else if (hour >= 6 && hour < 7) {
		night_bg_opacity = 1 - ((float)minute / (float)60);
		night_dark_opacity *= 1 - ((float)minute / (float)60);
	}

	// Backgrounds
	// Calculate bg
	int bg_height = al_get_bitmap_height(img_background[g->lvl->background_0]) - GAME_HEIGHT;
	int bg_width = bg_height * 2;

	DrawScaledImage(g, img_background[4], 0, 0, -bg_width, -bg_height, 0);
	if (hour >= 19 || hour < 7) {
		DrawScaledImage(g, img_background[26], 0, 0, -bg_width, -bg_height, 0, night_bg_opacity);
	}

	// Stars (if necessary, draw everything on black bg to cause darkness, but draw stars with 100% opacity)
	if (hour >= 19 || hour < 7) {
		for (int i = 0; i < MAX_STARS; i++) {
			DrawImage(g, img_sprite[123], g->Stars[i].x - g->menu_x / 10, g->Stars[i].y, 0, night_bg_opacity);
		}
	}

	DrawScaledImage(g, img_background[25], -g->menu_x/5, 0, -bg_width, -bg_height, 0);
	DrawScaledImage(g, img_background[25], 1440 - g->menu_x / 5, 0, -bg_width, -bg_height, 0);

	// Background platforms
	g->camera.x = -(g->menu_x - GAME_WIDTH/3);
	g->camera.y = -(g->menu_y - GAME_HEIGHT/2)/2;
	RenderBackgroundPlatforms(g, img_background, img_sprite, char_sprites);
	RenderPlatforms(g, img_sprite, char_sprites);
	Platform menu_platform_water = CreatePlatform(LVBG_WATER, 160, 544, 416, 416, false);
	RenderPlatform(g, img_sprite, char_sprites, &menu_platform_water,
		SPRITE_TILE_WATER_T1, SPRITE_TILE_WATER_T2,
		SPRITE_TILE_WATER_B1, SPRITE_TILE_WATER_B2,
		168, 223, 236);

	// Change camera for foregrounds
	g->camera.x = -(g->menu_x - GAME_WIDTH / 3)*1.2;
	g->camera.y = -(g->menu_y - GAME_HEIGHT / 2)/1.8;

	RenderForegrounds(g, img_sprite, char_sprites);
	Platform menu_platform = CreatePlatform(PLATFORM_GRASS, -580, 608, 3136, 352*2);
	RenderPlatform(g, img_sprite, char_sprites, &menu_platform,
		SPRITE_TILE_GRASS_T1, SPRITE_TILE_GRASS_T2,
		SPRITE_TILE_GRASS_B1, SPRITE_TILE_GRASS_B2,
		104, 65, 35);

	// 7pm - 8pm
	if (hour >= 19 || hour < 7) {
		// Not day time
		if (hour >= 19 && hour < 20) {
			//Evening
			DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, night_dark_opacity);
		} else if (hour >= 6 && hour < 7) {
			//Morning
			DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, night_dark_opacity);
		} else {
			//Night time
			DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, 0.9);
		}
	}

	// Game Title/Logo
	if (g->intro_a == 0
		&& g->Interfaces[INTERFACE_OPTIONS].visible == false
		&& g->Interfaces[INTERFACE_KEYBOARD].visible == false) {
		DrawImage(g, img_sprite[31], (GAME_WIDTH / 2) - 132, 10, 0);
	}

	// Fade
	if (g->intro_a > 0) al_draw_filled_rectangle(0, 0, g->s_x, g->s_y, al_map_rgba(0, 0, 0, g->intro_a));
}

void RenderLoading(Game* g, ALLEGRO_BITMAP** img_background, ALLEGRO_BITMAP** img_sprite, ALLEGRO_FONT** font){
	// r_x, r_y are the x and y ratios for scaling text sizes
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;

	// Loading text
	DrawText(font[0], 255, 255, 255, GAME_WIDTH - 100, GAME_HEIGHT - 50, ALLEGRO_ALIGN_CENTER, "Loading...");
}