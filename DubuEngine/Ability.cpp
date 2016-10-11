#include "Game.hpp"

int LearnBook(Game* g, int book_id) {
	for (int i = 0; i < 6; i++) {
		AbilityBook* book = &g->AbilityBooks[i];
		if (book->book_id == -1) {
			book->book_id = book_id;
			if (book_id == BOOK_NETIKI) book->sprite_id = 8;
			if (book_id == BOOK_ADMIN) book->sprite_id = 4;
			return i;
		}
	}
	return -1;
}

void ForgetBook(Game* g, int book_id) {
	for (int i = 0; i < 6; i++) {
		if (g->AbilityBooks[i].book_id == book_id) {
			g->AbilityBooks[i] = AbilityBook();
		}
	}
}