#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef GENETIC_GEN_ALG_H_
#define GENETIC_GEN_ALG_H_

int evaluate (const char * genotype, const char * correct);
int recombine (char * genotype_a, char * genotype_b);
int mutate (char ** genotype, const int chance, size_t maxlen);

#endif // GENETIC_GEN_ALG_H_
