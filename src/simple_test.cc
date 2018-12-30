#include <iostream>

#include "gen_alg.h"

int main (int argc, char * argv[]) {
	gen_alg::Genetic hi(20, 23, "hello worlds");
	while (hi.generations() < 30000)
		++hi;
	std::cout << "Generation " << hi.generations() << std::endl;
	std::cout << "[";
	for (int i = 0; i < hi.pop_size(); i++) {
		std::cout << '"' << hi.genome(i) << '"';
		if (i == hi.pop_size() - 1) {
			std::cout << ']' << std::endl;
		} else {
			std::cout << ", ";
		}
	}
	std::cout << "Max evaluation: " << hi.max() << std::endl;
	// printf("Max length: %d\n", hi.maxsize());
	std::cout << "Average evaluation: " << hi.avg() << std::endl;
	std::cout << "Mean Average Deveation eval: " <<  hi.mad() << std::endl;
	return 0;
}
