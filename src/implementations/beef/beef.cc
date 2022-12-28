/**
 * @file beef.cc
 * @author Aiden Woodruff
 * @brief Beef Interpreter implementation file.
 * 
 * @copyright Copyright (c) Aiden Woodruff 2022.
 * 
 */

#include "beef.h"

namespace beef {

void Interpreter::clear_memory() {
	// Consider replacing memory_ with char* and using memset.
	for (std::size_t i = 0; i < height_; ++i) memory_[i] = '\0';
}

void Interpreter::run() {
	auto ptr = memory_.begin();
	std::size_t iptr = 0;

	while (iptr < program_.size()) {
		switch (program_[iptr]) {
		case '+': ++(*ptr); break;
		case '-': --(*ptr); break;
		case '<':
			if (ptr == memory_.begin())
				ptr = memory_.end() - 1;
			else
				--ptr;
			break;
		case '>':
			if (ptr == memory_.end() - 1)
				ptr = memory_.begin();
			else
				++ptr;
			break;
		case ',': *ptr = get_input(); break;
		case '.': output(*ptr); break;
		case '[':
			if (*ptr == 0) {
				++iptr;
				for (int nest = 0; iptr < program_.size(); ++iptr) {
					if (program_[iptr] == ']' && nest == 0) break;
					if (program_[iptr] == '[') ++nest;
					if (program_[iptr] == ']') --nest;
				}
			}
			break;
		case ']':
			if (*ptr != 0) {
				--iptr;
				for (int nest = 0; iptr != 0; --iptr) {
					if (program_[iptr] == '[' && nest == 0) break;
					if (program_[iptr] == '[') --nest;
					if (program_[iptr] == ']') ++nest;
				}
				// If program_[0] == '[' then the matching ] character will not be
				// visited and so this check will only be true for badly matched
				// []-pairs.
				if (iptr == 0) iptr = program_.size();
			}
			break;
		}
		++iptr;
	}
}

}  // namespace beef
