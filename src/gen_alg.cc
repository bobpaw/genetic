#include <string>
#include <random>
#include <functional>
#include <numeric> // std::accumulate
#include <algorithm> // std::max_element

#include "gen_alg.h"

#ifdef VALGRIND_DEBUG
#include "fakerandom.h"
#endif

namespace gen_alg {
	#ifdef VALGRIND_DEBUG
	auto random = fakerandom::rand_device(0, 100);
	#else
	static auto engine = std::default_random_engine(std::random_device()());
	static auto distribution = std::uniform_int_distribution<int>(0);
	static auto random () { return distribution(engine); }
	#endif

	basic_genetic::basic_genetic (int pop_size, int chance, int str_size, std::string p):
	basic_genetic(pop_size, chance) {
		pool = p;
		str_size_ = str_size;
		for (auto &i : data_) {
			i.resize(str_size);
			for (auto &c : i) c = basic_genetic::generate(random());
		}
	}

	void basic_genetic::setPop_size (int size) {
		data_.reserve(size);
		for (dataIndex_t i = population_; i < size; ++i) {
			data_.emplace_back(str_size_, ' ');
			for (auto &c : data_.back()) c = generate(random());
		}
		fitness.resize(size);

		// mid_gen needs to be size divisible by 2
		mid_gen.resize(size + (size % 2 == 0 ? 0 : 1));
		population_ = size;
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
	static constexpr T abs (const T &x) noexcept { return x < 0 ? x * -1 : x; }

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
