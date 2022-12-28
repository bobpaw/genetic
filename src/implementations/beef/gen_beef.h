/**
 * @file gen_beef.h
 * @author Aiden Woodruff
 * @brief Genetic Beef interface.
 * 
 * @copyright Copyright (c) Aiden Woodruff 2022.
 * 
 */
#include "beef.h"
#include "gen_alg.h"

#ifndef GENETIC_GENBEEF_H_
#define GENETIC_GENBEEF_H_

namespace gen_alg {

class GeneticBeef: public basic_genetic {
	beef::Interpreter vm;

private:
	int evaluate(const std::string& genotype);
	void mutate(dataIndex_t i);

public:
	GeneticBeef(int p, int ch): basic_genetic(p, ch, 1, "+-<> [].,"), vm("", 64) {}
};

}  // namespace gen_alg

#endif  // GENETIC_GENBEEF_H_
