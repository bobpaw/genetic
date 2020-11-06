#include <iostream>

#include "mancala.h"

int main() {
	mancala::Board b(mancala::Capture);

  int choice;
	while (true) {
		std::cout << b;
		for (choice = -1; choice < 0 || choice > 13;) {
			std::cout << "\nPlayer " << (b.player ? "2" : "1") << " move > " << std::flush;;
			std::cin >> choice;
		}
    b.move(choice);
	}
  return 0;
}