#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Game.hpp"
#include "MenuHandler.hpp"
#include "NPC.h"
#include "Convo.h"

void AddChatMessage(Game* g, const char* parent, int re, int gr, int bl, const char* msg);

void HandleMenu(Game* g, ALLEGRO_SAMPLE** sample_sfx){
	HandleIntro(g);
	HandleMenuBackground(g);

	// Check if game is starting
	if (g->menu.starting == true && g->fade_out == 1) {
		g->Interfaces[INTERFACE_SLOTS].visible = true;
		g->Interfaces[INTERFACE_PLAYER].visible = true;
		g->Interfaces[INTERFACE_MAP].visible = true;

		g->lvl = LoadLevel("maps/Tutorial/T01.dem");

		if (g->lvl->rain) ToggleRain(g, true);

		g->scene = 1;
		Fade(g, 1);
		AddChatMessage(g, "__SYSTEM__", 0, 0, 0, "Welcome to Enso.");
		bool found_spawn = false;
		for (int j = 0; j < MAX_EVENTS; j++) {
			if (g->lvl->Event[j].type == EE_PLAYER_SPAWN) {
				g->pl.x = g->lvl->Event[j].x + rand() % g->lvl->Event[j].w;
				g->pl.y = g->lvl->Event[j].y + rand() % g->lvl->Event[j].h;
				j = MAX_EVENTS;
				found_spawn = true;
			}
		}
		if (!found_spawn) {
			AddChatMessage(g, "__SYSTEM__", 255, 0, 0, "Warning! Map doesn't have EE_PLAYER_SPAWN!");
			g->pl.x = 0;
			g->pl.y = -500;
		}
		// NPC Spawns
		int npc_id = 0;
		for (int j = 0; j < MAX_EVENTS; j++) {
			if (g->lvl->Event[j].type == EE_SKELETON_SPAWN) {
				Platform* spawn = &g->lvl->Event[j];

				// Calculate number of skeletons for the spawn zone
				int blocks = spawn->w / BLOCK_SIZE;
				int skeletons = 1 + (blocks / 15);

				for (int i = npc_id; i < npc_id + skeletons; i++) {
					if (i < MAX_MONSTERS) {
						g->monster[i].type = MONSTER_SKELETON;
						g->monster[i].x = spawn->x + rand() % spawn->w;
						g->monster[i].y = spawn->y + rand() % spawn->h;
						g->monster[i].direction = rand() % 2;
						g->monster[i].animation = rand() % 15;
					} else {
						std::cout << "Can't spawn any more NPCs, map full\n";
					}
				}
				npc_id += skeletons;
			}
			if (g->lvl->Event[j].type == EE_BUTTERFLY_SPAWN) {
				Platform* spawn = &g->lvl->Event[j];

				// Calculate number of butterflies for the spawn zone
				int blocks = spawn->w / BLOCK_SIZE;
				int butterflies = 1 + (blocks / 15);

				for (int i = npc_id; i < npc_id + butterflies; i++) {
					if (i < MAX_MONSTERS) {
						g->monster[i].type = MONSTER_BUTTERFLY;
						g->monster[i].x = spawn->x + rand() % spawn->w;
						g->monster[i].y = spawn->y + rand() % spawn->h;
						g->monster[i].direction = rand() % 2;
						g->monster[i].animation = rand() % 15;
						g->monster[i].frame = rand() % 3;
						g->monster[i].health = 99 + (rand() % 5) * 3;
						g->monster[i].zone = g->lvl->Event[j];
					} else {
						std::cout << "Can't spawn any more NPCs, map full\n";
					}
				}
				npc_id += butterflies;
			}
		}

		g->Interfaces[INTERFACE_CHAT].visible = true;


		// Start tutorial
		StartCutScene(g);
		g->story.missions[0].prog = 1;
		ShowStory(g, "Chapter 1:", "I Know That I Forgot", 5, true, 101);
		SetCamera(g, 860 - 3000, -1500 - 3000, CAMERA_MODE_CUTSCENE);
		MoveCamera(g, 1820, -540);
	}

	// Check if we're quitting game and we fully faded-out
	if (g->menu.quitting == true && g->fade_out == 1) {
		g->done = true;
	}
}

void HandleIntro(Game* g) {
	if (g->intro_a > 0) g->intro_a -= 0.6;
	if (g->intro_a < 0) g->intro_a = 0;
	if (g->intro_a == 0 && g->Buttons[0].opacity != 1) {
		float opacity = g->Buttons[0].opacity;
		opacity += 0.01;
		if (opacity >= 1) opacity = 1;
		g->Buttons[0].opacity = opacity;
		g->Buttons[241].opacity = opacity;
		g->Buttons[1].opacity = opacity;
		g->Buttons[242].opacity = opacity;
		g->Buttons[2].opacity = opacity;

		g->Buttons[0].y += 1;
		g->Buttons[241].y += 1;
		g->Buttons[1].y += 1;
		g->Buttons[242].y += 1;
		g->Buttons[2].y += 1;
	}
}

void HandleMenuBackground(Game* g) {
	g->menu.bg_x = (-1500 + GAME_WIDTH) + (1500 - GAME_WIDTH) * (float)((float)(GAME_WIDTH - g->pos_x) / (float)GAME_WIDTH);
	g->menu.bg_y = (-938 + GAME_HEIGHT) + (938 - GAME_HEIGHT) * (float)((float)(GAME_HEIGHT - g->pos_y) / (float)GAME_HEIGHT);
}