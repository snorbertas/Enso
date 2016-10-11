#pragma once
#include <allegro5/allegro_font.h>
#include "DEEvents.h"

class Editor
{
public:
	Editor();
	~Editor();
	bool loading = false;					// If true, loading state is active
	bool saving = false;					// If true, saving state is active
	int tool_id = 1;						// Tool type			<-.
	int selection_tool_type = -1;			// Selection tool type  <--- Don't ask please
	int select_type = -1;					// Selection type		<-'
	int select_id = -1;						// Select ID
	bool edit_mode = false;					// If true, edit mode is active
	bool camera_drag = false;				// If true, camera is in drag-mode
	int old_x, old_y, old_cx, old_cy;		// Coordinates before camera is dragged
	Platform* plt;							// Active platform pointer
	Platform* bg;							// Active background pointer
	Platform* fg;							// Active foreground pointer
	Platform* ev;							// Active event pointer
	bool target_drag = false;				// If true, dragging a platform object
	bool target_resize_w = false;			// If true, resizing a platform object horizontally
	bool target_resize_h = false;			// If true, resizing a platform object vertically
	int offset_x, offset_y;					// Coordinates before drag/resize, etc. starts
	bool grid = true;						// If true, the grid is rendered
	int debug_type = 0;						// What to show on debug interface
	int debug_scroll = 0;					// Debug Interface scrolling
	int last_click_x = -1;					// Last click x coord
	int last_click_y = -1;					// Last click y coord
};
#pragma once

// Editor structure
// Intended to be shit and untidy as it's temporary code, and not for the relase build)

/* ========================== RenderEditorEvents ==========================
*		Renders the event platforms in the Editor mode.
*
*		Called from the RenderGame function.
*/
void RenderEditorEvents(Game* g,
	ALLEGRO_FONT** font);


/* ============================= RenderEditor =============================
*		Renders the event platforms in the Editor mode.
*
*		Called from the RenderGame function.
*/
void RenderEditor(Game* g,
	ALLEGRO_BITMAP** img_sprite,
	ALLEGRO_FONT** font);


/* ============================= HandleEditor =============================
 *		Handles the Editor.
 *
 *		Called from HandleGame function.
 */
	void HandleEditor(Game* g);


int RegisterEditorEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
