#include <algorithm>
#include <iomanip>

#include "mancala.h"

namespace mancala {
bool Board::over() const {
	return std::count(board.cbegin() + 1, board.cbegin() + 7, 0) == 6 ||
				 std::count(board.cbegin() + 8, board.cend(), 0) == 6;
}
void Board::reset() { board = {{0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4}}; }
int Board::move_pieces(int position) {
	int initial_hand = board[position];
	board[position] = 0;
	for (int i = 0; i < initial_hand; ++i) {
		++position;
		if (position % 14 == (player ? 7 : 0))  // i.e. not their pocket
			++position;
		++board[position % 14];
	}
	return position % 14;
}
const int& Board::move(int position) {
	if (board[position] == 0) return board[player ? 0 : 7];
	if (mode) {
		// Play capture mancala

		position = move_pieces(position);

		if (position == (player ? 0 : 7))
			player = !player;  // Keep same player
		else if (board[position] == 1 && board[14 - position] != 0 &&
						 player == (position > 7)) {
			board[player ? 0 : 7] += board[14 - position] + 1;
			board[14 - position] = board[position] = 0;
		}
		if (std::all_of(board.cbegin() + 8, board.cbegin() + 14,
										[](int a) { return a == 0; })) {
			for (int i = 1; i < 7; ++i) {
				board[7] += board[i];
				board[i] = 0;
			}
		} else if (std::all_of(board.cbegin() + 1, board.cbegin() + 7,
													 [](int a) { return a == 0; })) {
			for (int i = 8; i < 14; ++i) {
				board[0] += board[i];
				board[i] = 0;
			}
		}
	} else {
		// Play avalanche mancala

		while (true) {
			position = move_pieces(position);

			if (position == (player ? 0 : 7)) {
				player = !player;  // Same player
				break;
			} else if (board[position] == 1)
				break;
		}
	}
	player = !player;
	return board[player ? 0 : 7];
}
}  // namespace mancala

std::ostream& operator<<(std::ostream& out, const mancala::Board& b) {
	out << "  " << std::setfill('0') << std::setw(2) << b[0] << std::endl;
	for (int i = 1; i < 7; ++i)
		out << std::setw(2) << b[i] << "  " << std::setw(2) << b[14 - i]
				<< std::endl;
	out << "  " << std::setw(2) << b[7] << std::endl;
	return out;
}