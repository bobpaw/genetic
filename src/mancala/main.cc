#include "main.h"

#ifndef stat_bar_print
// Print text to status bar in the nice way, without erasing or refreshing
#define stat_bar_print(win, ...)     \
	mvwprintw(win, 0, 0, __VA_ARGS__); \
	wchgat(win, -1, A_STANDOUT, COLOR_PAIR(0), NULL);
#endif

#ifndef CTRL
#define CTRL(ch) ((ch) &037)
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

int main(int argc, char* argv[]) {
	int c = 0, pop = 20, mutate = 3;
	bool quit_fast = false, playing = false, quiet = false;
	int ch = 0, timeout_val = 5;
	char* endptr = nullptr;
	const char usage_string[] =
			" [-VhuQqs] [-p INT] [-m INT] [-d INT] [correct string]";
	const char help_string[] =
			"-p INT\tPopulation size\n-m INT\t Mutation chance in range [0, "
			"100]\n-Q\tQuit immediately on correct string\n-s\tStart immediately\n-d "
			"INT\tInitial delay amount in the range [0, 40]\n-q\tRun without "
			"graphics and print out generations number\n-h\tDisplay this help "
			"text\n-u\tDisplay usage string\n-V\tDisplay version info and bug "
			"contact\nIf not specified, correct string defaults to 'hello world'";
	getopt_uni::opterr = 0;
	while ((c = getopt_uni::getopt(argc, argv, "VhusqQp:m:d:")) != -1) {
		switch (c) {
		case 'V':
			std::cout << PROJECT_NAME " " PROJECT_VERSION << std::endl;
			std::cout << "Please submit bug reports to github.com/bobpaw/genetic"
								<< std::endl;
			return 0;
		case 'h':
			std::cout << PROJECT_NAME " " PROJECT_VERSION << std::endl;
			std::cout << "Usage: " << argv[0] << usage_string << "\n\n"
								<< help_string << std::endl;
			return 0;
		case 'Q': quit_fast = true; break;
		case 's': playing = true; break;
		case 'q': quiet = true; break;
		case 'p':
			errno = 0;
			pop = std::strtol(getopt_uni::optarg, &endptr, 0);
			if (errno != 0 || endptr == getopt_uni::optarg)
				pop = 20;  // Silently ignore error and set to default
			break;
		case 'm':
			errno = 0;
			mutate = std::strtol(getopt_uni::optarg, &endptr, 0);
			if (errno != 0 || endptr == getopt_uni::optarg)
				mutate = 3;  // Silently ignore and default
			break;
		case 'd':
			errno = 0;
			timeout_val = std::strtol(getopt_uni::optarg, &endptr, 0);
			if (errno != 0 || endptr == getopt_uni::optarg)
				timeout_val = 5;  // Silently ignore and default
			break;
		case 'u':
			std::cout << "Usage: " << argv[0] << usage_string << std::endl;
			return 0;
		case '?':
			if (std::isprint(getopt_uni::optopt))
				std::cerr << "Invalid option - '" << (char) getopt_uni::optopt
									<< "'.\n";
			else
				std::cerr << "Invalid option - " << std::hex << std::showbase
									<< getopt_uni::optopt << ".\n";
		default:
			std::cerr << "Usage: " << argv[0] << usage_string << std::endl;
			return -1;
		}
	}
	gen_alg::GeneticMancala hello(pop, mutate);
	if (quiet) {
		while (hello.max() < 48) ++hello;
		std::cout << "Generation: " << hello.generations() << std::endl;
		return 0;
	}
	std::string chance_str(4, ' ');
	char entry_str[64] = "";
	std::string pop_str;

	const char stat_string[] =
			"m - set mutate chance    p - set population size    ^L - redraw screen"
			"    , . - Delay time (%d)";
	initscr();
	if (!has_colors()) {
		std::cerr << "Colors aren't enabled, so highlighting won't work."
							<< std::endl;
	} else {
		start_color();
		init_pair(1, COLOR_BLUE, COLOR_BLACK);
	}
	raw();
	curs_set(0);
	noecho();
	WINDOW* board = newwin(LINES - 2, 0, 0, 0);
	const int stat_string_len = sizeof(stat_string) / sizeof(stat_string[0]);
	int stat_bar_height =
			stat_string_len / COLS + (stat_string_len % COLS == 0 ? 0 : 1);
	WINDOW* stat_bar = newwin(stat_bar_height, 0, LINES - stat_bar_height - 1, 0);
	WINDOW* entry = newwin(1, 0, LINES - 1, 0);
	wtimeout(board, -1);
	wstandout(stat_bar);
	if (playing) wtimeout(board, timeout_val * 10);
	while (ch != 'q' && ch != CTRL('c') && !(quit_fast && hello.max() == 48)) {
		werase(board);
		werase(entry);
		werase(stat_bar);
		hello.statistics();
		mvwprintw(board, 0, 0, "Generation %lu\n[", hello.generations());
		for (int i = 0; i < hello.pop_size(); ++i) {
			if (getcurx(board) + hello.genome(i).size() + 4 > getmaxx(board))
				waddch(board, '\n');
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
		if (hello.max() == 48) wprintw(board, "Found a perfect move");
		stat_bar_print(stat_bar, stat_string, timeout_val);
		wnoutrefresh(board);
		wnoutrefresh(entry);
		wnoutrefresh(stat_bar);
		doupdate();
		if (hello.max() == 48) {
			playing = false;
			if (quit_fast) break;
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
				werase(stat_bar);
				stat_bar_print(stat_bar, "Mutate chance - currently: %d",
											 hello.chance());
				wrefresh(stat_bar);
				curs_set(1);
				echo();
				mvwgetnstr(entry, 0, 0, entry_str, 3);
				chance_str.assign(entry_str);
				try {
					if (std::stoi(chance_str) >= 0 && std::stoi(chance_str) <= 100)
						hello.chance() = std::stoi(chance_str);
					noecho();
					curs_set(0);
					werase(entry);
				} catch (const std::invalid_argument& e) {
					noecho();
					curs_set(0);
					werase(entry);
					waddstr(entry, "Invalid argument");
					wrefresh(entry);
				}
				break;
			case 'p':
				werase(stat_bar);
				stat_bar_print(stat_bar, "Population size - currently: %d",
											 hello.pop_size());
				wrefresh(stat_bar);
				wmove(entry, 0, 0);
				curs_set(1);
				echo();
				wgetnstr(entry, entry_str, 31);
				pop_str.assign(entry_str);
				try {
					if (std::stoi(pop_str) > 1) hello.setPop_size(std::stoi(pop_str));
					noecho();
					curs_set(0);
					werase(entry);
				} catch (const std::invalid_argument& e) {
					noecho();
					curs_set(0);
					werase(entry);
					waddstr(entry, "Invalid argument");
					wrefresh(entry);
				}
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
			wtimeout(board, timeout_val * 10);
			break;
		case '.':
		case '>':
			if (timeout_val < 40) ++timeout_val;
			wtimeout(board, timeout_val * 10);
			break;
		case CTRL('L'):  // Which has the same four LSBs as CTRL('l')
			clearok(board, TRUE);
			clearok(entry, TRUE);
			clearok(stat_bar, TRUE);
			break;
		}
		ch = wgetch(board);
	}
	endwin();
	delwin(stat_bar);
	delwin(entry);
	delwin(board);

	// Stuff that prob doesn't work; try to free as much mem as possible
	delwin(stdscr);
	delwin(curscr);
	delwin(newscr);
	// del_curterm(cur_term);
	board = stat_bar = entry = nullptr;
	return 0;
}
