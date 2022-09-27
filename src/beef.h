/*
 *
 * +-<>[].,
 *
 */

#include <string>

#ifndef BEEF_H_
#define BEEF_H_

namespace beef {
class Interpreter {
public:
	using input_type = std::function<char()>;

	Interpreter(std::string program, input_type input, size_t heap_height = 1024):
			program_(std::move(program)), input_(input), height_(heap_height) {}
  Interpreter(std::string program, size_t heap_height = 1024):
    Interpreter(program, {}, heap_height) {}
	Interpreter(): Interpreter("", {}) {}

	void run();
	inline const std::string& output() const { return output_; }
	inline void clear_output() { output_.clear(); }

private:
	std::string program_;
	std::string output_;
	input_type input_;
  size_t height_;

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
