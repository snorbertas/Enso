#pragma once

/* ============================== RenderGame ==============================
 *		Main render function for gameplay. Renders a whole frame for the game.
 *		This function's job is to call all functions related to game-rendering
 *		to render each layer/step in correct order.
 *
 *		Called once per tick from the main() function.
 */
	void RenderGame(Game* g,				// Game pointer
		MoreSprites* csprites,				// Sprites
		ALLEGRO_FONT** font);				// Pointer to array of fonts

	
/* ============================== RenderBlood =============================
 *		Renders the blood animation/particles.
 *
 *		Called from the RenderGame function.
 */
	void RenderBlood(Game* g,
		ALLEGRO_BITMAP** img_sprite);


/* ============================ RenderCharacter ===========================
 *		Renders a character with specified settings.
 *		Use flag 1 to flip horizontally.
 *
 *		Called from the RenderGame function.
 */
	void RenderCharacter(Game* g,
		MoreSprites* csprites,
		int frame,						// Animation frame for character
		int x, int y,					// x and y coordinates for character
		int flags,						// 1 = flip horizontally
		int body,						// Body sprite ID
		int hair,						// Hair sprite ID
		int acc,						// Accessory sprite ID
		int outfit,						// Outfit sprite ID
		float combat_timer,				/* Combat timer for character
										(if > 0, character is rendered in-combat) */
		bool eyes=false,				// Render eyes	
		int blink=-1);					// Blink timer


/* ====================== RenderBackgroundPlatforms =======================
 *		Renders the background platforms/tiles.
 *
 *		Called from the RenderGame function.
 */
	void RenderBackgroundPlatforms(Game* g, ALLEGRO_BITMAP** img_background, ALLEGRO_BITMAP** img_sprite, MoreSprites* csprites);


/* ========================== RenderBackgrounds ===========================
 *		Renders the very bottom layer of the game.
 *		Includes the scrolling background.
 *
 *		Called from the RenderGame function.
 */
	void RenderBackgrounds(Game* g,
		ALLEGRO_BITMAP** img_background,
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites);


/* ============================ RenderPlatform ============================
 *		Renders one platform with specified settings. (Will re-organize later)
 *
 *		Called from the RenderPlatforms function.
 */
	void RenderPlatform(Game* g,		// This one is used for BLOCK_SIZE only
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites,
		Platform* plt,					// Pointer to the platform
		int t1, int t2,					// Tile IDs for the texture on the top (loops)
		int b1, int b2,					// Tile IDs for the texture on the bottom (loops)
		int re, int gr, int bl);		// Red, Green, Blue colours for platform

	/*
			[t1]	[t2]	[t1]		# Loops

			[rgb]	[rgb]	[rgb]		# Plain colour

			[b1]	[b2]	[b1]		# Loops
	*/

	void RenderPlatform(Game* g,		// This one is used for BRICK_SIZE only (walls)
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites,
		Platform* plt,
		int t1, int t2, int t3,			// Top left, top middle (fill), top right
		int m1, int m2,					// Left middle, right middle
		int b1, int b2, int b3,			// Bottom left, bottom middle (fill), bottom right
		int re, int gr, int bl);		// Red, Green, Blue colours for platform

	/*
			[t1]	[t2]	[t3]		# t2 fills top

			[m1]	[rgb]	[m2]		# Plain colour

			[b1]	[b2]	[b3]		# b2 fills bottom
	*/

	void RenderPlatform(Game* g,		// This one is used for BLOCK_SIZE only (roofs)
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites,
		Platform* plt,
		int t,							// Top fill
		int m);							// Rest fill

	/*
			[t]		[t]		[t]			# t fills top

			[m]		[m]		[m]			# m fills the rest

			[m]		[m]		[m]			# m fills the rest
	*/


/* =========================== RenderPlatforms ============================
 *		Renders all collision platforms.
 *
 *		Called from the RenderGame function.
 */
	void RenderPlatforms(Game* g,
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites);


/* ========================== RenderForegrounds ===========================
 *		Renders all foreground platforms.
 *
 *		Called from the RenderGame function.
 */
	void RenderForegrounds(Game* g,
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites);


/* ============================ RenderPlayers =============================
 *		Renders the players (multiplayer clients).
 *
 *		Called from the RenderGame function.
 */
	void RenderPlayers(Game* g,
		MoreSprites* csprites,
		ALLEGRO_FONT** font);


/* ============================== RenderNPCs ==============================
 *		Renders all the NPC (Monster) entities.
 *
 *		Called from the RenderGame function.
 */
	void RenderNPCs(Game* g,
		ALLEGRO_BITMAP** img_sprite,
		MoreSprites* csprites);