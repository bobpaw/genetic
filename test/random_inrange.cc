#include <iostream>
#include "rpc/fakerandom.h"

int main() {
	fakerandom::rand_device random(0, 100);
	for (int i = 0; i < 2000; ++i) { std::cout << random() << '\n'; }
	std::cout << std::flush;
	return 0;
}
