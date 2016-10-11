#pragma once

// Ability book structure
struct AbilityBook {
	bool active = false;					// If true, this is the active book atm
	int book_id = -1;						// Book ID
	int sprite_id = -1;						// Icon sprite ID
	int ability_id[20];						// Array of ability IDs in the book
};

// Ability structure
struct Ability {
	int id = -1;							// Ability ID
	int sprite_id = -1;						// Icon Sprite ID
	const char* name;						// Ability name
	const char* description;				// Ability description
	int r = 100;							// Red colour
	int g = 100;							// Green colour
	int b = 255;							// Blue colour
};

// Ability slot structure
struct AbilitySlot {
	int key = -1;							// Binded key
	Ability ability;						// Ability in slot
	Item item;								// (or) Item in slot
};

/* ============================== LearnBook ===============================
 *		Learns the book with specified ID.
 *
 *		Used when desired.
 */
	int LearnBook(Game* g, int book_id);


/* ============================= ForgetBook ==============================
 *		Forgets the book(s) with specified ID.
 *
 *		Used when desired.
 */
	void ForgetBook(Game* g, int book_id);