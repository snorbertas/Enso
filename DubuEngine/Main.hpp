#pragma once

#define MAX_SAMPLES 5
#define MAX_FONTS 20

#include "Game.hpp"

// Main variables
	Game g;										// Main game object
	ALLEGRO_DISPLAY* display = NULL;			// Display object
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;	// Allegro event queue
	ALLEGRO_TIMER* timer = NULL;				// Allegro timer
	ALLEGRO_SAMPLE* sample_sfx[MAX_SAMPLES];	// Allegro sample array (for sound effects)
	ALLEGRO_FONT* font[MAX_FONTS];				// Allegro font array
	MoreSprites char_sprites;					// Array to sprites
	HSAMPLE menu_theme;							// Bass temp
	HCHANNEL main_channel;						// Bass temp
	HSAMPLE bg_theme;							// Bass temp
	HSAMPLE level_theme;						// Bass temp
	HCHANNEL bg_channel;						// Bass temp
	HCHANNEL level_channel;						// Bass temp


// Socket definitions
	WSADATA WsaDat;										// Information for socket
	SOCKET sock;										// Socket object
	int InitClient();									// Initiate client
	bool connected = false;								// Wether the client is connected or not
	void Connect(Game*);								// Connect function
	unsigned int __stdcall ClientThread(void *data);	// (Single) thread for client to communicate with sockets


// Arrays of pointers to event functions
TimerEventFunction TimerEvents[MAX_EVENT_FUNCTIONS];
MouseEventFunction MouseEvents[MAX_EVENT_FUNCTIONS];
KeyboardEventFunction KeyboardEvents[MAX_EVENT_FUNCTIONS];


/* =============================== MainLoop ===============================
 *		Main loop for the application.
 *		Handles timer, events, calls handlers and render function etc.
 *
 *		Called from main() function.
 */
	void MainLoop();


/* ============================== LoadSprites =============================
 *		Loads the sprite files. (Returns -1 on error).
 *		Simillar functionallity for other Load* functions.
 *
 *		Called from the main() function.
 */
	int LoadSprites();			// Sprites (numbered files from "sprites" folder
	int LoadAccessories();		// Accessories (from "sprites/accessory" folder)
	int LoadBodies();			// Bodies (from "sprites/body" folder)
	int LoadHairs();			// Hairs (from "sprites/hair" folder)
	int LoadOutfits();			// Outfits (from "sprites/outfit" folder)
	int LoadTiles();			// Tiles (from "sprites/tiles" folder)
	int LoadIcons();			// Icons (from "sprites/icons" folder)
	int LoadBackgrounds();		// Backgrounds (from "bg" folder")
	int LoadSamples();			// Sounds (from "sounds" folder)
	int LoadFonts();			// Fonts (from "fonts" folder)


/* ============================ DestroyBitmaps ============================
 *		Destroys all loaded bitmaps in order to free memory and prevent
 *		memory leaks.
 *
 *		Called from main() function.
 */
	void DestroyBitmaps();		// All bitmaps
	void DestroySamples();		// All samples
	void DestroyFonts();		// All fonts


/* ============================= LoadSettings =============================
 *		Loads "settings.ini" file.
 *
 *		Called from main() function.
 */
	void LoadSettings();		// Load "settings.ini"
	void SaveSettings();		// Save "settings.ini"