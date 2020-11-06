#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "main.h"
#include "mancala.h"

#ifndef CTRL
#	define CTRL(ch) ((ch) &037)
#endif

#define NCURSES_ERROR(func, msg)                               \
	do {                                                         \
		if (!isendwin()) {                                         \
			curs_set(1);                                             \
			clear();                                                 \
			endwin();                                                \
			delwin(stdscr);                                          \
		}                                                          \
		std::cerr << "ncurses error(" #func "):" msg << std::endl; \
		return -1;                                                 \
	} while (0)

struct CursedMancala: public mancala::Board {
	WINDOW* win;
	using mancala::Board::Board;
	virtual int move_pieces(int position) {
		int initial_hand = board[position];
		board[position] = 0;
		for (int i = 0; i < initial_hand; ++i) {
			++position;
			if (position % 14 == (player ? 7 : 0))  // i.e. not their pocket
				++position;

			position %= 14;
			++board[position];

			werase(win);
			display(position);

			if (position % 7 == 0)
				mvwprintw(win, player ? 1 : 6, 2, "%0.2d", initial_hand - i - 1);
			else
				mvwprintw(win, position < 7 ? position : 14 - position, 2, "%0.2d",
									initial_hand - i - 1);
			wrefresh(win);
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
		return position % 14;
	}

	void display(int position) const {
		wprintw(win, "  %0.2d\n", board[0]);
		for (int i = 1; i < 7; ++i) {
			wprintw(win, "%0.2d", board[i]);
			wprintw(win, "  ");
			wprintw(win, "%0.2d", board[14 - i]);
			wprintw(win, "\n");
		}
		wprintw(win, "  %0.2d", board[7]);

		if (position == 0)
			mvwchgat(win, 0, 2, 2, WA_STANDOUT, -1, NULL);
		else if (position == 7)
			mvwchgat(win, 7, 2, 2, WA_STANDOUT, -1, NULL);
		else if (position < 7)
			mvwchgat(win, position, 0, 2, WA_STANDOUT, -1, NULL);
		else
			mvwchgat(win, 14 - position, 4, 2, WA_STANDOUT, -1, NULL);
		wmove(win, 7, 4);
	}

	using mancala::Board::operator[];
};

int main() {
	CursedMancala b(mancala::Avalanche);
	bool quit_fast = false, player_tracker = b.player;
	int ch = 0, timeout_val = 5, highlight = 1;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	curs_set(0);
	noecho();
	timeout(-1);

	b.win = stdscr;
	while (ch != 'q' && ch != CTRL('c') && !b.over()) {
		switch (ch) {
		case ' ':
		case KEY_ENTER:
			b.move(highlight);
			if (b.player != player_tracker) {
				highlight = b.player ? 8 : 1;
				player_tracker = b.player;
			}
			break;

		case KEY_UP:
		case 'w':
			if (highlight != (b.player ? 13 : 1)) highlight += (b.player ? 1 : -1);
			break;
		case KEY_DOWN:
		case 's':
			if (highlight != (b.player ? 8 : 6)) highlight -= (b.player ? 1 : -1);
			break;

		case CTRL('z'):
			endwin();
			std::raise(SIGSTOP);
			doupdate();
			break;

		case CTRL('L'):  // Which has the same four LSBs as CTRL('l')
			clear();
			break;
		}
		
		erase();
		b.display(highlight);
		printw("\nPlayer %d move?", b.player ? 2 : 1);
    if (b.over()) mvprintw(8, 0, "Game over, player %d wins!", b[0] > b[7] ? 2 : 1);

		refresh();
		ch = getch();
	}
	endwin();
	// Stuff that prob doesn't work; try to free as much mem as possible
	delwin(stdscr);
	delwin(curscr);
	delwin(newscr);
	// del_curterm(cur_term);
	return 0;
}
