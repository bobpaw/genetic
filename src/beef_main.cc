#include <string>
#include <iostream>
#include <fstream>

#include "beef.h"

// 4845 4c4c 4f20 574f 524c 442e 0a

char input_func () { return std::cin.get(); }

int main (int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
    return -1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "ERROR: Opening file " << argv[0] << " failed." << std::endl;
    return -1;
  }

  std::string program;
  program.reserve(512);

  for (std::string line; std::getline(file, line); program += line);

  beef::Interpreter interpreter(program, input_func);

  /*for (int i = 0; i < 5000; ++i) {
    beef::Interpreter interp(program);
    interp.run();
    interp.clear_output();
  }*/
  interpreter.run();

  std::cout << interpreter.output() << std::endl;
  return 0;
}
