#include "gen_alg.h"

#ifdef VALGRIND_DEBUG
#include "fakerandom.h"
#endif

namespace gen_alg {
	const std::string alphabet = "abcdefghijklmnopqrstuvwxyz ";
	#ifdef VALGRIND_DEBUG
	auto random = fakerandom::rand_device(0, 100);
	#else
	auto random = [engine = std::default_random_engine(std::random_device()()), distribution = std::uniform_int_distribution<int>(0)] (void) mutable {
		return distribution(engine); };
	#endif

	GeneticString::GeneticString (int pop_size, int chance, std::string correct, int minsize):
		correct_(correct), minsize_(minsize), one_(false), basic_genetic(pop_size, chance, minsize, alphabet)
	{
		max_eval = evaluate(correct_);
	}

	basic_genetic::basic_genetic (int pop_size, int chance, int str_size, const std::string &pool):
	basic_genetic(pop_size, chance) {
		for (auto &i : data_) {
			i.resize(str_size);
			for (auto &c : i) c = pool[random() % pool.size()];
		}
	}

	void GeneticString::setPop_size (int size) {
		data_.reserve(size);
		for (dataIndex_t i = population_; i < size; ++i) {
			data_.emplace_back(minsize_, ' ');
			for (auto &c : data_.back()) c = alphabet[random() % alphabet.size()];
		}
		fitness.resize(size);

		// mid_gen needs to be size divisible by 2
		mid_gen.resize(size + (size % 2 == 0 ? 0 : 1));
		population_ = size;
	}

	int GeneticString::evaluate (const std::string &genotype) {
		int sum = 0;
		for (decltype(correct_.size()) i = 0; i < correct_.size(); ++i)
			if (alphabet.find(correct_[i]) != std::string::npos && correct_[i] == genotype[i]) ++sum;
		return sum * sum;
	}

	void GeneticString::recombine (std::string &genotype_a, std::string &genotype_b) {
		int length = std::min(genotype_a.size(), genotype_b.size());
		if (length == 0) {
			if (length == genotype_a.size()) {
				genotype_a = genotype_b; // Both are now b
			} else {
				genotype_b = genotype_a; // Both are now a
			}
		} else {
			auto ret = std::make_pair(std::string(), std::string());
			int split = random() % length;
			ret.first = genotype_b.substr(0, split) + genotype_a.substr(split);
			ret.second = genotype_a.substr(0, split) + genotype_b.substr(split);
			genotype_a = ret.first;
			genotype_b = ret.second;
		}
	}

	void GeneticString::mutate (dataIndex_t i) {
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

	basic_genetic::fitnessIndex_t basic_genetic::rand_genome (void) {
		fitnessIndex_t ret = 0;
		if (sum_ == 0) {
			ret = random() % fitness.size();
		} else {
			int number = random() % sum_;
			for (; ret < population_ != number < fitness[ret]; ++ret)
				number -= fitness[ret];
		}
		return ret;
	}

	template <typename T>
	static constexpr T abs (const T &x) { return x < 0 ? x * -1 : x; }

	// Calculate and update stats
	void basic_genetic::statistics (void) {
		for (dataIndex_t i = 0; i < population_; ++i) {
			fitness[static_cast<fitnessIndex_t>(i)] = evaluate(data_[i]);
		}
		sum_ = std::accumulate(fitness.begin(), fitness.end(), 0);
		max_ = *std::max_element(fitness.begin(), fitness.end());
		auto avg = avg_ = sum_ / population_;
		mad_ = std::accumulate(fitness.begin(), fitness.end(), 0.0, [avg](const auto &cursum, const auto &x){
			return cursum + abs(x - avg);
		});
		mad_ /= population_;
	}

	void GeneticString::statistics (void) {
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

	decltype(basic_genetic::gen_idx) basic_genetic::operator++ (void) {
		statistics();
		for (dataIndex_t i = 0; i < mid_gen.size(); ++i)
			mid_gen[i] = data_[rand_genome()];
		for (dataIndex_t i = 0; i < mid_gen.size(); i += 2)
			recombine(mid_gen[i], mid_gen[i + 1]);
		for (dataIndex_t i = 0; i < population_; ++i) {
			data_[i] = mid_gen[i];
			mutate(i);
		}
		return ++gen_idx;
	}
} // namespace gen_alg
