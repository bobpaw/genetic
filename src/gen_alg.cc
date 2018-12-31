#include "gen_alg.h"

#ifdef VALGRIND_DEBUG
#include "fakerandom.h"
#endif

namespace gen_alg {
	#ifdef VALGRIND_DEBUG
	auto random = fakerandom::rand_device(0, 100);
	#else
	auto random = std::bind(std::uniform_int_distribution<int>(0, 100),
		std::default_random_engine(std::random_device()()));
	#endif

	Genetic::Genetic (int pop_size, int chance, std::string correct, int maxsize) :
		population_(pop_size),
		chance_(chance),
		correct_(correct),
		maxsize_(maxsize),
		gen_idx(0),
		// Stats
		one_(false), max_(0), sum_(0), avg_(0.0), mad_(0.0),
		// Vectors
		data_(pop_size, std::string(maxsize, ' ')),
		mid_gen(pop_size),
		fitness(pop_size) {
		for (auto &i : data_) {
			for (auto &s : i) {
				s = alphabet[random() % alphabet.size()];
			}
		}
	}

	int Genetic::evaluate (dataIndex_t genotype) {
		int sum = 1;
		for (decltype(correct_.size()) i = 0; i < data_[genotype].size(); ++i)
			if (data_[genotype][i] == correct_[i]) ++sum;
		return sum;
	}

	void Genetic::recombine (std::string &genotype_a, std::string &genotype_b) {
		int length = std::min(genotype_a.size(), genotype_b.size());
		auto ret = std::make_pair(std::string(), std::string());
		int split = random() % length;
		ret.first = genotype_b.substr(0, split) + genotype_a.substr(split);
		ret.second = genotype_a.substr(0, split) + genotype_b.substr(split);
		genotype_a = ret.first;
		genotype_b = ret.second;
	}

	void Genetic::mutate (dataIndex_t i) {
		if (random() < chance_) {
			data_[i][random() % data_[i].size()] = alphabet[random() % alphabet.size()];
		}
	}

	Genetic::dataIndex_t Genetic::rand_genome (void) {
		int number = random() % sum_;
		dataIndex_t ret = 0;
		for (; ret < population_ != number < fitness[ret]; ++ret)
			number -= fitness[ret];
		return ret;
	}

	// Calculate and update stats
	void Genetic::statistics (void) {
		for (dataIndex_t i = 0; i < population_; ++i) {
			fitness[static_cast<fitnessIndex_t>(i)] = evaluate(i);
			if (fitness[static_cast<fitnessIndex_t>(i)] == maxsize_) one_ = true;
		}
		sum_ = std::accumulate(fitness.begin(), fitness.end(), 0);
		max_ = *std::max_element(fitness.begin(), fitness.end());
		/* auto avg = avg_ = sum_ / population_;
		std::vector<decltype(avg_)> dev(fitness.size());
		std::transform(fitness.begin(), fitness.end(), dev.begin(), [avg](const float &x) {return std::abs(x - avg);});
		mad_ = std::accumulate(dev.begin(), dev.end(), 0.0); */
		// mad_ /= population_;
	}

	Genetic &Genetic::operator++ (void) {
		statistics();
		for (dataIndex_t i = 0; i < population_; ++i)
			mid_gen[i] = data_[rand_genome()];
		for (dataIndex_t i = 0; i < population_; i += 2)
			recombine(mid_gen[i], mid_gen[i + 1]);
		for (dataIndex_t i = 0; i < population_; ++i) {
			data_[i] = mid_gen[i];
			mutate(i);
		}
		++gen_idx;
		return *this;
	}
} // namespace gen_alg
