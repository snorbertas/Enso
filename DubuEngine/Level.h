#pragma once
#include "DEEvents.h"
#include <string>

#define MAP_ID_INDOORS -400
#define MAP_ID_UNDERGROUND -401
#define MAP_ID_OVERGROUND -402
#define MAP_ID_LEVEL -403
#define MAP_ID_OTHER -404


#define MAX_PLATFORMS 1000
#define MAX_LV_BACKGROUNDS 1000
#define MAX_FOREGROUNDS 1000
#define MAX_EVENTS 1000

//Platform Structure
struct Platform {
	float x = -9999;		// x coordinate
	float y = -9999;		// y coordinate
	int w = 32;				// Width
	int h = 32;				// Height
	int type = -1;			// Type of platform
	bool texture_t = true;	// Show top (coating) texture
	bool texture_b = true;	// Show bottom texture
	bool texture_l = true;	// Show left shading
	bool texture_r = true;	// Show right shading
};

typedef Platform LPlat;		// Just because "Platform" is already in-use cuz im dumb and stupid

struct lsetting {
	char s[50];
};

//Level class version (before and including) Alpha 0.8.1 (old)
class Level_a0_8_1 {
public:
	Level_a0_8_1();
	~Level_a0_8_1();
	int id = -1;
	char name[50] = "untitled";
	Platform Foreground[1000];
	Platform Background[1000];
	Platform Event[100];
	Platform Platform[1000];

	lsetting setting[50];
};

//Level class version Alpha 0.8.2 (old)
class Level_a0_8_2 {
public:
	Level_a0_8_2();
	~Level_a0_8_2();
	int id = -1;
	char name[50] = "untitled";
	Platform Foreground[1000];
	Platform Background[1000];
	Platform Event[1000];
	Platform Platform[1000];

	lsetting setting[50];
};

//Level class version Alpha 0.8.3 (current)
class Level_a0_8_3 {
public:
	Level_a0_8_3();
	~Level_a0_8_3();
	LPlat Platform[MAX_PLATFORMS];
	LPlat Background[MAX_LV_BACKGROUNDS];
	LPlat Foreground[MAX_FOREGROUNDS];
	LPlat Event[MAX_EVENTS];

	//Integer settings
	int ground_level = 0;
	int left_bound = -100000;
	int right_bound = 100000;
	int background_0 = 0;
	int background_1 = 0;
	int background_2 = 0;
	int map_id = -1;

	//Reserved for future
	int var_8 = 0;
	int var_9 = 0;
	int var_10 = 0;
	int var_11 = 0;
	int var_12 = 0;
	int var_13 = 0;
	int var_14 = 0;
	int var_15 = 0;
	int var_16 = 0;
	int var_17 = 0;
	int var_18 = 0;
	int var_19 = 0;
	int var_20 = 0;

	//Boolean settings
	bool night = false;
	bool fog = false;
	bool rain = false;
	bool thunder = false;

	//Reserved for future
	bool var25 = false;
	bool var26 = false;
	bool var27 = false;
	bool var28 = false;
	bool var29 = false;
	bool var30 = false;
	bool var31 = false;
	bool var32 = false;
	bool var33 = false;
	bool var34 = false;
	bool var35 = false;
	bool var36 = false;
	bool var37 = false;
	bool var38 = false;
	bool var39 = false;
	bool var40 = false;
};

//Current version Level class
typedef Level_a0_8_3 Level;

/* ======================== HandlePlatformCollisions ======================
 *		Handles all platform collisions.
 *
 *		Called from HandleGame function.
 */
	void HandlePlatformCollisions(Game* g);

	
/* ============================= CreatePlatform ===========================
 *		Returns a platform with specified settings
 *
 *		Called when desired.
 */
	Platform CreatePlatform(int type,	// Platform type
		int x, int y,					// x, y coordinates for platform
		int w, int h,					// Width, height for platform size
		bool shade_l = true,			// Shade left
		bool shade_r = true,			// Shade right
		bool texture_t = true,			// Texture on top
		bool texture_b = true);			// Texture on bottom

int RegisterLevelEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);