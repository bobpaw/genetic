#include <chrono>

// For use with valgrind, which breaks sometimes?
// Keep in mind this is a different implementation than dist
// The code dist code therefore *must* work, b/c valgrind can't catch it
namespace fakerandom {

// Generate a non-random number in the range [min, max)
class rand_device {
	int min_;
	int max_;
	std::chrono::time_point<std::chrono::steady_clock> clock;

public:
	rand_device(int min, int max): min_(min), max_(max) {}
	int operator()(void) {
		clock = std::chrono::steady_clock::now();
		int ret = static_cast<int>(clock.time_since_epoch().count());
		if (ret < 0) ret *= -1;
		return (ret % (max_ - min_)) + min_;
	}
};
}  // namespace fakerandom
