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
	basic_genetic::statistics();
	for (auto f : fitness) {
		if (f == max_eval) one_ = true;
	}
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