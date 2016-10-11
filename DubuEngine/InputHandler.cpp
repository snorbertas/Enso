#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <fstream> 
#include <string.h>
#include <process.h>
#include <allegro5/allegro_native_dialog.h>
#include <Windows.h>
#include <sstream>
#include "Game.hpp"
#include "GameHandler.hpp"
#include "Interfaces.hpp"
#include "InputHandler.hpp"
#include "DEText.h"
using namespace std;

void LeftClick(Game* g, bool release, ALLEGRO_SAMPLE** sample_sfx){
	if(!release){
		// Login Interface
		if (g->Interfaces[INTERFACE_LOGIN].visible == true) {
			if (MouseIsOn(g, g->Interfaces[INTERFACE_LOGIN].x + 106, g->Interfaces[INTERFACE_LOGIN].y + 16, 258, 32)) {
				g->logini.type_username = true;
			} else if (MouseIsOn(g, g->Interfaces[INTERFACE_LOGIN].x + 106, g->Interfaces[INTERFACE_LOGIN].y + 58, 258, 32)) {
				g->logini.type_username = false;
			}
		}

		// Inventory Interface
		if (g->Interfaces[INTERFACE_INVENTORY].visible == true) {
			int slot = 0;
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 5; k++) {
					if (g->Inventory[slot].id != -1) {
						if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x + 14 + (k * 42), g->Interfaces[INTERFACE_INVENTORY].y + 44 + (j * 43), 34, 34)) {
							g->DraggingItem = g->Inventory[slot];
							g->drag_item_slot = slot;
							g->Inventory[slot].id = -1;
						}
					}
					slot++;
				}
			}
			for (int j = 0; j < 3; j++) {
				if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x + 35 + (j * 64), g->Interfaces[INTERFACE_INVENTORY].y + 248, 34, 34)) {
					g->DraggingItem = g->Inventory[slot];
					g->drag_item_slot = slot;
					g->Inventory[slot].id = -1;
				}
				slot++;
			}
		}

		// Abilities Interface
		if (g->Interfaces[INTERFACE_ABILITIES].visible == true) {
			if(MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 14, g->Interfaces[INTERFACE_ABILITIES].y + 44, 34, 34)){
				SelectBook(g, 0);
			}
			if (MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 57, g->Interfaces[INTERFACE_ABILITIES].y + 44, 34, 34)) {
				SelectBook(g, 1);
			}
			if (MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 100, g->Interfaces[INTERFACE_ABILITIES].y + 44, 34, 34)) {
				SelectBook(g, 2);
			}
			if (MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 143, g->Interfaces[INTERFACE_ABILITIES].y + 44, 34, 34)) {
				SelectBook(g, 3);
			}
			if (MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 185, g->Interfaces[INTERFACE_ABILITIES].y + 44, 34, 34)) {
				SelectBook(g, 4);
			}
			// Drag abilities
			int active_book = GetActiveBookID(g);
			int slot = 0;
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 5; k++) {
					if(MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 15 + (k * 42), g->Interfaces[INTERFACE_ABILITIES].y + 98 + (j * 43), 34, 34)){
						Ability ability = GetAbility(active_book, slot);
						if (ability.id != -1) {
							g->DraggingAbility = ability;
						}
					}
					slot++;
				}
			}

			// Dragging window
			if (MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x, g->Interfaces[INTERFACE_ABILITIES].y, 231, 15)) {
				g->InterfaceDrag[0] = true;
				if(MouseIsOn(g, g->Interfaces[INTERFACE_ABILITIES].x + 205, g->Interfaces[INTERFACE_ABILITIES].y, 26, 15)){
					g->InterfaceDrag[0] = false;
					g->Interfaces[INTERFACE_ABILITIES].visible = false;
				}
			}
		}

		// Inventory Interface
		if (g->Interfaces[INTERFACE_INVENTORY].visible == true) {
			// Dagging window
			if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x, g->Interfaces[INTERFACE_INVENTORY].y, 231, 15)) {
				g->InterfaceDrag[1] = true;
				if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x + 205, g->Interfaces[INTERFACE_INVENTORY].y, 26, 15)) {
					g->InterfaceDrag[1] = false;
					g->Interfaces[INTERFACE_INVENTORY].visible = false;
				}
			}
		}

		// Dragging abilities from slots
		for (int j = 0; j < 8; j++) {
			if (MouseIsOn(g, g->Interfaces[INTERFACE_SLOTS].x + 5 + (j * 39), g->Interfaces[INTERFACE_SLOTS].y + 5, 34, 34)) {
				g->DraggingAbility = g->AbilitySlots[j].ability;
				g->AbilitySlots[j].ability.id = -1;
			}
		}

		if (g->Editor.edit_mode && g->scene == 1) {
			int hover_x = g->pos_x - g->camera.x;
			int hover_y = g->pos_y - g->camera.y;
			if (g->pos_x > 280 && g->Editor.tool_id == 1 && g->Editor.select_type > -1 && g->Editor.select_id > -1 && g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == false) { //select tool clicks
				RememberEditorState(g);
				Platform* plt = NULL;
				bool memes = false;
				if (g->Editor.selection_tool_type == 2) {
					plt = g->Editor.plt;
					memes = true;
				}
				if (g->Editor.selection_tool_type == 3) {
					plt = g->Editor.bg;
					memes = true;
				}
				if (g->Editor.selection_tool_type == 4) {
					plt = g->Editor.fg;
					memes = true;
				}
				if (g->Editor.selection_tool_type == 5) {
					plt = g->Editor.ev;
					memes = true;
				}
				if (memes == true) {
					if (collide(hover_x, hover_y, 1, 1, plt->x + plt->w - 17, plt->y + plt->h / 2 - 6, 17, 13)) { // Width resize
						g->Editor.target_resize_w = true;
						g->Editor.offset_x = hover_x - plt->x;
						g->Editor.offset_y = hover_y - plt->y;
					} else if (collide(hover_x, hover_y, 1, 1, plt->x + plt->w / 2 - 8, plt->y + plt->h - 17, 13, 17)) { // Height resize
						g->Editor.target_resize_h = true;
						g->Editor.offset_x = hover_x - plt->x;
						g->Editor.offset_y = hover_y - plt->y;
					} else if (collide(hover_x, hover_y, 1, 1, plt->x, plt->y, plt->w, plt->h)) { // Drag
						g->Editor.target_drag = true;
						g->Editor.offset_x = hover_x - plt->x;
						g->Editor.offset_y = hover_y - plt->y;
					}
				}
			} else if (g->pos_x > 280 && g->Editor.tool_id >= 2 && g->Editor.select_type > -1 && g->Editor.select_id > -1 && g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == false) { //platform tool clicks
				Platform* plt = g->Editor.plt;
				g->Editor.selection_tool_type = 2;
				if (g->Editor.tool_id == 3) {
					plt = g->Editor.bg;
					g->Editor.selection_tool_type = 3;
				}
				if (g->Editor.tool_id == 4) {
					plt = g->Editor.fg;
					g->Editor.selection_tool_type = 4;
				}
				if (g->Editor.tool_id == 5) {
					plt = g->Editor.ev;
					g->Editor.selection_tool_type = 5;
				}
				g->Editor.tool_id = 1;
				plt->x = hover_x;
				plt->y = hover_y;
				float temp_xf = (float)plt->x / BLOCK_SIZE;
				int temp_xi = plt->x / BLOCK_SIZE;
				if (temp_xf != (float)temp_xi) {
					// Need to snap
					plt->x = BLOCK_SIZE * temp_xi;
				}
				float temp_yf = (float)plt->y / BLOCK_SIZE;
				int temp_yi = plt->y / BLOCK_SIZE;
				if (temp_yf != (float)temp_yi) {
					// Need to snap
					plt->y = (BLOCK_SIZE * temp_yi) - BLOCK_SIZE;
				}
				plt->w = BLOCK_SIZE;
				plt->h = BLOCK_SIZE;
				g->Editor.target_resize_w = true;
				g->Editor.target_resize_h = true;
				g->Editor.offset_x = hover_x - plt->x;
				g->Editor.offset_y = hover_y - plt->y;
			}
			if (g->Editor.selection_tool_type == 3 && g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == false) {
				if (g->Editor.bg->type >= SPRITE_TILE_HOUSE && g->Editor.bg->type <= 69 ||
					g->Editor.bg->type >= SPRITE_TILE_BRICK && g->Editor.bg->type <= 99 ||
					g->Editor.bg->type >= SPRITE_TILE_DOOR && g->Editor.bg->type <= 147) {
					if (g->Editor.target_resize_h || g->Editor.target_resize_w) {
						g->Editor.target_resize_w = false;
						g->Editor.target_resize_h = false;
						g->Editor.target_drag = true;
					}
				}
			}
			if (g->Editor.selection_tool_type == 4 && g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == false) {
				if (g->Editor.fg->type >= 15 && g->Editor.fg->type <= 69) {
					if (g->Editor.target_resize_h || g->Editor.target_resize_w) {
						g->Editor.target_resize_w = false;
						g->Editor.target_resize_h = false;
						g->Editor.target_drag = true;
					}
				}
			}
			if (g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == true) {
				if (g->Editor.debug_type == 1) {
					for (int r = 0; r < 28; r++) {
						// Dimensions
						int x_offset = 40;
						int y_offset = 42;
						int columns = 11;
						int rows = 1000;
						int table_width = GAME_WIDTH - x_offset * 2;
						int table_height = GAME_HEIGHT - y_offset * 2;
						int column_width = table_width / columns;
						int row_height = 22;
						int id = (r + g->Editor.debug_scroll);
						// Action
						if (MouseIsOn(g, x_offset + (column_width * 10), y_offset + row_height + (r * row_height), column_width, row_height)) {
							g->lvl->Platform[id] = Platform();
						}
					}
				}
				if (g->Editor.debug_type == 2) {
					for (int r = 0; r < 28; r++) {
						// Dimensions
						int x_offset = 40;
						int y_offset = 42;
						int columns = 11;
						int rows = 1000;
						int table_width = GAME_WIDTH - x_offset * 2;
						int table_height = GAME_HEIGHT - y_offset * 2;
						int column_width = table_width / columns;
						int row_height = 22;
						int id = (r + g->Editor.debug_scroll);
						// Action
						if (MouseIsOn(g, x_offset + (column_width * 10), y_offset + row_height + (r * row_height), column_width, row_height)) {
							g->lvl->Background[id] = Platform();
						}
					}
				}
				if (g->Editor.debug_type == 3) {
					for (int r = 0; r < 28; r++) {
						// Dimensions
						int x_offset = 40;
						int y_offset = 42;
						int columns = 11;
						int rows = 1000;
						int table_width = GAME_WIDTH - x_offset * 2;
						int table_height = GAME_HEIGHT - y_offset * 2;
						int column_width = table_width / columns;
						int row_height = 22;
						int id = (r + g->Editor.debug_scroll);
						// Action
						if (MouseIsOn(g, x_offset + (column_width * 10), y_offset + row_height + (r * row_height), column_width, row_height)) {
							g->lvl->Foreground[id] = Platform();
						}
					}
				}
				if (g->Editor.debug_type == 4) {
					for (int r = 0; r < 28; r++) {
						// Dimensions
						int x_offset = 40;
						int y_offset = 42;
						int columns = 11;
						int rows = 100;
						int table_width = GAME_WIDTH - x_offset * 2;
						int table_height = GAME_HEIGHT - y_offset * 2;
						int column_width = table_width / columns;
						int row_height = 22;
						int id = (r + g->Editor.debug_scroll);
						// Action
						if (MouseIsOn(g, x_offset + (column_width * 10), y_offset + row_height + (r * row_height), column_width, row_height)) {
							g->lvl->Event[id] = Platform();
						}
					}
				}
			}
		}
	} else {
		// Item drag release
		if (g->Interfaces[INTERFACE_INVENTORY].visible == true && g->DraggingItem.id != -1) {
			int slot = 0;
			bool snapped = false;
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 5; k++) {
					if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x + 14 + (k * 42), g->Interfaces[INTERFACE_INVENTORY].y + 44 + (j * 43), 34, 34)) {
						if (g->Inventory[slot].id != -1) {
							g->Inventory[g->drag_item_slot] = g->Inventory[slot];
						}
						g->Inventory[slot] = g->DraggingItem;
						g->DraggingItem.id = -1;
						snapped = true;
					}
					slot++;
				}
			}

			for (int j = 0; j < 3; j++) {
				if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x + 35 + (j *64), g->Interfaces[INTERFACE_INVENTORY].y + 248, 34, 34)) {
					if (g->Inventory[slot].id != -1) {
						g->Inventory[g->drag_item_slot] = g->Inventory[slot];
					}
					g->Inventory[slot] = g->DraggingItem;
					g->DraggingItem.id = -1;
					snapped = true;
				}
				slot++;
			}

			if (snapped == false) {
				if (MouseIsOn(g, g->Interfaces[INTERFACE_INVENTORY].x, g->Interfaces[INTERFACE_INVENTORY].y, 231, 300)){
					g->Inventory[g->drag_item_slot] = g->DraggingItem;
					g->DraggingItem.id = -1;
				} else {
					int offset = -40;
					if (g->pl.direction == 0) offset = 40 + g->pl.w;
					DropItem(g, g->DraggingItem, g->pl.x + offset, g->pl.y - 30);
					g->DraggingItem.id = -1;
				}
			}
		}

		// Ability drag release
		if (g->DraggingAbility.id != -1) {
			for (int j = 0; j < 8; j++) {
				if(MouseIsOn(g, g->Interfaces[INTERFACE_SLOTS].x + 5 + (j * 39), g->Interfaces[INTERFACE_SLOTS].y + 5, 34, 34)){
					for (int k = 0; k < 8; k++) {
						if (k != j) {
							if (g->AbilitySlots[k].ability.id == g->DraggingAbility.id) {
								g->AbilitySlots[k].ability.id = -1;
							}
						}
					}
					g->AbilitySlots[j].ability = g->DraggingAbility;
				}
			}
			g->DraggingAbility.id = -1;
		}

		for (int i = 0; i < 10; i++) {
			g->InterfaceDrag[i] = false;
		}
		bool done = false;
		if (g->Editor.edit_mode && g->Editor.target_drag) {
			g->Editor.target_drag = false;
		}
		if (g->Editor.edit_mode && g->Editor.target_resize_w) {
			g->Editor.target_resize_w = false;
		}
		if (g->Editor.edit_mode && g->Editor.target_resize_h) {
			g->Editor.target_resize_h = false;
		}
		for(int i = 0; i < MAX_INTERFACES; i++){
			if(g->Interfaces[i].visible == true && done == false){
				for(int j = 0; j < MAX_BUTTONS; j++){
					if(g->Buttons[j].interface_id == i && InterfaceIsOnTop(g, i)){
						if(g->pos_x > g->Buttons[j].x+g->Interfaces[i].x &&
						g->pos_x < g->Buttons[j].x+g->Buttons[j].w+g->Interfaces[i].x &&
						g->pos_y > g->Buttons[j].y+g->Interfaces[i].y &&
						g->pos_y < g->Buttons[j].y+g->Buttons[j].h+g->Interfaces[i].y &&
						g->Buttons[j].visible){
							std::cout << "Clicked Button: " << j << "\n";
							switch(j){
								case 0: // New Game (Main Menu)
									Fade(g, 0);
									g->menu.starting = true;
									NewGame(g);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									done = true;
									break;
								case 1: // Options (Main Menu)
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									g->Interfaces[INTERFACE_OPTIONS].visible = true;
									done = true;
									break;
								case 2: // Quit (Main Menu)
									Fade(g, 0);
									g->menu.quitting = true;
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									done = true;
									break;
								case 3: // New Game (In-game)
									NewGame(g);
									g->Interfaces[1].visible = false;
									g->Interfaces[2].visible = true;
									done = true;
									break;
								case 4: // Quit (In-game)
									g->scene = 0;
									HideAllInterfaces(g, -1);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									done = true;
									break;
								case 5: // Resolution drop-down
									if(!g->Interfaces[4].visible && !g->Interfaces[5].visible){
										g->Interfaces[4].visible = true;
										g->Interfaces[5].visible = false;
										done = true;
									}
									break;		
								case 6: // Window mode drop-down
									if(!g->Interfaces[4].visible && !g->Interfaces[5].visible){
										g->Interfaces[5].visible = true;
										g->Interfaces[4].visible = false;
										done = true;
									}
									break;
								case 8: // 1024x576
									g->Interfaces[4].visible = false;
									g->s_x = 1024;
									g->s_y = 576;
									g->BWIDTH = 1280;
									g->BHEIGHT = 720;
									g->remake_display = true;
									done = true;
									break;
								case 9: // 1280x720
									g->Interfaces[4].visible = false;
									g->s_x = 1280;
									g->s_y = 720;
									g->BWIDTH = 1280;
									g->BHEIGHT = 720;
									g->remake_display = true;
									done = true;
									break;
								case 10: // 1600x900
									g->Interfaces[4].visible = false;
									g->s_x = 1600;
									g->s_y = 900;
									g->BWIDTH = 1600;
									g->BHEIGHT = 900;
									g->remake_display = true;
									done = true;
									break;
								case 11: // 1920x1080
									g->s_x = 1920;
									g->s_y = 1080;
									g->BWIDTH = 1920;
									g->BHEIGHT = 1080;
									g->remake_display = true;
									g->Interfaces[4].visible = false;
									done = true;
									break;
								case 12:
									g->window_mode = ALLEGRO_WINDOWED;
									g->remake_display = true;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 13:
									g->window_mode = ALLEGRO_FULLSCREEN;
									g->remake_display = true;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 14:
									g->window_mode = ALLEGRO_FULLSCREEN_WINDOW;
									g->remake_display = true;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 15: // Okay
									g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									g->Interfaces[INTERFACE_OPTIONS].visible = false;
									g->Interfaces[4].visible = false;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 16: // Okay (Message Interface)
									if (g->Editor.edit_mode) {
										if (!g->Editor.loading) {
											g->Interfaces[INTERFACE_MESSAGE].visible = false;
											g->Buttons[16].sprite_id = 27;
											g->binding_key = NULL;
										}
									} else {
										g->Interfaces[INTERFACE_MESSAGE].visible = false;
										g->Buttons[16].sprite_id = 27;
										g->binding_key = NULL;
									}
									done = true;
									break;
								case 17: // New (Editor)
									g->Editor.bg = new Platform;
									g->Editor.fg = new Platform;
									g->Editor.plt = new Platform;
									g->lvl = new Level;
									g->pl.x = 96;
									g->pl.y = 0;
									g->lvl->Platform[0].x = 96;
									g->lvl->Platform[0].y = 96;
									g->lvl->Platform[0].w = BLOCK_SIZE * 2;
									g->lvl->Platform[0].h = BLOCK_SIZE;
									g->lvl->Platform[0].type = 1;
									done = true;
									g->last_save_loc = "";
									break;
								case 18: { // Load (Editor)
									ShowMessage(g, "(Browsing)", "");
									g->Editor.loading = true;
									g->Editor.saving = false;
									_beginthreadex(0, 0, BrowseFile, g, 0, 0);
									done = true;
									break;
								}
								case 19: { // Save (Editor)
									ShowMessage(g, "(Browsing)", "");
									g->Editor.loading = true;
									g->Editor.saving = true;
									_beginthreadex(0, 0, BrowseFile, g, 0, 0);
									done = true;
									break;
								}
								case 20:
									g->Editor.tool_id = 1;
									done = true;
									break;
								case 21:
									g->Interfaces[81].visible = true;
									done = true;
									break;
								case 22:
									g->Interfaces[82].visible = true;
									done = true;
									break;
								case 23:
									g->Interfaces[83].visible = true;
									done = true;
									break;
								case 186:
									g->Interfaces[84].visible = true;
									done = true;
									break;
								case 187:
									g->Interfaces[84].visible = false;
									done = true;
									break;
								case 24:
									g->Interfaces[81].visible = false;
									done = true;
									break;
								case 25: // Dirt platform
									ChoosePlatform(g, 1);
									done = true;
									break;
								case 26: // Sand platform
									ChoosePlatform(g, 2);
									done = true;
									break;
								case 27: // Mist platform
									ChoosePlatform(g, 3);
									done = true;
									break;
								case 28: // Invisible platform
									ChoosePlatform(g, 4);
									done = true;
									break;
								case 79: // Fence background
									ChooseBackground(g, 1);
									done = true;
									break;
								case 80: // Water background
									ChooseBackground(g, 2);
									done = true;
									break;
								case 81: // House background
									ChooseBackground(g, SPRITE_TILE_HOUSE + rand() % 6);
									done = true;
									break;
								case 82: // Tree background
									ChooseBackground(g, SPRITE_TILE_TREE + rand() % 32);
									done = true;
									break;
								case 83: // Grass background
									ChooseBackground(g, SPRITE_TILE_GRASS + rand() % 6);
									done = true;
									break;
								case 84: // Rock background
									ChooseBackground(g, SPRITE_TILE_ROCK + rand() % 10);
									done = true;
									break;
								case 85: // Wall background (for building houses)
									ChooseBackground(g, SPRITE_TILE_HOUSE_BEIGE_T1);
									done = true;
									break;
								case 86: // Wall bricks (for building houses)
									ChooseBackground(g, SPRITE_TILE_BRICK);
									done = true;
									break;
								case 87: // Roofs (for building houses)
									ChooseBackground(g, SPRITE_TILE_ROOF);
									done = true;
									break;
								case 88: // Doors (for building houses)
									ChooseBackground(g, SPRITE_TILE_DOOR);
									done = true;
									break;
								case 89: // Windows (for building houses)
									ChooseBackground(g, SPRITE_TILE_WINDOW);
									done = true;
									break;
								case 90: // Misc (for building houses)
									ChooseBackground(g, SPRITE_TILE_MISC);
									done = true;
									break;
								case 133: // Player spawn
									ChooseEvent(g, EE_PLAYER_SPAWN);
									done = true;
									break;
								case 134: // Skeleton spawn
									ChooseEvent(g, EE_SKELETON_SPAWN);
									done = true;
									break;
								case 135: // Butterfly spawn
									ChooseEvent(g, EE_BUTTERFLY_SPAWN);
									done = true;
									break;
								case 136: // Slime spawn
									ChooseEvent(g, EE_SLIME_SPAWN);
									done = true;
									break;
								case 137: // Exp spawn
									ChooseEvent(g, EE_EXPERIENCE_SPAWN);
									done = true;
									break;
								case 138: // Bat spawn
									ChooseEvent(g, EE_BAT_SPAWN);
									done = true;
									break;
								case 188: // Fence foreground
									ChooseForeground(g, 1);
									done = true;
									break;
								case 189: // Water foreground
									ChooseForeground(g, 2);
									done = true;
									break;
								case 190: // House background
									ChooseForeground(g, 15 + rand() % 6);
									done = true;
									break;
								case 191: // Tree background
									ChooseForeground(g, 21 + rand() % 32);
									done = true;
									break;
								case 192: // Grass background
									ChooseForeground(g, 53 + rand() % 6);
									done = true;
									break;
								case 193: // Rock background
									ChooseForeground(g, 60 + rand() % 10);
									done = true;
									break;
								case 78:
									g->Interfaces[82].visible = false;
									done = true;
									break;
								case 132:
									g->Interfaces[83].visible = false;
									done = true;
									break;
								case 241:
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									g->Interfaces[INTERFACE_LOGIN].visible = true;
									done = true;
									break;
								case 243:
									g->Interfaces[INTERFACE_INVENTORY].visible = !g->Interfaces[INTERFACE_INVENTORY].visible;
									done = true;
									break;
								case 245:
									g->Interfaces[INTERFACE_ABILITIES].visible = !g->Interfaces[INTERFACE_ABILITIES].visible;
									done = true;
									break;
								case 247:
									if (g->logini.username_input != "" && g->logini.password_input != "") {
										g->logini.logging_in = true;
										g->Buttons[247].visible = false;
										g->logini.try_login = true;
									}
									done = true;
									break;
								case 248:
									HideAllInterfaces(g, 3);
									g->Interfaces[INTERFACE_OPTIONS].visible = true;
									done = true;
									break;
								case 249:
									HideAllInterfaces(g, 9);
									g->Interfaces[INTERFACE_KEYBOARD].visible = true;
									done = true;
									break;
								case 250:
									RequestBindKeyInput(g, &g->AbilitySlots[0].key);
									done = true;
									break;
								case 251:
									RequestBindKeyInput(g, &g->AbilitySlots[1].key);
									done = true;
									break;
								case 252:
									RequestBindKeyInput(g, &g->AbilitySlots[2].key);
									done = true;
									break;
								case 253:
									RequestBindKeyInput(g, &g->AbilitySlots[3].key);
									done = true;
									break;
								case 254:
									RequestBindKeyInput(g, &g->AbilitySlots[4].key);
									done = true;
									break;
								case 255:
									RequestBindKeyInput(g, &g->AbilitySlots[5].key);
									done = true;
									break;
								case 256:
									RequestBindKeyInput(g, &g->AbilitySlots[6].key);
									done = true;
									break;
								case 257:
									RequestBindKeyInput(g, &g->AbilitySlots[7].key);
									done = true;
									break;
								case 258:
									RequestBindKeyInput(g, &g->keys.left_bind);
									done = true;
									break;
								case 259:
									RequestBindKeyInput(g, &g->keys.right_bind);
									done = true;
									break;
								case 260:
									RequestBindKeyInput(g, &g->keys.jump_bind);
									done = true;
									break;
								case 261:
									RequestBindKeyInput(g, &g->keys.crouch_bind);
									done = true;
									break;
								case 262:
									HideAllInterfaces(g, INTERFACE_MAP);
									g->Interfaces[INTERFACE_CHAT].visible = true;
									g->Interfaces[INTERFACE_SLOTS].visible = true;
									g->Interfaces[INTERFACE_PLAYER].visible = true;
									g->Interfaces[INTERFACE_MAP].visible = true;
									g->scene = 1;
									done = true;
									break;
								case 263:
									g->Interfaces[INTERFACE_PAUSE].visible = false;
									g->Interfaces[INTERFACE_OPTIONS].visible = true;
									done = true;
									break;
								case 264:
									g->scene = 0;
									HideAllInterfaces(g, 0);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									g->Interfaces[6].visible = true;
									done = true;
									break;
								case 265:
									RequestBindKeyInput(g, &g->keys.camera_bind);
									done = true;
									break;
								case 266:
									RequestBindKeyInput(g, &g->keys.inventory_bind);
									done = true;
									break;
								case 267:
									RequestBindKeyInput(g, &g->keys.skills_bind);
									done = true;
									break;
								case 268:
									RequestBindKeyInput(g, &g->keys.chat_bind);
									done = true;
									break;
							}
						}
					}
				}
			}
		}
		if (!done && g->scene == 1 && g->Editor.edit_mode) {
			if (g->pos_x > 280 && g->Editor.tool_id == 1 && g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == false) { // Select tool clicks
				Platform* plt = g->lvl->Platform;
				Platform* bg = g->lvl->Background;
				Platform* fg = g->lvl->Foreground;
				Platform* ev = g->lvl->Event;
				if (!done) {
					for (int i = 0; i < MAX_EVENTS; i++) {
						if (ev[i].type >= 0) {
							int hover_x = g->pos_x - g->camera.x;
							int hover_y = g->pos_y - g->camera.y;
							if (collide(hover_x, hover_y, 1, 1, ev[i].x, ev[i].y, ev[i].w, ev[i].h)) {
								g->Editor.select_type = 1;
								g->Editor.select_id = i;
								g->Editor.selection_tool_type = 5;
								g->Editor.ev = &g->lvl->Event[i];
								done = true;
							}
						}
					}
				}
				if (!done) {
					for (int i = 0; i < MAX_FOREGROUNDS; i++) {
						if (fg[i].type >= 0) {
							int hover_x = g->pos_x - g->camera.x;
							int hover_y = g->pos_y - g->camera.y;
							if (collide(hover_x, hover_y, 1, 1, fg[i].x, fg[i].y, fg[i].w, fg[i].h)) {
								g->Editor.select_type = 1;
								g->Editor.select_id = i;
								g->Editor.selection_tool_type = 4;
								g->Editor.fg = &g->lvl->Foreground[i];
								done = true;
							}
						}
					}
				}
				if (!done) {
					for (int i = 0; i < MAX_PLATFORMS; i++) {
						if (plt[i].type >= 0) {
							int hover_x = g->pos_x - g->camera.x;
							int hover_y = g->pos_y - g->camera.y;
							if (collide(hover_x, hover_y, 1, 1, plt[i].x, plt[i].y, plt[i].w, plt[i].h)) {
								g->Editor.select_type = 1; // Select type is a platform
								g->Editor.select_id = i;
								g->Editor.selection_tool_type = 2;
								g->Editor.plt = &g->lvl->Platform[i];
								done = true;
							}
						}
					}
				}
				if (!done) {
					for (int i = 0; i < MAX_LV_BACKGROUNDS; i++) {
						if (bg[i].type >= 0) {
							int hover_x = g->pos_x - g->camera.x;
							int hover_y = g->pos_y - g->camera.y;
							if (collide(hover_x, hover_y, 1, 1, bg[i].x, bg[i].y, bg[i].w, bg[i].h)) {
								g->Editor.select_type = 1;
								g->Editor.select_id = i;
								g->Editor.selection_tool_type = 3;
								g->Editor.bg = &g->lvl->Background[i];
								done = true;
							}
						}
					}
				}
				if (!done) {
					g->Editor.select_type = -1;
					g->Editor.select_id = -1;
					g->Editor.selection_tool_type = -1;
					g->Editor.plt = NULL;
					if (g->Editor.last_click_x == g->pos_x && g->Editor.last_click_y == g->pos_y) {
						g->Editor.last_click_x = -1;
						g->Editor.last_click_y = -1;
						g->pl.x = g->pos_x - g->camera.x;
						g->pl.y = g->pos_y - g->camera.y;
					} else {
						g->Editor.last_click_x = g->pos_x;
						g->Editor.last_click_y = g->pos_y;
					}
				}
			}
		}
	}
}

void RightClick(Game* g, bool release, ALLEGRO_SAMPLE** sample_sfx){
	if(!release){
		if (g->scene == 1 && g->Editor.edit_mode) {
			g->camera.mode = CAMERA_MODE_EDITOR;
			g->Editor.camera_drag = true;
			g->Editor.old_x = g->pos_x;
			g->Editor.old_y = g->pos_y;
			g->Editor.old_cx = g->camera.x;
			g->Editor.old_cy = g->camera.y;
		}
	} else {
		g->Editor.camera_drag = false;
	}
}

void KeyboardFunction(Game* g, int kid, bool release,  ALLEGRO_SAMPLE** sample_sfx, ALLEGRO_FONT** font){
	Player* p = &g->pl;
	bool can_type = true;
	if(!release){
		if (g->binding_key != NULL) {
			if (kid == ALLEGRO_KEY_ESCAPE || (kid >= ALLEGRO_KEY_F1 && kid <= ALLEGRO_KEY_F12)) {
				//
			} else {
				int* fix_key = KeyIsBound(g, kid);
				BindKey(g->binding_key, kid);
				g->Interfaces[INTERFACE_MESSAGE].visible = false;
				g->Buttons[16].sprite_id = 27;
				if (fix_key != NULL && fix_key != g->binding_key) *fix_key = -1;
				g->binding_key = NULL;
			}
		} else if (g->Interfaces[INTERFACE_LOGIN].visible == true && !g->logini.logging_in) {
			// Login interface typing
			if (g->logini.type_username) {
				int msg_lenght = strlen(g->logini.username_input.c_str());
				if (msg_lenght < 20) {
					g->logini.username_input = g->logini.username_input + GetCharacterFromKey(kid, g->keys.SHIFT);
				}
			} else {
				int msg_lenght = strlen(g->logini.password_input.c_str());
				if (msg_lenght < 20) {
					std::string old = g->logini.password_input;
					g->logini.password_input = g->logini.password_input + GetCharacterFromKey(kid, g->keys.SHIFT);
					if(old != g->logini.password_input)	g->logini.password_input_mask += "*";
				}
			}
			if (kid == ALLEGRO_KEY_TAB || kid == ALLEGRO_KEY_ENTER || kid == ALLEGRO_KEY_PAD_ENTER) {
				g->logini.type_username = !g->logini.type_username;
			}
			if (kid == ALLEGRO_KEY_BACKSPACE) {
				g->keys.backspace = true;
				if (g->logini.type_username == true) {
					if (g->logini.username_input != "") {
						g->logini.username_input.pop_back();
						g->logini.backspace_timer = 5;
					}
				} else {
					if (g->logini.password_input != "") {
						g->logini.password_input.pop_back();
						g->logini.password_input_mask.pop_back();
						g->logini.backspace_timer = 5;
					}
				}
			}
			if (kid == ALLEGRO_KEY_LSHIFT || kid == ALLEGRO_KEY_RSHIFT) {
				g->keys.SHIFT = true;
			}
		} else if (g->scene == 1) {
			if (!g->chat.type_chat && (g->convo->timer <= -1 || g->convo->exploding || g->cut_scene.state == CUTSCENE_STATE_CLOSING)) { // Handle key binds
				for (int i = 0; i < 8; i++) {
					if (kid == g->AbilitySlots[i].key && g->keys.CTRL == false && g->keys.SHIFT == false && g->chat.type_chat == false) {
						UseAbility(g, g->AbilitySlots[i].ability.id);
					}
				}
				if (kid == g->keys.left_bind) {
					g->keys.left = true;
					g->keys.right = false;
					if (!p->in_air) p->x_acc = 0;
				}
				if (kid == g->keys.right_bind) {
					g->keys.right = true;
					g->keys.left = false;
					if (!p->in_air) p->x_acc = 0;
				}
				if (kid == g->keys.jump_bind && g->story.missions[0].prog >= 5) {
					g->keys.jump = true;
					if (OnAFloor(g) && !g->pl.in_air) {
						Jump(g);
					} else if (OnAWall(g) && g->story.missions[0].prog >= 18) {
						if (g->pl.energy >= COST_WALL_JUMP || g->story.missions[0].prog == 18) {
							WallJump(g);
						}
					}
					if (p->in_water && p->y_vel > 0) p->y_vel = 0;
				}
				if (kid == g->keys.crouch_bind) {
					g->keys.down = true;
				}
				if (kid == g->keys.inventory_bind) {
					g->Interfaces[INTERFACE_INVENTORY].visible = !g->Interfaces[INTERFACE_INVENTORY].visible;
				}
				if (kid == g->keys.skills_bind) {
					g->Interfaces[INTERFACE_ABILITIES].visible = !g->Interfaces[INTERFACE_ABILITIES].visible;
				}
				if (kid == g->keys.camera_bind) {
					if (g->camera.mode != CAMERA_MODE_LOCKED) {
						g->camera.mode = CAMERA_MODE_LOCKED;
					} else {
						g->camera.mode = CAMERA_MODE_BOX;
					}
				}
				if (kid == g->keys.chat_bind) {
					g->chat.show_chat = true;
					if (!g->chat.type_chat) {
						g->chat.type_chat = true;
					}
					can_type = false;
				}
			}
			if (g->chat.type_chat && can_type) {
				int msg_lenght = al_get_text_width(font[5], (g->pl.name + g->chat.msg_input).c_str());
				if (msg_lenght < 480) {
					g->chat.msg_input = g->chat.msg_input + GetCharacterFromKey(kid, g->keys.SHIFT);
				}
				if (kid == ALLEGRO_KEY_BACKSPACE) {
					g->keys.backspace = true;
					if (g->chat.type_chat && g->chat.show_chat && g->chat.backspace_timer <= 0) {
						if (g->chat.msg_input != "") {
							g->chat.msg_input.pop_back();
						}
					}
				}
				if (kid == ALLEGRO_KEY_ENTER || kid == ALLEGRO_KEY_PAD_ENTER) {
					g->chat.show_chat = true;
					if (!g->chat.type_chat) {
						g->chat.type_chat = true;
					} else {
						g->chat.type_chat = false;
						g->chat.show_chat = false;
						if (g->chat.msg_input != "") {
							SubmitMessage(g, g->chat.msg_input.c_str());
							g->chat.msg_input.clear();
						}
					}
				}
				if (kid == ALLEGRO_KEY_LSHIFT || kid == ALLEGRO_KEY_RSHIFT) {
					g->keys.SHIFT = true;
				}
			} else {
				switch (kid) {
				case ALLEGRO_KEY_TAB:
					g->chat.show_chat = true;
					break;
				case ALLEGRO_KEY_S:
					if (g->keys.CTRL && g->Editor.edit_mode == true) {
						QuickSave(g);
					}
					break;
				case ALLEGRO_KEY_LCTRL:
					g->keys.CTRL = true;
					break;
				case ALLEGRO_KEY_SPACE:
					if (g->flag.visible == true) {
						g->flag.visible = false;
						p->x = g->flag.x - 26;
						p->y = g->flag.y - 26;
						std::cout << "Teleport to: " << p->x << ", " << p->y << "\n";
					} else {
						g->flag.visible = true;
						g->flag.x = p->x + 26;
						g->flag.y = p->y + 26;
						g->flag.r = 640;
						std::cout << "Set Flag " << g->flag.x << ", " << g->flag.y << "\n";
					}
					break;
				case ALLEGRO_KEY_R:
				{
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
					break;
				}
				case ALLEGRO_KEY_F5:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->Editor.selection_tool_type == 3) {
							if (g->Editor.bg->type >= SPRITE_TILE_HOUSE && g->Editor.bg->type <= 20) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_HOUSE) g->Editor.bg->type = 20;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_TREE && g->Editor.bg->type <= 52) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_TREE) g->Editor.bg->type = 52;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_GRASS && g->Editor.bg->type <= 59) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_GRASS) g->Editor.bg->type = 59;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_ROCK && g->Editor.bg->type <= 69) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_ROCK) g->Editor.bg->type = 69;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_HOUSE_BEIGE_T1 && g->Editor.bg->type <= SPRITE_TILE_HOUSE_GREY_T1) {
								g->Editor.bg->type -= 8;
								if (g->Editor.bg->type < SPRITE_TILE_HOUSE_BEIGE_T1) g->Editor.bg->type = SPRITE_TILE_HOUSE_GREY_T1;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_BRICK && g->Editor.bg->type <= 99) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_BRICK) g->Editor.bg->type = 99;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_ROOF && g->Editor.bg->type <= 102) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_ROOF) g->Editor.bg->type = 102;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_DOOR && g->Editor.bg->type <= 113) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_DOOR) g->Editor.bg->type = 113;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_WINDOW && g->Editor.bg->type <= 133) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_WINDOW) g->Editor.bg->type = 133;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_MISC && g->Editor.bg->type <= 147) {
								g->Editor.bg->type--;
								if (g->Editor.bg->type < SPRITE_TILE_MISC) g->Editor.bg->type = 147;
							}
						}
						if (g->Editor.selection_tool_type == 4) {
							if (g->Editor.fg->type >= SPRITE_TILE_HOUSE && g->Editor.fg->type <= 20) {
								g->Editor.fg->type--;
								if (g->Editor.fg->type < SPRITE_TILE_HOUSE) g->Editor.fg->type = 20;
							}
							if (g->Editor.fg->type >= SPRITE_TILE_TREE && g->Editor.fg->type <= 52) {
								g->Editor.fg->type--;
								if (g->Editor.fg->type < SPRITE_TILE_TREE) g->Editor.fg->type = 52;
							}
							if (g->Editor.fg->type >= SPRITE_TILE_GRASS && g->Editor.fg->type <= 59) {
								g->Editor.fg->type--;
								if (g->Editor.fg->type < SPRITE_TILE_GRASS) g->Editor.fg->type = 59;
							}
							if (g->Editor.fg->type >= SPRITE_TILE_ROCK && g->Editor.fg->type <= 69) {
								g->Editor.fg->type--;
								if (g->Editor.fg->type < SPRITE_TILE_ROCK) g->Editor.fg->type = 69;
							}
						}
					}
					break;
				case ALLEGRO_KEY_F6:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->Editor.selection_tool_type == 3) {
							if (g->Editor.bg->type >= SPRITE_TILE_HOUSE && g->Editor.bg->type <= 20) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 20) g->Editor.bg->type = SPRITE_TILE_HOUSE;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_TREE && g->Editor.bg->type <= 52) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 52) g->Editor.bg->type = SPRITE_TILE_TREE;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_GRASS && g->Editor.bg->type <= 59) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 59) g->Editor.bg->type = SPRITE_TILE_GRASS;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_ROCK && g->Editor.bg->type <= 69) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 69) g->Editor.bg->type = SPRITE_TILE_ROCK;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_HOUSE_BEIGE_T1 && g->Editor.bg->type <= SPRITE_TILE_HOUSE_GREY_T1) {
								g->Editor.bg->type += 8;
								if (g->Editor.bg->type > SPRITE_TILE_HOUSE_GREY_T1) g->Editor.bg->type = SPRITE_TILE_HOUSE_BEIGE_T1;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_BRICK && g->Editor.bg->type <= 99) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 99) g->Editor.bg->type = SPRITE_TILE_BRICK;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_ROOF && g->Editor.bg->type <= 102) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 102) g->Editor.bg->type = SPRITE_TILE_ROOF;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_DOOR && g->Editor.bg->type <= 113) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 113) g->Editor.bg->type = SPRITE_TILE_DOOR;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_WINDOW && g->Editor.bg->type <= 133) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 133) g->Editor.bg->type = SPRITE_TILE_WINDOW;
							}
							if (g->Editor.bg->type >= SPRITE_TILE_MISC && g->Editor.bg->type <= 147) {
								g->Editor.bg->type++;
								if (g->Editor.bg->type > 147) g->Editor.bg->type = SPRITE_TILE_MISC;
							}
						}
						if (g->Editor.selection_tool_type == 4) {
							if (g->Editor.fg->type >= 15 && g->Editor.fg->type <= 20) {
								g->Editor.fg->type++;
								if (g->Editor.fg->type > 20) g->Editor.fg->type = 15;
							}
							if (g->Editor.fg->type >= 21 && g->Editor.fg->type <= 52) {
								g->Editor.fg->type++;
								if (g->Editor.fg->type > 52) g->Editor.fg->type = 21;
							}
							if (g->Editor.fg->type >= 53 && g->Editor.fg->type <= 59) {
								g->Editor.fg->type++;
								if (g->Editor.fg->type > 59) g->Editor.fg->type = 53;
							}
							if (g->Editor.fg->type >= 60 && g->Editor.fg->type <= 69) {
								g->Editor.fg->type++;
								if (g->Editor.fg->type > 69) g->Editor.fg->type = 60;
							}
						}
					}
					break;
				case ALLEGRO_KEY_F7:
					if (g->Editor.edit_mode) {
						if (g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == true && g->Editor.debug_type == 1) {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = false;
						} else {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = true;
							g->Editor.debug_type = 1;
						}
					}
					break;
				case ALLEGRO_KEY_F8:
					if (g->Editor.edit_mode) {
						if (g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == true && g->Editor.debug_type == 2) {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = false;
						} else {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = true;
							g->Editor.debug_type = 2;
						}
					}
					break;
				case ALLEGRO_KEY_F9:
					if (g->Editor.edit_mode) {
						if (g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == true && g->Editor.debug_type == 3) {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = false;
						} else {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = true;
							g->Editor.debug_type = 3;
						}
					}
					break;
				case ALLEGRO_KEY_F10:
					if (g->Editor.edit_mode) {
						if (g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == true && g->Editor.debug_type == 4) {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = false;
						} else {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = true;
							g->Editor.debug_type = 4;
						}
					}
					break;
				case ALLEGRO_KEY_F11:
					if (g->Editor.edit_mode) {
						if (g->Interfaces[INTERFACE_EDITOR_DEBUG].visible == true && g->Editor.debug_type == 5) {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = false;
						} else {
							g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = true;
							g->Editor.debug_type = 5;
						}
					}
					break;
				case ALLEGRO_KEY_PAD_4:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->texture_l = !g->Editor.plt->texture_l;
						}
					}
					break;
				case ALLEGRO_KEY_PAD_6:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->texture_r = !g->Editor.plt->texture_r;
						}
					}
					break;
				case ALLEGRO_KEY_PAD_8:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->texture_t = !g->Editor.plt->texture_t;
						}
					}
					break;
				case ALLEGRO_KEY_PAD_2:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->texture_b = !g->Editor.plt->texture_b;
						}
					}
					break;
				case ALLEGRO_KEY_Z:
					if (g->Editor.edit_mode && !g->chat.type_chat) {
						if (g->keys.CTRL) {
							UndoEditor(g);
						}
					}
					break;
				case ALLEGRO_KEY_C:
					if (g->Editor.edit_mode && g->Editor.select_id > -1 && !g->chat.type_chat) {
						if (g->keys.CTRL) {
							if (g->Editor.selection_tool_type == 2) {
								std::string plt_string = std::to_string(g->Editor.selection_tool_type) + " ";
								plt_string += std::to_string(g->Editor.plt->type) + " ";
								plt_string += std::to_string(g->Editor.plt->w) + " ";
								plt_string += std::to_string(g->Editor.plt->h) + " ";
								plt_string += std::to_string(g->Editor.plt->texture_t) + " ";
								plt_string += std::to_string(g->Editor.plt->texture_b) + " ";
								plt_string += std::to_string(g->Editor.plt->texture_l) + " ";
								plt_string += std::to_string(g->Editor.plt->texture_r);
								ToClipboard(plt_string);
							}
							if (g->Editor.selection_tool_type == 3) {
								std::string plt_string = std::to_string(g->Editor.selection_tool_type) + " ";
								plt_string += std::to_string(g->Editor.bg->type) + " ";
								plt_string += std::to_string(g->Editor.bg->w) + " ";
								plt_string += std::to_string(g->Editor.bg->h) + " ";
								plt_string += std::to_string(g->Editor.bg->texture_t) + " ";
								plt_string += std::to_string(g->Editor.bg->texture_b) + " ";
								plt_string += std::to_string(g->Editor.bg->texture_l) + " ";
								plt_string += std::to_string(g->Editor.bg->texture_r);
								ToClipboard(plt_string);
							}
							if (g->Editor.selection_tool_type == 4) {
								std::string plt_string = std::to_string(g->Editor.selection_tool_type) + " ";
								plt_string += std::to_string(g->Editor.fg->type) + " ";
								plt_string += std::to_string(g->Editor.fg->w) + " ";
								plt_string += std::to_string(g->Editor.fg->h) + " ";
								plt_string += std::to_string(g->Editor.fg->texture_t) + " ";
								plt_string += std::to_string(g->Editor.fg->texture_b) + " ";
								plt_string += std::to_string(g->Editor.fg->texture_l) + " ";
								plt_string += std::to_string(g->Editor.fg->texture_r);
								ToClipboard(plt_string);
							}
							if (g->Editor.selection_tool_type == 5) {
								std::string plt_string = std::to_string(g->Editor.selection_tool_type) + " ";
								plt_string += std::to_string(g->Editor.ev->type) + " ";
								plt_string += std::to_string(g->Editor.ev->w) + " ";
								plt_string += std::to_string(g->Editor.ev->h) + " ";
								plt_string += std::to_string(g->Editor.ev->texture_t) + " ";
								plt_string += std::to_string(g->Editor.ev->texture_b) + " ";
								plt_string += std::to_string(g->Editor.ev->texture_l) + " ";
								plt_string += std::to_string(g->Editor.ev->texture_r);
								ToClipboard(plt_string);
							}
						}
					}
					break;
				case ALLEGRO_KEY_V:
					if (g->Editor.edit_mode && !g->chat.type_chat) {
						if (g->keys.CTRL) {
							Platform temp;
							std::istringstream plt_paste(FromClipboard());
							int tool = -1;
							plt_paste >> tool;
							if (tool != 1) {
								int type = -1;
								int w = 32;
								int h = 32;
								bool texture_t = true;
								bool texture_b = true;
								bool texture_l = true;
								bool texture_r = true;

								plt_paste >> type;
								plt_paste >> w;
								plt_paste >> h;
								plt_paste >> texture_t;
								plt_paste >> texture_b;
								plt_paste >> texture_l;
								plt_paste >> texture_r;

								temp.type = (int)type;
								temp.w = (int)w;
								temp.h = (int)h;
								temp.texture_t = (bool)texture_t;
								temp.texture_b = (bool)texture_b;
								temp.texture_l = (bool)texture_l;
								temp.texture_r = (bool)texture_r;
								temp.x = g->pos_x - g->camera.x;
								temp.y = g->pos_y - g->camera.y;

								if (tool == 2) {
									g->lvl->Platform[NextUnusedPlatform(g, tool)] = temp;
								}
								if (tool == 3) {
									g->lvl->Background[NextUnusedPlatform(g, tool)] = temp;
								}
								if (tool == 4) {
									g->lvl->Foreground[NextUnusedPlatform(g, tool)] = temp;
								}
								if (tool == 5) {
									g->lvl->Event[NextUnusedPlatform(g, tool)] = temp;
								}
							}
						}
					}
					break;
				case ALLEGRO_KEY_PAD_MINUS:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						RememberEditorState(g);
						if (g->Editor.selection_tool_type == 2) {
							if (g->Editor.select_id != 0) {
								Platform temp = g->lvl->Platform[g->Editor.select_id - 1];
								g->lvl->Platform[g->Editor.select_id - 1] = g->lvl->Platform[g->Editor.select_id];
								g->lvl->Platform[g->Editor.select_id] = temp;
								g->Editor.plt = &g->lvl->Platform[g->Editor.select_id - 1];
								g->Editor.select_id--;
							}
							else {
								ShowMessage(g, "On bottom already", "");
							}
						}
						if (g->Editor.selection_tool_type == 3) {
							if (g->Editor.select_id != 0) {
								Platform temp = g->lvl->Background[g->Editor.select_id - 1];
								g->lvl->Background[g->Editor.select_id - 1] = g->lvl->Background[g->Editor.select_id];
								g->lvl->Background[g->Editor.select_id] = temp;
								g->Editor.bg = &g->lvl->Background[g->Editor.select_id - 1];
								g->Editor.select_id--;
							}
							else {
								ShowMessage(g, "On bottom already", "");
							}
						}
						if (g->Editor.selection_tool_type == 4) {
							if (g->Editor.select_id != 0) {
								Platform temp = g->lvl->Foreground[g->Editor.select_id - 1];
								g->lvl->Foreground[g->Editor.select_id - 1] = g->lvl->Foreground[g->Editor.select_id];
								g->lvl->Foreground[g->Editor.select_id] = temp;
								g->Editor.fg = &g->lvl->Foreground[g->Editor.select_id - 1];
								g->Editor.select_id--;
							}
							else {
								ShowMessage(g, "On bottom already", "");
							}
						}
					}
					break;
				case ALLEGRO_KEY_PAD_PLUS:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						RememberEditorState(g);
						if (g->Editor.selection_tool_type == 2) {
							if (g->Editor.select_id != (MAX_PLATFORMS - 1)) {
								Platform temp = g->lvl->Platform[g->Editor.select_id + 1];
								g->lvl->Platform[g->Editor.select_id + 1] = g->lvl->Platform[g->Editor.select_id];
								g->lvl->Platform[g->Editor.select_id] = temp;
								g->Editor.plt = &g->lvl->Platform[g->Editor.select_id + 1];
								g->Editor.select_id++;
							}
							else {
								ShowMessage(g, "On top already", "");
							}
						}
						if (g->Editor.selection_tool_type == 3) {
							if (g->Editor.select_id != (MAX_LV_BACKGROUNDS - 1)) {
								Platform temp = g->lvl->Background[g->Editor.select_id + 1];
								g->lvl->Background[g->Editor.select_id + 1] = g->lvl->Background[g->Editor.select_id];
								g->lvl->Background[g->Editor.select_id] = temp;
								g->Editor.bg = &g->lvl->Background[g->Editor.select_id + 1];
								g->Editor.select_id++;
							}
							else {
								ShowMessage(g, "On top already", "");
							}
						}
						if (g->Editor.selection_tool_type == 4) {
							if (g->Editor.select_id != (MAX_FOREGROUNDS - 1)) {
								Platform temp = g->lvl->Foreground[g->Editor.select_id + 1];
								g->lvl->Foreground[g->Editor.select_id + 1] = g->lvl->Foreground[g->Editor.select_id];
								g->lvl->Foreground[g->Editor.select_id] = temp;
								g->Editor.fg = &g->lvl->Foreground[g->Editor.select_id + 1];
								g->Editor.select_id++;
							}
							else {
								ShowMessage(g, "On top already", "");
							}
						}
					}
					break;
				case ALLEGRO_KEY_DELETE:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat) {
						RememberEditorState(g);
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->x = -9999;
							g->Editor.plt->y = -9999;
							g->Editor.plt->w = BLOCK_SIZE;
							g->Editor.plt->h = BLOCK_SIZE;
							g->Editor.plt->type = -1;
							g->Editor.selection_tool_type = -1;
						}
						if (g->Editor.selection_tool_type == 3) {
							g->Editor.bg->x = -9999;
							g->Editor.bg->y = -9999;
							g->Editor.bg->w = BLOCK_SIZE;
							g->Editor.bg->h = BLOCK_SIZE;
							g->Editor.bg->type = -1;
							g->Editor.selection_tool_type = -1;
						}
						if (g->Editor.selection_tool_type == 4) {
							g->Editor.fg->x = -9999;
							g->Editor.fg->y = -9999;
							g->Editor.fg->w = BLOCK_SIZE;
							g->Editor.fg->h = BLOCK_SIZE;
							g->Editor.fg->type = -1;
							g->Editor.selection_tool_type = -1;
						}
						if (g->Editor.selection_tool_type == 5) {
							g->Editor.ev->x = -9999;
							g->Editor.ev->y = -9999;
							g->Editor.ev->w = BLOCK_SIZE;
							g->Editor.ev->h = BLOCK_SIZE;
							g->Editor.ev->type = -1;
							g->Editor.selection_tool_type = -1;
						}
					}
					break;
				case ALLEGRO_KEY_LEFT:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat && g->keys.CTRL) {
						RememberEditorState(g);
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->x -= BLOCK_SIZE / 2;
						}
						if (g->Editor.selection_tool_type == 3) {
							g->Editor.bg->x -= BLOCK_SIZE / 2;
						}
						if (g->Editor.selection_tool_type == 4) {
							g->Editor.fg->x -= BLOCK_SIZE / 2;
						}
						if (g->Editor.selection_tool_type == 5) {
							g->Editor.ev->x -= BLOCK_SIZE / 2;
						}
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (g->Editor.edit_mode && g->Editor.tool_id == 1 && g->Editor.select_id > -1 && !g->chat.type_chat && g->keys.CTRL) {
						RememberEditorState(g);
						if (g->Editor.selection_tool_type == 2) {
							g->Editor.plt->x += BLOCK_SIZE / 2;
						}
						if (g->Editor.selection_tool_type == 3) {
							g->Editor.bg->x += BLOCK_SIZE / 2;
						}
						if (g->Editor.selection_tool_type == 4) {
							g->Editor.fg->x += BLOCK_SIZE / 2;
						}
						if (g->Editor.selection_tool_type == 5) {
							g->Editor.ev->x += BLOCK_SIZE / 2;
						}
					}
					break;
				case ALLEGRO_KEY_G:
					if (g->Editor.edit_mode && g->scene == 1 && !g->chat.type_chat) {
						g->Editor.grid = !g->Editor.grid;
					}
					break;
				}
			}
		}
	} else {
	if (kid == g->keys.left_bind) {
		g->keys.left = false;
	}
	if (kid == g->keys.right_bind) {
		g->keys.right = false;
	}
	if (kid == g->keys.jump_bind) {
		g->keys.jump = false;
	}
	if (kid == g->keys.crouch_bind) {
		g->keys.down = false;
	}
	if (kid == ALLEGRO_KEY_LSHIFT || kid == ALLEGRO_KEY_RSHIFT) {
		g->keys.SHIFT = false;
	}
	if (kid == ALLEGRO_KEY_BACKSPACE && g->Interfaces[INTERFACE_LOGIN].visible) {
		g->keys.backspace = false;
		g->logini.backspace_timer_wait = 30;
	}
		if (g->scene == 1) {
			switch (kid) {
				case ALLEGRO_KEY_BACKSPACE:
					g->keys.backspace = false;
					g->chat.backspace_timer_wait = 30;
					break;
				case ALLEGRO_KEY_TAB:
					if (!g->chat.type_chat) {
						g->chat.show_chat = false;
						g->chat.ghost = 0;
					}
					break;
				case ALLEGRO_KEY_LCTRL:
					g->keys.CTRL = false;
					break;
			}
		}
		if(kid == ALLEGRO_KEY_ESCAPE && !g->logini.logging_in && g->binding_key == NULL) {
			if (g->chat.type_chat) {
				g->chat.type_chat = false;
				g->chat.show_chat = false;
			} else if (g->Interfaces[INTERFACE_PAUSE].visible){
				HideAllInterfaces(g, INTERFACE_MAP);
				g->Interfaces[INTERFACE_CHAT].visible = true;
				g->Interfaces[INTERFACE_SLOTS].visible = true;
				g->Interfaces[INTERFACE_PLAYER].visible = true;
				g->Interfaces[INTERFACE_MAP].visible = true;
				g->scene = 1;
			} else if (g->Interfaces[INTERFACE_INVENTORY].visible == true) {
				g->Interfaces[INTERFACE_INVENTORY].visible = false;
			} else if (g->Interfaces[INTERFACE_ABILITIES].visible == true) {
				g->Interfaces[INTERFACE_ABILITIES].visible = false;
			} else if(g->scene == 1 || g->scene == 2){
				g->scene = 2;
				HideAllInterfaces(g, 10);
				g->Interfaces[INTERFACE_PAUSE].visible = true;
				g->Interfaces[6].visible = true;
				g->Editor.edit_mode = false;
				g->Interfaces[INTERFACE_CHAT].x = 0;
				g->Interfaces[INTERFACE_CHAT].y = GAME_HEIGHT - 480;
				g->camera.mode = CAMERA_MODE_BOX;
			} else {
				g->scene = 0;
				HideAllInterfaces(g, 0);
				g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
			}
		}
		if (kid == ALLEGRO_KEY_F4 && !g->logini.logging_in && g->binding_key == NULL) {
			if (g->Editor.edit_mode == false) {
				if (g->window_mode != ALLEGRO_FULLSCREEN) {
					g->Interfaces[INTERFACE_CHAT].visible = true;
					g->scene = 1;
					g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
					g->Interfaces[1].visible = false;
					g->Interfaces[2].visible = false;
					g->Interfaces[INTERFACE_OPTIONS].visible = false;
					g->Interfaces[4].visible = false;
					g->Interfaces[5].visible = false;
					g->Interfaces[INTERFACE_SLOTS].visible = false;
					g->Interfaces[INTERFACE_PLAYER].visible = false;
					g->Interfaces[INTERFACE_MAP].visible = false;
					g->Interfaces[INTERFACE_EDITOR].visible = true;
					g->Editor.edit_mode = true;
					g->camera.mode = CAMERA_MODE_EDITOR;
					AddChatMessage(g, "__SYSTEM__", 0, 0, 0, "Level edit mode = ON");
					g->Interfaces[INTERFACE_CHAT].x = 280;
					g->Interfaces[INTERFACE_CHAT].y = GAME_HEIGHT - 400;
				} else {
					ShowMessage(g, "Can't open Editor in full-screen", "");
				}
			} else {
				g->Editor.edit_mode = false;
				g->Interfaces[INTERFACE_EDITOR].visible = false;
				g->Interfaces[81].visible = false;
				g->Interfaces[82].visible = false;
				g->Interfaces[83].visible = false;
				g->Interfaces[84].visible = false;
				g->Interfaces[INTERFACE_EDITOR_DEBUG].visible = false;
				g->camera.mode = CAMERA_MODE_BOX;
				AddChatMessage(g, "__SYSTEM__", 0, 0, 0, "Level edit mode = OFF");
				g->Interfaces[INTERFACE_CHAT].x = 0;
				g->Interfaces[INTERFACE_CHAT].y = GAME_HEIGHT - 480;
				if (g->scene == 1) {
					g->Interfaces[INTERFACE_SLOTS].visible = true;
					g->Interfaces[INTERFACE_PLAYER].visible = true;
					g->Interfaces[INTERFACE_MAP].visible = true;
				}
			}
		}
	}
}

int ChooseEvent(Game* g, int type) {
	for (int i = 0; i < MAX_EVENTS; i++) {
		if (g->lvl->Event[i].type == -1) {
			g->Editor.tool_id = 5;
			g->Editor.selection_tool_type = 5;
			g->Editor.select_type = type;
			g->Editor.select_id = i;
			g->Editor.ev = &g->lvl->Event[i];
			g->Editor.ev->type = type;
			g->Interfaces[83].visible = false;
			return i;
		}
	}
	ShowMessage(g, "Level event cap reached!", "Make a new level or delete some platforms");
	return -1;
}

int ChooseBackground(Game* g, int type) {
	for (int i = 0; i < MAX_LV_BACKGROUNDS; i++) {
		if (g->lvl->Background[i].type == -1) {
			g->Editor.tool_id = 3;
			g->Editor.selection_tool_type = 3;
			g->Editor.select_type = type;
			g->Editor.select_id = i;
			g->Editor.bg = &g->lvl->Background[i];
			g->Editor.bg->type = type;
			g->Interfaces[82].visible = false;
			return i;
		}
	}
	ShowMessage(g, "Level background cap reached!", "Make a new level or delete some backgrounds");
	return -1;
}

int ChooseForeground(Game* g, int type) {
	for (int i = 0; i < MAX_FOREGROUNDS; i++) {
		if (g->lvl->Foreground[i].type == -1) {
			g->Editor.tool_id = 4;
			g->Editor.selection_tool_type = 4;
			g->Editor.select_type = type;
			g->Editor.select_id = i;
			g->Editor.fg = &g->lvl->Foreground[i];
			g->Editor.fg->type = type;
			g->Interfaces[84].visible = false;
			return i;
		}
	}
	ShowMessage(g, "Level foreground cap reached!", "Make a new level or delete some foregrounds");
	return -1;
}

int ChoosePlatform(Game* g, int type) {
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if (g->lvl->Platform[i].type == -1) {
			g->Editor.tool_id = 2;
			g->Editor.selection_tool_type = 2;
			g->Editor.select_type = type;
			g->Editor.select_id = i;
			g->Editor.plt = &g->lvl->Platform[i];
			g->Editor.plt->type = type;
			g->Interfaces[81].visible = false;
			return i;
		}
	}
	ShowMessage(g, "Level platform cap reached!", "Make a new level or delete some platforms");
	return -1;
}

int NextUnusedPlatform(Game* g, int tool) {
	int MAX = -1;
	if (tool == 2) MAX = MAX_PLATFORMS;
	if (tool == 3) MAX = MAX_LV_BACKGROUNDS;
	if (tool == 4) MAX = MAX_FOREGROUNDS;
	if (tool == 5) MAX = MAX_EVENTS;
	for (int i = 0; i < MAX; i++) {
		if (tool == 2) {
			if (g->lvl->Platform[i].type == -1) {
				return i;
			}
		}
		if (tool == 3) {
			if (g->lvl->Background[i].type == -1) {
				return i;
			}
		}
		if (tool == 4) {
			if (g->lvl->Foreground[i].type == -1) {
				return i;
			}
		}
		if (tool == 5) {
			if (g->lvl->Event[i].type == -1) {
				return i;
			}
		}
	}
	return -1;
}

void BackupMap(const char* file) {
	// Local time
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	int year = timeinfo->tm_year + 1900;
	int month = timeinfo->tm_mon;
	int day = timeinfo->tm_mday;
	int hour = timeinfo->tm_hour;
	int minute = timeinfo->tm_min;
	int second = timeinfo->tm_sec;

	// Copy name
	std::string current_date_and_time;
	current_date_and_time = "[" +
		std::to_string(year) + "-" +
		std::to_string(month) + "-" +
		std::to_string(day) + "][" +
		std::to_string(hour) + "-" +
		std::to_string(minute) + "-" +
		std::to_string(second) + "]";

	// Source/Destination
	std::string source = file;
	source = "\"" + source + "\"";
	SubStrings new_folder = SplitString(file, '.');
	if (new_folder.items > 0) {
		std::string destination = new_folder.s[0];
		destination = "\"" + destination + "\\" + current_date_and_time + ".dem\"";
		std::string command = "copy " + source + " " + destination;
		std::string make_folder_cmd = "mkdir \"" + new_folder.s[0] + "\"";
		system(make_folder_cmd.c_str());
		system(command.c_str());
	}
}

// Temp
Level ReconstructMap(Level old_map) {
	Level new_map = Level();
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		// Platforms
		new_map.Platform[i].type = (int)old_map.Platform[i].type;
		new_map.Platform[i].x = (int)old_map.Platform[i].x;
		new_map.Platform[i].y = (int)old_map.Platform[i].y;
		new_map.Platform[i].w = (int)old_map.Platform[i].w;
		new_map.Platform[i].h = (int)old_map.Platform[i].h;
		new_map.Platform[i].texture_t = (bool)old_map.Platform[i].texture_t;
		new_map.Platform[i].texture_b = (bool)old_map.Platform[i].texture_b;
		new_map.Platform[i].texture_l = (bool)old_map.Platform[i].texture_l;
		new_map.Platform[i].texture_r = (bool)old_map.Platform[i].texture_r;

		// Backgrounds
		new_map.Background[i].type = (int)old_map.Background[i].type;
		new_map.Background[i].x = (int)old_map.Background[i].x;
		new_map.Background[i].y = (int)old_map.Background[i].y;
		new_map.Background[i].w = (int)old_map.Background[i].w;
		new_map.Background[i].h = (int)old_map.Background[i].h;
		new_map.Background[i].texture_t = (bool)old_map.Background[i].texture_t;
		new_map.Background[i].texture_b = (bool)old_map.Background[i].texture_b;
		new_map.Background[i].texture_l = (bool)old_map.Background[i].texture_l;
		new_map.Background[i].texture_r = (bool)old_map.Background[i].texture_r;

		// Foregrounds
		new_map.Foreground[i].type = (int)old_map.Foreground[i].type;
		new_map.Foreground[i].x = (int)old_map.Foreground[i].x;
		new_map.Foreground[i].y = (int)old_map.Foreground[i].y;
		new_map.Foreground[i].w = (int)old_map.Foreground[i].w;
		new_map.Foreground[i].h = (int)old_map.Foreground[i].h;
		new_map.Foreground[i].texture_t = (bool)old_map.Foreground[i].texture_t;
		new_map.Foreground[i].texture_b = (bool)old_map.Foreground[i].texture_b;
		new_map.Foreground[i].texture_l = (bool)old_map.Foreground[i].texture_l;
		new_map.Foreground[i].texture_r = (bool)old_map.Foreground[i].texture_r;

		// Events
		new_map.Event[i].type = (int)old_map.Event[i].type;
		new_map.Event[i].x = (int)old_map.Event[i].x;
		new_map.Event[i].y = (int)old_map.Event[i].y;
		new_map.Event[i].w = (int)old_map.Event[i].w;
		new_map.Event[i].h = (int)old_map.Event[i].h;
		new_map.Event[i].texture_t = (bool)old_map.Event[i].texture_t;
		new_map.Event[i].texture_b = (bool)old_map.Event[i].texture_b;
		new_map.Event[i].texture_l = (bool)old_map.Event[i].texture_l;
		new_map.Event[i].texture_r = (bool)old_map.Event[i].texture_r;
	}
	return new_map;
}

void QuickSave(Game* g) {
	if (g->last_save_loc != "") {
		BackupMap(g->last_save_loc.c_str());
		ofstream level_file(g->last_save_loc.c_str(), ios::binary);
		level_file.write((char*)g->lvl, sizeof(Level));
		level_file.close();

		AddChatMessage(g, "__SYSTEM__", 139, 49, 121, "Map Saved!");

		// Reload
		if (al_filename_exists(g->last_save_loc.c_str())) {
			ifstream level_file(g->last_save_loc.c_str(), ios::binary);
			level_file.read((char*)g->lvl, sizeof(Level));
			level_file.close();
		} else {
			AddChatMessage(g, "__SYSTEM__", 139, 49, 121, "Can't reload map.");
		}
	} else {
		std::cout << "No map in Editor.\n";
	}
}

unsigned int __stdcall BrowseFile(void *data){
	Game* g = (Game*)data;
	ALLEGRO_FILECHOOSER* file_dialog;
	if (g->Editor.saving) {
		// Save
		file_dialog = al_create_native_file_dialog(NULL, "Save map...", "*.*", ALLEGRO_FILECHOOSER_SAVE);
		al_show_native_file_dialog(NULL, file_dialog);
		const char* file_path = al_get_native_file_dialog_path(file_dialog, 0);
		if (file_path != NULL) {
			BackupMap(file_path);
			ofstream level_file(file_path, ios::binary);
			level_file.write((char*)g->lvl, sizeof(Level));
			level_file.close();

			g->last_save_loc = file_path;

			AddChatMessage(g, "__SYSTEM__", 139, 49, 121, "Map Saved!");

			// Reload
			if (al_filename_exists(file_path)) {
				ifstream level_file(file_path, ios::binary);
				level_file.read((char*)g->lvl, sizeof(Level));
				level_file.close();
			} else {
				AddChatMessage(g, "__SYSTEM__", 139, 49, 121, "Can't reload map.");
			}
		}
	} else {
		// Load
		file_dialog = al_create_native_file_dialog(NULL, "Load Map...", "*.*", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
		al_show_native_file_dialog(NULL, file_dialog);
		const char* file_path = al_get_native_file_dialog_path(file_dialog, 0);
		if(file_path != NULL){
			if (al_filename_exists(file_path)) {
				ifstream level_file(file_path, ios::binary);
				level_file.read((char*)g->lvl, sizeof(Level));
				level_file.close();

				g->last_save_loc = file_path;

				if (g->lvl->rain) ToggleRain(g, true);

				// Respawn
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
				// Eww lol
			}
		}
	}
	al_destroy_native_file_dialog(file_dialog);
	g->Editor.loading = false;
	g->Editor.saving = false;
	g->Interfaces[INTERFACE_MESSAGE].visible = false;
	return 0;
}

const char* GetCharacterFromKey(int key_id, bool shift) {
	if (shift) { // +SHIFT
		// Letters
		if (key_id == ALLEGRO_KEY_Q) return "Q";
		if (key_id == ALLEGRO_KEY_W) return "W";
		if (key_id == ALLEGRO_KEY_E) return "E";
		if (key_id == ALLEGRO_KEY_R) return "R";
		if (key_id == ALLEGRO_KEY_T) return "T";
		if (key_id == ALLEGRO_KEY_Y) return "Y";
		if (key_id == ALLEGRO_KEY_U) return "U";
		if (key_id == ALLEGRO_KEY_I) return "I";
		if (key_id == ALLEGRO_KEY_O) return "O";
		if (key_id == ALLEGRO_KEY_P) return "P";
		if (key_id == ALLEGRO_KEY_A) return "A";
		if (key_id == ALLEGRO_KEY_S) return "S";
		if (key_id == ALLEGRO_KEY_D) return "D";
		if (key_id == ALLEGRO_KEY_F) return "F";
		if (key_id == ALLEGRO_KEY_G) return "G";
		if (key_id == ALLEGRO_KEY_H) return "H";
		if (key_id == ALLEGRO_KEY_J) return "J";
		if (key_id == ALLEGRO_KEY_K) return "K";
		if (key_id == ALLEGRO_KEY_L) return "L";
		if (key_id == ALLEGRO_KEY_Z) return "Z";
		if (key_id == ALLEGRO_KEY_X) return "X";
		if (key_id == ALLEGRO_KEY_C) return "C";
		if (key_id == ALLEGRO_KEY_V) return "V";
		if (key_id == ALLEGRO_KEY_B) return "B";
		if (key_id == ALLEGRO_KEY_N) return "N";
		if (key_id == ALLEGRO_KEY_M) return "M";

		// Special
		if (key_id == ALLEGRO_KEY_TILDE) return "~";
		if (key_id == ALLEGRO_KEY_1) return "!";
		if (key_id == ALLEGRO_KEY_2) return "@";
		if (key_id == ALLEGRO_KEY_3) return "#";
		if (key_id == ALLEGRO_KEY_4) return "$";
		if (key_id == ALLEGRO_KEY_5) return "%";
		if (key_id == ALLEGRO_KEY_6) return "^";
		if (key_id == ALLEGRO_KEY_7) return "&";
		if (key_id == ALLEGRO_KEY_8) return "*";
		if (key_id == ALLEGRO_KEY_9) return "(";
		if (key_id == ALLEGRO_KEY_0) return ")";
		if (key_id == ALLEGRO_KEY_MINUS) return "_";
		if (key_id == ALLEGRO_KEY_EQUALS) return "+";
		if (key_id == ALLEGRO_KEY_OPENBRACE) return "{";
		if (key_id == ALLEGRO_KEY_CLOSEBRACE) return "}";
		if (key_id == ALLEGRO_KEY_SEMICOLON) return ":";
		if (key_id == ALLEGRO_KEY_QUOTE) return "\"";
		if (key_id == ALLEGRO_KEY_COMMA) return "<";
		if (key_id == ALLEGRO_KEY_FULLSTOP) return ">";
		if (key_id == ALLEGRO_KEY_SLASH) return "?";
	} else { // NO SHIFT
		// Letters
		if (key_id == ALLEGRO_KEY_Q) return "q";
		if (key_id == ALLEGRO_KEY_W) return "w";
		if (key_id == ALLEGRO_KEY_E) return "e";
		if (key_id == ALLEGRO_KEY_R) return "r";
		if (key_id == ALLEGRO_KEY_T) return "t";
		if (key_id == ALLEGRO_KEY_Y) return "y";
		if (key_id == ALLEGRO_KEY_U) return "u";
		if (key_id == ALLEGRO_KEY_I) return "i";
		if (key_id == ALLEGRO_KEY_O) return "o";
		if (key_id == ALLEGRO_KEY_P) return "p";
		if (key_id == ALLEGRO_KEY_A) return "a";
		if (key_id == ALLEGRO_KEY_S) return "s";
		if (key_id == ALLEGRO_KEY_D) return "d";
		if (key_id == ALLEGRO_KEY_F) return "f";
		if (key_id == ALLEGRO_KEY_G) return "g";
		if (key_id == ALLEGRO_KEY_H) return "h";
		if (key_id == ALLEGRO_KEY_J) return "j";
		if (key_id == ALLEGRO_KEY_K) return "k";
		if (key_id == ALLEGRO_KEY_L) return "l";
		if (key_id == ALLEGRO_KEY_Z) return "z";
		if (key_id == ALLEGRO_KEY_X) return "x";
		if (key_id == ALLEGRO_KEY_C) return "c";
		if (key_id == ALLEGRO_KEY_V) return "v";
		if (key_id == ALLEGRO_KEY_B) return "b";
		if (key_id == ALLEGRO_KEY_N) return "n";
		if (key_id == ALLEGRO_KEY_M) return "m";

		// Special
		if (key_id == ALLEGRO_KEY_TILDE) return "`";
		if (key_id == ALLEGRO_KEY_1) return "1";
		if (key_id == ALLEGRO_KEY_2) return "2";
		if (key_id == ALLEGRO_KEY_3) return "3";
		if (key_id == ALLEGRO_KEY_4) return "4";
		if (key_id == ALLEGRO_KEY_5) return "5";
		if (key_id == ALLEGRO_KEY_6) return "6";
		if (key_id == ALLEGRO_KEY_7) return "7";
		if (key_id == ALLEGRO_KEY_8) return "8";
		if (key_id == ALLEGRO_KEY_9) return "9";
		if (key_id == ALLEGRO_KEY_0) return "0";
		if (key_id == ALLEGRO_KEY_MINUS) return "-";
		if (key_id == ALLEGRO_KEY_EQUALS) return "=";
		if (key_id == ALLEGRO_KEY_OPENBRACE) return "[";
		if (key_id == ALLEGRO_KEY_CLOSEBRACE) return "]";
		if (key_id == ALLEGRO_KEY_SEMICOLON) return ";";
		if (key_id == ALLEGRO_KEY_QUOTE) return "'";
		if (key_id == ALLEGRO_KEY_COMMA) return ",";
		if (key_id == ALLEGRO_KEY_FULLSTOP) return ".";
		if (key_id == ALLEGRO_KEY_SLASH) return "/";
	}
	// Not capitalizable
	if (key_id == ALLEGRO_KEY_SPACE) return " ";
	if (key_id == ALLEGRO_KEY_PAD_0) return "0";
	if (key_id == ALLEGRO_KEY_PAD_1) return "1";
	if (key_id == ALLEGRO_KEY_PAD_2) return "2";
	if (key_id == ALLEGRO_KEY_PAD_3) return "3";
	if (key_id == ALLEGRO_KEY_PAD_4) return "4";
	if (key_id == ALLEGRO_KEY_PAD_5) return "5";
	if (key_id == ALLEGRO_KEY_PAD_6) return "6";
	if (key_id == ALLEGRO_KEY_PAD_7) return "7";
	if (key_id == ALLEGRO_KEY_PAD_8) return "8";
	if (key_id == ALLEGRO_KEY_PAD_9) return "9";
	if (key_id == ALLEGRO_KEY_PAD_SLASH) return "/";
	if (key_id == ALLEGRO_KEY_PAD_ASTERISK) return "*";
	if (key_id == ALLEGRO_KEY_PAD_MINUS) return "-";
	if (key_id == ALLEGRO_KEY_PAD_PLUS) return "+";
	if (key_id == ALLEGRO_KEY_PAD_DELETE) return ".";

	return "";
}

void ToClipboard(const std::string &s) {
	OpenClipboard(NULL);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

std::string FromClipboard(){
	OpenClipboard(NULL);
	HANDLE hData = GetClipboardData(CF_TEXT);
	char * pszText = static_cast<char*>(GlobalLock(hData));
	std::string text;
	if (pszText != nullptr) {
		text = pszText;
	} else {
		text = "null";
	}
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}

void RememberEditorState(Game* g) {
	for (int i = 9; i > 0; i--) {
		g->RemEditor[i] = g->RemEditor[i - 1];
		g->RemLevel[i] = g->RemLevel[i - 1];
	}
	g->RemEditor[0] = g->Editor;
	g->RemLevel[0] = *g->lvl;
}

void UndoEditor(Game* g) {
	std::cout << "sizeof(Undo) = " << sizeof(g->RemLevel) + sizeof(g->RemEditor) << "\n";
	g->Editor = g->RemEditor[0];
	memcpy(g->lvl, &g->RemLevel[0], sizeof(Level));
	for (int i = 0; i < 9; i++) {
		g->RemEditor[i] = g->RemEditor[i + 1];
		g->RemLevel[i] = g->RemLevel[i + 1];
	}
}

void RequestBindKeyInput(Game* g, int* target_bind) {
	g->binding_key = target_bind;
	ShowMessage(g, "Press the desired key", "");
	g->Buttons[16].sprite_id = 121;
}

void BindKey(int* target, int key_id) {
	*target = key_id;
}

int* KeyIsBound(Game* g, int key_id) {
	for (int i = 0; i < 8; i++) {
		if (g->AbilitySlots[i].key == key_id) return &g->AbilitySlots[i].key;
	}
	if (g->keys.left_bind == key_id) return &g->keys.left_bind;
	if (g->keys.right_bind == key_id) return &g->keys.right_bind;
	if (g->keys.jump_bind == key_id) return &g->keys.jump_bind;
	if (g->keys.crouch_bind == key_id) return &g->keys.crouch_bind;
	if (g->keys.inventory_bind == key_id) return &g->keys.inventory_bind;
	if (g->keys.skills_bind == key_id) return &g->keys.skills_bind;
	if (g->keys.camera_bind == key_id) return &g->keys.camera_bind;
	if (g->keys.chat_bind == key_id) return &g->keys.chat_bind;
	return NULL;
}