#include "gen_alg.h"

#define OK 0
#define ERR -1

// Evaluation function for each genotype
int evaluate (const char * genotype, const char * correct) {
  int sum = 1;
  for (int i = 0; i < strlen(genotype); i++) {
    if (genotype[i] == correct[i]) {
      sum++;
    }
  }
  return sum;
}

// Breed strings WARNING: This will modify them.
int recombine (char * genotype_a, char * genotype_b) {
  int length = (strlen(genotype_a) > strlen(genotype_b) ? strlen(genotype_a) : strlen(genotype_b));
  char ** ret = calloc(sizeof(char*), 2);
  ret[0] = calloc(strlen(genotype_a) + 1, sizeof(char));
  ret[1] = calloc(strlen(genotype_b) + 1, sizeof(char));
  strcpy(ret[0], genotype_a);
  strcpy(ret[1], genotype_b);
  int split = rand() % length;
  if (
      strncpy(genotype_a, ret[1], split) != genotype_a ||
      strncpy(genotype_b, ret[0], split) != genotype_b
      ) return ERR;
  free(ret[0]);
  free(ret[1]);
  free(ret);
  return OK;
}

// Randomly mutate; modifies string
int mutate (char ** genotype, const int chance, size_t maxlen) {
  const char * alphabet = "abcdefghijklmnopqrstuvwxyz ";
  char * newgen = strdup(*genotype);
  if (rand() % 100 < chance) {
    if (strlen(*genotype) < maxlen && rand() % 100 < 50) {
      size_t length = strlen(*genotype);
      free(newgen);
      newgen = malloc(length + 2);
      memset(newgen, 0, length + 2);
      strcpy(newgen, *genotype);
      newgen[length + 1] = alphabet[rand() % 27];
    } else {
      newgen[rand() % strlen(*genotype)] = alphabet[rand() % 27];
    }
  }
  free(*genotype);
  *genotype = newgen;
  return OK;
}
