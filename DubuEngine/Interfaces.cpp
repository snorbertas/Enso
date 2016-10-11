#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <sstream>
#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "Interfaces.hpp"
#include "SmallTalk.h"

// Temp
int ChooseEvent(Game* g, int type);

inline const char * const BoolToString(bool b)
{
	return b ? "Yes" : "No";
}

void LoadInterfaces(Game* g){
	g->Interfaces = new Interface[MAX_INTERFACES];
	g->Buttons = new Button[MAX_BUTTONS];
	for (int i = 0; i < MAX_INTERFACES; i++) {
		g->Interfaces[i].sprite_id = 5;
		g->Interfaces[i].visible = false;
		g->Interfaces[i].x = GAME_WIDTH;
		g->Interfaces[i].y = GAME_HEIGHT;
	}
	for (int i = 0; i < MAX_BUTTONS; i++) {
		g->Buttons[i].interface_id = -1;
		g->Buttons[i].sprite_id = 5;
		g->Buttons[i].x = GAME_WIDTH;
		g->Buttons[i].y = GAME_HEIGHT;
		g->Buttons[i].w = 0;
		g->Buttons[i].h = 0;
	}
	int temp_x, temp_y;

	// Interface 0 (Main Menu)
	g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
	NewInterface(&g->Interfaces[INTERFACE_MAIN_MENU], NO_SPRITE, (GAME_WIDTH/2)-(270/2), 210);
	NewButton(&g->Buttons[0], SPRITE_BUTTON_STORY, 0, 0, 270, 50, INTERFACE_MAIN_MENU, 0);			// Singleplayer
	NewButton(&g->Buttons[241], SPRITE_BUTTON_MULTIPLAYER, 0, 55, 270, 50, INTERFACE_MAIN_MENU, 0); // Multiplayer
	NewButton(&g->Buttons[1], SPRITE_BUTTON_OPTIONS, 0, 110, 270, 50, INTERFACE_MAIN_MENU, 0);		// Options
	NewButton(&g->Buttons[242], SPRITE_BUTTON_CREDITS, 0, 165, 270, 50, INTERFACE_MAIN_MENU, 0);	// Credits
	NewButton(&g->Buttons[2], SPRITE_BUTTON_QUIT, 0, 220, 270, 50, INTERFACE_MAIN_MENU, 0);			// Quit
	
	// Interface 3 //Options
	NewInterface(&g->Interfaces[INTERFACE_OPTIONS], SPRITE_INTERFACE_OPTIONS, (GAME_WIDTH/2) - 200, (GAME_HEIGHT/2) - 200);
	NewButton(&g->Buttons[5], 25, 215, 15, 168, 37, INTERFACE_OPTIONS);
	NewButton(&g->Buttons[6], 25, 215, 71, 168, 37, INTERFACE_OPTIONS);
	NewButton(&g->Buttons[249], SPRITE_BUTTON_CONTROLS, 127, 125, 142, 35, INTERFACE_OPTIONS);

	// Interface 4
	temp_x = g->Interfaces[INTERFACE_OPTIONS].x+g->Buttons[5].x;
	temp_y = g->Interfaces[INTERFACE_OPTIONS].y+g->Buttons[5].y;
	NewInterface(&g->Interfaces[4], NO_SPRITE, temp_x, temp_y);
	NewButton(&g->Buttons[8], 25, 0, 0, 168, 37, 4);
	NewButton(&g->Buttons[9], 25, 0, 37, 168, 37, 4);
	NewButton(&g->Buttons[10], 25, 0, 74, 168, 37, 4);
	NewButton(&g->Buttons[11], 25, 0, 111, 168, 37, 4);
	
	// Interface 5
	temp_x = g->Interfaces[INTERFACE_OPTIONS].x+g->Buttons[6].x;
	temp_y = g->Interfaces[INTERFACE_OPTIONS].y+g->Buttons[6].y;
	NewInterface(&g->Interfaces[5], NO_SPRITE, temp_x, temp_y);
	NewButton(&g->Buttons[12], 25, 0, 0, 168, 37, 5);
	NewButton(&g->Buttons[13], 25, 0, 37, 168, 37, 5);
	NewButton(&g->Buttons[14], 25, 0, 74, 168, 37, 5);
	
	// Interface 6
	g->Interfaces[6].visible = true;
	NewInterface(&g->Interfaces[6], NO_SPRITE, 0, 0);

	// Interface 7
	NewInterface(&g->Interfaces[7], 33, GAME_WIDTH - 49, GAME_HEIGHT - 150);

	// Interface 8 Login interface
	NewInterface(&g->Interfaces[INTERFACE_LOGIN], SPRITE_INTERFACE_LOGIN, (GAME_WIDTH / 2) - 190, (GAME_HEIGHT / 2) - 150);
	NewButton(&g->Buttons[247], SPRITE_BUTTON_LOGIN, 97, 100, 270, 50, INTERFACE_LOGIN);

	// Interface 9
	NewInterface(&g->Interfaces[INTERFACE_KEYBOARD], SPRITE_INTERFACE_KEYBOARD, (GAME_WIDTH / 2) - 200, (GAME_HEIGHT / 2) - 200);
	NewButton(&g->Buttons[248], SPRITE_BUTTON_DONE, 127, 256, 142, 35, INTERFACE_KEYBOARD);

	for (int i = 0; i < 8; i++) {
		NewButton(&g->Buttons[250 + i], SPRITE_BUTTON_KEYPICK, 112, 22 + (i * 25), 80, 20, INTERFACE_KEYBOARD);
	}
	for (int i = 0; i < 4; i++) {
		NewButton(&g->Buttons[258 + i], SPRITE_BUTTON_KEYPICK, 294, 22 + (i * 25), 80, 20, INTERFACE_KEYBOARD);
	}
	for (int i = 0; i < 4; i++) {
		NewButton(&g->Buttons[265 + i], SPRITE_BUTTON_KEYPICK, 294, 22 + (4 * 25) + (i * 25), 80, 20, INTERFACE_KEYBOARD);
	}

	// Interface 10
	NewInterface(&g->Interfaces[INTERFACE_PAUSE], 0, 0, 0);
	NewButton(&g->Buttons[262], SPRITE_BUTTON_CONTINUE, (GAME_WIDTH / 2) - (270 / 2), 210, 270, 50, INTERFACE_PAUSE);	// Continue
	NewButton(&g->Buttons[263], SPRITE_BUTTON_OPTIONS, (GAME_WIDTH / 2) - (270 / 2), 210+60, 270, 50, INTERFACE_PAUSE); // Options
	NewButton(&g->Buttons[264], SPRITE_BUTTON_QUIT, (GAME_WIDTH / 2) - (270 / 2), 210+120, 270, 50, INTERFACE_PAUSE);	// Quit


	// Interface 26 Objective
	NewInterface(&g->Interfaces[INTERFACE_OBJECTIVE], NO_SPRITE, 0, 50);
	g->Interfaces[INTERFACE_OBJECTIVE].opacity = 0.8;
	g->Interfaces[INTERFACE_OBJECTIVE].visible = true;

	// Interface 27 Reward
	NewInterface(&g->Interfaces[INTERFACE_REWARD], NO_SPRITE, 0, 50);
	g->Interfaces[INTERFACE_REWARD].visible = true;

	// Interface 28 Map
	NewInterface(&g->Interfaces[INTERFACE_MAP], SPRITE_INTERFACE_MAP, GAME_WIDTH - 222, GAME_HEIGHT - 124);

	// Interface 29 - Chat/Console
	NewInterface(&g->Interfaces[INTERFACE_CHAT], NO_SPRITE, 0, GAME_HEIGHT - 480);

	// Interface 30 HUD - Skills
	NewInterface(&g->Interfaces[INTERFACE_SLOTS], SPRITE_INTERFACE_SLOTS, GAME_WIDTH / 2 - 317 / 2, GAME_HEIGHT - 44);
	NewButton(&g->Buttons[243], SPRITE_BUTTON_INVENTORY, 350, 0, 44, 44, INTERFACE_SLOTS);
	NewButton(&g->Buttons[244], SPRITE_BUTTON_STATS, 400, 0, 44, 44, INTERFACE_SLOTS);
	NewButton(&g->Buttons[245], SPRITE_BUTTON_SKILLS, 450, 0, 44, 44, INTERFACE_SLOTS);
	NewButton(&g->Buttons[246], SPRITE_BUTTON_QUESTS, 500, 0, 44, 44, INTERFACE_SLOTS);

	// Interface 31 HUD - Player
	NewInterface(&g->Interfaces[INTERFACE_PLAYER], SPRITE_INTERFACE_PLAYER, 0, GAME_HEIGHT - 100);

	// Interface 32 Abilities
	NewInterface(&g->Interfaces[INTERFACE_ABILITIES], SPRITE_INTERFACE_BOOKS, 100, 100);

	// Interface 33 Inventory
	NewInterface(&g->Interfaces[INTERFACE_INVENTORY], SPRITE_INTERFACE_INVENTORY, 1000, 100);

	// Interface 34 Tutorial
	NewInterface(&g->Interfaces[INTERFACE_TUTORIAL], SPRITE_INTERFACE_TUTORIAL, GAME_WIDTH/2 - 173, 50);

	// Interface 35 Tutorial2
	NewInterface(&g->Interfaces[INTERFACE_TUTORIAL2], SPRITE_INTERFACE_TUTORIAL2, GAME_WIDTH / 2 - 173, 50);
	g->Interfaces[INTERFACE_TUTORIAL2].opacity = 0.5;

	// Interface 80 (Editor)
	NewInterface(&g->Interfaces[INTERFACE_EDITOR], SPRITE_INTERFACE_EDITOR, 0, 0);
	NewButton(&g->Buttons[17], SPRITE_BUTTON_E_NEW, 18, 88, 78, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[18], SPRITE_BUTTON_E_LOAD, 102, 88, 78, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[19], SPRITE_BUTTON_E_SAVE, 186, 88, 78, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[20], SPRITE_BUTTON_E_SELECT, 18, 178, 34, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[21], SPRITE_BUTTON_E_PLATFORM, 56, 178, 34, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[22], SPRITE_BUTTON_E_BACKGROUND, 94, 178, 34, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[23], SPRITE_BUTTON_E_EVENT, 170, 178, 34, 34, INTERFACE_EDITOR);
	NewButton(&g->Buttons[186], SPRITE_BUTTON_E_FOREGROUND, 132, 178, 34, 34, INTERFACE_EDITOR);

	// Interface 81 (Platforms page 1)
	NewInterface(&g->Interfaces[81], SPRITE_INTERFACE_EDITOR_B, 0, 0);
	temp_x = 17;
	temp_y = 218;
	for (int i = 0; i < 54; i++) {
		NewButton(&g->Buttons[24+i], SPRITE_BUTTON_E_BLOCK, temp_x, temp_y, 34, 34, 81);  // 77th button
		if (i == 0) {
			NewButton(&g->Buttons[24 + i], SPRITE_BUTTON_E_NTY, temp_x, temp_y, 34, 34, 81);
		}
		temp_x += 42;
		if (temp_x >= 38 * 6 + 17) {
			temp_x = 17;
			temp_y += 38;
		}
	}
	 
	// Interface 82 (Background)
	NewInterface(&g->Interfaces[82], SPRITE_INTERFACE_EDITOR_B, 0, 0);
	temp_x = 17;
	temp_y = 218;
	for (int i = 0; i < 54; i++) {
		NewButton(&g->Buttons[78 + i], SPRITE_BUTTON_E_BLOCK, temp_x, temp_y, 34, 34, 82);  // 131st button
		if (i == 0) {
			NewButton(&g->Buttons[78 + i], SPRITE_BUTTON_E_NTY, temp_x, temp_y, 34, 34, 82);
		}
		temp_x += 42;
		if (temp_x >= 38 * 6 + 17) {
			temp_x = 17;
			temp_y += 38;
		}
	}

	// Interface 83 (Objects/Events)
	NewInterface(&g->Interfaces[83], SPRITE_INTERFACE_EDITOR_B, 0, 0);
	temp_x = 17;
	temp_y = 218;
	for (int i = 0; i < 54; i++) {
		NewButton(&g->Buttons[132 + i], SPRITE_BUTTON_E_BLOCK, temp_x, temp_y, 34, 34, 83);  // 185th button
		if (i == 0) {
			NewButton(&g->Buttons[132 + i], SPRITE_BUTTON_E_NTY, temp_x, temp_y, 34, 34, 83);
		}
		temp_x += 42;
		if (temp_x >= 38 * 6 + 17) {
			temp_x = 17;
			temp_y += 38;
		}
	}

	// Interface 84 (Foreground)
	NewInterface(&g->Interfaces[84], SPRITE_INTERFACE_EDITOR_B, 0, 0);
	temp_x = 17;
	temp_y = 218;
	for (int i = 0; i < 54; i++) {
		NewButton(&g->Buttons[187 + i], SPRITE_BUTTON_E_BLOCK, temp_x, temp_y, 34, 34, 84);  //240th button
		if (i == 0) {
			NewButton(&g->Buttons[187 + i], SPRITE_BUTTON_E_NTY, temp_x, temp_y, 34, 34, 84);
		}
		temp_x += 42;
		if (temp_x >= 38 * 6 + 17) {
			temp_x = 17;
			temp_y += 38;
		}
	}
	// Interface 90 (Map debugging)
	NewInterface(&g->Interfaces[INTERFACE_EDITOR_DEBUG], NO_SPRITE, 0, 0);

	// Interface 99 (on-top)
	temp_x = GAME_WIDTH / 2 - 250;
	temp_y = GAME_HEIGHT / 2 - 84;
	NewInterface(&g->Interfaces[INTERFACE_MESSAGE], SPRITE_INTERFACE_MESSAGE, temp_x, temp_y);
	NewButton(&g->Buttons[16], SPRITE_BUTTON_OKAY, 195, 110, 112, 43, INTERFACE_MESSAGE);
	g->Message1 = "Hello, this is the first line of the message";
	g->Message2 = "and this is the second line of memes ex dee";
}

void NewInterface(Interface* in, int sprite, int x, int y){
	in->sprite_id = sprite;
	in->x = x;
	in->y = y;
	
}

void NewButton(Button* b, int sprite, int x, int y, int w, int h, int iid, float opacity){
	b->sprite_id = sprite;
	b->x = x;
	b->y = y;
	b->w = w;
	b->h = h;
	b->interface_id = iid; // This is the interface id that the button belongs to
	b->opacity = opacity;
}

void RenderInterfaces(Game* g, MoreSprites* spritos, ALLEGRO_FONT** font){
	// Temp fix
	ALLEGRO_BITMAP** img_sprite = spritos->img_sprite;
	ALLEGRO_BITMAP** img_icon = spritos->img_icons;
	ALLEGRO_BITMAP** img_background = spritos->img_background;

	// Render Interfaces
	for(int i = 0; i < MAX_INTERFACES; i++){
		if(g->Interfaces[i].visible == true){
			if (i == 99) {
				DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, 0.75);
			}
			DrawImage(g, img_sprite[g->Interfaces[i].sprite_id], g->Interfaces[i].x, g->Interfaces[i].y, 0, g->Interfaces[i].opacity);
			for(int j = 0; j < MAX_BUTTONS; j++){
				if(g->Buttons[j].interface_id == i){
					if (g->Buttons[j].visible) {
						if (g->pos_x > g->Buttons[j].x + g->Interfaces[i].x &&
							g->pos_x < g->Buttons[j].x + g->Buttons[j].w + g->Interfaces[i].x &&
							g->pos_y > g->Buttons[j].y + g->Interfaces[i].y &&
							g->pos_y < g->Buttons[j].y + g->Buttons[j].h + g->Interfaces[i].y &&
							InterfaceIsOnTop(g, i)) {
							DrawImage(g, img_sprite[g->Buttons[j].sprite_id + 1],
								g->Buttons[j].x + g->Interfaces[i].x,
								g->Buttons[j].y + g->Interfaces[i].y, 0, g->Buttons[j].opacity);
						}
						else {
							DrawImage(g, img_sprite[g->Buttons[j].sprite_id],
								g->Buttons[j].x + g->Interfaces[i].x,
								g->Buttons[j].y + g->Interfaces[i].y, 0, g->Buttons[j].opacity);
						}
					}
				}
			}
			
			float r_x = (float)g->s_x/(float)GAME_WIDTH;
			float r_y = (float)g->s_y/(float)GAME_HEIGHT;
			switch(i){
				case 3:
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 222, g->Interfaces[i].y + 22, ALLEGRO_ALIGN_LEFT, "%i x %i", g->s_x, g->s_y);
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 222, g->Interfaces[i].y + 78, ALLEGRO_ALIGN_LEFT, GetWindowMode(g->window_mode));
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 22, g->Interfaces[i].y + 20, ALLEGRO_ALIGN_LEFT, "Resolution:");
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 22, g->Interfaces[i].y + 75, ALLEGRO_ALIGN_LEFT, "Window Size:");
					break;
				case 4:	
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 7, ALLEGRO_ALIGN_LEFT, "1024 x 576");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 44, ALLEGRO_ALIGN_LEFT, "1280 x 720");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 81, ALLEGRO_ALIGN_LEFT, "1600 x 900");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 118, ALLEGRO_ALIGN_LEFT, "1920 x 1080");
					break;
				case 5:
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 7, ALLEGRO_ALIGN_LEFT, "Window");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 44, ALLEGRO_ALIGN_LEFT, "Fullscreen");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 81, ALLEGRO_ALIGN_LEFT, "Borderless");
					break;
				case 6: {
					DrawText(font[2], 255, 255, 255, 0, 0, ALLEGRO_ALIGN_LEFT, ENGINE_VERSION);
					int tick_diff = (int)((g->new_time - g->old_time) * 1000);
					int r = 0;
					int gr = 255;
					int b = 0;
					if (tick_diff > (int)(1000 / TPS)) {
						r = 255;
						gr = 0;
					} else if (tick_diff < (int)(1000 / TPS)) {
						gr = 0;
						b = 255;
					}
					DrawText(font[2], r, gr, b, 0, 15, ALLEGRO_ALIGN_LEFT, "Tick interval: %i/%ims", tick_diff, (1000/TPS));
					if (g->connected) {
						DrawText(font[2], 0, 0, 0, 0, 30, ALLEGRO_ALIGN_LEFT, "Ping: %i", g->ping);
					}
					break;
				}
				case 7:
					g->music_volume_cd--;
					if (g->music_volume_cd <= 0) g->Interfaces[i].visible = false;
					DrawRectangle(g, g->Interfaces[i].x+3, g->Interfaces[i].y+146, 43, 0-g->music_volume*100, 132, 187, 234);
					break;
				case 8:
				{

					DrawText(font[1], 0, 0, 0, g->Interfaces[i].x + 110, g->Interfaces[i].y + 22, ALLEGRO_ALIGN_LEFT, g->logini.username_input.c_str());
					DrawText(font[1], 0, 0, 0, g->Interfaces[i].x + 110, g->Interfaces[i].y + 64, ALLEGRO_ALIGN_LEFT, g->logini.password_input_mask.c_str());
					int input_lenght = 0;
					if (g->logini.type_username) {
						input_lenght = al_get_text_width(font[5], g->logini.username_input.c_str());
					} else {
						input_lenght = al_get_text_width(font[5], g->logini.password_input_mask.c_str());
					}
					if (g->logini.type_indicator_count < 60 && !g->logini.logging_in) {
						if (g->logini.type_username) {
							DrawRectangle(g, g->Interfaces[i].x + 110 + input_lenght, g->Interfaces[i].y + 22, 2, 16, 255, 255, 255);
						} else {
							DrawRectangle(g, g->Interfaces[i].x + 110 + input_lenght, g->Interfaces[i].y + 64, 2, 16, 255, 255, 255);
						}
					}
					if (g->logini.logging_in) {
						DrawText(font[0], 10, 10, 205,
							g->Interfaces[INTERFACE_LOGIN].x + 185,
							g->Interfaces[INTERFACE_LOGIN].y + 110,
							ALLEGRO_ALIGN_CENTER, "Connecting to server...");
						DrawRotatedImage(g, img_sprite[34],
							g->Interfaces[INTERFACE_LOGIN].x + 185,
							g->Interfaces[INTERFACE_LOGIN].y + 185,
							g->logini.loading_angle, 0);
						g->logini.loading_angle+=3;
						if (g->logini.loading_angle > 360) g->logini.loading_angle = 0;
					}
					break;
				}
				case INTERFACE_KEYBOARD:
				{
					for (int i = 0; i < 8; i++) {
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 24,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (i * 25),
							ALLEGRO_ALIGN_LEFT, "Skill Slot %i:", i+1);
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 150,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (i * 25),
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->AbilitySlots[i].key));
					}
						int offset = 0;
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21,
							ALLEGRO_ALIGN_LEFT, "Move Left:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.left_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Move Right:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.right_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Jump:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.jump_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Action:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.crouch_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Camera:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.camera_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Inventory:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.inventory_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Skills:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.skills_bind));
						DrawText(font[2], 0, 0, 0,
							g->Interfaces[INTERFACE_KEYBOARD].x + 202,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
							ALLEGRO_ALIGN_LEFT, "Chat:");
						DrawText(font[2], 255, 255, 255,
							g->Interfaces[INTERFACE_KEYBOARD].x + 333,
							g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
							ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.chat_bind));
					break;
				}
				case INTERFACE_PAUSE:
				{
					// Calculate bg
					int bg_width = al_get_bitmap_width(img_sprite[SPRITE_INTERFACE_PAUSE]) - GAME_WIDTH;
					int bg_height = bg_width / 16 * 9;
					DrawScaledImage(g, img_sprite[SPRITE_INTERFACE_PAUSE], 0, 0, -bg_width, -bg_height, 0);
					break;
				}
				case INTERFACE_OBJECTIVE:
				{
					DrawObjectiveDetails(g, spritos);
					for (int j = 0; j < MAX_OBJECTIVES; j++) {
						if (g->objectives[j].open == true) {
							DrawImage(g, img_sprite[SPRITE_INTERFACE_OBJECTIVE],
								0 + g->objectives[j].x, g->Interfaces[INTERFACE_OBJECTIVE].y + g->objectives[j].y,
								0, g->Interfaces[INTERFACE_OBJECTIVE].opacity);
							float opac = 0.0;
							int opaci = g->objectives[j].timer;
							if (g->objectives[j].timer < (255 / 2)) {
								opaci = 255 - (g->objectives[j].timer * 2);
							} else {
								opaci = ((g->objectives[j].timer - (255 / 2)) * 2);
							}
							opac = ((float)opaci / 256.0);
							DrawTextA(font[9], opac, opac, 0,
								4 + g->objectives[j].x, g->Interfaces[INTERFACE_OBJECTIVE].y + g->objectives[j].y + 2,
								ALLEGRO_ALIGN_LEFT, opac, g->objectives[j].line[0].c_str());
							DrawTextA(font[9], opac, opac, opac,
								4 + g->objectives[j].x, g->Interfaces[INTERFACE_OBJECTIVE].y + g->objectives[j].y + 16,
								ALLEGRO_ALIGN_LEFT, opac, g->objectives[j].line[1].c_str());
							DrawTextA(font[9], opac, opac, opac,
								4 + g->objectives[j].x, g->Interfaces[INTERFACE_OBJECTIVE].y + g->objectives[j].y + 30,
								ALLEGRO_ALIGN_LEFT, opac, g->objectives[j].line[2].c_str());
						}
					}
					break;
				}
				case INTERFACE_REWARD:
				{
					int reward_x = g->pl.x + g->camera.x - 80;
					int reward_y = g->pl.y + g->camera.y - 10;
					for (int j = MAX_REWARDS - 1; j >= 0; j--)
					{
						RewardInterface* ReIn = &g->rewards[j];
						if (ReIn->state == REWARD_STATE_OPEN) {
							// Render it
							float opacity = ((float)ReIn->timer / (float)SecondsToTicks(5.0));
							int re = ReIn->r;
							int gr = ReIn->g;
							int bl = ReIn->b;
							reward_y -= 45;
							DrawImage(g, spritos->img_sprite[SPRITE_INTERFACE_REWARD], reward_x, reward_y, 0, opacity);
							DrawTextA(font[9],
								re, gr, bl,
								reward_x, reward_y,
								ALLEGRO_ALIGN_LEFT, opacity, ReIn->msg.c_str());
						}
					}
					break;
				}
				case 29:
					if (g->chat.show_chat && g->scene == 1) {
						DrawRectangle(g, g->Interfaces[i].x + 2, g->Interfaces[i].y + 2, 500 - 4, 400 - 4, 0, 0, 0, 0.3);
						DrawRectangle(g, g->Interfaces[i].x, g->Interfaces[i].y, 500, 400, 0, 0, 0, 0.1);
						DrawRectangle(g, g->Interfaces[i].x + 2, g->Interfaces[i].y + 378, 500 - 4, 20, 0, 0, 0, 0.3);
						DrawText(font[1], 255, 255, 255,
							g->Interfaces[i].x + 2,
							g->Interfaces[i].y + 376,
							ALLEGRO_ALIGN_LEFT, "%s: %s", g->pl.name.c_str(), g->chat.msg_input.c_str());
						int input_lenght = al_get_text_width(font[5], g->pl.name.c_str())
							+ al_get_text_width(font[5], ": ")
							+ al_get_text_width(font[5], g->chat.msg_input.c_str());
						if (g->chat.type_chat && g->chat.chat_indicator_count < 60) {
							DrawRectangle(g, g->Interfaces[i].x + 2 + input_lenght, g->Interfaces[i].y + 380, 2, 16, 255, 255, 255);
						}

						// Chat messages
						for (int j = 0; j < MAX_CHAT_MESSAGES; j++) {
							DrawText(font[1], g->chat.messages[j].r, g->chat.messages[j].g, g->chat.messages[j].b,
								g->Interfaces[i].x + 2, g->Interfaces[i].y + 358 - (j * 18), ALLEGRO_ALIGN_LEFT,
								g->chat.messages[j].parent.c_str());
							int msg_lenght = al_get_text_width(font[5], g->chat.messages[j].parent.c_str());
							DrawText(font[1], 255, 255, 255, g->Interfaces[i].x + 2 + msg_lenght, g->Interfaces[i].y + 358 - (j * 18),
								ALLEGRO_ALIGN_LEFT,	g->chat.messages[j].message.c_str());
						}
					} else if (g->chat.ghost > 0) {
						// Chat messages
						for (int j = 0; j < MAX_CHAT_MESSAGES; j++) {
							DrawText(font[1], g->chat.messages[j].r, g->chat.messages[j].g, g->chat.messages[j].b,
								g->Interfaces[i].x + 2, g->Interfaces[i].y + 358 - (j * 18), ALLEGRO_ALIGN_LEFT,
								g->chat.messages[j].parent.c_str());
							int msg_lenght = al_get_text_width(font[5], g->chat.messages[j].parent.c_str());
							DrawText(font[1], 255, 255, 255, g->Interfaces[i].x + 2 + msg_lenght, g->Interfaces[i].y + 358 - (j * 18),
								ALLEGRO_ALIGN_LEFT, g->chat.messages[j].message.c_str());
						}
					}
					break;
				case INTERFACE_SLOTS:
					// Tooltip
					for (int j = 0; j < 8; j++) {
						if (g->AbilitySlots[j].ability.id != -1) {
							DrawImage(g, img_icon[g->AbilitySlots[j].ability.sprite_id], g->Interfaces[i].x + 5 + (j * 39), g->Interfaces[i].y + 5, 0);
						}
					}
					for (int j = 0; j < 8; j++) {
						if(MouseIsOn(g, g->Interfaces[i].x + 5 + (j * 39), g->Interfaces[i].y + 5, 34, 34)){
							if (g->AbilitySlots[j].ability.id != -1) {
								DrawSkillTooltip(g, font, &g->AbilitySlots[j].ability);
							}
						}
					}
					for (int j = 0; j < 8; j++) {
						DrawText(font[3], 255, 255, 255,
							g->Interfaces[i].x + 5 + (j * 39),
							g->Interfaces[i].y + 2,
							ALLEGRO_ALIGN_LEFT, GetKeyName(g->AbilitySlots[j].key));
					}

					if(MouseIsOn(g, g->Interfaces[i].x + 350, g->Interfaces[i].y, 44, 44)){
						DrawRectangle(g, g->pos_x, g->pos_y - 20, 183, 20, 100, 100, 100, 0.1);
						DrawRectangle(g, g->pos_x + 1, g->pos_y - 19, 181, 18, 0, 0, 0, 0.7);
						DrawText(font[2], 255, 255, 255, g->pos_x + 2, g->pos_y - 20, ALLEGRO_ALIGN_LEFT, "Inventory (CTRL+I)");
					}
					if (MouseIsOn(g, g->Interfaces[i].x + 400, g->Interfaces[i].y, 44, 44)) {
						DrawRectangle(g, g->pos_x, g->pos_y - 20, 145, 20, 100, 100, 100, 0.1);
						DrawRectangle(g, g->pos_x + 1, g->pos_y - 19, 143, 18, 0, 0, 0, 0.7);
						DrawText(font[2], 255, 255, 255, g->pos_x + 2, g->pos_y - 20, ALLEGRO_ALIGN_LEFT, "Stats (CTRL+S)");
					}
					if (MouseIsOn(g, g->Interfaces[i].x + 450, g->Interfaces[i].y, 44, 44)) {
						DrawRectangle(g, g->pos_x, g->pos_y - 20, 183, 20, 100, 100, 100, 0.1);
						DrawRectangle(g, g->pos_x + 1, g->pos_y - 19, 181, 18, 0, 0, 0, 0.7);
						DrawText(font[2], 255, 255, 255, g->pos_x + 2, g->pos_y - 20, ALLEGRO_ALIGN_LEFT, "Abilities (CTRL+A)");
					}
					if (MouseIsOn(g, g->Interfaces[i].x + 500, g->Interfaces[i].y, 44, 44)) {
						DrawRectangle(g, g->pos_x, g->pos_y - 20, 153, 20, 100, 100, 100, 0.1);
						DrawRectangle(g, g->pos_x + 1, g->pos_y - 19, 151, 18, 0, 0, 0, 0.7);
						DrawText(font[2], 255, 255, 255, g->pos_x + 2, g->pos_y - 20, ALLEGRO_ALIGN_LEFT, "Quests (CTRL+Q)");
					}
					break;
				case 31:
					if (g->pl.health > 0) {
						DrawImage(g, img_sprite[SPRITE_HEALTH_0], g->Interfaces[i].x +  40, g->Interfaces[i].y +  33, 0);
						int health_length = (float)(g->pl.health / 100.0) * 380;
						DrawScaledImage(g, img_sprite[SPRITE_HEALTH_1], g->Interfaces[i].x + 50, g->Interfaces[i].y + 33, health_length, 0, 0);
						DrawImage(g, img_sprite[SPRITE_HEALTH_2], g->Interfaces[i].x + 50 + health_length, g->Interfaces[i].y + 33, 0);
					}
					if (g->pl.energy > 0 && g->story.missions[0].prog >= 19) {
						DrawImage(g, img_sprite[SPRITE_MANA_0], g->Interfaces[i].x + 40, g->Interfaces[i].y + 53, 0);
						int energy_length = (float)(g->pl.energy / (float)(g->pl.max_energy + g->pl.bonus_energy)) * 380;
						DrawScaledImage(g, img_sprite[SPRITE_MANA_1], g->Interfaces[i].x + 50, g->Interfaces[i].y + 53, energy_length, 0, 0);
						DrawImage(g, img_sprite[SPRITE_MANA_2], g->Interfaces[i].x + 50 + energy_length, g->Interfaces[i].y + 53, 0);
					}
					if (g->pl.exp > 0) {
						DrawImage(g, img_sprite[SPRITE_EXP_0], g->Interfaces[i].x + 40, g->Interfaces[i].y + 73, 0);
						int exp_length = (float)(g->pl.exp / (float)(ExpReq(g->pl.level))) * 380;
						DrawScaledImage(g, img_sprite[SPRITE_EXP_1], g->Interfaces[i].x + 50, g->Interfaces[i].y + 73, exp_length, 0, 0);
						DrawImage(g, img_sprite[SPRITE_EXP_2], g->Interfaces[i].x + 50 + exp_length, g->Interfaces[i].y + 73, 0);
					}
					DrawText(font[2], 0, 0, 0, g->Interfaces[31].x + 235, g->Interfaces[31].y + 73, ALLEGRO_ALIGN_CENTER, "Level: %i", g->pl.level);
					break;
				case INTERFACE_ABILITIES:
				{
					if (g->AbilityBooks[0].book_id != -1) {
						DrawImage(g, img_icon[g->AbilityBooks[0].sprite_id], g->Interfaces[i].x + 14, g->Interfaces[i].y + 44, 0);
						if (g->AbilityBooks[0].active == true) {
							DrawOutline(g, g->Interfaces[i].x + 14, g->Interfaces[i].y + 44, 34, 34, 255, 255, 255, 2);
						}
					}
					if (g->AbilityBooks[1].book_id != -1) {
						DrawImage(g, img_icon[g->AbilityBooks[1].sprite_id], g->Interfaces[i].x + 57, g->Interfaces[i].y + 44, 0);
						if (g->AbilityBooks[1].active == true) {
							DrawOutline(g, g->Interfaces[i].x + 57, g->Interfaces[i].y + 44, 34, 34, 255, 255, 255, 2);
						}
					}
					if (g->AbilityBooks[2].book_id != -1) {
						DrawImage(g, img_icon[g->AbilityBooks[2].sprite_id], g->Interfaces[i].x + 100, g->Interfaces[i].y + 44, 0);
						if (g->AbilityBooks[2].active == true) {
							DrawOutline(g, g->Interfaces[i].x + 100, g->Interfaces[i].y + 44, 34, 34, 255, 255, 255, 2);
						}
					}
					if (g->AbilityBooks[3].book_id != -1) {
						DrawImage(g, img_icon[g->AbilityBooks[3].sprite_id], g->Interfaces[i].x + 143, g->Interfaces[i].y + 44, 0);
						if (g->AbilityBooks[3].active == true) {
							DrawOutline(g, g->Interfaces[i].x + 143, g->Interfaces[i].y + 44, 34, 34, 255, 255, 255, 2);
						}
					}
					if (g->AbilityBooks[4].book_id != -1) {
						if (g->AbilityBooks[4].active == true) {
							DrawOutline(g, g->Interfaces[i].x + 185, g->Interfaces[i].y + 44, 34, 34, 255, 255, 255, 2);
						}
					}

					// Abilities + tooltip
					int active_book = GetActiveBookID(g);
					int slot = 0;
					for (int j = 0; j < 4; j++) {
						for (int k = 0; k < 5; k++) {
							Ability ability = GetAbility(active_book, slot);
							if (ability.id != -1) {
								DrawImage(g, img_icon[ability.sprite_id], g->Interfaces[i].x + 15 + (k * 42), g->Interfaces[i].y + 98 + (j * 43), 0);
							}
							slot++;
						}
					}
					slot = 0;
					for (int j = 0; j < 4; j++) {
						for (int k = 0; k < 5; k++) {
							Ability ability = GetAbility(active_book, slot);
							if (ability.id != -1) {
								if(MouseIsOn(g, g->Interfaces[i].x + 15 + (k * 42), g->Interfaces[i].y + 98 + (j * 43), 34, 34)){
									DrawSkillTooltip(g, font, &ability);
								}
							}
							slot++;
						}
					}
				}
					break;
				case INTERFACE_INVENTORY:
				{
					// Inventory
					int slot = 0;
					for (int j = 0; j < 4; j++) {
						for (int k = 0; k < 5; k++) {
							if (g->Inventory[slot].id != -1) {
								DrawImage(g, spritos->img_icons[g->Inventory[slot].sprite_id], g->Interfaces[i].x + 14 + (k * 42), g->Interfaces[i].y + 44 + (j * 43), 0);
							}
							slot++;
						}
					}
					slot = 0;
					for (int j = 0; j < 4; j++) {
						for (int k = 0; k < 5; k++) {
							if (g->Inventory[slot].id != -1) {
								if (MouseIsOn(g, g->Interfaces[i].x + 14 + (k * 42), g->Interfaces[i].y + 44 + (j * 43), 34, 34)) {
									DrawItemTooltip(g, font, &g->Inventory[slot]);
								}
							}
							slot++;
						}
					}

					// Equipped
					for (int j = 0; j < 3; j++) {
						if (g->Inventory[slot].id != -1) {
							DrawImage(g, spritos->img_icons[g->Inventory[slot].sprite_id], g->Interfaces[i].x + 35 + (j * 64), g->Interfaces[i].y + 248, 0);
						}
						slot++;
					}
					slot -= 3;
					for (int j = 0; j < 3; j++) {
						if (g->Inventory[slot].id != -1) {
							if (MouseIsOn(g, g->Interfaces[i].x + 35 + (j * 64), g->Interfaces[i].y + 248, 34, 34)) {
								DrawItemTooltip(g, font, &g->Inventory[slot]);
							}
						}
						slot++;
					}

					// Dragging Item
					if (g->DraggingItem.id != -1) {
						DrawImage(g, spritos->img_icons[g->DraggingItem.sprite_id], g->pos_x - 17, g->pos_y - 17, 0, 0.5);
					}
				}
					break;
				case INTERFACE_TUTORIAL:
					// Draw depending on proggress...
					if (g->story.missions[0].prog == 2 || g->story.missions[0].prog == 3) {
						if (g->story.missions[0].var1) {
							DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY + 1], g->Interfaces[i].x + 50, g->Interfaces[i].y + 25, 0);
						} else {
							DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY], g->Interfaces[i].x + 50, g->Interfaces[i].y + 25, 0);
						}
						if (g->story.missions[0].var2) {
							DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY + 1], g->Interfaces[i].x + 200, g->Interfaces[i].y + 25, 0);
						} else {
							DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY], g->Interfaces[i].x + 200, g->Interfaces[i].y + 25, 0);
						}

						// Temp solution to arrow key symbols
						if (GetKeyName(g->keys.left_bind) == "Left") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 50, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.left_bind) == "Right") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 50, g->Interfaces[i].y + 25, 1);
						} else if (GetKeyName(g->keys.left_bind) == "Up") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 50, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.left_bind) == "Down") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 50, g->Interfaces[i].y + 25, ALLEGRO_FLIP_VERTICAL);
						} else {
							DrawText(font[3], 255, 255, 255, g->Interfaces[i].x + 100, g->Interfaces[i].y + 35, ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.left_bind));
						}
						if (GetKeyName(g->keys.right_bind) == "Left") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 200, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.right_bind) == "Right") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 200, g->Interfaces[i].y + 25, 1);
						} else if (GetKeyName(g->keys.right_bind) == "Up") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 200, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.right_bind) == "Down") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 200, g->Interfaces[i].y + 25, ALLEGRO_FLIP_VERTICAL);
						} else {
							DrawText(font[3], 255, 255, 255, g->Interfaces[i].x + 250, g->Interfaces[i].y + 35, ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.right_bind));
						}
						DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 115, ALLEGRO_ALIGN_CENTER, "Use %s/%s keys to move", GetKeyName(g->keys.left_bind), GetKeyName(g->keys.right_bind));
					}
					if (g->story.missions[0].prog == 6 || g->story.missions[0].prog == 7) {
						if (g->story.missions[0].var3) {
							DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY + 1], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);
						} else {
							DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);
						}

						// Temp solution to arrow key symbols
						if (GetKeyName(g->keys.jump_bind) == "Left") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.jump_bind) == "Right") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 1);
						} else if (GetKeyName(g->keys.jump_bind) == "Up") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.jump_bind) == "Down") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, ALLEGRO_FLIP_VERTICAL);
						} else {
							DrawText(font[3], 255, 255, 255, g->Interfaces[i].x + 178, g->Interfaces[i].y + 35, ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.jump_bind));
						}
						DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 115, ALLEGRO_ALIGN_CENTER, "Use %s key to jump up", GetKeyName(g->keys.jump_bind));
					}
					if (g->story.missions[0].prog == 13) {
						DrawImage(g, img_sprite[SPRITE_TUTORIAL_KEY], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);

						// Temp solution to arrow key symbols
						if (GetKeyName(g->keys.crouch_bind) == "Left") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.crouch_bind) == "Right") {
							DrawImage(g, img_sprite[SPRITE_KEY_LEFT], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 1);
						} else if (GetKeyName(g->keys.crouch_bind) == "Up") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, 0);
						} else if (GetKeyName(g->keys.crouch_bind) == "Down") {
							DrawImage(g, img_sprite[SPRITE_KEY_UP], g->Interfaces[i].x + 128, g->Interfaces[i].y + 25, ALLEGRO_FLIP_VERTICAL);
						} else {
							DrawText(font[3], 255, 255, 255, g->Interfaces[i].x + 178, g->Interfaces[i].y + 35, ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.crouch_bind));
						}
						DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 115, ALLEGRO_ALIGN_CENTER, "Use %s key to open doors", GetKeyName(g->keys.crouch_bind));
					}
					break;
				case INTERFACE_TUTORIAL2:
					// Draw depending on proggress...
					if (g->story.missions[0].prog == 10 || g->story.missions[0].prog == 11 && g->Interfaces[INTERFACE_INVENTORY].visible == false) {
						DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Open your inventory");
						if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
							DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW], g->Interfaces[30].x + 350, g->Interfaces[30].y - 70, 0, 0.8);
						}
					}
					if (g->story.missions[0].prog == 11 && g->Interfaces[INTERFACE_INVENTORY].visible == true) {
						DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Drag the Torch to the Equip slot");
						if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
							DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW],
								g->Interfaces[INTERFACE_INVENTORY].x + 30, g->Interfaces[INTERFACE_INVENTORY].y + 170, 0, 0.8);
						}
					}
					if (g->story.missions[0].prog == 18) {
						int x = g->camera.x;
						int y = g->camera.y;
						if (OnAWall(g) && g->pl.y <= -80) {
							DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Now jump again!");
							if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
								DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW_H], 1610 + x, -310 + y, 1, 0.8);
							}
						} else if (GetFloorID(g) == 8){
							g->convo_with = &g->monster[0];
							DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Good job!");
							if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
								DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW], g->convo_with->x + x, g->convo_with->y - 70 + y, 0, 0.8);
							}
						} else {
							DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Jump on the wall.");
							if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
								DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW_H], 1590 + x, -140 + y, 0, 0.8);
							}
						}
					}
					if (g->story.missions[0].prog >= 22 && g->story.missions[0].prog <= 32) {
						int x = g->monster[0].x + g->camera.x;
						int y = g->monster[0].y + g->camera.y - 100;
						DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Follow Asuwish.");
						if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
							DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW], x, y, 0, 0.8);
						}
					}
					if (g->story.missions[0].prog == 38) {
						if (!g->Interfaces[INTERFACE_ABILITIES].visible) {
							DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Open your Skills");
							if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
								DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW], g->Interfaces[30].x + 450, g->Interfaces[30].y - 70, 0, 0.8);
							}
						} else if (GetActiveBookID(g) != BOOK_NETIKI) {
							DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Choose Book of Netiki");
							if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
								DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW],
									g->Interfaces[INTERFACE_ABILITIES].x + 10, g->Interfaces[INTERFACE_ABILITIES].y - 30, 0, 0.8);
							}
						} else {
							DrawText(font[3], 0, 0, 0, g->Interfaces[i].x + 175, g->Interfaces[i].y + 25, ALLEGRO_ALIGN_CENTER, "Drag <Basic Slash> here");
							if (g->story.missions[0].timer > SecondsToTicks(0.5)) {
								DrawImage(g, img_sprite[SPRITE_INTERFACE_ARROW],
									g->Interfaces[30].x, g->Interfaces[30].y - 70, 0, 0.8);
							}
						}
					}
					break;
				case 80: {
					// Editor
					if (g->Editor.edit_mode) {
						int player_x = g->pl.x;
						int player_y = g->pl.y;
						int hover_x = g->pos_x - g->camera.x;
						int hover_y = g->pos_y - g->camera.y;
						int cam_x = -g->camera.x;
						int cam_y = -g->camera.y;
						DrawRectangle(g, 0, GAME_HEIGHT - 20, GAME_WIDTH, 20, 0, 0, 0, 0.5);
						DrawText(font[1], 255, 255, 255, GAME_WIDTH - 600, GAME_HEIGHT - 22, ALLEGRO_ALIGN_LEFT,
							"player_x:%i, player_y:%i, x:%i, y:%i, cam_x:%i, cam_y:%i", player_x, player_y, hover_x, hover_y, cam_x, cam_y);
						if (g->Editor.selection_tool_type == 2) {
							DrawText(font[1], 0, 255, 0, 18, 604, ALLEGRO_ALIGN_LEFT,
								"[Platform] (X:%i, Y:%i)", (int)g->Editor.plt->x, (int)g->Editor.plt->y);
							DrawText(font[1], 255, 255, 255, 18, 625, ALLEGRO_ALIGN_LEFT,
								"Platform ID: %i", g->Editor.select_id);
							DrawText(font[1], 255, 255, 255, 18, 646, ALLEGRO_ALIGN_LEFT,
								"Platform Type: %i", g->Editor.plt->type);
							DrawText(font[1], 255, 255, 255, 18, 667, ALLEGRO_ALIGN_LEFT,
								"Width: %i", g->Editor.plt->w);
							DrawText(font[1], 255, 255, 255, 18, 688, ALLEGRO_ALIGN_LEFT,
								"Height: %i", g->Editor.plt->h);
						}
						if (g->Editor.selection_tool_type == 3) {
							DrawText(font[1], 0, 255, 0, 18, 604, ALLEGRO_ALIGN_LEFT,
								"[Background] (X:%i, Y:%i)", (int)g->Editor.bg->x, (int)g->Editor.bg->y);
							DrawText(font[1], 255, 255, 255, 18, 625, ALLEGRO_ALIGN_LEFT,
								"Background ID: %i", g->Editor.select_id);
							DrawText(font[1], 255, 255, 255, 18, 646, ALLEGRO_ALIGN_LEFT,
								"Background Type: %i", g->Editor.bg->type);
							DrawText(font[1], 255, 255, 255, 18, 667, ALLEGRO_ALIGN_LEFT,
								"Width: %i", g->Editor.bg->w);
							DrawText(font[1], 255, 255, 255, 18, 688, ALLEGRO_ALIGN_LEFT,
								"Height: %i", g->Editor.bg->h);
						}
						if (g->Editor.selection_tool_type == 4) {
							DrawText(font[1], 0, 255, 0, 18, 604, ALLEGRO_ALIGN_LEFT,
								"[Foreground] (X:%i, Y:%i)", (int)g->Editor.fg->x, (int)g->Editor.fg->y);
							DrawText(font[1], 255, 255, 255, 18, 625, ALLEGRO_ALIGN_LEFT,
								"Foreground ID: %i", g->Editor.select_id);
							DrawText(font[1], 255, 255, 255, 18, 646, ALLEGRO_ALIGN_LEFT,
								"Foreground Type: %i", g->Editor.fg->type);
							DrawText(font[1], 255, 255, 255, 18, 667, ALLEGRO_ALIGN_LEFT,
								"Width: %i", g->Editor.fg->w);
							DrawText(font[1], 255, 255, 255, 18, 688, ALLEGRO_ALIGN_LEFT,
								"Height: %i", g->Editor.fg->h);
						}
						if (g->Editor.selection_tool_type == 5) {
							DrawText(font[1], 0, 255, 0, 18, 604, ALLEGRO_ALIGN_LEFT,
								"[Event] (X:%i, Y:%i)", (int)g->Editor.ev->x, (int)g->Editor.ev->y);
							DrawText(font[1], 255, 255, 255, 18, 625, ALLEGRO_ALIGN_LEFT,
								"Event ID: %i", g->Editor.select_id);
							DrawText(font[1], 255, 255, 255, 18, 646, ALLEGRO_ALIGN_LEFT,
								"Event Type: %i", g->Editor.ev->type);
							DrawText(font[1], 255, 255, 255, 18, 667, ALLEGRO_ALIGN_LEFT,
								"Width: %i", g->Editor.ev->w);
							DrawText(font[1], 255, 255, 255, 18, 688, ALLEGRO_ALIGN_LEFT,
								"Height: %i", g->Editor.ev->h);
						}
					}
					break;
				}
				case 81: {
					int temp_x = 20;
					int temp_y = 221;
					for (int j = 0; j < 54; j++) {
						if (j == 1) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 122, 220, 85);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Grass");
						}
						if (j == 2) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 214, 194, 173);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Sand");
						}
						if (j == 3) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 187, 189, 191);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Mist");
						}
						if (j == 4) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 200, 200, 200);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Invis");
						}
						temp_x += 42;
						if (temp_x >= 38 * 6 + 20) {
							temp_x = 20;
							temp_y += 38;
						}
					}
					break;
				}
				case 82: {
					int temp_x = 20;
					int temp_y = 221;
					for (int j = 0; j < 54; j++) {
						if (j == 1) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 195, 153, 107);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Fence");
						}
						if (j == 2) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 0, 0, 0);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Black");
						}
						if (j == 3) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 127, 127, 127);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "House");
						}
						if (j == 4) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 50, 200, 50);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Tree");
						}
						if (j == 5) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 60, 250, 0);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Grass");
						}
						if (j == 6) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 150, 100, 10);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Rock");
						}
						if (j == 7) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 224, 209, 175);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Wall");
						}
						if (j == 8) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 200, 180, 160);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Brick");
						}
						if (j == 9) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 180, 120, 80);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Roof");
						}
						if (j == 10) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 160, 110, 60);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Door");
						}
						if (j == 11) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 200, 240, 240);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Glass");
						}
						if (j == 12) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 230, 100, 20);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Misc");
						}
						temp_x += 42;
						if (temp_x >= 38 * 6 + 20) {
							temp_x = 20;
							temp_y += 38;
						}
					}
					break;
				}
				case 83: {
					int temp_x = 20;
					int temp_y = 221;
					for (int j = 0; j < 54; j++) {
						if (j == 1) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 195, 153, 107);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Play");
						}
						if (j == 2) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 168, 223, 236);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Skel");
						}
						if (j == 3) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 240, 200, 0);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Butt");
						}
						if (j == 4) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 180, 230, 30);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Slime");
						}
						if (j == 5) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 22, 194, 3);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "EXP");
						}
						if (j == 6) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 200, 200, 200);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Bat");
						}
						temp_x += 42;
						if (temp_x >= 38 * 6 + 20) {
							temp_x = 20;
							temp_y += 38;
						}
					}
					break;
				}
				case 84: {
					int temp_x = 20;
					int temp_y = 221;
					for (int j = 0; j < 54; j++) {
						if (j == 1) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 195, 153, 107);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Fence");
						}
						if (j == 2) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 168, 223, 236);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Water");
						}
						if (j == 3) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 127, 127, 127);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "House");
						}
						if (j == 4) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 50, 200, 50);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Tree");
						}
						if (j == 5) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 60, 250, 0);
							DrawText(font[4], 0, 0, 0, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Grass");
						}
						if (j == 6) {
							DrawRectangle(g, temp_x, temp_y, 28, 28, 150, 100, 10);
							DrawText(font[4], 255, 255, 255, temp_x, temp_y, ALLEGRO_ALIGN_LEFT, "Rock");
						}
						temp_x += 42;
						if (temp_x >= 38 * 6 + 20) {
							temp_x = 20;
							temp_y += 38;
						}
					}
					break;
				}
				case INTERFACE_EDITOR_DEBUG:
				{
					DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 2, 10, 30);
					if (g->Editor.debug_type >=1 && g->Editor.debug_type <= 4) {
						// Pointer to the specific map object
						Platform* temp_object = g->lvl->Platform;
						if (g->Editor.debug_type == 2) temp_object = g->lvl->Background;
						if (g->Editor.debug_type == 3) temp_object = g->lvl->Foreground;
						if (g->Editor.debug_type == 4) temp_object = g->lvl->Event;

						// Dimensions
						int x_offset = 40;
						int y_offset = 42;
						int columns = 11;
						int rows = MAX_PLATFORMS;
						if (g->Editor.debug_type == 2) rows = MAX_LV_BACKGROUNDS;
						if (g->Editor.debug_type == 3) rows = MAX_FOREGROUNDS;
						if (g->Editor.debug_type == 4) rows = MAX_EVENTS;
						int table_width = GAME_WIDTH - x_offset * 2;
						int table_height = GAME_HEIGHT - y_offset * 2;
						int column_width = table_width / columns;
						int row_height = 22;

						// Bottom Layer
						DrawRectangle(g, x_offset, y_offset, table_width, table_height, 150, 150, 150);
						DrawRectangle(g, x_offset + table_width, y_offset, 10, table_height, 100, 100, 100);
						DrawRectangle(g, x_offset + table_width, y_offset + (((float)g->Editor.debug_scroll/(float)rows) * table_height), 10, 17, 240, 240, 240);
						DrawRectangle(g, x_offset + column_width * 10, y_offset, column_width + 2, table_height, 150, 175, 150);

						// Middle Layer (Values)
						if (g->Editor.debug_scroll < 0) g->Editor.debug_scroll = 0;
						if (g->Editor.debug_scroll > rows - 28) g->Editor.debug_scroll = rows - 28;
						for (int r = 0; r < 28; r++) {
							int id = (r + g->Editor.debug_scroll);
							DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "%i", id);
							if (temp_object[id].type == -1 && temp_object[id].x == -9999 && temp_object[id].y == -9999 &&
								temp_object[id].w == 32 && temp_object[id].h == 32 &&
								temp_object[id].texture_t && temp_object[id].texture_b &&
								temp_object[id].texture_l && temp_object[id].texture_r) {

								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "Blank");
							} else {
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", temp_object[id].type);
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 2), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", (int)temp_object[id].x);
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 3), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", (int)temp_object[id].y);
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 4), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", temp_object[id].w);
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 5), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", temp_object[id].h);
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 6), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(temp_object[id].texture_t));
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 7), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(temp_object[id].texture_b));
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 8), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(temp_object[id].texture_l));
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 9), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(temp_object[id].texture_r));
							}
							// Action
							if (MouseIsOn(g, x_offset + (column_width * 10), y_offset + row_height + (r * row_height), column_width, row_height)) {
								DrawText(font[2], 100, 0, 0, x_offset + (column_width / 2) + (column_width * 10), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_CENTER, "Reset");
							}
						}

						// Top Layer
						DrawRectangle(g, 0, 0, GAME_WIDTH, y_offset, 2, 10, 30);
						if (g->Editor.debug_type == 1) DrawText(font[3], 1, 0, 255, 0, 0, ALLEGRO_ALIGN_LEFT, "Debugging: PLATFORMS");
						if (g->Editor.debug_type == 2) DrawText(font[3], 1, 0, 255, 0, 0, ALLEGRO_ALIGN_LEFT, "Debugging: BACKGROUNDS");
						if (g->Editor.debug_type == 3) DrawText(font[3], 1, 0, 255, 0, 0, ALLEGRO_ALIGN_LEFT, "Debugging: FOREGROUNDS");
						if (g->Editor.debug_type == 4) DrawText(font[3], 1, 0, 255, 0, 0, ALLEGRO_ALIGN_LEFT, "Debugging: EVENTS");
						if (g->Editor.debug_type == 5) DrawText(font[3], 1, 0, 255, 0, 0, ALLEGRO_ALIGN_LEFT, "Debugging: MISC/SETTINGS");
						DrawText(font[3], 0, 255, 0, GAME_WIDTH - 1, 0, ALLEGRO_ALIGN_RIGHT, "F7 - Platforms | F8 - Backgrounds | F9 - Foregrounds | F10 - Events | F11 - Misc");
						DrawRectangle(g, 0, GAME_HEIGHT - y_offset, GAME_WIDTH, y_offset, 2, 10, 30);
						DrawRectangle(g, x_offset, y_offset, table_width, row_height, 10, 10, 10);
						DrawOutline(g, x_offset, y_offset, table_width, row_height, 200, 200, 200, 2);
						for (int i = 0; i < columns - 1; i += 2) {
							DrawOutline(g, x_offset + column_width + (i * column_width), y_offset, column_width, table_height, 100, 100, 100, 1);
						}
						for (int i = 0; i < (table_height / row_height) - 1; i += 2) {
							DrawOutline(g, x_offset, y_offset + (row_height * 2) + (i * row_height), table_width, row_height, 100, 100, 100, 1);
						}
						DrawOutline(g, x_offset, y_offset, table_width, table_height, 200, 200, 200, 2);
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2), y_offset, ALLEGRO_ALIGN_CENTER, "Index/ID");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 1, y_offset, ALLEGRO_ALIGN_CENTER, "Type");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 2, y_offset, ALLEGRO_ALIGN_CENTER, "x");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 3, y_offset, ALLEGRO_ALIGN_CENTER, "y");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 4, y_offset, ALLEGRO_ALIGN_CENTER, "Width");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 5, y_offset, ALLEGRO_ALIGN_CENTER, "Height");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 6, y_offset, ALLEGRO_ALIGN_CENTER, "Top");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 7, y_offset, ALLEGRO_ALIGN_CENTER, "Bottom");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 8, y_offset, ALLEGRO_ALIGN_CENTER, "Left");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 9, y_offset, ALLEGRO_ALIGN_CENTER, "Right");
						DrawText(font[2], 0, 50, 0, x_offset + (column_width / 2) + column_width * 10, y_offset, ALLEGRO_ALIGN_CENTER, "Action");
					} else if (g->Editor.debug_type == 5) {
						// Dimensions
						int x_offset = 40;
						int y_offset = 42;
						int columns = 2;
						int rows = MAX_PLATFORMS;
						rows = 60;
						int table_width = GAME_WIDTH - x_offset * 2;
						int table_height = GAME_HEIGHT - y_offset * 2;
						int column_width = table_width / columns;
						int row_height = 22;

						// Bottom Layer
						DrawRectangle(g, x_offset, y_offset, table_width, table_height, 150, 150, 150);
						DrawRectangle(g, x_offset + table_width, y_offset, 10, table_height, 100, 100, 100);
						DrawRectangle(g, x_offset + table_width, y_offset + (((float)g->Editor.debug_scroll/(float)rows) * table_height), 10, 17, 240, 240, 240);
						DrawRectangle(g, x_offset + column_width * 3, y_offset, column_width + 2, table_height, 150, 175, 150);

						// Middle Layer (Values)
						if (g->Editor.debug_scroll < 0) g->Editor.debug_scroll = 0;
						if (g->Editor.debug_scroll > rows - 28) g->Editor.debug_scroll = rows - 28;
						for (int r = 0; r < 28; r++) {
							int id = (r + g->Editor.debug_scroll);
							if (id == 0) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Ground Level");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->ground_level);
							}
							if (id == 1) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Left Bound");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->left_bound);
							}
							if (id == 2) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Right Bound");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->right_bound);
							}
							if (id == 3) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Background[0]");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->background_0);
							}
							if (id == 4) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Background[1]");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->background_1);
							}
							if (id == 5) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Background[2]");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->background_2);
							}
							if (id == 6) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Night");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(g->lvl->night));
							}
							if (id == 7) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Rain");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(g->lvl->rain));
							}
							if (id == 8) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Thunder");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(g->lvl->thunder));
							}
							if (id == 9) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "Fog");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%s", BoolToString(g->lvl->fog));
							}
							if (id == 10) {
								DrawText(font[2], 0, 0, 0, x_offset, y_offset + row_height + (r * row_height), ALLEGRO_ALIGN_LEFT, "ID");
								DrawText(font[2], 0, 0, 0, x_offset + (column_width * 1), y_offset + row_height + (r * row_height),
									ALLEGRO_ALIGN_LEFT, "%i", g->lvl->map_id);
							}
						}

						// Top Layer
						DrawRectangle(g, 0, 0, GAME_WIDTH, y_offset, 2, 10, 30);
						DrawText(font[3], 1, 0, 255, 0, 0, ALLEGRO_ALIGN_LEFT, "Debugging: MISC/SETTINGS");
						DrawText(font[3], 0, 255, 0, GAME_WIDTH - 1, 0, ALLEGRO_ALIGN_RIGHT, "F7 - Platforms | F8 - Backgrounds | F9 - Foregrounds | F10 - Events | F11 - Misc");
						DrawRectangle(g, 0, GAME_HEIGHT - y_offset, GAME_WIDTH, y_offset, 2, 10, 30);
						DrawRectangle(g, x_offset, y_offset, table_width, row_height, 10, 10, 10);
						DrawOutline(g, x_offset, y_offset, table_width, row_height, 200, 200, 200, 2);
						for (int i = 0; i < columns - 1; i += 2) {
							DrawOutline(g, x_offset + column_width + (i * column_width), y_offset, column_width, table_height, 100, 100, 100, 1);
						}
						for (int i = 0; i < (table_height / row_height) - 1; i += 2) {
							DrawOutline(g, x_offset, y_offset + (row_height * 2) + (i * row_height), table_width, row_height, 100, 100, 100, 1);
						}
						DrawOutline(g, x_offset, y_offset, table_width, table_height, 200, 200, 200, 2);
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2), y_offset, ALLEGRO_ALIGN_CENTER, "Setting");
						DrawText(font[2], 255, 255, 255, x_offset + (column_width / 2) + column_width * 1, y_offset, ALLEGRO_ALIGN_CENTER, "Value");
					}
					break;
				}
				case 99:
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 250, g->Interfaces[i].y + 20, ALLEGRO_ALIGN_CENTER, g->Message1);
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 250, g->Interfaces[i].y + 52, ALLEGRO_ALIGN_CENTER, g->Message2);
					break;
			}
		}
		// Dragging ability
		if (g->DraggingAbility.id != -1) {
			DrawImage(g, img_icon[g->DraggingAbility.sprite_id], g->pos_x - 17, g->pos_y - 17, 0, 0.5);
		}
	}

	// Fade
	if (g->fade_in > SecondsToTicks(0.1)) {
		float a = ((float)(g->fade_in - SecondsToTicks(0.1)) / (float)SecondsToTicks(2.0));
		DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, a);
	}
	if (g->fade_out > SecondsToTicks(0.1)) {
		float a = ((float)(g->fade_out - SecondsToTicks(0.1)) / (float)SecondsToTicks(2.0));
		DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0, 1-a);
	} else if (g->fade_out > 0) {
		DrawRectangle(g, 0, 0, GAME_WIDTH, GAME_HEIGHT, 0, 0, 0);
	}

	// Cursor
	if (g->Editor.camera_drag) {
		DrawImage(g, img_sprite[56], g->pos_x, g->pos_y, 0); // Cursor
	} else if (g->Editor.edit_mode && g->Editor.tool_id >= 2) {
		DrawImage(g, img_sprite[74], g->pos_x - 11, g->pos_y - 11, 0); // Cursor
	} else if (g->Editor.edit_mode && (g->Editor.target_resize_w || g->Editor.target_resize_h)) {
		// N/A
	} else {
		DrawImage(g, img_sprite[7], g->pos_x, g->pos_y, 0); // Cursor
	}

	// Temp
	RenderCutScene(g);
}

const char* GetWindowMode(int wid){
	if(wid == ALLEGRO_WINDOWED) return "Window";
	if(wid == ALLEGRO_FULLSCREEN) return "Fullscreen";
	if(wid == ALLEGRO_FULLSCREEN_WINDOW) return "Borderless";
	return "???";
}

bool InterfaceIsOnTop(Game* g, int id) {
	int top_i = -1;
	for (int i = 0; i < MAX_INTERFACES; i++) {
		if (g->Interfaces[i].visible == true) {
			if(i != 6 && i != 2 && i != 7 && i != 31 && i != 32 && i != 33 && i != 29 && i != 27 && i != 28 && i != 34 && i != 35 && i != 26) top_i = i;
		}
	}
	if (top_i == id) {
		return true;
	} else {
		return false;
	}
}

void HideAllInterfaces(Game* g, int except) {
	for (int i = 0; i < MAX_INTERFACES; i++) {
		if(i != except) g->Interfaces[i].visible = false;
	}
}

void HandleInterfaces(Game* g) {
	g->fade_in--;
	if (g->fade_in < 0) g->fade_in = 0;
	g->fade_out--;
	if (g->fade_out < 0) g->fade_out = 0;

	if (g->InterfaceDrag[0] == true) {
		g->Interfaces[INTERFACE_ABILITIES].x = g->pos_x - 115;
		g->Interfaces[INTERFACE_ABILITIES].y = g->pos_y - 7;
	}
	if (g->InterfaceDrag[1] == true) {
		g->Interfaces[INTERFACE_INVENTORY].x = g->pos_x - 115;
		g->Interfaces[INTERFACE_INVENTORY].y = g->pos_y - 7;
	}

	HandleChatInterface(g);
	HandleLoginInterface(g);	
}

void HandleChatInterface(Game* g) {
	g->chat.chat_indicator_count++;
	if (g->chat.chat_indicator_count >= 120) g->chat.chat_indicator_count = 0;
	if (g->chat.backspace_timer > 0) g->chat.backspace_timer--;
	if (g->chat.ghost > 0) g->chat.ghost--;
	if (g->keys.backspace) {
		if (g->chat.type_chat && g->chat.show_chat && g->chat.backspace_timer <= 0) {
			g->chat.backspace_timer_wait--;
			if (g->chat.backspace_timer_wait <= 0) {
				if (g->chat.msg_input != "") {
					g->chat.msg_input.pop_back();
					g->chat.backspace_timer = 5;
				}
			}
		}
	}
}

void HandleLoginInterface(Game* g) {
	if (!g->logini.logging_in) {
		g->logini.type_indicator_count++;
		if (g->logini.type_indicator_count >= 120) g->logini.type_indicator_count = 0;
		if (g->logini.backspace_timer > 0) g->logini.backspace_timer--;
		if (g->keys.backspace) {
			if (g->Interfaces[INTERFACE_LOGIN].visible && g->logini.backspace_timer <= 0) {
				g->logini.backspace_timer_wait--;
				if (g->logini.backspace_timer_wait <= 0) {
					if (g->logini.type_username == true) {
						if (g->logini.username_input != "") {
							g->logini.username_input.pop_back();
							g->logini.backspace_timer = 5;
						}
					}
					else {
						if (g->logini.password_input != "") {
							g->logini.password_input.pop_back();
							g->logini.password_input_mask.pop_back();
							g->logini.backspace_timer = 5;
						}
					}
				}
			}
		}
	}
}

void DrawSkillTooltip(Game *g, ALLEGRO_FONT** font, Ability* a) {
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;
	DrawRectangle(g, g->pos_x, g->pos_y - 40, 283, 40, 100, 100, 100, 0.1);
	DrawRectangle(g, g->pos_x + 1, g->pos_y - 39, 281, 38, 0, 0, 0, 0.7);
	DrawText(font[3], a->r, a->g, a->b, g->pos_x + 2, g->pos_y - 43, ALLEGRO_ALIGN_LEFT, "%s", a->name);
	DrawText(font[2], 255, 255, 255, g->pos_x + 2, g->pos_y - 21, ALLEGRO_ALIGN_LEFT, "%s", a->description);
}

void DrawItemTooltip(Game *g, ALLEGRO_FONT** font, Item* i) {
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;
	DrawRectangle(g, g->pos_x, g->pos_y - 40, 283, 40, 100, 100, 100, 0.1);
	DrawRectangle(g, g->pos_x + 1, g->pos_y - 39, 281, 38, 0, 0, 0, 0.7);
	DrawText(font[3], i->r, i->g, i->b, g->pos_x + 2, g->pos_y - 43, ALLEGRO_ALIGN_LEFT, "%s", i->name);
	DrawText(font[2], 255, 255, 255, g->pos_x + 2, g->pos_y - 21, ALLEGRO_ALIGN_LEFT, "%s", i->description);
}

const char* GetKeyName(int key_id) {
	if (key_id == -1) return "(none)";
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
	if (key_id == ALLEGRO_KEY_LEFT) return "Left";
	if (key_id == ALLEGRO_KEY_RIGHT) return "Right";
	if (key_id == ALLEGRO_KEY_UP) return "Up";
	if (key_id == ALLEGRO_KEY_DOWN) return "Down";
	if (key_id == ALLEGRO_KEY_SPACE) return "Space";
	if (key_id == ALLEGRO_KEY_ENTER) return "Enter";
	return "???";
}

void AddChatMessage(Game* g, const char* parent, int re, int gr, int bl, const char* msg) {
	g->chat.ghost = SecondsToTicks(5);
	for (int i = MAX_CHAT_MESSAGES - 2; i >= 0; i--) {
		g->chat.messages[i + 1].message = g->chat.messages[i].message;
		g->chat.messages[i + 1].parent = g->chat.messages[i].parent;
		g->chat.messages[i + 1].r = g->chat.messages[i].r;
		g->chat.messages[i + 1].g = g->chat.messages[i].g;
		g->chat.messages[i + 1].b = g->chat.messages[i].b;
	}
	if ((std::string)parent == "__SYSTEM__") {
		g->chat.messages[0].parent = msg;
		g->chat.messages[0].message = "";
		g->chat.messages[0].r = re;
		g->chat.messages[0].g = gr;
		g->chat.messages[0].b = bl;
	} else {
		g->chat.messages[0].parent = ((std::string)parent + ": ").c_str();
		g->chat.messages[0].message = msg;
		g->chat.messages[0].r = re;
		g->chat.messages[0].g = gr;
		g->chat.messages[0].b = bl;
	}
}

void SubmitMessage(Game* g, const char* msg) {
	if (msg[0] == '/') {
		HandleCommand(g, msg);
	} else {
		if (g->connected) {
			AddPacketToQueue(g, MakePacket(PACKET_TYPE_CHAT_MESSAGE, g->pl.name.c_str(), msg));
		} else {
			AddChatMessage(g, g->pl.name.c_str(), 100, 100, 100, msg);
		}
	}
}

#define SYSTEM_COLOUR 139, 49, 121
void HandleCommand(Game* g, const char* msg) {
	if ((std::string)msg == "/version") {
		AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ((std::string)"Running: " + ENGINE_VERSION).c_str());
	}
	else {
		std::string type;
		std::string command = msg;
		std::istringstream args(command);

		args >> type;
		if (type == "/tele") {
			// Teleport
			int x = 0;
			int y = 0;
			args >> x;
			args >> y;
			g->pl.x = x;
			g->pl.y = y;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Teleported to: " + std::to_string(x) + ", " + std::to_string(y)).c_str());
		} else if (type == "/fixplatforms"){
			int index_start = -1;
			int index_end = -1;
			args >> index_start;
			args >> index_end;
			if (index_start > -1 && index_end > -1 && index_end < MAX_PLATFORMS) {
				int count = 0;
				for (int i = index_start; i < index_end + 1; i++) {
					g->lvl->Platform[i] = Platform();
					count++;
				}
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Done! Fixed " + std::to_string(count) + " platforms").c_str());
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid range. Please use: start > -1, end < 1000");
			}
		} else if (type == "/fixbackgrounds") {
			int index_start = -1;
			int index_end = -1;
			args >> index_start;
			args >> index_end;
			if (index_start > -1 && index_end > -1 && index_end < MAX_LV_BACKGROUNDS) {
				int count = 0;
				for (int i = index_start; i < index_end + 1; i++) {
					g->lvl->Background[i] = Platform();
					count++;
				}
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Done! Fixed " + std::to_string(count) + " backgrounds").c_str());
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid range. Please use: start > -1, end < 1000");
			}
		} else if (type == "/fixforegrounds") {
			int index_start = -1;
			int index_end = -1;
			args >> index_start;
			args >> index_end;
			if (index_start > -1 && index_end > -1 && index_end < MAX_FOREGROUNDS) {
				int count = 0;
				for (int i = index_start; i < index_end + 1; i++) {
					g->lvl->Foreground[i] = Platform();
					count++;
				}
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Done! Fixed " + std::to_string(count) + " foregrounds").c_str());
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid range. Please use: start > -1, end < 1000");
			}
		} else if (type == "/fixevents") {
			int index_start = -1;
			int index_end = -1;
			args >> index_start;
			args >> index_end;
			if (index_start > -1 && index_end > -1 && index_end < MAX_EVENTS) {
				int count = 0;
				for (int i = index_start; i < index_end + 1; i++) {
					g->lvl->Event[i] = Platform();
					count++;
				}
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Done! Fixed " + std::to_string(count) + " events").c_str());
			}
			else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid range. Please use: start > -1, end < 100");
			}
		} else if (type == "/setid"){
			args >> g->lvl->map_id;
		} else if (type == "/help") {
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "List of usable commands:");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/help");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/version");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/getpos");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/tele <x> <y>");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/anim <type>");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/name <name>");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/setbg <layer> <id>");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/setground <y>");
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "/sprite <id> <eyes>");
		} else if (type == "/o1") {
			NewObjective(g, 1, "OBJECTIVE: Collect 1 experience point.", "This is an Alpha test, have fun.", "Kthxbye.");
		} else if (type == "/o2") {
			NewObjective(g, 2, "I am the meme king", "Please enjoy xD", "Hi");
		} else if (type == "/o3") {
			NewObjective(g, 3, "Test #3, lets go with next one", "Please", "mhm");
		} else if (type == "/o4") {
			NewObjective(g, 4, "Here we goooo44444");
		} else if (type == "/r1") {
			NewReward(g, "Obtained: x1 Meme", 255, 255, 255);
		} else if (type == "/r2") {
			NewReward(g, "Obtained: 22222222", 255, 0, 0);
		} else if (type == "/r3") {
			NewReward(g, "Obtdr5: 14 yhgfh", 0, 255, 0);
		} else if (type == "/thunderon") {
			ToggleThunder(g, true);
		} else if (type == "/thunderoff") {
			ToggleThunder(g, false);
		} else if (type == "/rainon") {
			ToggleRain(g, true);
		} else if (type == "/rainoff") {
			ToggleRain(g, false);
		} else if (type == "/getpos") {
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Current coordinates: x=" + std::to_string((int)g->pl.x) + ", y=" + std::to_string((int)g->pl.y)).c_str());
		} else if (type == "/name") {
			args >> g->pl.name;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed player name to: " + g->pl.name).c_str());
			DEPacket pname = MakePacket(PACKET_TYPE_PLAYER_NAME, "", g->pl.name.c_str());
			AddPacketToQueue(g, pname);
		} else if (type == "/item") {
			int item_id = -1;
			args >> item_id;
			if (item_id > -1) {
				AddItemToInventory(g, g->ItemDB->item[item_id]);
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Addded item to inventory: " + std::to_string(item_id)).c_str());
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid item ID.");
			}
		} else if (type == "/setground"){
			args >> g->lvl->ground_level;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed ground level to: " + std::to_string(g->lvl->ground_level)).c_str());
		} else if (type == "/setboundl") {
			args >> g->lvl->left_bound;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed left bound to: " + std::to_string(g->lvl->left_bound)).c_str());
		} else if (type == "/setboundr") {
			args >> g->lvl->right_bound;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed right bound to: " + std::to_string(g->lvl->right_bound)).c_str());
		} else if (type == "/sprite") {
			args >> g->pl.sprite;
			int eyes = 8;
			args >> eyes;
			g->pl.eyes = eyes;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed player sprite to: " + std::to_string(g->pl.sprite)).c_str());
		} else if (type == "/hair") {
			args >> g->pl.hair;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed player hair sprite to: " + std::to_string(g->pl.hair)).c_str());
		} else if (type == "/outfit") {
			args >> g->pl.outfit;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed player outfit sprite to: " + std::to_string(g->pl.outfit)).c_str());
		} else if (type == "/acc") {
			args >> g->pl.acc;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed player acc sprite to: " + std::to_string(g->pl.acc)).c_str());
			
		} else if (type == "/wipenpc") {
			DestroyAllExperience(g);
			WipeNPC(g);
		} else if (type == "/npc") {
			int npc_id = 0;
			args >> npc_id;
			SpawnNPC(g, g->NPCDB->NPC[npc_id], g->pl.x, g->pl.y);
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Spawned NPC: " + std::to_string(npc_id)).c_str());
		} else if (type == "/setbg") {
			int layer = 0;
			int bg_id = 0;
			args >> layer;
			args >> bg_id;
			if (layer == 0) g->lvl->background_0 = bg_id;
			if (layer == 1) g->lvl->background_1 = bg_id;
			if (layer == 2) g->lvl->background_2 = bg_id;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed background[" + std::to_string(layer) + "] to: " + std::to_string(bg_id)).c_str());
		} else if (type == "/checkprog") {
			int index = 0;
			args >> index;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Mission[" + std::to_string(index) + "].prog: " + std::to_string(g->story.missions[index].prog)).c_str());
		} else if (type == "/setprog") {
			int index = 0;
			args >> index;
			args >> g->story.missions[index].prog;
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Changed mission[" + std::to_string(index) + "].prog to: " + std::to_string(g->story.missions[index].prog)).c_str());
		} else if (type == "/anim") {
			int atype = 0;
			args >> atype;
			int result = StartAnimation(g, atype, g->pl.x, g->pl.y, g->pl.direction, true, 0);
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Spawned animation " + std::to_string(atype) + " in array [" + std::to_string(result) + "]").c_str());
		} else if (type == "/convo") {
			// Nothing now
		} else if(type == "/smalltalk"){
			int id = 0;
			args >> id;
			g->convo_with = &g->monster[0];
			SmallTalk(g, id);
		} else if (type == "/fogon") {
			g->lvl->fog = true;
		} else if (type == "/fogoff") {
			g->lvl->fog = false;
		} else if (type == "/night") {
			g->lvl->night = true;
		} else if (type == "/day") {
			g->lvl->night = false;
		} else if (type == "/reborn"){
			HandleExperienceSpawns(g);
			HandleEventNPCSpawns(g);
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "NPC/Exp spawns have been respawned.");
		} else if (type == "/warp") {
			std::string temp;
			args >> temp;
			if (al_filename_exists(temp.c_str())) {
				ChangeMap(g, temp.c_str());
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("The map " + temp + " is not found").c_str());
			}
		} else if (type == "/etool" && g->Editor.edit_mode) {
			int event_id = 0;
			args >> event_id;
			if (event_id >= 100) {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Picked event tool type: " + std::to_string(event_id)).c_str());
				ChooseEvent(g, event_id);
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Event type must be 100 or hhigher.");
			}
		} else if (type == "/cmap_a") { // a0.8.1 to a0.8.2
			std::string file_name = "null";
			std::string file_path = "";
			args >> file_name;
			if (file_name != "null") {
				// Old version map
				Level_a0_8_1* old_map = new Level_a0_8_1;

				// Open the old version map
				file_path = "maps/_cmap/" + file_name;
				if (al_filename_exists(file_path.c_str())) {
					ALLEGRO_FILE* file = al_fopen(file_path.c_str(), "r");
					al_fread(file, old_map, sizeof(Level_a0_8_1));
					al_fclose(file);
				} else {
					AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid file");
				}

				// Reconstruct the new version
				Level_a0_8_2 new_map;
				for (int i = 0; i < 1000; i++) {
					// Platforms
					new_map.Platform[i].type = (int)old_map->Platform[i].type;
					new_map.Platform[i].x = (int)old_map->Platform[i].x;
					new_map.Platform[i].y = (int)old_map->Platform[i].y;
					new_map.Platform[i].w = (int)old_map->Platform[i].w;
					new_map.Platform[i].h = (int)old_map->Platform[i].h;
					new_map.Platform[i].texture_t = (bool)old_map->Platform[i].texture_t;
					new_map.Platform[i].texture_b = (bool)old_map->Platform[i].texture_b;
					new_map.Platform[i].texture_l = (bool)old_map->Platform[i].texture_l;
					new_map.Platform[i].texture_r = (bool)old_map->Platform[i].texture_r;

					// Backgrounds
					new_map.Background[i].type = (int)old_map->Background[i].type;
					new_map.Background[i].x = (int)old_map->Background[i].x;
					new_map.Background[i].y = (int)old_map->Background[i].y;
					new_map.Background[i].w = (int)old_map->Background[i].w;
					new_map.Background[i].h = (int)old_map->Background[i].h;
					new_map.Background[i].texture_t = (bool)old_map->Background[i].texture_t;
					new_map.Background[i].texture_b = (bool)old_map->Background[i].texture_b;
					new_map.Background[i].texture_l = (bool)old_map->Background[i].texture_l;
					new_map.Background[i].texture_r = (bool)old_map->Background[i].texture_r;

					// Foregrounds
					new_map.Foreground[i].type = (int)old_map->Foreground[i].type;
					new_map.Foreground[i].x = (int)old_map->Foreground[i].x;
					new_map.Foreground[i].y = (int)old_map->Foreground[i].y;
					new_map.Foreground[i].w = (int)old_map->Foreground[i].w;
					new_map.Foreground[i].h = (int)old_map->Foreground[i].h;
					new_map.Foreground[i].texture_t = (bool)old_map->Foreground[i].texture_t;
					new_map.Foreground[i].texture_b = (bool)old_map->Foreground[i].texture_b;
					new_map.Foreground[i].texture_l = (bool)old_map->Foreground[i].texture_l;
					new_map.Foreground[i].texture_r = (bool)old_map->Foreground[i].texture_r;
				}
				for (int i = 0; i < 100; i++) {
					// Events
					new_map.Event[i].type = (int)old_map->Event[i].type;
					new_map.Event[i].x = (int)old_map->Event[i].x;
					new_map.Event[i].y = (int)old_map->Event[i].y;
					new_map.Event[i].w = (int)old_map->Event[i].w;
					new_map.Event[i].h = (int)old_map->Event[i].h;
					new_map.Event[i].texture_t = (bool)old_map->Event[i].texture_t;
					new_map.Event[i].texture_b = (bool)old_map->Event[i].texture_b;
					new_map.Event[i].texture_l = (bool)old_map->Event[i].texture_l;
					new_map.Event[i].texture_r = (bool)old_map->Event[i].texture_r;
				}

				// Save it to a file
				file_path = "maps/_cmap/a0_8_2/" + file_name;
				ALLEGRO_FILE* file = al_fopen(file_path.c_str(), "w");
				al_fwrite(file, &new_map, sizeof(Level_a0_8_2));
				al_fclose(file);

				// Done
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Succesfully converted map!");
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid syntax. Use: /cmap_a <file_name>");
			}
		} else if (type == "/cmap_b") { // a0.8.2 to a0.8.3
			std::string file_name = "null";
			std::string file_path = "";
			args >> file_name;
			if (file_name != "null") {
				// Old version map
				Level_a0_8_2* old_map = new Level_a0_8_2;

				// Open the old version map
				file_path = "maps/_cmap/" + file_name;
				if (al_filename_exists(file_path.c_str())) {
					ALLEGRO_FILE* file = al_fopen(file_path.c_str(), "r");
					al_fread(file, old_map, sizeof(Level_a0_8_2));
					al_fclose(file);
				} else {
					AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid file");
				}

				// Reconstruct the new version
				Level_a0_8_3 new_map;
				for (int i = 0; i < 1000; i++) {
					// Platforms
					new_map.Platform[i].type = (int)old_map->Platform[i].type;
					new_map.Platform[i].x = (int)old_map->Platform[i].x;
					new_map.Platform[i].y = (int)old_map->Platform[i].y;
					new_map.Platform[i].w = (int)old_map->Platform[i].w;
					new_map.Platform[i].h = (int)old_map->Platform[i].h;
					new_map.Platform[i].texture_t = (bool)old_map->Platform[i].texture_t;
					new_map.Platform[i].texture_b = (bool)old_map->Platform[i].texture_b;
					new_map.Platform[i].texture_l = (bool)old_map->Platform[i].texture_l;
					new_map.Platform[i].texture_r = (bool)old_map->Platform[i].texture_r;

					// Backgrounds
					new_map.Background[i].type = (int)old_map->Background[i].type;
					new_map.Background[i].x = (int)old_map->Background[i].x;
					new_map.Background[i].y = (int)old_map->Background[i].y;
					new_map.Background[i].w = (int)old_map->Background[i].w;
					new_map.Background[i].h = (int)old_map->Background[i].h;
					new_map.Background[i].texture_t = (bool)old_map->Background[i].texture_t;
					new_map.Background[i].texture_b = (bool)old_map->Background[i].texture_b;
					new_map.Background[i].texture_l = (bool)old_map->Background[i].texture_l;
					new_map.Background[i].texture_r = (bool)old_map->Background[i].texture_r;

					// Foregrounds
					new_map.Foreground[i].type = (int)old_map->Foreground[i].type;
					new_map.Foreground[i].x = (int)old_map->Foreground[i].x;
					new_map.Foreground[i].y = (int)old_map->Foreground[i].y;
					new_map.Foreground[i].w = (int)old_map->Foreground[i].w;
					new_map.Foreground[i].h = (int)old_map->Foreground[i].h;
					new_map.Foreground[i].texture_t = (bool)old_map->Foreground[i].texture_t;
					new_map.Foreground[i].texture_b = (bool)old_map->Foreground[i].texture_b;
					new_map.Foreground[i].texture_l = (bool)old_map->Foreground[i].texture_l;
					new_map.Foreground[i].texture_r = (bool)old_map->Foreground[i].texture_r;
				}
				for (int i = 0; i < 100; i++) {
					// Events
					new_map.Event[i].type = (int)old_map->Event[i].type;
					new_map.Event[i].x = (int)old_map->Event[i].x;
					new_map.Event[i].y = (int)old_map->Event[i].y;
					new_map.Event[i].w = (int)old_map->Event[i].w;
					new_map.Event[i].h = (int)old_map->Event[i].h;
					new_map.Event[i].texture_t = (bool)old_map->Event[i].texture_t;
					new_map.Event[i].texture_b = (bool)old_map->Event[i].texture_b;
					new_map.Event[i].texture_l = (bool)old_map->Event[i].texture_l;
					new_map.Event[i].texture_r = (bool)old_map->Event[i].texture_r;
				}

				// Save it to a file
				file_path = "maps/_cmap/a0_8_3/" + file_name;
				ALLEGRO_FILE* file = al_fopen(file_path.c_str(), "w");
				al_fwrite(file, &new_map, sizeof(Level_a0_8_3));
				al_fclose(file);

				// Done
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Succesfully converted map!");
			} else {
				AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, "Invalid syntax. Use: /cmap_a <file_name>");
			}
		} else {
			AddChatMessage(g, "__SYSTEM__", SYSTEM_COLOUR, ("Unrecognized command: " + type).c_str());
		}
	}
}