#include <iostream>
#include "Game.hpp"
#include "Convo.h"
#include "ScaledDraw.hpp"
#include "DEText.h"
#include "SmallTalk.h"

Convo::Convo() {

}

Convo::~Convo() {

}

void HandleConvo(Game* g) {
	if (g->convo->timer > -1) {
		g->convo->timer--;
		if (g->convo->timer == 0) {
			if (g->convo->exploding) {
				g->convo_with = NULL;
			} else if(g->convo->convo_type != CONVO_TYPE_STORY) {
				g->convo->timer = CONVO_BLINK_FRAMES;
			}
		}
	} else {
		if (g->convo->convo_type == CONVO_TYPE_STORY) {
			g->convo->event_id = g->convo->next_event_id;
			g->convo->next_event_id = -1;
			if (g->convo->event_id == -1) {
				// Close
				if ((g->cut_scene.timer > -1 || g->cut_scene.state == CUTSCENE_STATE_REST) && g->cut_scene.state != CUTSCENE_STATE_CLOSING) {
					CloseCutScene(g);
				}
			} else {
				// Next
				HandleConvoEvent(g);
			}
		}
	}
}

void HideConvo(Game* g) {
	g->convo->exploding = true;
	g->convo->timer = 100;
}

void StoryPause(Game* g, float duration, int next_event_id) {
	StopPlayer(g);
	g->convo = new Convo();
	g->convo->name = "";
	g->convo->message = "";
	g->convo->convo_type = CONVO_TYPE_STORY;
	g->convo->timer = SecondsToTicks(duration);
	g->convo->next_event_id = next_event_id;
	g->convo->title = "";
}

void ShowStory(Game* g, std::string name, std::string message, float duration, bool title, int next_event_id) {
	StopPlayer(g);
	g->convo = new Convo();
	g->convo->name = name;
	g->convo->message = message;
	g->convo->convo_type = CONVO_TYPE_STORY;
	g->convo->timer = SecondsToTicks(duration);
	g->convo->next_event_id = next_event_id;
	g->convo->title = title;
}

void ShowSTChoices(Game* g, std::string question,
	std::string choice_1, int next_id_1,
	std::string choice_2, int next_id_2,
	std::string choice_3, int next_id_3,
	std::string choice_4, int next_id_4) {

	// Constructs the choice interface
	StopPlayer(g);
	g->convo = new Convo();
	g->convo->small_talk = true;
	g->convo->exploding = false;
	g->convo->sorted = true;
	g->convo->question = question;
	g->convo->choice[0] = choice_1;
	g->convo->choice_next_event_id[0] = next_id_1;
	g->convo->choice[1] = choice_2;
	g->convo->choice_next_event_id[1] = next_id_2;
	g->convo->choice[2] = choice_3;
	g->convo->choice_next_event_id[2] = next_id_3;
	g->convo->choice[3] = choice_4;
	g->convo->choice_next_event_id[3] = next_id_4;
	g->convo->convo_type = CONVO_TYPE_CHOICE;
	g->convo->timer = 1;
}

void ShowChoices(Game* g, std::string question,
	std::string choice_1, int next_id_1,
	std::string choice_2, int next_id_2,
	std::string choice_3, int next_id_3,
	std::string choice_4, int next_id_4) {

	// Constructs the choice interface
	StopPlayer(g);
	g->convo = new Convo();
	g->convo->exploding = false;
	g->convo->sorted = true;
	g->convo->question = question;
	g->convo->choice[0] = choice_1;
	g->convo->choice_next_event_id[0] = next_id_1;
	g->convo->choice[1] = choice_2;
	g->convo->choice_next_event_id[1] = next_id_2;
	g->convo->choice[2] = choice_3;
	g->convo->choice_next_event_id[2] = next_id_3;
	g->convo->choice[3] = choice_4;
	g->convo->choice_next_event_id[3] = next_id_4;
	g->convo->convo_type = CONVO_TYPE_CHOICE;
	g->convo->timer = 1;
}

void ShowST(Game* g, std::string name, std::string message, int x, int y, int type, int next_event_id) {
	StopPlayer(g);
	g->convo = new Convo();
	g->convo->small_talk = true;
	g->convo->name = name;
	g->convo->message = message;
	g->convo->convo_type = type;
	int anim_duration = CONVO_BLINK_FRAMES + message.length();
	g->convo->timer = anim_duration;
	g->convo->parent_x = x;
	g->convo->parent_y = y;
	g->convo->exploding = false;
	g->convo->sorted = false;
	g->convo->next_event_id = next_event_id;

	//Facing
	if (g->convo_with != NULL) {
		if (NPC_X <= PLR_X) {
			g->convo_with->direction = FACING_RIGHT;
			if (g->convo->name == NPC_NAME) {
				g->convo->facing = FACING_RIGHT;
			} else {
				g->convo->facing = FACING_LEFT;
			}
		} else {
			g->convo_with->direction = FACING_LEFT;
			if (g->convo->name == NPC_NAME) {
				g->convo->facing = FACING_LEFT;
			} else {
				g->convo->facing = FACING_RIGHT;
			}
		}
	}
}

void ShowConvo(Game* g, std::string name, std::string message, int x, int y, int type, int next_event_id) {
	StopPlayer(g);
	g->convo = new Convo();
	g->convo->name = name;
	g->convo->message = message;
	g->convo->convo_type = type;
	int anim_duration = CONVO_BLINK_FRAMES + message.length();
	g->convo->timer = anim_duration;
	g->convo->parent_x = x;
	g->convo->parent_y = y;
	g->convo->exploding = false;
	g->convo->sorted = false;
	g->convo->next_event_id = next_event_id;

	// Facing
	if (g->convo_with != NULL) {
		if (NPC_X <= PLR_X) {
			g->convo_with->direction = FACING_RIGHT;
			if (g->convo->name == NPC_NAME) {
				g->convo->facing = FACING_RIGHT;
			} else {
				g->convo->facing = FACING_LEFT;
			}
		} else {
			g->convo_with->direction = FACING_LEFT;
			if (g->convo->name == NPC_NAME) {
				g->convo->facing = FACING_LEFT;
			} else {
				g->convo->facing = FACING_RIGHT;
			}
		}
	}

	// Hide Interface and other crap
	g->Interfaces[INTERFACE_INVENTORY].visible = false;
	g->Interfaces[INTERFACE_ABILITIES].visible = false;
}

void RenderConvo(Game* g, ALLEGRO_BITMAP** img_sprite, ALLEGRO_FONT** font) {
	// Ratios for re-scaled text
	float r_x = (float)g->s_x / (float)GAME_WIDTH;
	float r_y = (float)g->s_y / (float)GAME_HEIGHT;
	// Camera
	int x = g->camera.x;
	int y = g->camera.y;

	if (g->convo->timer > -1) {
		// Sort lines first time
		if (!g->convo->sorted) {
			g->convo->sorted = true;
			SubStrings message_words = SplitString(g->convo->message, ' ');
			bool sort = true;
			int line_id = 0;
			int word_id = 0;
			while (sort) {
				int line_lenght = al_get_text_width(font[1], g->convo->line[line_id].c_str());
				int word_lenght = al_get_text_width(font[1], message_words.s[word_id].c_str());
				if (line_lenght + word_lenght < 240) {
					g->convo->line[line_id] += (" " + message_words.s[word_id]);
					word_id++;
					if (word_id >= message_words.items) {
						sort = false;
					}
				} else {
					line_id++;
					g->convo->line[line_id] += (" " + message_words.s[word_id]);
					word_id++;
					if (line_id > 4 || word_id >= message_words.items) {
						sort = false;
					}
				}
			}
		}

		if (g->convo->convo_type == CONVO_TYPE_STORY) {
			if (g->convo->timer > SecondsToTicks(4)) {
				// Fading in
				float clr = 1 - ((g->convo->timer - SecondsToTicks(4)) / (float)SecondsToTicks(1));
				if (g->convo->title) {
					DrawTextA(font[8], clr, clr, clr, GAME_WIDTH / 2, GAME_HEIGHT / 2 - 75, ALLEGRO_ALIGN_CENTER, clr, g->convo->name.c_str());
					DrawTextA(font[7], clr, clr, clr, GAME_WIDTH / 2, GAME_HEIGHT / 2 + 25, ALLEGRO_ALIGN_CENTER, clr, g->convo->message.c_str());
				} else {
					DrawTextA(font[6], clr, clr, clr, GAME_WIDTH / 2, GAME_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, clr, g->convo->message.c_str());
				}
			} else if (g->convo->timer > SecondsToTicks(1)) {
				// Standing still
				if (g->convo->title) {
					DrawTextA(font[8], 1, 1, 1, GAME_WIDTH / 2, GAME_HEIGHT / 2 - 75, ALLEGRO_ALIGN_CENTER, 1, g->convo->name.c_str());
					DrawTextA(font[7], 1, 1, 1, GAME_WIDTH / 2, GAME_HEIGHT / 2 + 25, ALLEGRO_ALIGN_CENTER, 1, g->convo->message.c_str());
				} else {
					DrawTextA(font[6], 1, 1, 1, GAME_WIDTH / 2, GAME_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, 1, g->convo->message.c_str());
				}

			} else {
				// Fading out
				float clr = 1 - ((SecondsToTicks(1) - g->convo->timer) / (float)SecondsToTicks(1));
				if (g->convo->title) {
					DrawTextA(font[8], clr, clr, clr, GAME_WIDTH / 2, GAME_HEIGHT / 2 - 75, ALLEGRO_ALIGN_CENTER, clr, g->convo->name.c_str());
					DrawTextA(font[7], clr, clr, clr, GAME_WIDTH / 2, GAME_HEIGHT / 2 + 25, ALLEGRO_ALIGN_CENTER, clr, g->convo->message.c_str());
				} else {
					DrawTextA(font[6], clr, clr, clr, GAME_WIDTH / 2, GAME_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, clr, g->convo->message.c_str());
				}

			}
		} else if (g->convo->convo_type == CONVO_TYPE_CHOICE) {
			int x = GAME_WIDTH / 2 - 300;
			int y = GAME_HEIGHT / 2 - 100;
			DrawImage(g, img_sprite[134], x, y, 0);
			DrawText(font[0], 0, 0, 0, x + 300, y + 10, ALLEGRO_ALIGN_CENTER, g->convo->question.c_str());
			DrawText(font[4], 200, 200, 200, x + 300, y + 175, ALLEGRO_ALIGN_CENTER, "(Use the number keys on keyboard to pick a reply)");
			for (int i = 0; i < 4; i++) {
				if (MouseIsOn(g, x + 26, y + 48 + (i * 32), 540, 26)) {
					DrawText(font[1], 0, 100, 0, x + 31, y + 50 + (i * 32), ALLEGRO_ALIGN_LEFT, g->convo->choice[i].c_str());
				} else {
					DrawText(font[1], 0, 50, 0, x + 31, y + 50 + (i * 32), ALLEGRO_ALIGN_LEFT, g->convo->choice[i].c_str());
				}
			}
		} else {
			// Calculate x/y values on screen
			int convo_x = 0;
			int convo_y = (g->convo->parent_y - 200) + y;
			if (g->convo->facing == FACING_RIGHT) {
				convo_x = g->convo->parent_x - 300 + x;
			} else {
				convo_x = g->convo->parent_x - 33 + g->pl.w + x;
			}
			// Correct if outside screen
			if (convo_x < 10) convo_x = 10;
			if (convo_x > GAME_WIDTH - 343) convo_x = GAME_WIDTH - 343;
			if (convo_y < 110) convo_y = 110;
			if (convo_y > GAME_HEIGHT - 287) convo_y = GAME_HEIGHT - 287;

			if (!g->convo->exploding) {
				// Render
				int sprite_id = SPRITE_INTERFACE_CONVO_THINK - 1 + g->convo->convo_type;
				if (g->convo->timer <= CONVO_BLINK_FRAMES) { //Flat
					DrawImage(g, img_sprite[sprite_id], convo_x, convo_y, 0);
					// Split into lines...
					for (int i = 0; i < 5; i++) {
						DrawText(font[1], 0, 0, 0,
							convo_x + 50, convo_y + 30 + (i * 20),
							ALLEGRO_ALIGN_LEFT, g->convo->line[i].c_str());
					}
					DrawText(font[4], 100, 100, 100,
						convo_x + 300, convo_y + 130,
						ALLEGRO_ALIGN_RIGHT, "[ENTER] to continue...");
				} else {
					DrawImage(g, img_sprite[sprite_id], convo_x, convo_y, 0);
					int chars_left = g->convo->message.length() - (g->convo->timer - CONVO_BLINK_FRAMES);
					for (int i = 0; i < 5; i++) {
						if (chars_left > 0) {
							std::string this_line = "";
							for (unsigned int j = 0; j < g->convo->line[i].length(); j++) {
								if (chars_left > 0) {
									this_line += g->convo->line[i][j];
									chars_left--;
								}
							}
							DrawText(font[1], 0, 0, 0,
								convo_x + 50, convo_y + 30 + (i * 20),
								ALLEGRO_ALIGN_LEFT, this_line.c_str());
						}
					}
				}
			} else {
				// Fade out
				int sprite_id = SPRITE_INTERFACE_CONVO_THINK - 1 + g->convo->convo_type;
				DrawImage(g, img_sprite[sprite_id], convo_x, convo_y, 0, ((float)g->convo->timer / 100));
			}
		}
	}
}

void RenderCutScene(Game* g) {
	if (g->cut_scene.state == CUTSCENE_STATE_OPENING) {
		int height = 100 - (g->cut_scene.timer);
		DrawRectangle(g, 0, 0, GAME_WIDTH, height, 0, 0, 0);
		DrawRectangle(g, 0, GAME_HEIGHT-height, GAME_WIDTH, height, 0, 0, 0);
	}
	if (g->cut_scene.state == CUTSCENE_STATE_REST) {
		int height = 100;
		DrawRectangle(g, 0, 0, GAME_WIDTH, height, 0, 0, 0);
		DrawRectangle(g, 0, GAME_HEIGHT - height, GAME_WIDTH, height, 0, 0, 0);
	}
	if (g->cut_scene.state == CUTSCENE_STATE_CLOSING) {
		int height = g->cut_scene.timer;
		DrawRectangle(g, 0, 0, GAME_WIDTH, height, 0, 0, 0);
		DrawRectangle(g, 0, GAME_HEIGHT - height, GAME_WIDTH, height, 0, 0, 0);
	}
}

void CloseCutScene(Game* g) {
	g->cut_scene.timer = 100;
	g->Interfaces[INTERFACE_PLAYER].visible = true;
	g->Interfaces[INTERFACE_SLOTS].visible = true;
	g->Interfaces[INTERFACE_MAP].visible = true;
	g->Interfaces[INTERFACE_CHAT].visible = true;
	g->camera.mode = CAMERA_MODE_BOX;
	g->cut_scene.state = CUTSCENE_STATE_CLOSING;
}

void StartCutScene(Game* g) {
	StopPlayer(g);
	g->cut_scene.timer = 100;
	g->Interfaces[INTERFACE_PLAYER].visible = false;
	g->Interfaces[INTERFACE_SLOTS].visible = false;
	g->Interfaces[INTERFACE_MAP].visible = false;
	g->Interfaces[INTERFACE_CHAT].visible = false;
	g->cut_scene.state = CUTSCENE_STATE_OPENING;
}

void HandleCutScene(Game* g) {
	g->cut_scene.timer--;
	if (g->cut_scene.timer < -1) {
		g->cut_scene.timer = -1;
		if (g->cut_scene.state == CUTSCENE_STATE_OPENING) {
			g->cut_scene.state = CUTSCENE_STATE_REST;
		}
		if (g->cut_scene.state == CUTSCENE_STATE_CLOSING) {
			g->cut_scene.state = CUTSCENE_STATE_CLOSED;
		}
	}
}

int HandleEventTriggersOnAction(Game* g) {
	for (int i = 0; i < MAX_EVENTS; i++) {
		if (g->lvl->Event[i].type >= 100) {
			if (collide(&g->pl, &g->lvl->Event[i])) {
				switch (g->lvl->Event[i].type) {
				case 101:
					if (g->flash_light) {
						StartCutScene(g);
						ShowConvo(g, "", "This door is open, I'll take a look inside.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 116);
						g->Interfaces[INTERFACE_TUTORIAL].visible = false;
					} else {
						ShowConvo(g, "", "This door is open... But I can't see what's inside.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					}
					return i;
				case 102:
					if (g->flash_light) {
						ShowConvo(g, "", "It says \"CLOSED\" on the door.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					} else {
						ShowConvo(g, "", "There's a sign but I can't read it.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					}
					return 0;
				case 103:
					ShowConvo(g, "", "It's locked...", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					return i;
				case 104:
					ShowConvo(g, "", "*knock* *knock*", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 122);
					return i;
				case 105:
					ShowConvo(g, "", "This door seems to be locked.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					return i;
				case 106:
					ShowConvo(g, "", "Somebody please help me.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 114);
					return i;
				case 107:
					ShowConvo(g, "", "Maybe no one is home.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					return i;
				case 108:
					ShowConvo(g, "", "There's no use... This is a ghost town.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					return i;
				case 109:
					ShowConvo(g, "", "HELP!!!", PLR_X, PLR_Y, CONVO_TYPE_SHOUT, 115);
					return i;
				case 110:
					// Leaving house trigger
					return i;
				case 111:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 136);
					return i;
				case CONVO_LOCKED_DOOR:
					ShowConvo(g, "", "This door is locked.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
					return i;
				case 114:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 137);
					return i;
				case 115:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 140);
					return i;
				case 116:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 139);
					return i;
				case 117:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 141);
					return i;
				case 118:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 142);
					return i;
				case 119:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 138);
					return i;
				case 120:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 143);
					return i;
				case 121:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 144);
					return i;
				case 122:
					if (g->story.missions[0].prog != 16 && g->story.missions[0].prog != 18) {
						Fade(g, FADE_OUT);
						StoryPause(g, 2.0, 145);
					}
					return i;
				case 133:
					//level 1 Window
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 267);
					return i;
				}
			}
		}
	}
	return -1;
}

void HandleEventTriggers(Game* g) {
	for (int i = 0; i < MAX_EVENTS; i++) {
		if (g->lvl->Event[i].type >= 100) {
			if (collide(&g->pl, &g->lvl->Event[i])) {
				switch (g->lvl->Event[i].type) {
				case 100:
					if (g->story.missions[0].prog == 4) {
						g->story.missions[0].prog = 5;
						StartCutScene(g);
						MoveCamera(g, 1050, -600);
						ShowConvo(g, "", "I sh-should be able to j-jump here.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 111);
						DropItem(g, g->ItemDB->item[0], 1055, -64);
					}
					break;
				case 123:
					if (g->story.missions[0].prog == 18) {
						g->story.missions[0].prog = 19;
						StartCutScene(g);
						ShowConvo(g, PLR_NAME, "It worked, but I feel so tired... I don't think I could do that ever again.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 181);
						g->Interfaces[INTERFACE_TUTORIAL2].visible = false;
						MoveCamera(g, 1170, -690);
						break;
				case 124:
					if (!g->warping) {
						g->warping = true;
						Fade(g, FADE_OUT);
						StoryPause(g, 2.0, 197);
					}
					break;
				case 125:
					if (!(g->story.missions[0].prog >= 23 && g->story.missions[0].prog <= 34)) {
						if (!g->warping) {
							g->warping = true;
							Fade(g, FADE_OUT);
							StoryPause(g, 2.0, 198);
						}
					}
					break;
				case 126:
					if (!g->warping) {
						g->warping = true;
						Fade(g, FADE_OUT);
						StoryPause(g, 2.0, 199);
					}
					break;
				case 127:
					if (!(g->story.missions[0].prog >= 23 && g->story.missions[0].prog <= 34)) {
						if (!g->warping) {
							g->warping = true;
							Fade(g, FADE_OUT);
							StoryPause(g, 2.0, 200);
						}
					}
					break;
				case 128:
					if (g->story.missions[0].prog == 23) {
						g->story.missions[0].prog = 24;
						g->monster[1] = NPC();
					} else if (g->story.missions[0].prog < 23) {
						g->pl.x -= MAX_X_VEL;
					}
					break;
				case 129:
					if (g->story.missions[0].prog == 25) {
						g->story.missions[0].prog = 26;
					}
					break;
				case 130:
					if (g->story.missions[0].prog == 27) {
						g->story.missions[0].prog = 28;
					}
					break;
				case 131:
					if (g->story.missions[0].prog == 29) {
						g->story.missions[0].prog = 30;
					}
					break;
				case 132:
					if (g->story.missions[0].prog == 31) {
						g->story.missions[0].prog = 32;
					}
					break;
				case 134:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 273);
					break;
				case 135:
					Fade(g, FADE_OUT);
					StoryPause(g, 2.0, 288);
					break;
				case 136:
					if(g->story.missions[0].prog < 39) g->pl.x += MAX_X_VEL;
					break;
					}
				}
			}
		}
	}
}

void HandleMissions(Game* g) {
	//Timers
	for (int i = 0; i < MAX_MISSIONS; i++) {
		if (g->story.missions[i].timer > 0) g->story.missions[i].timer--;
	}

	//Missions
	Mission* mis;

	/* MISSION 1*/
	mis = &g->story.missions[0];
	if (mis->prog == 2) {
		if (!mis->var1 && g->keys.left) {
			mis->var1 = true;
		}
		if (!mis->var2 && g->keys.right) {
			mis->var2 = true;
		}
		if (mis->var1 && mis->var2) {
			mis->prog = 3;
			mis->timer = SecondsToTicks(2.0);
		}
	}
	if (mis->prog == 3) {
		if (mis->timer <= 0) {
			mis->prog = 4;
			g->Interfaces[INTERFACE_TUTORIAL].visible = false;
		}
	}
	if (mis->prog == 6) {
		if (g->keys.jump) {
			mis->var3 = true;
			mis->prog = 7;
			mis->timer = SecondsToTicks(2.0);
		}
	}
	if (mis->prog == 7) {
		if (mis->timer <= 0) {
			mis->prog = 8;
			g->Interfaces[INTERFACE_TUTORIAL].visible = false;
		}
	}
	if (mis->prog == 8) {
		if (PlayerHasItem(g, 0)) {
			mis->prog = 9;
			StartCutScene(g);
			ShowConvo(g, "", "I should use this to see b-better in the dark.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 112);
		}
	}
	if (mis->prog == 10) {
		if (mis->timer <= 0) {
			mis->timer = SecondsToTicks(1.0);
		}
		if (g->Interfaces[INTERFACE_INVENTORY].visible) {
			mis->prog = 11;
		}
	}
	if (mis->prog == 11) {
		if (mis->timer <= 0) {
			mis->timer = SecondsToTicks(1.0);
		}
		if (g->flash_light) {
			mis->prog = 12;
			StartCutScene(g);
			g->Interfaces[INTERFACE_TUTORIAL2].visible = false;
			ShowConvo(g, "", "That's much better!", PLR_X, PLR_Y, CONVO_TYPE_THINK, 113);
		}
	}
	if (mis->prog == 18) {
		if (mis->timer <= 0) {
			mis->timer = SecondsToTicks(1.0);
		}
	}
	if (mis->prog >= 22 && mis->prog <= 32) {
		if (mis->timer <= 0) {
			mis->timer = SecondsToTicks(1.0);
		}
	}
	if (mis->prog == 38) {
		if (mis->timer <= 0) {
			mis->timer = SecondsToTicks(1.0);
		}
		if (g->AbilitySlots[0].ability.id == ABILITY_BASIC_SLASH) {
			g->Interfaces[INTERFACE_TUTORIAL2].visible = false;
			NewObjective(g, 1, "STORY OBJECTIVE:", "Explore Netiki's underground cave.");
			mis->prog = 39;
		}
	}
}

/* ============================= Convo Events ==========================
 *		The big function that handles all conversations and events
 *		in the story mode (except small talk).
 */
void HandleConvoEvent(Game* g) {
	switch (g->convo->event_id) {
	case 101:
		if (g->music_volume == 0) {
			ShowStory(g, "Chapter 1:", "...but what if we already forgot our true identity?", 5, false, 108);
			SetCamera(g, 1820, -540, CAMERA_MODE_BOX);
		} else {
			ShowStory(g, "Chapter 1:", "\"Knowledge is power\"", 3, false, 102);
		}
		break;
	case 102:
		ShowStory(g, "", "However, too much knowledge can close our minds", 5, false, 103);
		break;
	case 103:
		ShowStory(g, "", "When we feel content with some knowledge...", 3, false, 104);
		break;
	case 104:
		ShowStory(g, "", "...we stop looking for the truth", 5, false, 105);
		break;
	case 105:
		ShowStory(g, "", "Knowledge itself does not change the truth", 5, false, 106);
		break;
	case 106:
		ShowStory(g, "", "We might think we know who we are...", 3, false, 107);
		break;
	case 107:
		ShowStory(g, "", "...but what if we already forgot our true identity?", 5, false, 108);
		break;
	case 108:
		ShowConvo(g, PLR_NAME, "I'm c-cold... where am I?", PLR_X, PLR_Y, CONVO_TYPE_THINK, 109);
		break;
	case 109:
		ShowConvo(g, PLR_NAME, "Need... to find... sh-shelter...", PLR_X, PLR_Y, CONVO_TYPE_THINK, 110);
		break;
	case 110:
		CloseCutScene(g);
		HideConvo(g);
		g->Interfaces[INTERFACE_TUTORIAL].visible = true;
		g->story.missions[0].prog = 2;
		break;
	case 111:
		CloseCutScene(g);
		HideConvo(g);
		g->Interfaces[INTERFACE_TUTORIAL].visible = true;
		g->story.missions[0].prog = 6;
		break;
	case 112:
		CloseCutScene(g);
		HideConvo(g);
		g->Interfaces[INTERFACE_TUTORIAL2].visible = true;
		g->story.missions[0].prog = 10;
		break;
	case 113:
		CloseCutScene(g);
		HideConvo(g);
		g->story.missions[0].prog = 13;
		g->Interfaces[INTERFACE_TUTORIAL].visible = true;
		break;
	case 114:
		ShowConvo(g, PLR_NAME, "No one is responding...", PLR_X, PLR_Y, CONVO_TYPE_THINK);
		break;
	case 115:
		ShowConvo(g, PLR_NAME, "I'll die here all alone...", PLR_X, PLR_Y, CONVO_TYPE_THINK);
		break;
	case 116:
		// Go Inside
		StoryPause(g, 2.0, 117);
		Fade(g, 0);
		g->story.missions[0].prog = 14;
		break;
	case 117:
		ChangeMap(g, "maps/tutorial/T02.dem");
		SetCamera(g, 790, -540, CAMERA_MODE_CUTSCENE);
		Fade(g, 1);
		StoryPause(g, 2.0, 118);
		break;
	case 118:
		ShowConvo(g, PLR_NAME, "Hello? Anyone live here?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 119);
		break;
	case 119:
		ShowConvo(g, PLR_NAME, "This place seems to be completely empty.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 120);
		break;
	case 120:
		ShowConvo(g, PLR_NAME, "I hope no one will mind if I spend the night here.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 121);
		break;
	case 121:
		ShowConvo(g, PLR_NAME, "*yawn*", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 124);
		break;
	case 122:
		ShowConvo(g, "", "Nobody's home?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL);
		break;
	case 123:
		// SKIPPED
		break;
	case 124:
		Fade(g, FADE_OUT);
		StoryPause(g, 2.0, 125);
		break;
	case 125:
		ChangeMap(g, "maps/Netiki/Home.dem");
		Fade(g, FADE_IN);
		ShowStory(g, "Morning", "7:00 A.M.", 3, true, 126);
		g->lvl->thunder = false;
		break;
	case 126:
		ShowConvo(g, PLR_NAME, "Uh..? Where... oh - that's right!", PLR_X, PLR_Y, CONVO_TYPE_THINK, 127);
		break;
	case 127:
		ShowConvo(g, PLR_NAME, "I was looking for shelter so I spent the night here.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 128);
		break;
	case 128:
		ShowConvo(g, PLR_NAME, "But how did I end up alone, in the rain, at night?", PLR_X, PLR_Y, CONVO_TYPE_THINK, 129);
		break;
	case 129:
		ShowConvo(g, PLR_NAME, "I can't seem to recall... Wait...", PLR_X, PLR_Y, CONVO_TYPE_THINK, 130);
		break;
	case 130:
		StoryPause(g, 2.0, 131);
		break;
	case 131:
		ShowConvo(g, PLR_NAME, "Who am I...?", PLR_X, PLR_Y, CONVO_TYPE_THINK, 132);
		break;
	case 132:
		ShowConvo(g, PLR_NAME, "I can't remember anything. I actually have NO idea who I am.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 133);
		break;
	case 133:
		// Play bg sound for crowd noises...
		StoryPause(g, 2.0, 134);
		break;
	case 134:
		ShowConvo(g, PLR_NAME, "I can hear voices!", PLR_X, PLR_Y, CONVO_TYPE_THINK, 135);
		break;
	case 135:
		ShowConvo(g, PLR_NAME, "I should go outside and see if anyone can help me.", PLR_X, PLR_Y, CONVO_TYPE_THINK);
		break;
	case 136:
		// From tutorial
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 137:
		ChangeMap(g, "maps/Netiki/WestBorder.dem");
		if (!PlayerHasItem(g, 3)) {
			g->ItemOnMap[0].item = g->ItemDB->item[3];
			g->ItemOnMap[0].x = 1620;
			g->ItemOnMap[0].y = -162;
		}
		Fade(g, FADE_IN);
		break;
	case 138:
		// From west border
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 139:
		// From netiki
		ChangeMap(g, "maps/Netiki/Home.dem");
		Fade(g, FADE_IN);
		break;
	case 140:
		// From netiki
		ChangeMap(g, "maps/Netiki/Hostle.dem");
		Fade(g, FADE_IN);
		break;
	case 141:
		// From netiki
		ChangeMap(g, "maps/Netiki/Shop.dem");
		Fade(g, FADE_IN);
		break;
	case 142:
		// From netiki
		ChangeMap(g, "maps/Netiki/Cafe.dem");
		Fade(g, FADE_IN);
		break;
	case 143:
		// From west hostle1
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 144:
		// From west shop
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 145:
		// From west hostle2
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 146:
		// West Guard
		ShowConvo(g, PLR_NAME, "Hello.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 147);
		break;
	case 147:
		// Hello
		ShowConvo(g, NPC_NAME, "Hello, how can I help you?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 148);
		break;
	case 148:
		ShowChoices(g, "What do you say?",
			"1) What's behind that door?", 149,
			"2) Can you let me trough please?", 152,
			"", -2,
			"4) [Leave without saying anything]", -1);
		break;
	case 149:
		ShowConvo(g, PLR_NAME, "What's behind the door?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 150);
		break;
	case 150:
		ShowConvo(g, NPC_NAME, "Well the edge of the world of course.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 151);
		break;
	case 151:
		ShowConvo(g, NPC_NAME, "Have you not learnt anything at school?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 152:
		ShowConvo(g, PLR_NAME, "Can you let me trough please?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 153);
		break;
	case 153:
		ShowConvo(g, NPC_NAME, "Ofcourse not! That's the edge of the world on the other side!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 151);
		break;
	case 154:
		// Maid
		ShowConvo(g, PLR_NAME, "Uh... Sorry, I have no money. I'm just looking around, trying to remember.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 155);
		break;
	case 155:
		ShowConvo(g, NPC_NAME, "Have you lost your wallet?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 156);
		break;
	case 156:
		ShowConvo(g, PLR_NAME, "No no, it might sound crazy, but... I forgot everything, where I live, who I am and why I'm here.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 157);
		break;
	case 157:
		ShowConvo(g, NPC_NAME, "Oh you poor thing! You're right! That does sound crazy.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 158);
		break;
	case 158:
		ShowConvo(g, NPC_NAME, "But no need to make up stories... Let me call the owner, he might be willing to help you.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 159);
		break;
	case 159:
		g->story.missions[0].prog = 15;
		StoryPause(g, 2.0, 170);
		break;
	case 170:
		g->convo_with = &g->monster[1];
		ShowConvo(g, NPC_NAME, "Is there a problem?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 171);
		break;
	case 171:
		ShowConvo(g, PLR_NAME, "No-", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 172);
		break;
	case 172:
		g->convo_with = &g->monster[0];
		ShowConvo(g, NPC_NAME, "This man seems to be homeless.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 173);
		break;
	case 173:
		ShowConvo(g, PLR_NAME, "They keep talking over me. I never said I'm homeless. Ugh.", PLR_X, PLR_Y, CONVO_TYPE_THINK, 174);
		break;
	case 174:
		g->convo_with = &g->monster[1];
		ShowConvo(g, NPC_NAME, "Oh right, right. Please take a seat upstairs. We will serve you with a complimentary meal.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		g->story.missions[0].prog = 16;
		break;
	case 175:
		// Fill
		break;
	case 176:
		ShowConvo(g, PLR_NAME, "How am I meant to get \"upstairs\"?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 177);
		break;
	case 177:
		ShowConvo(g, PLR_NAME, "There are NO \"stairs\" and I can't jump high enough.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 178);
		break;
	case 178:
		ShowConvo(g, NPC_NAME, "Oh silly, just jump off the wall.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 179);
		break;
	case 179:
		StoryPause(g, 5.0); // It'll get overwritten anyway
		g->story.missions[0].prog = 17;
		g->convo_with->direction = 1;
		break;
	case 180:
		CloseCutScene(g);
		HideConvo(g);
		g->Interfaces[INTERFACE_TUTORIAL2].visible = true;
		g->story.missions[0].prog = 18;
		break;
	case 181:
		g->convo_with = &g->monster[0];
		ShowConvo(g, NPC_NAME, "Here's your order sir.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 182);
		break;
	case 182:
		ShowConvo(g, PLR_NAME, "Thank you so much. Now that I think about it, I feel really hungry.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 183);
		break;
	case 183:
		ShowConvo(g, PLR_NAME, "*omnom nom nom nom*", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 184);
		break;
	case 184:
		ShowConvo(g, PLR_NAME, "*burp*", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 185);
		break;
	case 185:
		ShowConvo(g, PLR_NAME, "Sorry. Seems I haven't eaten for a while now.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 186);
		break;
	case 186:
		ShowConvo(g, NPC_NAME, "Hihihihi, it's fine!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 187);
		break;
	case 187:
		ShowConvo(g, PLR_NAME, "Thank you again. I feel more energy now.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 188);
		break;
	case 188:
		StoryPause(g, 2.0);
		// Idk yet
		break;
	case 189:
		ShowChoices(g, "What do you say?",
			"1) Do you know who I am?", 190,
			"2) Can you help me remember?", 191,
			"3) Sorry, I don't have any money", 196,
			"4) [Leave without saying anything]", -1);
		break;
	case 190:
		ShowConvo(g, NPC_NAME, "Look mate, I don't care who you are. Either buy something or leave.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 191:
		ShowConvo(g, NPC_NAME, "Remember what?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 192);
		break;
	case 192:
		ShowChoices(g, "What do you say?",
			"1) Who am I?", 193,
			"2) Where am I?", 194,
			"3) Where do I live?", 195,
			"4) [Leave without saying anything]", -1);
		break;
	case 193:
		ShowConvo(g, NPC_NAME, "I have no idea mate. If you don't wanna buy anything, please leave.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 194:
		ShowConvo(g, NPC_NAME, "You're in Netiki city. Did you get hit in the head or something? Leave if you're not buying anything.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 195:
		ShowConvo(g, NPC_NAME, "I don't want any homeless people around here, bugger off mate.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 196:
		ShowConvo(g, NPC_NAME, "I don't want any poor people lurking around my shop, you might steal something. Leave please.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 197:
		// From netiki1 to netiki2
		ChangeMap(g, "maps/Netiki/Netiki-02.dem");
		Fade(g, FADE_IN);
		break;
	case 198:
		// From netiki2 to netiki1
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 199:
		// From netiki1 to netiki2 top
		ChangeMap(g, "maps/Netiki/Netiki-02.dem");
		Fade(g, FADE_IN);
		break;
	case 200:
		// From netiki2 to netiki1 top
		ChangeMap(g, "maps/Netiki/Netiki-01.dem");
		Fade(g, FADE_IN);
		break;
	case 201:
		// Homeless Man tutorial...
		ShowConvo(g, NPC_NAME, "Can I help you somehow?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 202);
		break;
	case 202:
		ShowChoices(g, "What do you say?",
			"1) Where are we?", 203,
			"2) What are you doing here?", 205,
			"3) I forgot who I am.", 211);
		break;
	case 203:
		ShowConvo(g, NPC_NAME, "You're in Netiki city. Are you new here?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 204);
		break;
	case 204:
		ShowConvo(g, PLR_NAME, "I'm not sure...", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 202);
		break;
	case 205:
		ShowConvo(g, NPC_NAME, "I live here.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 206);
		break;
	case 206:
		ShowConvo(g, PLR_NAME, "Where?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 207);
		break;
	case 207:
		ShowConvo(g, NPC_NAME, "Right here, where you're standing right now.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 208);
		break;
	case 208:
		ShowConvo(g, PLR_NAME, "Do you mean you're homeless?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 209);
		break;
	case 209:
		ShowConvo(g, NPC_NAME, "Absolutately not! No one is homeless. This world is everyone's home.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 210);
		break;
	case 210:
		ShowConvo(g, PLR_NAME, "I think right now I can relate. Your words are comforting. Thank-you.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 202);
		break;
	case 211:
		ShowConvo(g, NPC_NAME, "What happened? Did you hit your head?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 212);
		break;
	case 212:
		ShowConvo(g, PLR_NAME, "No... I can't recall. But my head doesn't hurt so I don't think it was an injury.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 213);
		break;
	case 213:
		ShowConvo(g, NPC_NAME, "I once forgot my identity as well. In fact it's more common than you might think.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 214);
		break;
	case 214:
		ShowChoices(g, "What do you say?",
			"1) What's your name?", 215,
			"2) What happened?", 217,
			"3) How did you remember again?", 218);
		break;
	case 215:
		ShowConvo(g, NPC_NAME, "I don't have a name, call me as you wish.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 216);
		break;
	case 216:
		ShowConvo(g, PLR_NAME, "Alright... I'll call you Asuwish.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 214);
		break;
	case 217:
		ShowConvo(g, NPC_NAME, "Got distracted from the truth... Long story, might tell you some other time.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 214);
		break;
	case 218:
		ShowConvo(g, NPC_NAME, "I had to travel all the way to the east.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 219);
		break;
	case 219:
		ShowConvo(g, NPC_NAME, "Once I reached the end... I remembered everything, it was an amazing feeling.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 220);
		break;
	case 220:
		ShowConvo(g, PLR_NAME, "What's in the east?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 221);
		break;
	case 221:
		ShowConvo(g, NPC_NAME, "I could tell you. But my words would be nothing but a story.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 222);
		break;
	case 222:
		ShowConvo(g, NPC_NAME, "You must travel there yourself if you want to find the truth.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 223);
		break;
	case 223:
		ShowConvo(g, PLR_NAME, "Can you show me the way?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 224);
		break;
	case 224:
		ShowConvo(g, NPC_NAME, "I can only give you the tools. You must go on the journey alone.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 225);
		break;
	case 225:
		ShowConvo(g, PLR_NAME, "I'm not even able to jump on this little cliff. Any tips on that?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 226);
		break;
	case 226:
		ShowConvo(g, NPC_NAME, "Nonsense. You just need some shoes, look at your dirty feet.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 227);
		break;
	case 227:
		ShowConvo(g, NPC_NAME, "With shoes you'll have more friction, you'll be able to make those longer jumps without a sweat.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 228);
		break;
	case 228:
		ShowConvo(g, PLR_NAME, "I don't think I can get any shoes... I don't have any money. Even if I have some I would of forgotten where I placed it.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 229);
		break;
	case 229:
		ShowConvo(g, NPC_NAME, "I'll make you a deal! I've lost a piece of cloth at the west border.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 230);
		break;
	case 230:
		ShowConvo(g, NPC_NAME, "I don't need it anymore. But it might serve a purpose for your journey.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 231);
		break;
	case 231:
		g->story.missions[0].prog = 20;
		ShowConvo(g, PLR_NAME, "Alright, I will go look for it.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL);
		break;
	case 232:
		if (PlayerHasItem(g, 3)) {
			StartCutScene(g);
			ShowConvo(g, PLR_NAME, "Is this yours?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 234);
		} else {
			ShowConvo(g, PLR_NAME, "Sorry, I haven't found it yet.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 233);
		}
		break;
	case 233:
		ShowConvo(g, NPC_NAME, "You should find it at the west border somewhere.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 234:
		ShowConvo(g, NPC_NAME, "Sure was!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 235);
		break;
	case 235:
		ShowConvo(g, PLR_NAME, "\"was\"?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 236);
		break;
	case 236:
		ShowConvo(g, NPC_NAME, "I'm glad you found it, please keep it for your journey as it will serve a purpose.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 237);
		break;
	case 237:
		ShowConvo(g, PLR_NAME, "I will take good care of it.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 238);
		break;
	case 238:
		ShowConvo(g, NPC_NAME, "Here you go as promised.\n *hands you a pair of shoes*", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 239);
		break;
	case 239:
		ShowConvo(g, NPC_NAME, "I should probably teach you how to manouver around this town without transport.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 240);
		AddItemToInventory(g, g->ItemDB->item[2]);
		break;
	case 240:
		ShowConvo(g, NPC_NAME, "Follow me.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 241);
		break;
	case 241:
		g->story.missions[0].prog = 21;
		HideConvo(g);
		CloseCutScene(g);
		break;
	case 242:
		HideConvo(g);
		g->story.missions[0].prog = 23;
		break;
	case 243:
		ShowConvo(g, PLR_NAME, "Yeah, it seems two jumps is all I can do before I run out of stamina.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 244);
		break;
	case 244:
		ShowConvo(g, NPC_NAME, "Don't worry though, you can increase your stamina as you level up.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 245);
		break;
	case 245:
		ShowConvo(g, NPC_NAME, "Also some items can passively increase your stamina as well.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 246);
		break;
	case 246:
		ShowConvo(g, NPC_NAME, "But be wise, as you can only equip 3 items at a time.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 247);
		break;
	case 247:
		ShowConvo(g, PLR_NAME, "Thanks for the tips. So what's my next step?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 248);
		break;
	case 248:
		ShowConvo(g, NPC_NAME, "Well if you want to leave this city, you will need some sort of ID.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 249);
		break;
	case 249:
		ShowConvo(g, NPC_NAME, "I have a friend who can make a fake ID for you, but he lives underground.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 250);
		break;
	case 250:
		ShowConvo(g, NPC_NAME, "If you want to meet him, you will need to go into the sewers as the main entrance is guarded.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 251);
		break;
	case 251:
		ShowConvo(g, NPC_NAME, "But don't worry, it's not as dangerious as it sounds.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 252);
		break;
	case 252:
		ShowConvo(g, NPC_NAME, "Nothing more than a few bats and killer slimes.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 253);
		break;
	case 253:
		ShowConvo(g, PLR_NAME, "K...k-killer? S-slimes...?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 254);
		break;
	case 254:
		ShowConvo(g, NPC_NAME, "That's not a worry right now. First you need to find a weapon.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 255);
		break;
	case 255:
		ShowChoices(g, "What do you say?",
			"1) I think I wanna go home now...", 256,
			"2) Hell yeah! This sounds like an adventure!", 257,
			"3) Where can I find a weapon?", 258);
		break;
	case 256:
		ShowConvo(g, NPC_NAME, "Nonsense! Don't let fear take over your mind!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 258);
		break;
	case 257:
		ShowConvo(g, NPC_NAME, "That's what I want to hear! You have a fire inside you.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 258);
		break;
	case 258:
		ShowConvo(g, NPC_NAME, "Since weapons are banned in this city, the only place I can think of is an old dojo.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 259);
		break;
	case 259:
		ShowConvo(g, NPC_NAME, "It has been locked away for a long time now, but the rooftop is still accessible.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 260);
		break;
	case 260:
		ShowConvo(g, NPC_NAME, "Just sneak in trough the window, grab a wooden sword and come back.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 261);
		break;
	case 261:
		ShowConvo(g, NPC_NAME, "Then I'll teach you how to use it.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 262);
		break;
	case 262:
		g->story.missions[0].prog = 33;
		Fade(g, FADE_OUT);
		StoryPause(g, 2.0, 264);
		break;
	case 264:
		//Transition
		ChangeMap(g, "maps/Netiki/Level-01.dem");
		g->story.missions[0].prog = 34;
		Fade(g, FADE_IN);
		StoryPause(g, 2.0, 265);
		break;
	case 265:
		ShowConvo(g, NPC_NAME, "I'm too old for this so I'll wait here...", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 266);
		break;
	case 266:
		ShowConvo(g, NPC_NAME, "Break a leg.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		DropItem(g, g->ItemDB->item[4], 15690, -2436);
		break;
	case 267:
		Fade(g, FADE_IN);
		g->pl.x = 15500;
		g->pl.y = -2415;
		break;
	case 268:
		ShowConvo(g, NPC_NAME, "Oh you were faster than I imagined.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 269);
		break;
	case 269:
		ShowConvo(g, NPC_NAME, "Let's go underground. I will teach you how to use a weapon.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 270);
		break;
	case 270:
		// Jump off
		g->story.missions[0].prog = 36;
		StoryPause(g, 2.0, 271);
		Fade(g, FADE_OUT);
		break;
	case 271:
		ChangeMap(g, "maps/Netiki/Netiki-02.dem");
		g->monster[0].x = 2048;
		g->monster[0].y = -1400;
		g->pl.x = 2000;
		g->pl.y = -1400;
		Fade(g, FADE_IN);
		// Jump near cave
		g->story.missions[0].prog = 37;
		StoryPause(g, 2.0, 272);
		break;
	case 272:
		ShowConvo(g, NPC_NAME, "Here's the cave.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 274);
		g->monster[0].direction = 1;
		g->story.missions[0].prog = 38;
		break;
	case 273:
		ChangeMap(g, "maps/Netiki/Level-02.dem");
		Fade(g, FADE_IN);
		g->story.missions[0].prog = 40;
		NewObjective(g, 2, "STORY OBJECTIVE:", "Find someone who will help you", "with making a fake ID.");
		NewObjective(g, 2, "Level Tip:", "Equip flashlight to scare away bats.");
		NewObjective(g, 2, "Level Tip:", "Equip sword to enable melee skills.");
		NewObjective(g, 2, "Level Tip:", "Use the <Basic Slash> skill to fight", "slimes and bats.");
		break;
	case 274:
		ShowConvo(g, NPC_NAME, "I'm giving you the Book Of Netiki. You can access it in your skills menu.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 275);
		LearnBook(g, BOOK_NETIKI);
		break;
	case 275:
		ShowConvo(g, NPC_NAME, "Your first skill is Basic Slash. Drag it onto a skill slot to use it.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 276);
		break;
	case 276:
		HideConvo(g);
		CloseCutScene(g);
		NewObjective(g, 3, "OBJECTIVE:", "Equip a skill to use it in battle.");
		g->Interfaces[INTERFACE_TUTORIAL2].visible = true;
		break;
	case 277:
		ShowConvo(g, PLR_NAME, "Asuwish sent me... He said you can make me a fake ID.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 278);
		break;
	case 278:
		ShowConvo(g, NPC_NAME, "Asuwish? Are you sure you have the right name?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 279);
		break;
	case 279:
		ShowConvo(g, NPC_NAME, "Wait, nevermind. I think I know exactly who you have in mind.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 280);
		break;
	case 280:
		ShowConvo(g, NPC_NAME, "I can make you an ID right now. Give me a moment. All you have to do is smile.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 281);
		break;
	case 281:
		StoryPause(g, 2.0, 282);
		Fade(g, FADE_OUT);
		break;
	case 282:
		StoryPause(g, 2.0, 283);
		Fade(g, FADE_IN);
		break;
	case 283:
		ShowConvo(g, NPC_NAME, "Here you go.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 284);
		AddItemToInventory(g, g->ItemDB->item[5]);
		break;
	case 284:
		ShowConvo(g, PLR_NAME, "My name is... Bond? James Bond?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 285);
		break;
	case 285:
		ShowConvo(g, NPC_NAME, "Don't worry, this ID is temporary for now, use it to leave the city.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 286);
		break;
	case 286:
		ShowConvo(g, PLR_NAME, "Thank you, I will be going now.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 287);
		break;
	case 287:
		g->story.missions[0].prog = 41;
		NewObjective(g, 4, "STORY OBJECTIVE:", "Leave the cave to return back", "on ground level.");
		break;
	case 288:
		ChangeMap(g, "maps/Netiki/Netiki-02.dem");
		g->monster[0].x = 2827;
		g->monster[0].y = -48;
		g->pl.x = 2536;
		g->pl.y = -48;
		Fade(g, FADE_IN);
		if (PlayerHasItem(g, 5)) {
			g->story.missions[0].prog = 42;
		} else {
			NewObjective(g, 1, "STORY OBJECTIVE:", "Explore Netiki's underground cave.");
		}
		break;
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
	HandleMissions(g);
	HandleCutScene(g);
	HandleConvo(g);
	HandleEventTriggers(g);
}

static void Click(Game* g, int button, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Click
	if (g->convo->convo_type == CONVO_TYPE_CHOICE) {
		int x = GAME_WIDTH / 2 - 300;
		int y = GAME_HEIGHT / 2 - 100;
		for (int i = 0; i < 4; i++) {
			if (g->allow_input) {
				if (MouseIsOn(g, x + 26, y + 48 + (i * 32), 540, 26)) {
					if (g->convo->choice[i] != "" && g->convo->choice_next_event_id[i] != -2) {
						g->allow_input = false;
						if (g->convo->choice_next_event_id[i] == -1) {
							g->convo->timer = -1;
							if (g->cut_scene.timer > -1 || g->cut_scene.state == CUTSCENE_STATE_REST) {
								CloseCutScene(g);
							}
						} else {
							g->convo->event_id = g->convo->choice_next_event_id[i];
							if (g->convo->small_talk) {
								HandleSTEvent(g);
							} else {
								HandleConvoEvent(g);
							}
						}
					}
				}
			}
		}
	}
}

static void Press(Game* g, int kid, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	if (kid == ALLEGRO_KEY_ENTER && g->convo->timer > -1 && !g->convo->exploding && g->convo->convo_type != CONVO_TYPE_STORY && g->convo->convo_type != CONVO_TYPE_CHOICE) {
		g->allow_input = false;
		g->convo->event_id = g->convo->next_event_id;
		g->convo->next_event_id = -1;
		if (g->convo->event_id == -1) {
			// Close
			HideConvo(g);
			if (g->cut_scene.timer > -1 || g->cut_scene.state == CUTSCENE_STATE_REST) {
				CloseCutScene(g);
			}
		} else {
			// Next
			if (g->convo->small_talk) {
				HandleSTEvent(g);
			} else {
				HandleConvoEvent(g);
			}
		}
	} else if(g->convo->convo_type == CONVO_TYPE_CHOICE && g->convo->timer > -1){
		for (int i = 0; i < 4; i++) {
			if (kid == (ALLEGRO_KEY_1 + i) || kid == (ALLEGRO_KEY_PAD_1 + i)) {
				if (g->convo->choice[i] != "" && g->convo->choice_next_event_id[i] != -2) {
					g->allow_input = false;
					if (g->convo->choice_next_event_id[i] == -1) {
						g->convo->timer = -1;
						if (g->cut_scene.timer > -1 || g->cut_scene.state == CUTSCENE_STATE_REST) {
							CloseCutScene(g);
						}
					} else {
						g->convo->event_id = g->convo->choice_next_event_id[i];
						if (g->convo->small_talk) {
							HandleSTEvent(g);
						} else {
							HandleConvoEvent(g);
						}
					}
				}
			}
		}
	} else if (kid == g->keys.crouch_bind && !g->chat.type_chat && (g->convo->timer <= -1 || g->convo->exploding || g->cut_scene.state == CUTSCENE_STATE_CLOSING)) {
		if (HandleEventTriggersOnAction(g) > -1) {
			g->allow_input = false;
		}
	}
	// Press
}


/* =========================== Register Function ==========================
*		The register function.
*		Registers the event functions to be triggered by the main loop.
*		May change the name and the event types to match the class purpose.
*
*		Call it once with the rest of init functions on main().
*/
int RegisterConvoEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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