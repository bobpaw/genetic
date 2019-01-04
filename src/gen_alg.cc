#include "gen_alg.h"

#ifdef VALGRIND_DEBUG
#include "fakerandom.h"
#endif

namespace gen_alg {
	const std::string alphabet = "abcdegfhijklmnopqrstuvwxyz";
	#ifdef VALGRIND_DEBUG
	auto random = fakerandom::rand_device(0, 100);
	#else
	auto random = [engine = std::default_random_engine(std::random_device()()), distribution = std::uniform_int_distribution<int>(0)] (void) mutable {
		return distribution(engine); };
	#endif

	Genetic::Genetic (int pop_size, int chance, std::string correct, int minsize) :
		population_(pop_size),
		chance_(chance),
		correct_(correct),
		minsize_(minsize),
		gen_idx(0),
		// Stats
		one_(false), max_(0), sum_(0), avg_(0.0), mad_(0.0),
		// Vectors
		data_(pop_size, std::string(minsize, ' ')),
		mid_gen(pop_size),
		fitness(pop_size)
	{
		max_eval = evaluate(correct_);
		for (auto &i : data_) {
			i.reserve(minsize_);
			for (auto &c : i) c = alphabet[random() % alphabet.size()];
		}
	}

	Genetic &Genetic::setPop_size (int size) {
		data_.reserve(size);
		for (Genetic::dataIndex_t i = population_; i < size; ++i) {
			data_.emplace_back(minsize_, ' ');
			for (auto &c : data_.back()) c = alphabet[random() % alphabet.size()];
		}
		fitness.resize(size);

		// mid_gen needs to be size divisible by 2
		mid_gen.resize(size + (size % 2 == 0 ? 0 : 1));
		population_ = size;
		return *this;
	}

	int Genetic::evaluate (const std::string &genotype) {
		int sum = 1;
		for (decltype(correct_.size()) i = 0; i < genotype.size(); ++i)
			if (genotype[i] == correct_[i]) ++sum;
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
		if (data_[i].size() < correct_.size()) {
			switch (random() % 2) {
			case 0:
				if (random() % 100 < chance_)
					data_[i][random() % data_[i].size()] = alphabet[random() % alphabet.size()];
				break;
			case 1:
				data_[i].append(1, alphabet[random() % alphabet.size()]);
			}
		} else {
			if (random() % 100 < chance_)
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

	template <typename T>
	static constexpr T abs (const T &x) { return x < 0 ? x * -1 : x; }

	// Calculate and update stats
	void Genetic::statistics (void) {
		for (dataIndex_t i = 0; i < population_; ++i) {
			fitness[static_cast<fitnessIndex_t>(i)] = evaluate(data_[i]);
			if (fitness[static_cast<fitnessIndex_t>(i)] == max_eval) one_ = true;
		}
		sum_ = std::accumulate(fitness.begin(), fitness.end(), 0);
		max_ = *std::max_element(fitness.begin(), fitness.end());
		auto avg = avg_ = sum_ / population_;
		mad_ = std::accumulate(fitness.begin(), fitness.end(), 0.0, [avg](const auto &cursum, const auto &x){
			return cursum + abs(x - avg);
		});
		mad_ /= population_;
	}

	Genetic &Genetic::operator++ (void) {
		statistics();
		for (dataIndex_t i = 0; i < mid_gen.size(); ++i)
			mid_gen[i] = data_[rand_genome()];
		for (dataIndex_t i = 0; i < mid_gen.size(); i += 2)
			recombine(mid_gen[i], mid_gen[i + 1]);
		for (dataIndex_t i = 0; i < population_; ++i) {
			data_[i] = mid_gen[i];
			mutate(i);
		}
		++gen_idx;
		return *this;
	}
} // namespace gen_alg
