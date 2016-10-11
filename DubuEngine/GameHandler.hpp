#pragma once

/* ================================ NewGame ===============================
 *		This function should be called to reset all the game variables.
 *
 *		Call on death events, new game, etc.
 */
	void NewGame(Game* g);				// Game pointer


/* ============================== HandleGame ==============================
 *		Handles a game-tick. This function's job is to call all handling
 *		functions. Physics, animations, movement of any kind, etc.
 *
 *		Called from the main function.
 */
	void HandleGame(Game* g,			// Game pointer
		ALLEGRO_SAMPLE** sample_sfx);	// Pointer to array of sound effects


/* =========================== HandleAnimations ===========================
 *		Handles the game's animation array.
 *
 *		Called from HandleGame function.
 */
	void HandleAnimations(Game*);


/* ============================== HandleWater =============================
 *		Handles water mechanics.
 *
 *		Called from HandleGame function.
 */
	int HandleWater(Game* g);


/* ============================ HandleGameInput ===========================
 *		Handles tick-relative input.
 *
 *		Called from HandleGame function.
 */
	void HandleGameInput(Game* g,
		ALLEGRO_SAMPLE** sample_sfx);


/* ============================ HandleGameMisc ============================
 *		Handles miscelanious mechanics/animations. (TEMPORARY)
 *
 *		Called from HandleGame function.
 */
	void HandleGameMisc(Game* g);


/* =========================== HandleGamePackets ==========================
 *		Handles socket/client-specific jobs.
 *
 *		Called from HandleGame function.
 */
	void HandleGamePackets(Game* g);