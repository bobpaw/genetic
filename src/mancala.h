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

private:
	int move_pieces(int position) {
		int initial_hand = board[position];
		board[position] = 0;
		for (int i = 0; i < initial_hand; ++i) {
			if (position % 14 == (player ? 7 : 0))  // i.e. not their pocket
				++position;

			++position;
			++board[position % 14];
		}
		return position % 14;
	}

public:
	Board(bool p, bool m): player(p), mode(m) {}
	Board(): Board(false, false) {}

	const int& operator[](int n) const { return board[n]; }
	int& operator[](int n) { return board[n]; }

	const int& move(int position) {
		if (mode) {
			// Play capture mancala

			int position = move_pieces(position);

			if (position == (player ? 0 : 7))
				;  // Keep same player
			else if (board[position] == 1 && player == position > 7) {
				board[0] += board[14 - position];
				board[14 - position] = 0;
				player = !player;
			}
		} else {
			// Play avalanche mancala

			while (true) {
				position = move_pieces(position);

				if (position == (player ? 0 : 7))
					break;  // Same player
				else if (board[position] == 1) {
					player = !player;
					break;
				}
			}
		}
		return board[player ? 0 : 7];
	}

};  // class Board
}  // namespace mancala