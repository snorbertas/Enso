#define MAX_PACKETS 50

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include "Level.h"
#include "Game.hpp"
#include "NPC.h"
#include "Item.h"

// Temp
void HandleCommand(Game* g, const char* msg);

Game::Game(){
	this->PacketQueue = new DEPacket[50];
	for (int i = 0; i < 50; i++) {
		this->PacketQueue[i].packet_type = -1;
	}

	logini.logging_in = false;
	logini.try_login = false;

	// Load databases
	this->ItemDB = new ItemDatabase();
	this->NPCDB = new NPCDatabase();

	this->monster = new Monster[MAX_MONSTERS];

	LearnBook(this, BOOK_ADMIN);

	// Ability slots default
	AbilitySlots[0].key = ALLEGRO_KEY_Q;
	AbilitySlots[1].key = ALLEGRO_KEY_W;
	AbilitySlots[2].key = ALLEGRO_KEY_E;
	AbilitySlots[3].key = ALLEGRO_KEY_R;
	AbilitySlots[4].key = ALLEGRO_KEY_A;
	AbilitySlots[5].key = ALLEGRO_KEY_S;
	AbilitySlots[6].key = ALLEGRO_KEY_D;
	AbilitySlots[7].key = ALLEGRO_KEY_F;

	this->cloud = new Cloud[3];
	cloud[0].x = -100;
	cloud[0].y = -200;
	cloud[1].y = -800;
	cloud[1].x = 300;
	cloud[2].type = 7;
	this->pl.hbox = new HitBox[MAX_PLAYER_HITBOXES];
	this->anim = new GFXAnimation[MAX_ANIMATIONS];

	// Stars
	for (int i = 0; i < MAX_STARS; i++) {
		this->Stars[i].x = -100 + rand() % (BWIDTH + 500);
		this->Stars[i].y = rand() % BHEIGHT;
	}

	this->trail = new TrailAnim[50];
	for (int i = 0; i < 50; i++) {
		this->trail[i].angle = rand() % 360;
	}
	this->window_mode = 1;
	this->done = false;
	this->remake_display = false;
	this->music_volume = 1;

	this->lvl = new Level;

	this->Editor.target_drag = false;
	this->Editor.target_resize_h = false;
	this->Editor.target_resize_w = false;

	for (int i = 0; i < MAX_PLATFORMS; i++) {
		lvl->Platform[i].x = -999999;
		lvl->Platform[i].y = -999999;
		lvl->Platform[i].w = 30;
		lvl->Platform[i].h = 30;
	}
	this->lvl = LoadLevel("maps/Multiplayer.dem");
}

bool collide(Monster* m, Platform* pl) {
	if (m->x <= pl->x + pl->w &&
		m->x + m->w >= pl->x &&
		m->y <= pl->y + pl->h &&
		m->h + m->y >= pl->y) {
		return true;
	}
	return false;
}

bool collide(Monster* m, HitBox* h) {
	if (m->x <= h->x + h->w &&
		m->x + m->w >= h->x &&
		m->y <= h->y + h->h &&
		m->h + m->y >= h->y) {
		return true;
	}
	return false;
}

bool collide(Player* p, HitBox* h) {
	if (p->x <= h->x + h->w &&
		p->x + p->w >= h->x &&
		p->y <= h->y + h->h &&
		p->h + p->y >= h->y) {
		return true;
	}
	return false;
}

bool collide(Player* p, Platform* pl) {
	if (p->x <= pl->x + pl->w &&
		p->x + p->w >= pl->x &&
		p->y <= pl->y + pl->h &&
		p->h + p->y >= pl->y) {
		return true;
	}
	return false;
}

bool collide(Player* p, Monster* m) {
	if (p->x <= m->x + m->w &&
		p->x + p->w >= m->x &&
		p->y <= m->y + m->h &&
		p->h + p->y >= m->y) {
		return true;
	}
	return false;
}

bool collide(int o1x, int o1y, int o1w, int o1h, int o2x, int o2y, int o2w, int o2h) {
	if (o1x <= o2x + o2w &&
		o1x + o1w >= o2x &&
		o1y <= o2y + o2h &&
		o1h + o1y >= o2y) {
		return true;
	}
	return false;
}

bool collide(int o1x, int o1y, int o1r, int o2x, int o2y, int o2r) {
	int dx = o1x - o2x;
	int dy = o1y - o2y;
	int distance = sqrt(dx * dx + dy * dy);

	if (distance < o1r + o2r) {
		return true;
	}
	return false;
}

void Jump(Game* g) {
	Player* p = &g->pl;
	p->y_vel = JUMP_VEL;
	if (g->pl.in_water){
		p->y_vel = JUMP_VEL / 10;
	}
	p->in_air = true;
	p->letgo = 0;
}

void WallJump(Game* g) {

	std::cout << "yeah brosgklfdg\n";
	Player* p = &g->pl;
	p->energy -= COST_WALL_JUMP;
	p->y_vel = JUMP_VEL;
	p->y_acc = 0;
	p->x_vel = MAX_X_VEL;
	if (g->pl.in_water) {
		p->y_vel = JUMP_VEL / 10;
		p->x_vel = MAX_X_VEL / 2;
	}
	if (p->direction == 0) {
		p->x_vel *= -1;
		p->direction = 1;
	} else {
		p->direction = 0;
	}
	p->x_acc = 0;
	p->in_air = true;
	p->letgo = 0;
}

int GetFloorID(Game* g) {
	Player* p = &g->pl;
	Platform* plt = g->lvl->Platform;
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if ((int)p->y == plt[i].y - p->h &&
			(int)p->x >= plt[i].x - p->h &&
			(int)p->x <= plt[i].x + plt[i].w) {
			if (plt[i].type == 1000) {
				if (p->colour != 3) return -1;
			}
			return i;
		}
	}
	return -1;
}

bool OnAFloor(Game* g) {
	Player* p = &g->pl;
	Platform* plt = g->lvl->Platform;
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if ((int)p->y == plt[i].y - p->h &&
			(int)p->x >= plt[i].x - p->h &&
			(int)p->x <= plt[i].x + plt[i].w) {
			if (plt[i].type == 1000) {
				if (p->colour != 3) return false;
			}
			return true;
		}
	}
	return false;
}

bool OnAWall(Game* g) {
	Player* p = &g->pl;
	Platform* plt = g->lvl->Platform;
	if(p->in_water || p->wet) return false;
	for (int i = 0; i < MAX_PLATFORMS; i++) {
		if ((int)p->x == plt[i].x + (plt[i].w + 1) &&
			(int)p->y >= plt[i].y - (p->h - 1) &&
			(int)p->y <= plt[i].y + plt[i].h) {
			p->direction = 1;
			if (plt[i].type == 1000) {
				if (p->colour != 3) return false;
			}
			return true;
		}
		if ((int)p->x == plt[i].x - (p->w + 1) &&
			(int)p->y >= plt[i].y - (p->h - 1) &&
			(int)p->y <= plt[i].y + plt[i].h) {
			p->direction = 0;
			if (plt[i].type == 1000) {
				if (p->colour != 3) return false;
			}
			return true;
		}
	}
	return false;
}

// Lazy
#define AF(t, r) if(type==t) return r-1
int AnimFrames(int type) {
	AF(76, 3);
	AF(77, 3);
	AF(80, 3);
	AF(81, 3);
	AF(84, 3);
	AF(85, 3);
	AF(95, 2);
	return 3;
}

int StartAnimation(Game* g, int type, int x, int y, int flip, bool animated, int loop, int relative_to_player_id) {
	for (int i = 0; i < MAX_ANIMATIONS; i++) {
		if (g->anim[i].type == -1) {
			g->anim[i].animated = 0;
			if (animated) g->anim[i].animated = 1;
			g->anim[i].type = type;
			g->anim[i].flip = flip;
			g->anim[i].frames = AnimFrames(type);
			g->anim[i].x = x;
			g->anim[i].y = y;
			g->anim[i].loop = loop;
			g->anim[i].frame = 0;
			g->anim[i].relative_to_player = false;
			if (relative_to_player_id > MAX_PLAYERS) {
				g->anim[i].p = &g->pl;
				g->anim[i].relative_to_player = true;
			} else if (relative_to_player_id > -1) {
				g->anim[i].p = &g->Players[relative_to_player_id];
				g->anim[i].relative_to_player = true;
			}
			return i;
		}
	}
	return -1; // Can't play animation cuz out of memory
}

int CreateProjectile(Game* g, int type, int x, int y, int dir, HitBox* hbox) {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (g->projectiles[i].type == -1) {
			g->projectiles[i].type = type;
			g->projectiles[i].x = x;
			g->projectiles[i].y = y;
			g->projectiles[i].dir = dir;
			g->projectiles[i].timer = 0;
			g->projectiles[i].hitbox = hbox;
			return i;
		}
	}
	return -1;
}

int CreateHitBox(Game* g, Player* p, int type, int x, int y, int w, int h, Projectile* proj) {
	for (int i = 0; i < MAX_PLAYER_HITBOXES; i++) {
		if (p->hbox[i].type == -1) {
			p->hbox[i].type = type;
			p->hbox[i].x = x;
			p->hbox[i].y = y;
			p->hbox[i].w = w;
			p->hbox[i].h = h;
			p->hbox[i].projectile = proj;
			return i;
		}
	}
	return -1;
}

void ShowMessage(Game* g, const char* msg1, const char* msg2) {
	g->Message1 = msg1;
	g->Message2 = msg2;
	g->Interfaces[INTERFACE_MESSAGE].visible = true;
}

int SecondsToTicks(float seconds) {
	return (int)(seconds * TPS);
}

Level* LoadLevel(const char* file_path) {
	Level* temp = new Level;
	std::ifstream level_file(file_path, std::ios::binary);
	level_file.read((char*)temp, sizeof(Level));
	level_file.close();
	return temp;
}

void ClearItems(Game* g) {
	for (int i = 0; i < MAX_ITEMS_DROPPED; i++) {
		g->ItemOnMap[i].item = Item();
	}
}

void ChangeMap(Game* g, const char* file_path) {
	ClearItems(g);
	g->warping = false;
	WipeNPC(g);
	g->lvl = LoadLevel(file_path);

	if(g->lvl->rain) ToggleRain(g, true);

	g->last_save_loc = "";
	bool found_spawn = false;
	for (int j = 0; j < MAX_EVENTS; j++) {
		if (g->lvl->Event[j].type == EE_PLAYER_SPAWN) {
			g->pl.x = g->lvl->Event[j].x + rand() % g->lvl->Event[j].w;
			g->pl.y = g->lvl->Event[j].y + rand() % g->lvl->Event[j].h;
			j = MAX_EVENTS;
			found_spawn = true;
		}
	}
	HandleEventNPCSpawns(g);
	DestroyAllExperience(g);
	HandleExperienceSpawns(g);
}

void Fade(Game* g, int oi) {
	StopPlayer(g);
	if (oi == 0) g->fade_out = SecondsToTicks(2.1);
	if (oi == 1) g->fade_in = SecondsToTicks(2.1);
}

int CombatFeedback(Game* g, int value, int x, int y) {
	for (int i = 0; i < MAX_COMBAT_FEEDBACK; i++) {
		if (g->cfeedback[i].expire == 0) {
			g->cfeedback[i].value = value;
			g->cfeedback[i].x = x;
			g->cfeedback[i].y = y;
			g->cfeedback[i].expire = 60;
			return i;
		}
	}
	return -1;
}

int GetActiveBookID(Game* g) {
	for (int i = 0; i < 5; i++) {
		if (g->AbilityBooks[i].active) return g->AbilityBooks[i].book_id;
	}
	return -1;
}

void SelectBook(Game* g, int book_id) {
	g->AbilityBooks[0].active = false;
	g->AbilityBooks[1].active = false;
	g->AbilityBooks[2].active = false;
	g->AbilityBooks[3].active = false;
	g->AbilityBooks[4].active = false;
	g->AbilityBooks[book_id].active = true;
}

Ability CreateAbility(int id, int sprite_id, const char* name, const char* description, int r=100, int g=100, int b=255) {
	Ability ability;
	ability.id = id;
	ability.sprite_id = sprite_id;
	ability.name = name;
	ability.description = description;
	ability.r = r;
	ability.g = g;
	ability.b = b;
	return ability;
}

Ability GetAbility(int book_id, int slot) {
	if (book_id == BOOK_NETIKI) {
		if (slot == 0) {
			return CreateAbility(ABILITY_BASIC_SLASH, 7, "Basic Slash", "Basic melee attack.");
		}
	} else if (book_id == BOOK_ADMIN) {
		if (slot == 0) {
			return CreateAbility(ABILITY_ALPHASLASH, 0, "AlphaSlash", "Test purposes.");
		}
		if (slot == 1) {
			return CreateAbility(93, 4, "NULL", "NULL", 255, 100, 100);
		}
		if (slot == 2) {
			return CreateAbility(ABILITY_MEMEDASH, 5, "M3m3 D4sh", "xD", 255, 100, 100);
		}
		if (slot == 3) {
			return CreateAbility(ABILITY_BASIC_SPELL, 1, "43hy5ntjkrdfn", "WOW!!", 255, 50, 200);
		}
	}
	return CreateAbility(-1, 0, "NULL", "NULL", 0, 0, 0);
}

bool MouseIsOn(Game* g, int x, int y, int w, int h) {
	if (collide(x, y, w, h, g->pos_x, g->pos_y, 0, 0)) {
		return true;
	}
	return false;
}

bool UseAbility(Game* g, int ability_id) {
	Player* p = &g->pl;

	if(ability_id == ABILITY_ALPHASLASH || ability_id == ABILITY_BASIC_SLASH){
		if (OnAWall(g)) return false;
		if (p->last_cast > 0) return false;
		p->x_vel += 3;
		if (p->direction == 1) p->x_vel -=6;
		int r;
		if (p->slash) {
			r = 0;
			p->slash = false;
		} else {
			r = 1;
			p->slash = true;
		}
		if (g->connected) {
			DEPacket action_packet = MakePacket(PACKET_TYPE_ACTION, "", std::to_string(ABILITY_ALPHASLASH).c_str());
			if(r == 0) action_packet.val1 = 76;
			if(r == 1) action_packet.val1 = 80;
			AddPacketToQueue(g, action_packet);
			g->ReadyToSendPackets = true;
		}
		if (r == 0) {
			if (p->direction == 0) {
				p->last_cast = 20;
				StartAnimation(g, 77, p->x - 72, p->y - 72, p->direction, true, 0);
				StartAnimation(g, 76, -72, -72, p->direction, false, 0, MAX_PLAYERS + 1);
				CreateHitBox(g, &g->pl, 0, g->pl.x + g->pl.w, g->pl.y, 52, 52);
			} else {
				p->last_cast = 20;
				StartAnimation(g, 77, p->x - 72, p->y - 72, p->direction, true, 0);
				StartAnimation(g, 76, -72, -72, p->direction, false, 0, MAX_PLAYERS + 1);
				CreateHitBox(g, &g->pl, 0, g->pl.x - 52, g->pl.y, 52, 52);
			}
			p->ability_animation = 3;
		}
		if (r == 1) {
			if (p->direction == 0) {
				p->last_cast = 20;
				StartAnimation(g, 81, p->x - 72, p->y - 72, p->direction, true, 0);
				StartAnimation(g, 80, -72, -72, p->direction, false, 0, MAX_PLAYERS + 1);
				CreateHitBox(g, &g->pl, 0, g->pl.x + g->pl.w, g->pl.y, 52, 52);
			} else {
				p->last_cast = 20;
				StartAnimation(g, 81, p->x - 72, p->y - 72, p->direction, true, 0);
				StartAnimation(g, 80, -72, -72, p->direction, false, 0, MAX_PLAYERS + 1);
				CreateHitBox(g, &g->pl, 0, g->pl.x - 52, g->pl.y, 52, 52);
			}
			p->ability_animation = 2;
		}
	}
	if (ability_id == ABILITY_BASIC_SPELL) {
		// Shoot
		if (p->direction == 0) {
			p->last_cast = 20;
			int hbox_id = CreateHitBox(g, &g->pl, ABILITY_BASIC_SPELL, g->pl.x + g->pl.w, g->pl.y, 32, 32);
			if (hbox_id > -1) {
				CreateProjectile(g, ABILITY_BASIC_SPELL, g->pl.x + g->pl.w, g->pl.y, p->direction, &g->pl.hbox[hbox_id]);
			}
		} else {
			p->last_cast = 20;
			int hbox_id = CreateHitBox(g, &g->pl, ABILITY_BASIC_SPELL, g->pl.x - 52, g->pl.y, 32, 32);
			if (hbox_id > -1) {
				CreateProjectile(g, ABILITY_BASIC_SPELL, g->pl.x - 52, g->pl.y, p->direction, &g->pl.hbox[hbox_id]);
			}
		}
		p->ability_animation = 2;
		if (g->connected) {
			DEPacket action_packet = MakePacket(PACKET_TYPE_ACTION, "", std::to_string(ABILITY_BASIC_SPELL).c_str());
			AddPacketToQueue(g, action_packet);
			g->ReadyToSendPackets = true;
		}
	}

	if (ability_id == 93) {
		g->pl.y_vel = -10;
		g->pl.colour = 3;
		if (g->connected) {
			DEPacket action_packet = MakePacket(PACKET_TYPE_ACTION, "", std::to_string(93).c_str());
			AddPacketToQueue(g, action_packet);
			g->ReadyToSendPackets = true;
		}
	}
	if (ability_id == ABILITY_MEMEDASH) {
		g->pl.x_vel = MAX_X_VEL * 3;
		if(g->pl.direction == 1) g->pl.x_vel = -MAX_X_VEL * 3;
		g->pl.colour = 4;
		g->pl.meme_dash_timer = SecondsToTicks(0.1);
		if (g->connected) {
			DEPacket action_packet = MakePacket(PACKET_TYPE_ACTION, "", std::to_string(ABILITY_MEMEDASH).c_str());
			AddPacketToQueue(g, action_packet);
			g->ReadyToSendPackets = true;
		}
	}
	return true;
}

int AddPacketToQueue(Game* g, DEPacket p) {
	for (int i = 0; i < MAX_PACKETS; i++) {
		if (g->PacketQueue[i].packet_type == -1) {
			g->PacketQueue[i] = p;
			return i;
		}
	}
	return -1;
}