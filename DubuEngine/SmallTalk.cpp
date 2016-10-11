#include "SmallTalk.h"

void SmallTalk(Game* g, int id) {
	g->convo->small_talk = true;
	switch (id) {
	case 0:
		ShowST(g, PLR_NAME, "*burp*", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 0);
		break;
	case 1:
		ShowSTChoices(g, "What do you say?",
			"1) Excuse me, do know what's the time by any chance?", 100,
			"2) What time is it?", 102);
		break;
	case 2:
		ShowST(g, NPC_NAME, "Nice weather we're having today.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 200);
		break;
	case 3:
		ShowST(g, PLR_NAME, "Hello!", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 300);
		break;
	case 4:
		ShowST(g, PLR_NAME, "Hello there~", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 400);
		break;
	case 5:
		ShowST(g, NPC_NAME, "What'd you lookin at mate.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 500);
		break;
	case 6:
		ShowST(g, NPC_NAME, "I want to turn back the clock to before...", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 600);
		break;
	case 7:
		ShowST(g, NPC_NAME, "Well, this is where I live.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 700);
		break;
	case 8:
		ShowST(g, NPC_NAME, "Where does a child hide in a small town like this?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 800);
		break;
	case 9:
		ShowST(g, NPC_NAME, "Would you come to my funeral?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 900);
		break;
	case 10:
		ShowST(g, NPC_NAME, "What have you done with my pills? I need them!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1000);
		break;
	case 11:
		ShowST(g, NPC_NAME, "It looks like you're in trouble there. Can I help?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1100);
		break;
	case 12:
		ShowST(g, NPC_NAME, "Oi! What are you doing here?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1200);
		break;
	case 13:
		ShowST(g, PLR_NAME, "Greetings Stranger~", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 1300);
		break;
	case 14:
		ShowST(g, NPC_NAME, "Hey, do you like Gangnam style?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1400);
		break;
	case 15:
		ShowST(g, NPC_NAME, "Wake up!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1500);
		break;
	case 16:
		ShowST(g, NPC_NAME, "Hello, world!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1600);
		break;
	}
}

// Format is convo number + 00 (x00)
void HandleSTEvent(Game* g) {
	switch (g->convo->event_id) {
	case 0:
		ShowST(g, NPC_NAME, "...", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 1:
		ShowST(g, PLR_NAME, "...", PLR_X, PLR_Y, CONVO_TYPE_NORMAL);
		break;
	case 100:
		ShowST(g, NPC_NAME, "It's twenty minutes past four, sir.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 101);
		break;
	case 101:
		ShowST(g, PLR_NAME, "Thank you so much.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL);
		break;
	case 102:
		ShowST(g, NPC_NAME, "It's time that I don't have for rude people like you.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 103);
		break;
	case 103:
		ShowSTChoices(g, "What do you say?",
			"1) My aplogies, I won't bother you again.", 104,
			"2) Whatever bro.", 0);
		break;
	case 104:
		ShowST(g, NPC_NAME, "No no... I'm sorry, I overeacted.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 200:
		ShowSTChoices(g, "What do you say?",
			"1) Indeed we do.", 0,
			"2) Uhm... righto...", 0,
			"3) I can't believe you just said that. You're boring.", 201);
		break;
	case 201:
		ShowST(g, NPC_NAME, "How rude!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 300:
		ShowST(g, NPC_NAME, "Hello!?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 400:
		ShowST(g, NPC_NAME, "Hi... Do I know you?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 401);
		break;
	case 401:
		ShowSTChoices(g, "What do you say?",
			"1) Yes, it's me... You don't remember?", 402,
			"2) No, not yet.", 405);
		break;
	case 402:
		ShowST(g, NPC_NAME, "I'm sorry, I don't remember you.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 403);
		break;
	case 403:
		ShowST(g, PLR_NAME, "It's me, David.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 404);
		break;
	case 404:
		ShowST(g, NPC_NAME, "I don't know any David, I have important things to do. Goodbye.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 405:
		ShowST(g, NPC_NAME, "Please leave me alone.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 406);
		break;
	case 406:
		ShowST(g, PLR_NAME, "I-I just wanted to make some friends.", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 407);
		break;
	case 407:
		ShowST(g, NPC_NAME, "I don't need friends. Goodbye.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 500:
		ShowSTChoices(g, "What do you say?",
			"1) Uhh.. Nothing.", 501,
			"2) Youuuu.", 502);
		break;
	case 501:
		ShowST(g, NPC_NAME, "You got an eye problem? Huh? Don't bloody look at me again.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 502:
		ShowST(g, NPC_NAME, "You better jog on mate before I bash ya face in.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 600:
		ShowST(g, PLR_NAME, "Umm.... Hi?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 601);
		break;
	case 601:
		ShowST(g, NPC_NAME, "Do you have a time machine?", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 602);
		break;
	case 602:
		ShowST(g, PLR_NAME, "No?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 603);
		break;
	case 603:
		ShowST(g, NPC_NAME, "Then go away!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 700:
		ShowST(g, PLR_NAME, "Live where?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 701);
		break;
	case 701:
		ShowST(g, NPC_NAME, "Here.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 702);
		break;
	case 702:
		ShowST(g, PLR_NAME, "What do you mean here??", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 703);
		break;
	case 703:
		ShowST(g, NPC_NAME, "Here.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 800:
		ShowSTChoices(g, "What do you say?",
			"1) In a barrel.", 801,
			"2) In my house.", 801,
			"3) I don't know?", 801);
		break;
	case 801:
		ShowST(g, NPC_NAME, "Its not like I wanted to find my child anyway...", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 900:
		ShowST(g, PLR_NAME, "I dont think you want to ask that question...", PLR_X, PLR_Y, CONVO_TYPE_NORMAL);
		break;
	case 1000:
		ShowST(g, PLR_NAME, "Sorry?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 1001);
		break;
	case 1001:
		ShowST(g, NPC_NAME, "My pingas Bra, I need my Pingas!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1002);
		break;
	case 1002:
		ShowST(g, PLR_NAME, "I will be going now...", PLR_X, PLR_Y, CONVO_TYPE_NORMAL);
		break;
	case 1100:
		ShowST(g, PLR_NAME, "Yes!!!", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 1101);
		break;
	case 1101:
		ShowST(g, NPC_NAME, "Its just a prank, Its just a prank!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1);
		break;
	case 1200:
		ShowST(g, NPC_NAME, "This is private property mate!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1201);
		break;
	case 1201:
		ShowST(g, NPC_NAME, "You better get out of here!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 1300:
		ShowST(g, NPC_NAME, "Help! I'm stuck in bronze.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL, 1301);
		break;
	case 1301:
		ShowST(g, PLR_NAME, "What??", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 1302);
		break;
	case 1302:
		ShowST(g, NPC_NAME, "Uhh..Nothing.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 1400:
		ShowSTChoices(g, "What do you say?",
			"1) Yeah.", 1401,
			"2) Goodbye.", 0);
		break;
	case 1401:
		ShowST(g, NPC_NAME, "Oh, same.", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	case 1500:
		ShowST(g, PLR_NAME, "What?", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 0);
		break;
	case 1600:
		ShowST(g, PLR_NAME, "Hey!", PLR_X, PLR_Y, CONVO_TYPE_NORMAL, 1601);
		break;
	case 1601:
		ShowST(g, NPC_NAME, "Woah! That was an unexpected exception!", NPC_X, NPC_Y, CONVO_TYPE_NORMAL);
		break;
	}
}