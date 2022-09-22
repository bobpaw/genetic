/*
 *
 * +-<>[].,
 *
 */

#include <string>

#ifndef BEEF_HEAP_HEIGHT
#define BEEF_HEAP_HEIGHT 1024
#endif

#ifndef BEEF_H_
#define BEEF_H_

namespace beef {
class Interpreter {
public:
	Interpreter(std::string program): program_(std::move(program)) {}
	Interpreter(): program_() {}

	void run();
	inline const std::string& output() const { return output_; }
	inline void clear_output() { output_.clear(); }

private:
	std::string program_;
	std::string output_;
	std::function<char()> input_;

	inline void output(char c) { output_ += c; }
};
}  // namespace beef

#endif  // BEEF_H_
