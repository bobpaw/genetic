#include "main.h"

#ifndef stat_bar_print
// Print text to status bar in the nice way, without erasing or refreshing
#define stat_bar_print(win,...)																		\
	wprintw(win, __VA_ARGS__);																		 \
	for (int _i = getcurx(win); _i < COLS; _i++) waddch(win, ' ');
#endif

#define NCURSES_ERROR(func, msg) do {if (!isendwin()) { curs_set(1); clear(); endwin(); delwin(stdscr); } \
std::cerr << "ncurses error(" #func "):" msg << std::endl; return -1; } while (0)

int main (int argc, char * argv[]) {
	gen_alg::Genetic hello(20, 3, "hello world");
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
	timeout(-1);
	WINDOW * stat_bar = newwin(1, 0, LINES - 2, 0);
	WINDOW * entry = newwin(1, 0, LINES - 1, 0);
	wstandout(stat_bar);
	int ch = 0;
	bool playing = false;
	while (!hello.one() && ch != 'q') {
		erase();
		werase(stat_bar);
		werase(entry);
		hello.statistics();
		printw("Generation %d\n", hello.generations());
		printw("[");
		for (int i = 0; i < hello.pop_size(); ++i) {
			if (hello.genome_fitness(i) == hello.max()) {
				attron(COLOR_PAIR(1));
				printw("\"%s\"", hello.genome(i).c_str());
				attroff(COLOR_PAIR(1));
			} else {
				printw("\"%s\"", hello.genome(i).c_str());
			}
			printw(i == hello.pop_size() - 1 ? "]\n" : ", ");
		}
		printw("Max evaluation: %d\n", hello.max());
		printw("Average evaluation: %.2f\n", hello.avg());
		printw("Mean Average Deveation eval: %.2f\n", hello.mad());
		stat_bar_print(stat_bar, "m - set mutate chance\tS - set correct string\tp - set population size");
		refresh();
		wrefresh(entry);
		wrefresh(stat_bar);
		if (playing) {
			if (ch == ' ') {
				playing = false;
				timeout(-1);
			}
			++hello;
		} else {
			switch (ch) {
			case ' ':
				playing = true;
				timeout(10);
				break;
			case 'm':
				werase(stat_bar);
				stat_bar_print(stat_bar, "Mutate chance - currently: %d", hello.chance());
				wrefresh(stat_bar);
				refresh();
				curs_set(1);
				echo();
				wgetnstr(entry, c_chance_str, 3);
				chance_str.assign(c_chance_str);
				if (std::stoi(chance_str) >= 0 && std::stoi(chance_str) <= 100) hello.chance() = std::stoi(chance_str);
				noecho();
				curs_set(0);
				werase(entry);
				break;
			case 'p':
				werase(stat_bar);
				stat_bar_print(stat_bar, "Population size - currently: %d", hello.pop_size());
				wrefresh(stat_bar);
				refresh();
				curs_set(1);
				echo();
				wgetnstr(entry, c_pop_str, 31);
				pop_str.assign(c_pop_str);
				if (std::stoi(pop_str) > 1) hello.setPop_size(std::stoi(pop_str));
				noecho();
				curs_set(0);
				werase(entry);
				break;
			}
		}
		ch = getch();
	}
	delwin(stat_bar);
	delwin(entry);
	delwin(stdscr);
	stat_bar = entry = nullptr;
	endwin();
	return 0;
}
