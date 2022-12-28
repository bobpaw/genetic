/**
 * @file beef.h
 * @author Aiden Woodruff
 * @brief Beef Interpreter interface file.
 * 
 * @copyright Copyright (c) Aiden Woodruff 2022.
 * 
 */
/*
 *
 * +-<>[].,
 *
 */

#include <string>
#include <vector>

#ifndef BEEF_H_
#define BEEF_H_

namespace beef {
class Interpreter {
public:
	using input_type = std::function<char()>;

	Interpreter(std::string program, input_type input, size_t heap_height = 1024):
			program_(std::move(program)), input_(input), height_(heap_height),
			memory_(heap_height, '\0') {}
	Interpreter(std::string program, size_t heap_height = 1024):
			Interpreter(program, {}, heap_height) {}
	Interpreter(): Interpreter("", {}) {}

	void run();
	inline const std::string& output() const { return output_; }
	inline void set_program(std::string p) { program_ = p; }
	void clear_memory();
	inline void clear_output() { output_.clear(); }

private:
	std::string program_;
	std::vector<char> memory_;
	size_t height_;
	std::string output_;
	input_type input_;

	inline char get_input() {
		if (input_) {
			return input_();
		} else {
			return '\0';
		}
	}
	inline void output(char c) { output_ += c; }
};
}  // namespace beef

#endif  // BEEF_H_
