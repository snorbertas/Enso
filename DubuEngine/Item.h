#pragma once
#include "DEEvents.h"
#define MAX_DB_ITEMS 100

class Item
{
public:
	Item();
	~Item();
	int id = -1;							// Item ID
	int sprite_id = -1;						// Sprite ID
	int quantity = 1;						// Quantity
	const char* name;						// Item name
	const char* description;				// Item description
	int r = 100;							// Red colour
	int g = 100;							// Green colour
	int b = 255;							// Blue colour
};

class ItemDatabase
{
public:
	ItemDatabase();
	Item item[MAX_DB_ITEMS];				// Items in database
};

// Item (on ground) structure
struct DroppedItem {
	Item item;								// Item object
	int x = -1000;							// x coordinate on map
	int y = -1000;							// y coordinate on map
	int timer = 0;							// Timer till despawn
};


/* =============================== CreateItem =============================
 *		Returns an item object with specified settings.
 *
 *		Used when desired.
 */
	Item CreateItem(int id,							// Item ID (sprite ID)
		int quantity,								// Quantity of the item (if stackable)
		const char* name,							// Name of the item
		const char* description,					// Description of the item
		int r = 100, int g = 100, int b = 255);		// Red, green and blue colours for tooltip

	// Same story but with different sprite, will be the primary function in future
	Item CreateItem(int id, int quantity, int sprite_id, const char* name, const char* description, int r, int g, int b);


/* ============================ HandleItemPickup ==========================
*		Checks if the player is coliding with an item on the ground.
*
*		Called from HandleGame function.
*/
	void HandleItemPickup(Game* g);


/* ============================= RenderItems ==============================
 *		Renders the items on the ground.
 *
 *		Called from the RenderGame function.
 */
	void RenderItems(Game* g,
		MoreSprites* sprites);


/* ================================ DropItem ==============================
 *		Drops an item.
 *		Returns the slot id on the ground the item was dropped to.
 *		Returns -1 If map is full.
 *
 *		Called when an item is dragged from inventory onto the outside.
 */
	int DropItem(Game* g,
		Item item,								// Item object to drop
		int x, int y);							// x, y coordinates on map to drop to


/* =========================== AddItemToInventory =========================
 *		Adds an item to the player's inventory.
 *		Returns inventory slot id.
 *
 *		Used when desired.
 */
	int AddItemToInventory(Game* g,
		Item item,								// Item object to add to inventory
		int quantitity = 1);					// Quantity of the item

	
/* ============================= PlayerHasItem ============================
 *		Returns true if the player has one of the item
 *		Returns false if not.
 *
 *		Used when desired.
 */
	bool PlayerHasItem(Game* g, int item_id);


int RegisterItemEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
