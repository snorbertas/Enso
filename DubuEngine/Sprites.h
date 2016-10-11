#pragma once

#define MAX_ACCESSORY 100
#define MAX_BODY 100
#define MAX_HAIR 100
#define MAX_OUTFIT 100
#define MAX_TILES 200
#define MAX_ICONS 100
#define MAX_SPRITES 200
#define MAX_BACKGROUNDS 40

#include <allegro5\allegro.h>

struct MoreSprites {
	ALLEGRO_BITMAP *img_acc[MAX_ACCESSORY];
	ALLEGRO_BITMAP *img_body[MAX_BODY];
	ALLEGRO_BITMAP *img_hair[MAX_HAIR];
	ALLEGRO_BITMAP *img_outfit[MAX_OUTFIT];
	ALLEGRO_BITMAP *img_tiles[MAX_TILES];
	ALLEGRO_BITMAP *img_icons[MAX_ICONS];
	ALLEGRO_BITMAP *img_sprite[MAX_SPRITES];
	ALLEGRO_BITMAP *img_background[MAX_BACKGROUNDS];
};

