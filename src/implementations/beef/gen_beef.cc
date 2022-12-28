/**
 * @file gen_beef.cc
 * @author Aiden Woodruff
 * @brief Genetic Beef implementation.
 * 
 * @copyright Copyright (c) Aiden Woodruff 2022.
 * 
 */
#include "gen_beef.h"

namespace gen_alg {

int GeneticBeef::evaluate (const std::string& genotype) {
  vm.clear_memory();
  vm.set_program(genotype);
  vm.run();
  return vm.output().size();
}

} // namespace gen_alg
