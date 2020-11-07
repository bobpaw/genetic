#include <algorithm>  // std::max_element
#include <numeric>    // std::accumulate

#include "genetic_string.h"

namespace gen_alg {
const std::string alphabet = "abcdefghijklmnopqrstuvwxyz ";
GeneticString::GeneticString(int pop_size, int chance, std::string correct,
														 int minsize):
		correct_(correct),
		one_(false), basic_genetic(pop_size, chance, minsize, alphabet) {
	max_eval = evaluate(correct_);
}

int GeneticString::evaluate(const std::string& genotype) {
	int sum = 0;
	for (decltype(correct_.size()) i = 0; i < correct_.size(); ++i)
		if (alphabet.find(correct_[i]) != std::string::npos &&
				correct_[i] == genotype[i])
			++sum;
	return sum * sum;
}

void GeneticString::recombine(std::string& genotype_a,
															std::string& genotype_b) {
	int length = std::min(genotype_a.size(), genotype_b.size());
	if (length == 0) {
		if (length == genotype_a.size()) {
			genotype_a = genotype_b;  // Both are now b
		} else {
			genotype_b = genotype_a;  // Both are now a
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

void GeneticString::mutate(dataIndex_t i) {
	if (data_[i].size() < correct_.size()) {
		switch (random() % 2) {
		case 0:
			if (random() % 100 < chance_)
				data_[i][random() % data_[i].size()] = generate(random());
			break;
		case 1: data_[i].append(1, generate(random()));
		}
	} else {
		if (random() % 100 < chance_)
			data_[i][random() % data_[i].size()] = generate(random());
	}
}

void GeneticString::statistics(void) {
	for (dataIndex_t i = 0; i < population_; ++i) {
		fitness[static_cast<fitnessIndex_t>(i)] = evaluate(data_[i]);
		if (fitness[static_cast<fitnessIndex_t>(i)] == max_eval) one_ = true;
	}
	sum_ = std::accumulate(fitness.begin(), fitness.end(), 0);
	max_ = *std::max_element(fitness.begin(), fitness.end());
	auto avg = avg_ = sum_ / population_;
	mad_ = std::accumulate(fitness.begin(), fitness.end(), 0.0,
												 [avg](const auto& cursum, const auto& x) {
													 return cursum + abs(x - avg);
												 });
	mad_ /= population_;
}

void GeneticString::setCorrect(std::string arg) {
	correct_ = std::move(arg);
	max_eval = evaluate(correct_);
	for (auto& i : data_) {
		i.resize(correct_.size());
		for (int c = str_size_; c < i.size(); ++c) i[c] = generate(random());
	}
	str_size_ = correct_.size();
}
}  // namespace gen_alg