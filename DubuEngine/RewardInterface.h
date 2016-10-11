#pragma once
#include "DEEvents.h"

#define MAX_REWARDS 10
#define REWARD_STATE_OPEN 0
#define REWARD_STATE_CLOSED -1

class RewardInterface
{
public:
	RewardInterface();
	~RewardInterface();
	int timer = -1;						// Reward interface timer (for animation)
	int r = 255;						// Red colour for text
	int g = 0;							// Green colour for text
	int b = 0;							// Blue colour for text
	int state = REWARD_STATE_CLOSED;	// Reward Interface state
	std::string msg = "";				// Message (ex.: x1 \"Sword\" Obtained)
};

int NewReward(Game* g,					// Game pointer
	std::string msg,					// Message
	int re, int gr, int bl);			// Colours

int RegisterRewardInterfaceEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
