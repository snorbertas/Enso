#include <fstream>
#include <sstream>
#include "Game.hpp"
#include "ScaledDraw.hpp"
#include "Item.h"
#include "DEText.h"

void HandleItemPickup(Game* g) {
	for (int j = 0; j < MAX_ITEMS_DROPPED; j++) {
		if (g->ItemOnMap[j].item.id != -1) {
			if (collide(g->ItemOnMap[j].x, g->ItemOnMap[j].y, 34, 34, g->pl.x, g->pl.y, g->pl.w, g->pl.h)) {
				AddItemToInventory(g, g->ItemOnMap[j].item);
				g->ItemOnMap[j].item.id = -1;
			}
		}
	}
}

Item::Item() {

}

Item::~Item() {

}

ItemDatabase::ItemDatabase() {
	this->item[0] = CreateItem(0, 1, 1, "Torch", "Lights up dark areas.", 70, 200, 255);
	this->item[1] = CreateItem(1, 1, 5, "Obama", "Better president than Trump.", 250, 170, 70);
	this->item[2] = CreateItem(2, 1, 2, "Shoes", "+1 Movement Speed.", 70, 200, 255);
	this->item[3] = CreateItem(3, 1, 3, "Cloth", "Looks like a blindfold.", 10, 10, 10);
	this->item[4] = CreateItem(4, 1, 0, "Wooden Sword", "+5 Damage", 255, 200, 70);
	this->item[5] = CreateItem(5, 1, 6, "Fake ID", "Helps to travel.", 150, 150, 0);
}

Item CreateItem(int id, int quantity, int sprite_id, const char* name, const char* description, int r, int g, int b) {
	Item item = CreateItem(id, quantity, name, description, r, g, b);
	item.sprite_id = sprite_id;
	return item;
}

Item CreateItem(int id, int quantity, const char* name, const char* description, int r, int g, int b) {
	Item item;
	item.id = id;
	item.quantity = quantity;
	item.name = name;
	item.description = description;
	item.r = r;
	item.g = g;
	item.b = b;
	return item;
}

void HandleEquips(Game* g) {
	g->flash_light = false;
	g->pl.speed = MAX_X_VEL / 2;
	for (int i = 0; i < 3; i++) {
		if (g->Inventory[20 + i].id == 0) {
			g->flash_light = true;
		}
		if (g->Inventory[20 + i].id == 2) {
			g->pl.speed = (MAX_X_VEL / 2) + 1;
		}
	}
}

int DropItem(Game* g, Item item, int x, int y) {
	for (int i = 0; i < MAX_ITEMS_DROPPED; i++) {
		if (g->ItemOnMap[i].item.id == -1) {
			g->ItemOnMap[i].item = item;
			g->ItemOnMap[i].x = x;
			g->ItemOnMap[i].y = y;
			return i;
		}
	}
	return -1; // Map is full of junk
}

int AddItemToInventory(Game* g, Item item, int quantity) {
	item.quantity = quantity;
	for (int i = 0; i < 20; i++) {
		if (g->Inventory[i].id == -1) {
			g->Inventory[i] = item;
			return i;
		}
	}
	return -1;
}

bool PlayerHasItem(Game* g, int item_id) {
	for (int i = 0; i < 23; i++) {
		if (g->Inventory[i].id == item_id) {
			return true;
		}
	}
	return false;
}

void RenderItems(Game* g, MoreSprites* sprites) {
	// Shortened variable names
	int x = g->camera.x;
	int y = g->camera.y;

	// Render each item on map
	for (int i = 0; i < MAX_ITEMS_DROPPED; i++) {
		if (g->ItemOnMap[i].item.id != -1) {
			DrawImage(g, sprites->img_icons[g->ItemOnMap[i].item.sprite_id], g->ItemOnMap[i].x + x, g->ItemOnMap[i].y + y, 0);
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
	// Tick
	HandleItemPickup(g);
	HandleEquips(g);
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
int RegisterItemEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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