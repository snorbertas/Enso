#include <iostream>
#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include "Game.hpp"
#include "DEEvents.h"
#include "NPC.h"
#include "DEText.h"
#include "SmallTalk.h"

void HandleAI(Game* g) {
	// Shortened pointers
	Player* p = &g->pl;

	for (int i = 0; i < MAX_MONSTERS; i++) {
		Monster* m = &g->monster[i];
		if(m->health > 0 && !m->server_npc){
			if (m->type == NPC_NETIKI_VILLAGER || m->type == NPC_MANNID) {
				m->timer--;
				if (g->convo_with != m) {
					if (m->timer <= 0 && m->state == NPC_STATE_IDLE) {
						// Initiate walking
						m->state = NPC_STATE_WALKING;
						m->target_x = m->zone.x + rand() % m->zone.w;
					}
					if (m->state == NPC_STATE_WALKING) {
						// Check if we're at the target
						if (m->x > m->target_x - 5 && m->x < m->target_x + 5) {
							m->timer = SecondsToTicks(1.0) + rand() % SecondsToTicks(1.0);
							m->state = NPC_STATE_IDLE;
						}

						// Check which direction to go
						if (m->target_x > m->x) {
							m->x_vel = MAX_X_VEL / 4;
						} else {
							m->x_vel = -MAX_X_VEL / 4;
						}
					}
				}
			}
			if (m->type == NPC_MAID) {
				// Jump demo
				if (g->story.missions[0].prog == 17) {
					if (m->direction == 1) {
						// Approach wall (to the left)
						// Move left
						m->x_vel = -MAX_X_VEL / 3;
						if (m->x <= 1566 && m->in_air == false) {
							// Jump to wall
							m->y_vel = JUMP_VEL;
							m->in_air = true;
						}
						if (m->x <= 1537) {
							// Jump off wall
							m->direction = 0;
							m->x++;
							m->y_vel = JUMP_VEL;
							m->x_vel = MAX_X_VEL;
						}
					} else {
						// Jumping from wall towards table
						if (m->in_air == true) {
							m->x_vel = MAX_X_VEL;
							if (m->y_vel == 0) {
								m->in_air = false;
							}
						} else {
							if (m->x <= 1790) {
								m->x_vel = MAX_X_VEL / 3;
							} else {
								// Gg done
								ShowConvo(g, m->name, "Give it a go!", m->x, m->y, CONVO_TYPE_NORMAL, 180);
								g->story.missions[0].prog = 18; // Temp
							}
						}
					}
				}
			}
			if (m->type == NPC_HOMELESS_MAN) {
				if (m->in_air) {
					if (m->direction == 0) {
						if (m->x_vel < 3.5) m->x_vel = 3.5;
					} else {
						if (m->x_vel > -3.5) m->x_vel = -3.5;
					}
					if (m->OnAFloor(g)) {
						m->in_air = false;
					}
				}
				if (g->story.missions[0].prog == 21) {
					if (m->y == -48) {
						m->x_vel = -3.5;
						m->y_vel = JUMP_VEL;
						m->in_air = true;
					}
					if (m->y == -112) {
						m->x_vel = 3.5;
						if (m->x >= 1440){
							m->y_vel = JUMP_VEL;
							m->in_air = true;
						}
					}
					if (m->y == -208) {
						if (m->x < 1660) {
							m->x_vel = 3.5;
						} else {
							m->x_vel = -1;
							g->convo_with = m;
							ShowConvo(g, NPC_NAME, "Come on! I'll teach you! Just don't forget to wear your shoes!", NPC_X, NPC_Y, CONVO_TYPE_SHOUT, 242);
							g->Interfaces[INTERFACE_TUTORIAL2].visible = true;
							g->story.missions[0].prog = 22;
						}
					}
				}
				if (g->story.missions[0].prog == 24) {
					if (m->y == -208) {
						m->x_vel = 3.5;
						if (m->x >= 1880) {
							m->y_vel = JUMP_VEL;
							m->in_air = true;
						}
					}
					if (m->x == 1967) {
						m->x_vel = -MAX_X_VEL;
						m->y_vel = JUMP_VEL;
						m->in_air = true;
					}
					if (m->y == -432) {
						m->x_vel = -3.5;
						if (m->x <= 1630) {
							g->story.missions[0].prog = 25;
							m->x_vel = 1;
						}
					}
				}
				if (g->story.missions[0].prog == 26) {
					if (m->y == -432) {
						m->x_vel = -3.5;
						if (m->x <= 1580 && m->x > 1400) {
							m->y_vel = JUMP_VEL;
							m->in_air = true;
						}
						if (m->x <= 1200) {
							g->story.missions[0].prog = 27;
							m->x_vel = 1;
						}
					}
				}
				if (g->story.missions[0].prog == 28) {
					if (m->y == -432) {
						m->x_vel = -3.5;
						if (m->x <= 1000) {
							m->x_vel = -3.5;
							m->y_vel = JUMP_VEL;
							m->in_air = true;
						}
					} else if (m->x == 929) {
						m->x_vel = MAX_X_VEL;
						m->y_vel = JUMP_VEL;
						m->in_air = true;
					}
					if (m->y == -688) {
						m->x_vel = 3.5;
						if (m->x >= 1160) {
							g->story.missions[0].prog = 29;
							m->x_vel = -1;
						}
					}
				}
				if (g->story.missions[0].prog == 30) {
					if (m->x < 2300) {
						m->x_vel = 3.5;
						if (m->y == -688) {
							if (m->x >= 1380) {
								m->y_vel = JUMP_VEL;
								m->in_air = true;
							}
						}
						if (m->y == -784) {
							if (m->x >= 1590) {
								m->y_vel = JUMP_VEL;
								m->in_air = true;
							}
						}
						if (m->y == -880) {
							if (m->x >= 1900) {
								m->y_vel = JUMP_VEL;
								m->in_air = true;
							}
						}
					} else {
						g->story.missions[0].prog = 31;
						m->x_vel = -1;
						// Can talk to him now
					}
				}
				if (g->story.missions[0].prog == 33) {
					if (m->x < 2600) {
						m->x_vel = 3.5;
						g->pl.x_vel = 3.5;
						if (m->y == -944 && m->x >= 2380) {
							m->y_vel = JUMP_VEL;
							m->in_air = true;
						}
						if (g->pl.y == -944 && g->pl.x >= 2380) {
							g->pl.y_vel = JUMP_VEL;
							g->pl.in_air = true;
						}
					} else {
						g->story.missions[0].prog = 34;
					}
				}
				if (g->story.missions[0].prog == 36) {
						m->x_vel = 3.5;
						g->pl.x_vel = 3.5;
				}
				if (g->story.missions[0].prog == 37) {
					if (p->x < 2770) {
						m->x_vel = 3.5;
						g->pl.x_vel = 3.5;
					}
				}
			}
			if (m->type == MONSTER_BUTTERFLY) {
				if (m->x > m->zone.x + m->zone.w) m->direction = 1;
				if (m->x < m->zone.x) m->direction = 0;
				m->y_vel = -3 + rand() % 7;
				m->x_vel = 1 + rand() % 3;
				if (m->direction == 1) {
					m->x_vel = -3 + rand() % 3;
				}
				m->animation++;
				if (m->animation == 30) m->animation = 0;
				m->frame = 0;
				if (m->animation > 10) {
					m->frame = 1;
				}
				if (m->animation > 20) {
					m->frame = 2;
				}
				m->timer--;
				if (m->timer <= 0) {
					m->timer = 100 + rand() % 900;
				}
			}
			if (m->type == NPC_SLIME) {
				if (m->combat_timer <= SecondsToTicks(1.8)) {
					if (collide(&g->pl, &m->zone)) {
						if (m->x < g->pl.x) {
							m->x_vel = 1;
						} else if (m->x > g->pl.x) {
							m->x_vel = -1;
						}
						if (collide(&g->pl, m)) {
							// Temp
							if (g->pl.combat_timer <= SecondsToTicks(1.8)) {
								g->pl.y_vel = -5;
								g->pl.x_vel = -5;
								if (g->pl.x > m->x) g->pl.x_vel = 5;
								g->pl.health--;
								CombatFeedback(g, 1, g->pl.x + (g->pl.w / 2), g->pl.y + (g->pl.h / 2));
								g->pl.combat_timer = SecondsToTicks(2.0);
							}
						}
					} else {
						m->timer--;
						if (m->timer <= 0 && m->state == NPC_STATE_IDLE) {
							// Initiate walking
							m->state = NPC_STATE_WALKING;
							m->target_x = m->zone.x + rand() % m->zone.w;
						}
						if (m->state == NPC_STATE_WALKING) {
							// Check if we're at the target
							if (m->x > m->target_x - 5 && m->x < m->target_x + 5) {
								m->timer = SecondsToTicks(1.0) + rand() % SecondsToTicks(1.0);
								m->state = NPC_STATE_IDLE;
							}

							// Check which direction to go
							if (m->target_x > m->x) {
								m->x_vel = MAX_X_VEL / 4;
							} else {
								m->x_vel = -MAX_X_VEL / 4;
							}
						}
					}
				}
			}
			if (m->type == NPC_BAT) {
				if (m->combat_timer <= SecondsToTicks(1.8)) {
					m->y_vel = 0;
					if (collide(&g->pl, &m->zone)) {
						if (g->flash_light && m->state != NPC_STATE_IDLE) {
							// Run away
							if (p->x > m->x) {
								m->x_vel = -MAX_X_VEL / 4;
							} else {
								m->x_vel = MAX_X_VEL / 4;
							}
							if (p->y > m->y) {
								m->y_vel = -MAX_Y_VEL / 4;
							} else {
								m->y_vel = MAX_Y_VEL / 4;
							}
							if (m->y > m->zone.y + m->zone.h) m->y = m->zone.y + m->zone.h;
							if (m->x < m->zone.x) m->x = m->zone.x;
							if (m->x > m->zone.x + m->zone.w) m->x = m->zone.x + m->zone.w;
							if (m->y <= m->zone.y + 1) {
								m->y = m->zone.y;
								if (m->x - p->x >= -300 || m->x - p->x <= 300) {
									m->state = NPC_STATE_IDLE;
								}
							}
						} else if(!g->flash_light){
							// Agro
							m->state = NPC_STATE_WALKING;
							if (p->x > m->x) {
								m->x_vel = MAX_X_VEL / 4;
							} else {
								m->x_vel = -MAX_X_VEL / 4;
							}
							if (p->y > m->y) {
								m->y_vel = MAX_Y_VEL / 4;
							} else {
								m->y_vel = -MAX_Y_VEL / 4;
							}
							if (collide(&g->pl, m)) {
								// Temp
								if (g->pl.combat_timer <= SecondsToTicks(1.8)) {
									g->pl.y_vel = -5;
									g->pl.x_vel = -5;
									if (g->pl.x > m->x) g->pl.x_vel = 5;
									g->pl.health--;
									CombatFeedback(g, 1, g->pl.x + (g->pl.w / 2), g->pl.y + (g->pl.h / 2));
									g->pl.combat_timer = SecondsToTicks(2.0);
								}
							}
						} else {
							if (m->zone.y + 1 < (int)m->y) {
								m->state = NPC_STATE_WALKING;
							}
						}
					} else {
						// Sleep
						if (m->state != NPC_STATE_IDLE) {
							// Seek where to sleep
							if (m->zone.y + 1 < (int)m->y) {
								m->y_vel = -MAX_Y_VEL / 4;
								if ((int)m->x != (int)(m->zone.x + m->zone.w / 2)) {
									if (m->x < m->zone.x + m->zone.w / 2) {
										m->x_vel = MAX_X_VEL / 4;
									} else {
										m->x_vel = -MAX_X_VEL / 4;
									}
								} else {
									m->x_vel = 0;
								}
							} else {
								m->state = NPC_STATE_IDLE;
							}
						} else {
							// Zzz
							if (m->zone.y + 1 < (int)m->y) {
								m->state = NPC_STATE_WALKING;
							}
						}
					}
				}
			}
		}

		if (m->combat_timer > 0) m->combat_timer--;
		if (m->combat_timer <= 0 && m->health <= 0) {
			if (m->type == NPC_SLIME || m->type == NPC_BAT) {
				m->type = -1;
				SpawnExperience(g, m->x + (m->w / 2), m->y, 2);
			}
		}
	}
}

bool NPC::OnAFloor(Game* g) {
	Platform* plt = g->lvl->Platform;
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if ((int)this->y == plt[i].y - this->h &&
			(int)this->x >= plt[i].x - this->h &&
			(int)this->x <= plt[i].x + plt[i].w) {
			return true;
		}
	}
	return false;
}

void HandleNPC(Game* g) {
	for (int i = 0; i < MAX_MONSTERS; i++) {
		Monster* npc = &g->monster[i];
		if (npc->type > -1) {
			npc->animation++;
			if (npc->animation >= 40) npc->animation = 0;

			npc->blink--;
			if (npc->blink <= 0) npc->blink = 500;

			if (npc->health <= 0 && npc->type == NPC_SLIME) {
				npc->frame = 2;
			} else if (npc->health <= 0 && npc->type == NPC_BAT) {
				npc->frame = 2;
			} else if (npc->type == NPC_BAT) {
				if (npc->state == NPC_STATE_IDLE) {
					npc->frame = 3;
				} else {
					if (npc->animation > 14) {
						npc->frame = 0;
					} else {
						npc->frame = 1;
					}
				}
			} else if (npc->y_vel == 0) {
				if (npc->animation > 14) {
					npc->frame = 0;
				} else {
					npc->frame = 1;
				}
			} else if (npc->y_vel > 0) {
				npc->frame = 3;
			} else if (npc->y_vel < 0) {
				if (npc->type == NPC_SLIME) {
					// Nothing
				} else {
					npc->frame = 2;
				}
			}
		}
	}
}

int HandleNPCTriggerOnAction(Game* g) {
	for (int i = 0; i < MAX_MONSTERS; i++) {
		if (collide(&g->pl, &g->monster[i])) {
			g->convo_with = &g->monster[i];
			switch (g->monster[i].type) {
			case NPC_WEST_GUARD:
				ShowChoices(g, "What do you say?",
					"1) Hello", 146,
					"2) What's behind that door?", 149,
					"3) Can you let me trough please?", 152,
					"4) [Leave without saying anything]", -1);
				return i;
			case NPC_NETIKI_VILLAGER:
				SmallTalk(g, 1 + (rand() % SMALL_TALKS));
				return i;
			case NPC_CAFE_OWNER:
				ShowConvo(g, NPC_NAME, "I'm very sorry, but I'm really busy. Please ask the maid to assist you.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
				return i;
			case NPC_MAID:
				if (g->story.missions[0].prog == 14) {
					StartCutScene(g);
					ShowConvo(g, NPC_NAME, "Welcome to Netiki's Cafe, please take a seat upstairs so I can take your order.",
						NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 154);
				} else if (g->story.missions[0].prog == 16) {
					StartCutScene(g);
					ShowConvo(g, NPC_NAME, "Is everything alright sir? I have your order already. But there are no tables downstairs.",
						NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 176);
				} else {
					ShowConvo(g, NPC_NAME, "Hello again! (TODO: extend this confo)", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
				}
				return i;
			case NPC_SHOP_OWNER:
				ShowConvo(g, NPC_NAME, "Welcome to my shop, how can I help you?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 189);
				return i;
			case NPC_HOMELESS_MAN:
				if (g->story.missions[0].prog == 19) {
					StartCutScene(g);
					ShowConvo(g, PLR_NAME, "Excuse me, sir.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 201);
				} else if (g->story.missions[0].prog == 20) {
					ShowConvo(g, NPC_NAME, "Have you found my cloth?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 232);
					g->lvl->right_bound = 1180;
				} else if (g->story.missions[0].prog == 31) {
					ShowConvo(g, NPC_NAME, "Have you noticed how wall jumps drain your stamina?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 243);
					g->story.missions[0].prog = 32;
					g->Interfaces[INTERFACE_TUTORIAL2].visible = false;
				} else if (PlayerHasItem(g, 4) && g->story.missions[0].prog < 35) {
					StartCutScene(g);
					ShowConvo(g, PLR_NAME, "I found the sword!", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 268);
					g->story.missions[0].prog = 35;
				} else {
					ShowConvo(g, NPC_NAME, "Good luck! Have a good journey to the east. [TODO: add more]", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
				}
				return i;
			case NPC_MANNID:
				if (PlayerHasItem(g, 5)) {
					ShowConvo(g, NPC_NAME, "Can I do anything else for you? [TODO: add more]", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
				} else {
					StartCutScene(g);
					ShowConvo(g, NPC_NAME, "It's rare for me to have guests here. What brings you to my home?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 277);
				}
				return i;
			}
		}
	}
	return -1;
}

NPC::NPC() {

}

NPC::~NPC() {

}

void WipeNPC(Game* g) {
	for (int i = 0; i < MAX_MONSTERS; i++) {
		g->monster[i].type = -1;
	}
}

int SpawnNPC(Game* g, NPC npc, int x, int y, Platform* zone) {
	NPC temp = npc;
	temp.x = x;
	temp.y = y;
	if (zone != NULL) temp.zone = *zone;
	temp.blink = rand() % 500;
	temp.animation = rand() % 40;

	for (int i = 0; i < MAX_MONSTERS; i++) {
		if (g->monster[i].type < 0) {
			g->monster[i] = temp;
			return i;
		}
	}
	return -1;
}

void HandleEventNPCSpawns(Game* g) {
	for (int j = 0; j < MAX_EVENTS; j++) {
		if (g->lvl->Event[j].type > -1) {
			Platform* spawn = &g->lvl->Event[j];
			switch (spawn->type) {
			case 1000:
				SpawnNPC(g, g->NPCDB->NPC[0], spawn->x, spawn->y, spawn);
				break;
			case 1001:
				SpawnNPC(g, g->NPCDB->NPC[1], spawn->x, spawn->y, spawn);
				break;
			case 1002:
				SpawnNPC(g, g->NPCDB->NPC[2], spawn->x, spawn->y, spawn);
				break;
			case 1003:
				SpawnNPC(g, g->NPCDB->NPC[3], spawn->x, spawn->y, spawn);
				break;
			case 1004:
				SpawnNPC(g, g->NPCDB->NPC[4], spawn->x, spawn->y, spawn);
				break;
			case 1005:
				SpawnNPC(g, g->NPCDB->NPC[5], spawn->x, spawn->y, spawn);
				break;
			case 1006:
				SpawnNPC(g, g->NPCDB->NPC[6], spawn->x, spawn->y, spawn);
				break;
			case EE_SLIME_SPAWN:
				SpawnNPC(g, g->NPCDB->NPC[NPC_SLIME], spawn->x + (spawn->w/2), spawn->y, spawn);
				break;
			case EE_BAT_SPAWN:
				SpawnNPC(g, g->NPCDB->NPC[NPC_BAT], spawn->x + (spawn->w / 2), spawn->y, spawn);
				break;
			case EE_MANNID_SPAWN:
				SpawnNPC(g, g->NPCDB->NPC[NPC_MANNID], spawn->x + (spawn->w / 2), spawn->y, spawn);
				break;
			}
		}
	}
}

NPC CreateNPC(int type, int w, int h, int body_id, int hair_id, int acc_id, int outfit_id, int level, int hp, const char* name, bool eyes) {
	NPC temp;
	temp.type = type;
	temp.w = w;
	temp.h = h;
	temp.sprite = body_id;
	temp.hair = hair_id;
	temp.acc = acc_id;
	temp.outfit = outfit_id;
	temp.level = level;
	temp.health = hp;
	temp.max_health = hp;
	temp.name = name;
	temp.eyes = eyes;
	return temp;
}

NPCDatabase::NPCDatabase() {
	// Body, hair, acc, outfit
	this->NPC[0] = CreateNPC(NPC_WEST_GUARD, 48, 48, 0, -1, 4, 4, 0, 1, "West Guard", true);
	this->NPC[1] = CreateNPC(NPC_SHOP_OWNER, 48, 48, 0, 0, -1, 12, 0, 1, "Hostle Owner", true);
	this->NPC[2] = CreateNPC(NPC_BUTTERFLY, 48, 48, -1, -1, -1, -1, 0, 1, "Butterfly", false);
	this->NPC[3] = CreateNPC(NPC_MAID, 48, 48, 0, -1, 8, 16, 0, 1, "Maid", true);
	this->NPC[4] = CreateNPC(NPC_NETIKI_VILLAGER, 48, 48, 0, -1, -1, -1, 0, 1, "Netiki", true);
	this->NPC[5] = CreateNPC(NPC_CAFE_OWNER, 48, 48, 0, 0, -1, 8, 0, 1, "Cafe Owner", true);
	this->NPC[6] = CreateNPC(NPC_HOMELESS_MAN, 48, 48, 78, 12, -1, 20, 432, 1, "Homeless Man", true);
	this->NPC[7] = CreateNPC(NPC_SLIME, 57, 34, 25, -1, -1, -1, 1, 50, "Slime", false);
	this->NPC[8] = CreateNPC(NPC_BAT, 88, 50, 14, -1, -1, -1, 1, 20, "Bat", false);
	this->NPC[9] = CreateNPC(NPC_MANNID, 48, 48, 70, -1, 0, 0, 1111, 1, "Mannid", true);
}

NPCDatabase::~NPCDatabase() {

}

/* ============================= Event Functions ==========================
 *		Used according to the event triggered. (Do NOT define in header)
 *			- Game ticks
 *			- Mouse clicks
 *			- Keyboard presses
 */
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	HandleAI(g);
	HandleNPC(g);
}

static void Click(Game* g, int button, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Click
}

static void Press(Game* g, int kid, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Press
	if (kid == g->keys.crouch_bind && !g->chat.type_chat && (g->convo->timer <= -1 || g->convo->exploding || g->cut_scene.state == CUTSCENE_STATE_CLOSING)) {
		if (HandleNPCTriggerOnAction(g) > -1) {
			g->allow_input = false;
		}
	}
}


/* =========================== Register Function ==========================
 *		The register function.
 *		Registers the event functions to be triggered by the main loop.
 *		May change the name and the event types to match the class purpose.
 *
 *		Call it once with the rest of init functions on main().
 */
int RegisterNPCEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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