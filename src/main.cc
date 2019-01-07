#include "main.h"

#ifndef stat_bar_print
// Print text to status bar in the nice way, without erasing or refreshing
#define stat_bar_print(win,...)																		\
	mvwprintw(win, 0, 0, __VA_ARGS__);																		 \
	for (int _i = getcurx(win); _i < COLS; _i++) waddch(win, ' ');
#endif

#ifndef CTRL
#define CTRL(ch) ((ch) & 037)
#endif

#define NCURSES_ERROR(func, msg) do {if (!isendwin()) { curs_set(1); clear(); endwin(); delwin(stdscr); } \
std::cerr << "ncurses error(" #func "):" msg << std::endl; return -1; } while (0)

int main (int argc, char * argv[]) {
	gen_alg::GeneticString hello(20, 3, "hello world");
	std::string chance_str(4, ' ');
	char c_chance_str[4] = "";
	std::string pop_str;
	char c_pop_str[32] = "";

	initscr();
	if (!has_colors()) {
		std::cerr << "Colors aren't enabled, so highlighting won't work." << std::endl;
	} else {
		start_color();
		init_pair(1, COLOR_BLUE, COLOR_BLACK);
	}
	raw();
	curs_set(0);
	noecho();
	WINDOW * board = newwin(LINES - 2, 0, 0, 0);
	WINDOW * stat_bar = newwin(1, 0, LINES - 2, 0);
	WINDOW * entry = newwin(1, 0, LINES - 1, 0);
	wstandout(stat_bar);
	wtimeout(board, -1);
	int ch = 0, timeout_val = 5;
	bool playing = false;
	while (ch != 'q' && ch != CTRL('c')) {
		werase(board);
		werase(entry);
		werase(stat_bar);
		hello.statistics();
		mvwprintw(board, 0, 0, "Generation %lu\n[\n", hello.generations());
		for (int i = 0; i < hello.pop_size(); ++i) {
			if (getcurx(board) + hello.genome(i).size() + 4 > COLS) waddch(board, '\n');
			if (hello.genome_fitness(i) == hello.max()) {
				wattron(board, COLOR_PAIR(1));
				wprintw(board, "\"%s\"", hello.genome(i).c_str());
				wattroff(board, COLOR_PAIR(1));
			} else {
				wprintw(board, "\"%s\"", hello.genome(i).c_str());
			}
			wprintw(board, i == hello.pop_size() - 1 ? "]\n" : ", ");
		}
		wprintw(board, "Max evaluation: %d\n", hello.max());
		wprintw(board, "Average evaluation: %.2f\n", hello.avg());
		wprintw(board, "Mean Average Deveation eval: %.2f\n", hello.mad());
		if (hello.one()) wprintw(board, "Wow, one is correct!");
		stat_bar_print(stat_bar, "m - set mutate chance    s - set correct string    p - set population size    ^L - redraw screen    , . - Delay time (%d)", timeout_val);
		wnoutrefresh(board);
		wnoutrefresh(entry);
		wnoutrefresh(stat_bar);
		doupdate();
		if (hello.one()) {
			playing = false;
			wtimeout(board, -1);
		} else if (playing) {
			if (ch == ' ') {
				playing = false;
				wtimeout(board, -1);
			} else {
				++hello;
			}
		} else {
			switch (ch) {
			case ' ':
				playing = true;
				wtimeout(board, timeout_val * 10);
				break;
			case 'm':
				stat_bar_print(stat_bar, "Mutate chance - currently: %d", hello.chance());
				wrefresh(stat_bar);
				curs_set(1);
				echo();
				mvwgetnstr(entry, 0, 0, c_chance_str, 3);
				chance_str.assign(c_chance_str);
				try {
					if (std::stoi(chance_str) >= 0 && std::stoi(chance_str) <= 100) hello.chance() = std::stoi(chance_str);
					noecho();
					curs_set(0);
					werase(entry);
				} catch (const std::invalid_argument &e) {
					noecho();
					curs_set(0);
					werase(entry);
					waddstr(entry, "Invalid argument");
					wrefresh(entry);
				}
				break;
			case 'p':
				stat_bar_print(stat_bar, "Population size - currently: %d", hello.pop_size());
				wrefresh(stat_bar);
				wmove(entry, 0, 0);
				curs_set(1);
				echo();
				wgetnstr(entry, c_pop_str, 31);
				pop_str.assign(c_pop_str);
				if (std::stoi(pop_str) > 1) hello.setPop_size(std::stoi(pop_str));
				noecho();
				curs_set(0);
				werase(entry);
				break;
			case CTRL('z'):
				endwin();
				std::raise(SIGSTOP);
				doupdate();
				break;
			}
		}

		// Things that can change regardless of state
		switch (ch) {
		case ',':
		case '<':
			if (timeout_val > 0) --timeout_val;
			break;
		case '.':
		case '>':
			if (timeout_val < 40) ++timeout_val;
			break;
		case CTRL('L'): // Which has the same four LSBs as CTRL('l')
			clearok(board, TRUE);
			clearok(entry, TRUE);
			clearok(stat_bar, TRUE);
			break;
		}
		ch = wgetch(board);
	}
	delwin(stat_bar);
	delwin(entry);
	delwin(board);
	delwin(stdscr);
	board = stat_bar = entry = nullptr;
	endwin();
	return 0;
}
