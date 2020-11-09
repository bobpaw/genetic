#include <string>

#include "gen_manc.h"

namespace gen_alg {
int GeneticMancala::evaluate(const std::string& genotype) {
	b.reset();
	for (char c : genotype) {
		b.move(c - '0');
		if (b.player) break;
	}
	return b[7];
}

void GeneticMancala::mutate(dataIndex_t i) {
	evaluate(data_[i]);
	if (!b.player && random() % 100 < chance_ / 2) {
		data_[i] += generate(random());
	} else if (random() % 100 < chance_)
		data_[i][random() % data_[i].size()] = generate(random());
}
}  // namespace gen_alg
