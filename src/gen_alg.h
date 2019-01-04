#include <string>
#include <vector>
#include <random>
#include <utility>
#include <functional>
#include <numeric>
#include <algorithm>
#include <cstdlib>

#ifndef GENETIC_GEN_ALG_H_
#define GENETIC_GEN_ALG_H_

// int evaluate (const char * genotype, const char * correct);
// int recombine (char * genotype_a, char * genotype_b);
// int mutate (char ** genotype, const int chance, size_t maxlen);

namespace gen_alg {
	class Genetic {
	private:
		std::vector<std::string> data_;
		std::vector<std::string> mid_gen;
		std::string correct_;
		int chance_{0};
		int population_{0};
		unsigned long gen_idx{0};
		int maxsize_{0};
		int max_{0};
		int sum_{0};
		bool one_{false};
		float avg_{0.0};
		float mad_{0.0};
		std::vector<int> fitness;

		using dataIndex_t = decltype(data_)::size_type;
		using fitnessIndex_t = decltype(fitness)::size_type;

	public:
		// Getters and Normal (ref) Setters
		auto generations (void) const noexcept { return gen_idx; }
		auto genome (dataIndex_t i) const { return data_[i]; }
		auto genome_fitness (fitnessIndex_t i) const { return fitness[i]; }
		auto pop_size (void) const noexcept { return population_; }
		auto chance (void) const noexcept { return chance_; }
		auto &chance (void) noexcept { return chance_; }
		auto one (void) const noexcept { return one_; }
		auto max (void) const noexcept { return max_; }
		auto sum (void) const noexcept { return sum_; }
		auto avg (void) const noexcept { return avg_; }
		auto mad (void) const noexcept { return mad_; }
		auto maxsize (void) const noexcept { return maxsize_; }

		// Special Setters
		Genetic &setPop_size (int size);

		// Constructors
		Genetic (int population_size, int chance, std::string correct) : Genetic(population_size, chance, correct, correct.size()) {}
		Genetic (int pop_size, int chance, std::string correct, int maxsize);

		int evaluate (dataIndex_t genotype);
		static void recombine (std::string &genotype_a, std::string &genotype_b);
		void mutate (dataIndex_t i);
		dataIndex_t rand_genome (void);
		void statistics (void);
		Genetic &operator++ (void);
	};
} // namespace gen_alg

#endif // GENETIC_GEN_ALG_H_
