#include <array>
#include <vector>

namespace mancala {
class Board {
	std::array<int, 14> board{0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4};
	/*
	 * player true
	 *  0
	 * 1 13
	 * 2 12
	 * 3 11
	 * 4 10
	 * 5 9
	 * 6 8
	 *  7
	 * player false
	 *
	 */

	bool player{false};
	bool mode{false};  // false: capture true: avalanche

	const int& move(int position) {
		// FIXME: Use multiple if (mode) throughout instead of one
		if (mode) {
			// Play capture mancala

			int position_f = position;
			int initial_hand = board[position];
			board[position] = 0;
			for (int i = 1; i < initial_hand; ++i) {
				if (player && position + i == 7)
					continue;
				else if (!player && position + i == 0)
					continue;

				++position_f;
				++board[position + i];
			}

			if (position_f == (player ? 0 : 7)) {
				// Keep same player
				;
			} else if (board[position_f] == 1 && position_f % 7 != 0) {
				// FIXME: Make more compact
				if (player && position_f > 7) {
					board[0] += board[14 - position_f];
					board[14 - position_f] = 0;
				} else if (!player && position_f < 7) {
					board[7] += board[14 - position_f];
					board[14 - position_f] = 0;
				}
			}
		} else {
			// Play avalanche mancala

			int position_f;
			do {
				position_f = position;
				int initial_hand = board[position];
				board[position] = 0;
				for (int i = 1; i < initial_hand; ++i) {
					if (player && position + i == 7)
						continue;
					else if (!player && position + i == 0)
						continue;

					++position_f;
					++board[position + i];
				}

				if (position_f == (player ? 0 : 7)) break;
			} while (board[position_f] != 1);
		}
		return board[player ? 0 : 7];
	}

};  // class Board
}  // namespace mancala