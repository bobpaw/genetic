#include <string>
#include <vector>
#include <random>
#include <utility>
#include <functional>
#include <numeric>
#include <algorithm>

#ifndef GENETIC_GEN_ALG_H_
#define GENETIC_GEN_ALG_H_

// int evaluate (const char * genotype, const char * correct);
// int recombine (char * genotype_a, char * genotype_b);
// int mutate (char ** genotype, const int chance, size_t maxlen);

namespace gen_alg {
	class basic_genetic {
	protected:
		std::vector<std::string> data_;
		std::vector<std::string> mid_gen;
		std::vector<int> fitness;
		int chance_{0};
		int population_{0};
		unsigned long gen_idx{0};

		// Statistics
		int max_{0};
		int sum_{0};
		float avg_{0.0};
		float mad_{0.0};

		using dataIndex_t = typename decltype(data_)::size_type;
		using fitnessIndex_t = typename decltype(fitness)::size_type;

		// Constructors
		basic_genetic (int pop_size, int chance):
		population_(pop_size),
		chance_(chance),
		gen_idx(0),
		// Stats
		max_(0), sum_(0), avg_(0.0), mad_(0.0),
		data_(pop_size),
		mid_gen(pop_size),
		fitness(pop_size) {};
		basic_genetic (int pop_size, int chance, int str_size, const std::string &pool);

	public:
		// Getters and Normal (ref) Setters
		auto generations (void) const noexcept { return gen_idx; }
		auto genome_fitness (fitnessIndex_t i) const { return fitness[i]; }
		auto genome (dataIndex_t i) const { return data_[i]; }
		auto pop_size (void) const noexcept { return population_; }
		auto chance (void) const noexcept { return chance_; }
		auto &chance (void) noexcept { return chance_; }
		auto max (void) const noexcept { return max_; }
		auto sum (void) const noexcept { return sum_; }
		auto avg (void) const noexcept { return avg_; }
		auto mad (void) const noexcept { return mad_; }

		virtual void statistics (void);
		decltype(gen_idx) operator++ (void);

	private:
		virtual int evaluate (const std::string &) = 0;
		virtual void recombine (std::string &, std::string &) = 0;
		virtual void mutate (dataIndex_t) = 0;
		fitnessIndex_t rand_genome (void);
	};

	class GeneticString : public basic_genetic {
	private:
		std::string correct_;
		int minsize_{0};
		int max_eval{0};
		bool one_{false};

		int evaluate (const std::string &genotype);
		void recombine (std::string &genotype_a, std::string &genotype_b);
		void mutate (dataIndex_t i);

	public:
		// Getters and Normal (ref) Setters
		auto one (void) const noexcept { return one_; }
		auto minsize (void) const noexcept { return minsize_; }

		// Special Setters
		void setPop_size (int size);

		// Constructors
		GeneticString (int population_size, int chance, std::string correct) : GeneticString(population_size, chance, correct, correct.size()) {}
		GeneticString (int pop_size, int chance, std::string correct, int minsize);

		virtual void statistics (void);
	};
} // namespace gen_alg

#endif // GENETIC_GEN_ALG_H_
