#pragma once

#include <string.h>

#define MAX_CHAT_MESSAGES 21

// Chat message structure
struct Message {
	std::string message = " ";				// Message string
	std::string parent = " ";				// Sender string
	int r = 255;							// Red colour
	int g = 255;							// Green colour
	int b = 255;							// Blue colour
};

// Chat interface structure
struct Chat {
	int ghost = 0;							// If > 0, renders chat as a "ghost"
	bool show_chat = false;					// If true, chat is visible
	bool type_chat = false;					// If true, typing in chat is enabled
	int chat_indicator_count = 0;			//
	int backspace_timer = 0;				//	Timers
	int backspace_timer_wait = 60;			//
	std::string msg_input = "";				// Message input
	Message messages[MAX_CHAT_MESSAGES];	// Array of all messages (history)
};