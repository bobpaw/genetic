#include "main.h"

#ifndef stat_bar_print
// Print text to status bar in the nice way, without erasing or refreshing
#define stat_bar_print(win,...)                                    \
  wprintw(win, __VA_ARGS__);                                     \
  for (int _i = getcurx(win); _i < COLS; _i++) waddch(win, ' ');
#endif

// An array of weights and the number of weights
int rand_choice (const int * weights, int n_memb) {
  int cumsum = 0;
  for (int i = 0; i < n_memb; i++) {
    cumsum += weights[i];
  }
  int number = rand() % cumsum;
  int ret = 0;
  for (; ret < n_memb; ret++) {
    if (number < weights[ret]) {
      break;
    } else {
      number -= weights[ret];
    }
  }
  return ret;
}

int main (int argc, char * argv[]) {
  char * correct_string = "hello world";
  const char * alphabet = "abcdefghijklmnopqrstuvxyz ";
  int mutate_chance = 3;
  int population_size = 20;
  int correct_size = strlen(correct_string);
  char ** generation = calloc(population_size, sizeof(char*));
  for (int i = 0; i < population_size; i++) {
    generation[i] = malloc(correct_size + 1);
    memset(generation[i], 0, correct_size + 1);
  }
  int * gen_fitness = calloc(population_size, sizeof(int));
  char ** mid_gen = calloc(population_size, sizeof(char*));
  // Generation count
  int gen_idx = 0;
  // If one string is correct
  int one = 0;
  int max = 0;
  float avg = 0;
  float sum = 0;
  float mad = 0;
  for (int i = 0; i < population_size; i++) {
    for (int c = 0; c < correct_size; c++) {
      generation[i][c] = alphabet[rand() % strlen(alphabet)];
    }
  }
  initscr();
  if (has_colors() == FALSE) {
    fprintf(stderr, "Colors aren't enabled, so highlighting won't work.");
  }
  if (has_colors() == TRUE) {
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
  }
  raw();
  curs_set(0);
  noecho();
  srand(time(NULL));
  timeout(-1);
  WINDOW * stat_bar = newwin(1, 0, LINES - 2, 0);
  WINDOW * entry = newwin(1, 0, LINES - 1, 0);
  wstandout(stat_bar);
  int ch = 0;
  int playing = FALSE;
  while (one != 1 && ch != 'q') {
    max = sum = 0;
    avg = 0;
    mad = 0;
    erase();
    werase(stat_bar);
    werase(entry);
    for (int i = 0; i < population_size; i++) {
      gen_fitness[i] = evaluate(generation[i], correct_string);
    }
    for (int i = 0; i < population_size; i++) {
      if (gen_fitness[i] > max) {
        max = gen_fitness[i];
      }
      sum += gen_fitness[i];
    }
    avg = sum / population_size;
    sum = 0;
    for (int i = 0; i < population_size; i++) {
      sum += fabs(avg - gen_fitness[i]);
    }
    mad = sum / population_size;
    printw("Generation %d\n", gen_idx);
    printw("[");
    for (int i = 0; i < population_size; i++) {
      if (gen_fitness[i] == max) {
        attron(COLOR_PAIR(1));
        printw("\"%s\"", generation[i]);
        attroff(COLOR_PAIR(1));
      } else {
        printw("\"%s\"", generation[i]);
      }
      if (i == population_size - 1) {
        printw("]\n");
      } else {
        printw(", ");
      }
    }
    printw("Max evaluation: %d\n", max);
    printw("Average evaluation: %.2f\n", avg);
    printw("Mean Average Deveation eval: %.2f\n", mad);
    stat_bar_print(stat_bar, "M - set mutate chance\tS - set correct string\tP - set population size");
    refresh();
    wrefresh(entry);
    wrefresh(stat_bar);
    if (playing == TRUE) {
      if (ch == ' ') {
        playing = FALSE;
        timeout(-1);
      }
      for (int i = 0; i < population_size; i++) {
        mid_gen[i] = strdup(generation[rand_choice(gen_fitness, population_size)]);
      }
      for (int i = 0; i < population_size; i += 2) {
        if (recombine(mid_gen[i], mid_gen[i+1]) != OK) {
          delwin(stat_bar);
          delwin(entry);
          endwin();
          fprintf(stderr, "Error breeding strings.\n");
          exit(EXIT_FAILURE);
        }
      }
      for (int i = 0; i < population_size; i++) {
        strcpy(generation[i], mid_gen[i]);
        free(mid_gen[i]);
        mutate(&(generation[i]), mutate_chance, correct_size);
      }
      gen_idx++;
      for (int i = 0; i < population_size; i++) {
        if (strcmp(generation[i], correct_string) == 0) {
          one = 1;
          break;
        }
      }
    } else {
      if (ch == ' ') {
        playing = TRUE;
        timeout(10);
      } else if (ch == 'm') {
        werase(stat_bar);
        stat_bar_print(stat_bar, "Mutate chance - currently: %d", mutate_chance);
        wrefresh(stat_bar);
        refresh();
        char a[4] = "";
        curs_set(1);
        echo();
        wgetnstr(entry, a, 3);
        if (strtol(a, NULL, 10) > 0 && strtol(a, NULL, 10) < 100) {
          mutate_chance = strtol(a, NULL, 10);
        }
        noecho();
        curs_set(0);
        werase(entry);
      }
    }
    ch = getch();
  }
  for (int i = 0; i < population_size; i++) {
    free(generation[i]);
  }
  free(generation);
  free(mid_gen);
  free(gen_fitness);
  delwin(stat_bar);
  delwin(entry);
  delwin(stdscr);
  generation = mid_gen = NULL;
  gen_fitness = NULL;
  stat_bar = entry = NULL;
  endwin();
  exit(EXIT_SUCCESS);
}
