#include <string>

#include "gen_alg.h"

namespace gen_alg {
  class GeneticString : public basic_genetic {
	private:
		std::string correct_;
		int max_eval{0};
		bool one_{false};

		int evaluate (const std::string &genotype);
		void recombine (std::string &genotype_a, std::string &genotype_b);
		void mutate (dataIndex_t i);

	public:
		// Getters and Normal (ref) Setters
		auto one (void) const noexcept { return one_; }

		// Setters
		void setCorrect (std::string arg);

		// Constructors
		GeneticString (int population_size, int chance, std::string correct) : GeneticString(population_size, chance, correct, correct.size()) {}
		GeneticString (int pop_size, int chance, std::string correct, int minsize);

		virtual void statistics (void);
	};
}