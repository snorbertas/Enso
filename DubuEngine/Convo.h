#pragma once
#include "DEEvents.h"
#include <string>

#define CONVO_BLINK_FRAMES 24
#define CONVO_TYPE_THINK 1
#define CONVO_TYPE_SHOUT 2
#define CONVO_TYPE_NORMAL 3
#define CONVO_TYPE_STORY 4
#define CONVO_TYPE_CHOICE 5
#define CUTSCENE_STATE_CLOSED -1
#define CUTSCENE_STATE_OPENING 1
#define CUTSCENE_STATE_REST 2
#define CUTSCENE_STATE_CLOSING 3
#define MAX_MISSIONS 100

// Names, x and y (This is a PREFERENCE for the programmer, not a must)
#define NPC_NAME g->convo_with->name
#define PLR_NAME g->pl.name.c_str()
#define PLR_X g->pl.x
#define PLR_Y g->pl.y
#define NPC_X g->convo_with->x
#define NPC_Y g->convo_with->y

struct CutScene {
	int timer = -1;			// Cutscene's timer
	int state = -1;			// Cutscene's state
};

struct Mission {
	int timer = -1;			// Mission's timer
	int prog = -1;			// Mission's proggress
	bool var1 = false;		// Mission's variable 1 - 8
	bool var2 = false;
	bool var3 = false;
	bool var4 = false;
	bool var5 = false;
	bool var6 = false;
	bool var7 = false;
	bool var8 = false;
};

struct MissionList
{
	Mission missions[MAX_MISSIONS];		// Mission array
};

class Convo
{
public:
	Convo();
	~Convo();
	std::string name = "Me";						// Conversationist name
	std::string message = "";						// Full message for conversation
	std::string line[5] = {"", "", "", "", ""};		// Conversation message split into 5 lines
	std::string question = "";						// Conversation question
	std::string choice[4] = {"", "", "", ""};		// Conversation choices (for questions)
	int choice_next_event_id[4] = {-2, -2, -2, -2};	// Next event ID to trigger after the choice of conversation (-2 Unavailable)
	int convo_type = CONVO_TYPE_THINK;				// Conversation type
	int timer = -1;									// Conversation timer
	int parent_x = -9999;							// Conversation's parent (character) x coordinate
	int parent_y = -9999;							// Conversation's parent (character) y coordinate
	bool exploding = false;							// Wether the conversation is in the explosive state
	bool sorted = true;								// Wether the conversation has already been sorted
	int event_id = -1;								// Event ID for the conversation
	int next_event_id = -1;							// Next event ID to trigger after the covnersation (-1 No event)
	int duration = -1;								// Conversation duration (only for cutscenes)
	bool title = false;								// Cutscene title
	bool small_talk = false;						// Wether to trigger a smalltalk
	int facing = FACING_LEFT;						// Direction the parent is facing
};


/* ============================ StartCutScene =============================
 *		Function to start the cutscene animation.
 *
 *		Called from RenderGame function.
 */
	void StartCutScene(Game* g);

	
/* ============================= ShowChoices ==============================
 *		Function to start the cutscene animation.
 *
 *		Called from RenderGame function.
 */
	void ShowChoices(Game* g,
		std::string question,							// Question asked to the player
		std::string choice_1, int next_id_1,			// Choice 1 and its' result
		std::string choice_2, int next_id_2,			// Choice 2 and its' result 
		std::string choice_3 = "", int next_id_3 = -2,	// Choice 3 and its' result
		std::string choice_4 = "", int next_id_4 = -2);	// Choice 4 and its' result

	//For SmallTalk
	void ShowSTChoices(Game* g,
		std::string question,							// Question asked to the player
		std::string choice_1, int next_id_1,			// Choice 1 and its' result
		std::string choice_2, int next_id_2,			// Choice 2 and its' result 
		std::string choice_3 = "", int next_id_3 = -2,	// Choice 3 and its' result
		std::string choice_4 = "", int next_id_4 = -2);	// Choice 4 and its' result


/* ============================== ShowConvo ===============================
 *		Starts a convo with these settings.
 *
 *		Called from RenderGame function.
 */
	void ShowConvo(Game* g,							// Game pointer
		std::string name,							// Name (parent)
		std::string message,						// Message
		int x, int y,								// Parent x, y coordinates on the map
		int type,									// Convo type
		int next_event_id = -1);					// At the end of convo trigger this convo event ID

	//For Smalltalk
	void ShowST(Game* g, std::string name, std::string message, int x, int y, int type, int next_event_id = -1);

	
/* ============================== ShowStory ===============================
 *		Function to initiate a story cutscene.
 *
 *		Called when desired.
 */
	void ShowStory(Game* g,							// Game pointer
		std::string name,							// Name of title
		std::string message,						// Message
		float duration,								// Duration in seconds
		bool title,									// If true, bigger font (title)
		int next_event_id = -1);					// At the end of the duration, trigger this convo event ID
	
	
/* ============================== StoryPause ==============================
 *		Function to initiate a story pause.
 *
 *		Called when desired.
 */
	void StoryPause(Game* g,						// Game pointer
		float duration,								// Duration of pause (must be > 1.0)
		int next_event_id = -1);					// At the end of the duration, trigger this convo event ID

	
/* ============================ RenderCutScene ============================
 *		Function to render the cutscene.
 *
 *		Called from RenderGame function.
 */
	void RenderCutScene(Game* g);

	
/* ========================== HandleEventTriggers =========================
 *		Function to trigger events due to player's action key input.
 *
 *		Called from InputHandler.
 */
	void HandleEventTriggers(Game* g);


/* ============================= RenderConvo ==============================
 *		Function to render conversation related graphics.
 *
 *		Called from RenderGame function.
 */
	void RenderConvo(Game* g,
		ALLEGRO_BITMAP** img_sprite,
		ALLEGRO_FONT** font);


/* ============================= RenderConvo ==============================
 *		Function to render conversation related graphics.
 *
 *		Called from RenderGame function
 */
	void RenderConvo(Game* g,
		ALLEGRO_BITMAP** img_sprite,
		ALLEGRO_FONT** font);

	
/* ============================ CloseCutScene =============================
 *		Closes the cutscene.
 *
 *		Called when desired.
 */
	void CloseCutScene(Game* g);

	
/* ========================== HandleConvoEvents ===========================
 *		Closes the cutscene.
 *
 *		Called when desired.
 */
	void HandleConvoEvent(Game* g);


int RegisterConvoEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
