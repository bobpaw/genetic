#include <iostream>

#include "gen_alg.h"

int main(int argc, char* argv[]) {
	auto p_genome = [](const gen_alg::GeneticString& x) {
		std::cout << "Generation " << x.generations() << std::endl;
		std::cout << "[";
		for (int i = 0; i < x.pop_size(); i++) {
			std::cout << '"' << x.genome(i) << '"';
			if (i == x.pop_size() - 1) {
				std::cout << ']' << std::endl;
			} else {
				std::cout << ", ";
			}
		}
		std::cout << "Max evaluation: " << x.max() << std::endl;
		// printf("Max length: %d\n", x.maxsize());
		std::cout << "Average evaluation: " << x.avg() << std::endl;
		std::cout << "Mean Average Deveation eval: " << x.mad() << std::endl;
	};

	gen_alg::GeneticString hi(20, 23, "hello worlds");
	while (hi.generations() < 1500) ++hi;
	p_genome(hi);
	hi.setPop_size(35);
	while (hi.generations() < 3000) ++hi;
	p_genome(hi);
	hi.setPop_size(40);
	while (hi.generations() < 4500) ++hi;
	return 0;
}
