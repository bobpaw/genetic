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
	const std::string alphabet = "abcdegfhijklmnopqrstuvwxyz";

	class Genetic {
	private:
		std::vector<std::string> data_;
		std::vector<std::string> mid_gen;
		std::string correct_;
		int chance_;
		int population_;
		unsigned long gen_idx;
		int maxsize_;
		int max_;
		int sum_;
		float avg_;
		float mad_;
		std::vector<int> fitness;

		using dataIndex_t = decltype(data_)::size_type;
		using fitnessIndex_t = decltype(fitness)::size_type;

	public:
		// Getters
		auto generations (void) const { return gen_idx; }
		auto genome (dataIndex_t i) const { return data_[i]; }
		auto pop_size (void) const { return population_; }
		auto chance (void) const { return chance_; }
		auto max (void) const { return max_; }
		auto sum (void) const { return sum_; }
		auto avg (void) const { return avg_; }
		auto mad (void) const { return mad_; }

		// Special Setters
		Genetic &setPop_size (int size) {
			population_ = size;
			data_.resize(size);
			fitness.resize(size);
			mid_gen.resize(size);
			return *this;
		}

		// Constructors
		Genetic (int population_size, int chance, std::string correct) : Genetic(population_size, chance, correct, correct.size()) {}
		Genetic (int pop_size, int chance, std::string correct, int maxsize);

		int evaluate (dataIndex_t genotype);
		static void recombine (std::string &genotype_a, std::string &genotype_b);
		void mutate (dataIndex_t i);
		dataIndex_t rand_genome (void);
		Genetic &operator++ (void);
	};
} // namespace gen_alg

#endif // GENETIC_GEN_ALG_H_
