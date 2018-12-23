#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "gen_alg.h"

#define OK 0
#define ERR -1

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
  char correct_string[12] = "hello worlds";
  const char * alphabet = "abcdefghijklmnopqrstuvxyz ";
  int mutate_chance = 23;
  int population_size = 20;
  int correct_size = 11;
  char ** generation = (char**) calloc(population_size, sizeof(char*));
  for (int i = 0; i < population_size; i++) {
    generation[i] = malloc(correct_size + 1);
    memset(generation[i], 0, correct_size + 1);
  }
  int * gen_fitness = (int*) calloc(population_size, sizeof(int));
  char ** mid_gen = (char**) calloc(population_size, sizeof(char*));
  // Generation count
  int gen_idx = 0;
  // If one string is correct
  int one = 0;
  int max = 0;
  float avg = 0.0;
  float sum = 0.0;
  float mad = 0.0;
  for (int i = 0; i < population_size; i++) {
    for (int c = 0; c < correct_size; c++) {
      generation[i][c] = alphabet[rand() % strlen(alphabet)];
    }
  }
  srand(time(NULL));
  while (one != 1 && gen_idx < 30000) {
    for (int i = 0; i < population_size; i++) {
      gen_fitness[i] = evaluate(generation[i], correct_string);
    }
    for (int i = 0; i < population_size; i++) {
      mid_gen[i] = strdup(generation[rand_choice(gen_fitness, population_size)]);
    }
    for (int i = 0; i < population_size; i += 2) {
      if (recombine(mid_gen[i], mid_gen[i+1]) != OK) {
        fprintf(stderr, "Error breeding strings.\n");
        exit(EXIT_FAILURE);
      }
    }
    for (int i = 0; i < population_size; i++) {
      strcpy(generation[i], mid_gen[i]);
      free(mid_gen[i]);
      mutate(&(generation[i]), mutate_chance, 12);
    }
    gen_idx++;
    for (int i = 0; i < population_size; i++) {
      if (strcmp(generation[i], correct_string) == 0) {
        one = 1;
        break;
      }
    }
  }
  max = sum = 0;
  int max_len = 0;
  avg = 0.0;
  mad = 0.0;

  for (int i = 0; i < population_size; i++) {
    if (gen_fitness[i] > max) {
      max = gen_fitness[i];
    }
    if (strlen(generation[i]) > max_len) {
      max_len = strlen(generation[i]);
    }
    sum += gen_fitness[i];
  }
  avg = sum / population_size;
  sum = 0;
  for (int i = 0; i < population_size; i++) {
    sum += fabs(avg - gen_fitness[i]);
  }
  mad = sum / population_size;
  printf("Generation %d\n", gen_idx);
  printf("[");
  for (int i = 0; i < population_size; i++) {
    printf("\"%s\"", generation[i]);
    if (i == population_size - 1) {
      printf("]\n");
    } else {
      printf(", ");
    }
  }
  printf("Max evaluation: %d\n", max);
  printf("Max length: %d\n", max_len);
  printf("Average evaluation: %.2f\n", avg);
  printf("Mean Average Deveation eval: %.2f\n", mad);

  for (int i = 0; i < population_size; i++) {
    free(generation[i]);
    generation[i] = NULL;
    mid_gen[i] = NULL;
  }
  free(generation);
  free(mid_gen);
  free(gen_fitness);
  generation = mid_gen = NULL;
  gen_fitness = NULL;
  exit(EXIT_SUCCESS);
}
