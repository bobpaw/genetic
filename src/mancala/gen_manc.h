#include <string>

#include "gen_alg.h"
#include "mancala.h"

#ifndef GENETIC_GENMANC_H_
#define GENETIC_GENMANC_H_

namespace gen_alg {
class GeneticMancala: public basic_genetic {
	static mancala::Board b;

private:
	int evaluate(const std::string& genotype);
	void mutate(dataIndex_t i);

public:
	GeneticMancala(int p, int ch): GeneticMancala(p, ch, 1) {}
	GeneticMancala(int p, int ch, int m):
			basic_genetic(p, ch, m, "123456") {}
};
}  // namespace gen_alg

#endif  // GENETIC_GENMANC_H_
