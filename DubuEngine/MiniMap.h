#pragma once
#include "DEEvents.h"

class MiniMap
{
public:
	MiniMap();
	~MiniMap();
};


/* ========================== RenderMiniMapTile ===========================
 *		Renders a single platform tile on the minimap with specified settings.
 *
 *		Called from the RenderMiniMap function.
 */
	void RenderMiniMapTile(Game* g,
		Platform* plt,					// Pointer to the platform
		int x, int y,					// x, y values on the screen
		int r1, int g1, int  b1,		// Red, Green, Blue colours (main)
		bool cover,						// True = will cover the top of the tile with r2, g2, b2
		int r2=0, int g2=0, int b2=0);	// Red, Green, Blue colours (top cover)


/* ============================ RenderMiniMap =============================
 *		Renders the minimap.
 *		The interface ID for minimap is INTERFACE_MAP (defined in EnsoDefnitions.h)
 *
 *		Other functions not included in the header but used
 *		specifically by RenderMiniMap:
 *			bool ExceedingMiniMap(Game* g, Platform* plt)
 *			bool WithinMiniMap(Game* g, Platform* plt)
 *			Platform AdjustPlatformForMiniMap(Game* g, Platform* plt)
 *
 *		Called from the RenderGame function.
 */
	void RenderMiniMap(Game* g);

int RegisterMiniMapEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
