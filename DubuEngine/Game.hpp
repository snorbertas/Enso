#pragma once

#define TPS 120
#define ENGINE_VERSION "Enso Client (Alpha v0.9.1)"
#define MAX_ANIMATIONS 1000
#define MAX_PLAYER_HITBOXES 10
#define MAX_MONSTER_HITBOXES 10
#define MAX_PROJECTILES 50
#define MAX_MONSTERS 100
#define MAX_PLAYERS 50
#define MAX_COMBAT_FEEDBACK 100
#define MAX_ITEMS_DROPPED 100
#define RAIN_DROPS 500
#define MAX_OBJECTIVES 14

#define BLOCK_SIZE 32
#define BRICK_SIZE 70

//This is temporary, I used GAME_WIDTH/GAME_HEIGHT as a predefined base source resolution for the game
//However now I wanna make it mutable soooooooooooo fuck it
#define GAME_WIDTH g->BWIDTH
#define GAME_WIDTH_ g.BWIDTH
#define GAME_HEIGHT g->BHEIGHT
#define GAME_HEIGHT_ g.BHEIGHT

#define JUMP_VEL -10
#define GRAVITY 0.4
#define RESISTANCE 0.5
#define MAX_X_VEL 5
#define MAX_Y_VEL 8
#define X_ACC 0.2
#define WALL_SLIDE 1

#include "EnsoDefinitions.h"
#include "../DubuEngineServer/DEProtocol.hpp"
#include "Sprites.h"
#include "Level.h"
#include "Combat.h"
#include "Chat.h"
#include "Weather.h"
#include "Item.h"
#include "Ability.h"
#include "Editor.h"
#include "NPC.h"
#include "Animation.h"
#include "Ability.h"
#include "Player.h"
#include "Convo.h"
#include "Camera.h"
#include "Experience.h"
#include "ObjectiveInterface.h"
#include "RewardInterface.h"
#include <allegro5/allegro.h>
#include <iostream>
#include <string.h>

// Login interface structure
	struct LoginInterface {
		bool try_login = false;					// If true, client will attempt to connect
		bool logging_in = false;				// If true, means client is trying to connect
		int loading_angle = 0;					// Loading animation angle
		bool type_username = true;				// True if typing username. If false, typing password
		int type_indicator_count = 0;			//
		int backspace_timer = 0;				//	Timers
		int backspace_timer_wait = 60;			//
		std::string username_input = "";		// Username input
		std::string password_input = "";		// Password input
		std::string password_input_mask = "";	// Password input converted into starts ****
	};

// Keys strucutre (key bindings and input)
	struct Keys {
		int left_bind = ALLEGRO_KEY_LEFT;		// Binded key to move left
		int right_bind = ALLEGRO_KEY_RIGHT;		// Binded key to move right
		int jump_bind = ALLEGRO_KEY_UP;			// Binded key to jump
		int crouch_bind = ALLEGRO_KEY_DOWN;		// Binded key to crouch
		int camera_bind = ALLEGRO_KEY_Y;		// Binded key to change camera mode
		int inventory_bind = ALLEGRO_KEY_I;		// Binded key to open inventory
		int skills_bind = ALLEGRO_KEY_O;		// Binded key to open skills
		int chat_bind = ALLEGRO_KEY_ENTER;		// Binded key to chat/type
		bool left = false;						// If true, then moving left key is inputted
		bool right = false;						// If true, then moving right key is inputted
		bool down = false;						// If true, then crouch key is inputted
		bool jump = false;						// If true, then jump key is inputted
		bool CTRL = false;						// If true, then CTRL is being held down
		bool SHIFT = false;						// If true, then SHIFT is being held down
		bool backspace = false;					// If true, then Backspace is being held down
	};

// Menu structure
	struct Menu {
		bool starting = false;					// Starting (fade/animations)
		bool quitting = false;					// Quitting (fade/animations)
		float bg_x = -20;						// Background x position
		float bg_y = -20;						// Background y position
		float bg_x_vel = 0.2;					// Background horizontal velocity
		float bg_y_vel = 0.3;					// Background vertical velocity
	};


// Button structure
	struct Button {
		bool visible = true;					// If true, button is rendered and active
		int sprite_id = 5;						// Sprite ID for button (sprite_id + 1 is highlighted when mouse is colliding)
		int w, h;								// Width and height for button (hitbox)
		int interface_id;						// Interface ID the button belongs to
		int x = -1;								// x position for button relative to interface
		int y = -1;								// y position for button relative to interface
		float opacity = 1;						// Button transperency (<1 is transperent)
	};

// Interface structure
	struct Interface {
		int sprite_id;							// Sprite ID for interface
		int x, y;								// x, y coordinates
		bool visible;							// If true, interface is visible and active
		float opacity = 1.0;					// Transperency
	};

// Background layers structure
	struct BackgroundLayers {
		int layer[3] = { 1, 12, 13 };			// Array of background layers with bg ids
		int y_offset[3] = { 0, 0, 0 };			// Array of horizontal offsets for these layers
	};

/* =============================== Game Class =============================
 *		The big, main, scary Game class. Used as the primary object.
 *		This whole game code is orientated around the Game class.
 *
 *		Used everywhere.
 */
	class Game
	{
		public:
			// Constructor
			Game();

			// Socket-related variables
			int ticks_since_last_ping = 0;		// Used to calculate ping
			double time_sent_ping = 0;			// Used to calculate ping
			int ping = 0;						// Time taken to hear a response to last ping
			bool ReadyToSendPackets;			// If true, the main socket thread will send all packets
			bool connected = false;				// If true, socket is succesfully connected to the server
			DEPacket* PacketQueue;				// A packet queue to store unsent packets
			Player Players[MAX_PLAYERS];		// Array of player objects for other clients in multiplayer

			// Non-socket related variables
			int fade_in = 0;					// Fade-in timer
			int fade_out = 0;					// Fade-out timer
			double old_time = 0;				// Last tick
			double new_time = 0;				// New tick
			int animation_tick = 0;				// Animation timer
			//int ground_level = 0;				// Ground level (y coordinate)
			//int left_bound = 350;				// Left camera bound
			//int right_bound = 100000;			// Right camera bound
			Camera camera;						// Camera object
			//bool night_time = false;			// If true, it's night-time in-game
			bool fog_time = false;
			bool flash_light = false;			// If true, a flashlight will help player illuminate area around
			float music_volume;					// Music volume
			float music_volume_cd;				// Music volume cooldown
			bool remake_display;				// If true, the display will be recreated next tick
			int s_x = 1280;						// Scale to x
			int s_y = 720;						// Scale to y
			int BWIDTH = 1600;					// Base width
			int BHEIGHT = 900;					// Base height
			int scene;							// Scene ID
			float intro_a = 255;				// Intro alpha
			bool done;							// If true, the main loop will exit and exit program
			int pos_x = 0;						// Cursor x position
			int pos_y = 0;						// Cursor y position
			int menu_x = 0;						// Virtual cursor for menu
			int menu_y = 0;						// Virtual cursor for menu
			const char* Message1 = "Line1";		// Top (first) line message
			const char* Message2 = "Line2";		// Bottom (second) line message
			int* binding_key = NULL;			// Pointer to current key being binded
			int window_mode;					// Window mode
			const char* server_ip = "127.0.0.1";// Server IP address
			int server_port = 25565;			// Server port
			bool gameover = false;				// If true, the game is over, different handling
			bool InterfaceDrag[10] = {			// Wether the interface[i] is being dragged or not
				false, false, false, false, false,
				false, false, false, false, false };
			bool allow_input = true;			// Wether input is allowed or not
			Chat chat;							// Chat object
			LoginInterface logini;				// Login Interface
			Ability DraggingAbility;			// Placeholder for the dragged ability
			Button* Buttons;					// Pointer to array of buttons
			Interface* Interfaces;				// Pointer to array of interfaces
			Player pl;							// Player object
			TrailAnim* trail;					// Trail for animations (this was added for fun, temporary)
			Menu menu;							// Menu object
			Keys keys;							// Keys struct, holds the information on some input
			Level* lvl;							// The current map/level pointer
			Flag flag;							// Flag object for teleporting (this was added for fun, temporary)
			Level RemLevel[10];					// Used by editor to remember last editor states
			Editor RemEditor[10];				// Used by editor to remember last editor states
			Editor Editor;						// Editor object
			std::string last_save_loc = "";		// Last save location for quicksave (CTRL+S)
			Cloud* cloud;						// Array to cloud objects
			Monster* monster;					// Array to monsters/npc
			CbFeedback cfeedback[MAX_COMBAT_FEEDBACK];	// Array to combat feedbacks
			GFXAnimation* anim;					// Array to animations
			AbilitySlot AbilitySlots[8];		// Array to active ability slots
			AbilityBook AbilityBooks[5];		// Array to active ability books
			Item Inventory[23];					// Array to items in inventory
			Item DraggingItem;					// Placeholder for the dragged item
			int drag_item_slot;					// The item slot being dragged out
			DroppedItem ItemOnMap[MAX_ITEMS_DROPPED];	// Items on the map ready for collection
			RainDrop rdrop[RAIN_DROPS];			// Array to raindrop objects
			Star Stars[MAX_STARS];				// Array to stars objects
			ItemDatabase* ItemDB;				// Item database
			NPCDatabase* NPCDB;					// NPC database
			Convo* convo = new Convo();			// Current conversation
			NPC* convo_with = NULL;				// Target the player is in conversation with
			CutScene cut_scene;					// Cutscene object
			MissionList story;					// The mission list/tracker (used for storyline)
			Experience exp[MAX_EXPERIENCE];		// Experience objects on the map
			ObjectiveInterface objectives[MAX_OBJECTIVES];	// Interfaces for objectives
			RewardInterface rewards[MAX_REWARDS];	// Interfaces for rewards
			int thunder_timer = 500;			// Timer for thunder/lighting
			bool warping = false;				// Wether the player is currently in a warp (map/x/y)
			int packet_debug = 0;				// Temporary
			Projectile projectiles[MAX_PROJECTILES];	// Array for projectiles
	};


/* ================================ collide ===============================
 *		Returns true if collision occurs between two objects or shapes.
 *
 *		Used to check collision.
 */
	bool collide(Monster* m,		// Monster pointer
		Platform* pl);				// Platform pointer

	bool collide(Player* p,			// Player pointer
		Platform* pl);				// Platform pointer

	bool collide(Player* p,			// Player pointer
		Monster* m);				// Monster pointer

	bool collide(Monster* m,		// Monster pointer
		HitBox* h);					// HitBox pointer

	bool collide(Player* p,			// Player pointer
		HitBox* h);					// Hitbox Pointer

	// Rectangle collision
	bool collide(int o1x, int o1y,	// Object 1 x, y coordinates
		int o1w, int o1h,			// Object 1 width and height
		int o2x, int o2y,			// Object 2 x, y coordinates
		int o2w, int o2h);			// Object 2 width and height

	// Circle collision
	bool collide(int o1x, int o1y,	// Object 1 x, y coordinates
		int o1r,					// Object 1 radius
		int o2x, int o2y,			// Object 2 x, y coordinates
		int o2r);					// Object 2 radius


/* ================================= Jump =================================
 *		Player jump.
 *
 *		Called when jump key is inputted.
 */
	void Jump(Game* g);				// Game pointer
	void WallJump(Game* g);


/* ================================ OnAWall ===============================
 *		Returns true if player is on a wall.
 *
 *		Used to check for wall-slides and wall-jumps.
 */
	bool OnAWall(Game* g);


/* ============================== GetFloorID ==============================
 *		Returns the platform ID the player is standing on.
 *		Returns -1 if not standing on the floor.
 *
 *		Used when desired.
 */
	int GetFloorID(Game* g);


/* =============================== OnAFloor ===============================
 *		Returns true if standing on a platform.
 *
 *		Used when desired.
 */
	bool OnAFloor(Game* g);


/* ============================= StartAnimation ===========================
 *		Starts an animation with specified settings.
 *		Returns the animation ID.
 *
 *		Used when desired.
 */
	int StartAnimation(Game* g,
		int type,							// Animation type (sprite ID)
		int x, int y,						// x, y coordinates to spawn animation
		int flip,							// 1 = flip horizontally
		bool animated,						/* True means it's animated (progressing sprites),
											if false only one still sprite is used */
		int loop,							// How many times the animation should loop
		int relative_to_player_id = -1);	// The player id animation is relative to, -1 for local


/* ============================ CreateProjectile ==========================
 *		Creates a projectile with desired settings.
 *		Returns the projectile ID.
 *
 *		Used when desired.
 */
	int CreateProjectile(Game* g,
		int type,							// Projectile type
		int x, int y,						// x, y coordinates
		int dir,							// Direction
		HitBox* hbox = NULL);				// HitBox pointer


/* ============================== CreateHitBox ============================
 *		Creates a hitbox with desired settings.
 *		Returns the hitbox ID.
 *
 *		Used when desired.
 */
	int CreateHitBox(Game* g,
		Player* p,							// Player pointer
		int type,							// HitBox type
		int x, int y,						// x, y values for hitbox
		int h, int w,						// Width and height for hitbox
		Projectile* proj=NULL);				// Direction of projectile


/* ============================== ShowMessage =============================
 *		Displays the message interface with specified text.
 *
 *		Used when desired.
 */
	void ShowMessage(Game* g,
		const char* msg1,					// First (top) part of message
		const char* msg2);					// Second (bottom) part of message


/* ============================ SecondsToTicks ============================
 *		Coverts seconds into a number of game ticks.
 *		Returns the number of game ticks.
 *
 *		Used when desired.
 */
	int SecondsToTicks(float seconds);		// Seconds to convert into ticks


/* =============================== LoadLevel ==============================
 *		Returns a level object loaded from a file.
 *
 *		Used to load levels from editor, or in general.
 */
	Level* LoadLevel(const char* file_path);	// File path


/* =============================== ChangeMap ==============================
 *		Changes the map and respawns the player
 *
 *		Used on doors, end of maps, etc.
 */
	void ChangeMap(Game* g,
		const char* file_path);				// File path


/* ================================= Fade =================================
 *		Starts a fade-in or fade-out
 *
 *		Used when desired.
 */
	void Fade(Game* g,
		int oi);							// 0 = Fade-in, 1 = Fade-out


/* ============================= CombatFeedback ===========================
 *		Creates a combat feedback (damage, heal, etc.).
 *		Returns the ID for created CF.
 *
 *		Used when desired.
 */
	int CombatFeedback(Game* g,
		int value,							// Damage (positive) or heal (negative) done
		int x, int y);						// x, y coordinates to spawn CF


/* =============================== SelectBook =============================
 *		Sets the active book as the specfied book ID.
 *
 *		Called when a book is clicked.
 */
	void SelectBook(Game* g,
		int book_id);						// Book ID


/* ============================ GetActiveBookID ===========================
 *		Returns the book ID that is currently active.
 *
 *		Used when desired.
 */
	int GetActiveBookID(Game* g);


/* =============================== GetAbility =============================
 *		Returns the ability beloning to a specified book and slot.
 *
 *		Used when desired.
 */
	Ability GetAbility(int book_id,			// Book ID
		int slot);							// Slot ID


/* =============================== MouseIsOn ==============================
 *		Returns true if mouse is on specified rectangle.
 *
 *		Used when desired.
 */
	bool MouseIsOn(Game* g,
		int x, int y,						// x, y coordinates of rectangle
		int w, int h);						// Width and height of rectangle


/* =============================== UseAbility =============================
 *		Attempts to use an ability.
 *		Returns true on success or false if player unavaible to cast the ability.
 *
 *		Called when a key binded to an ability is inputted.
 */
	bool UseAbility(Game* g,
		int ability_id);					// Ability ID


/* ============================= LoadLevelConfig ==========================
 *		Loads the configuration commands for current map.
 *
 *		Used when desired.
 */
	//void LoadLevelConfig(Game* g);
	//void LoadLevelConfig(Game* g, Level* lvl);


/* =========================== ExecuteLevelConfig =========================
 *		Executes the configuration commands for current map.
 *
 *		Used when desired.
 */
	//int ExecuteLevelConfig(Game* g);


/* ============================ AddPacketToQueue ==========================
 *		Adds a packet to the packet queue.
 *		Returns the slot ID in queue.
 *		Returns -1 if queue is full.
 *
 *		Used when desired.
 */
	int AddPacketToQueue(Game* g,			// Game pointer
		DEPacket p);						// Packet to add