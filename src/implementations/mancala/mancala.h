#include <array>
#include <iostream>

#ifndef GENETIC_MANCALA_H_
#define GENETIC_MANCALA_H_

namespace mancala {

enum Mode : bool { Avalanche = false, Capture = true };
struct Board {
	std::array<int, 14> board = {{0, 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4}};
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
	bool over() const;
	void reset ();

protected:
	virtual int move_pieces(int position);

public:
	Board(bool m, bool p): player(p), mode(m) {}
	Board(bool m): Board(m, false) {}
	Board(): Board(false, false) {}
	virtual ~Board() {}

	const int& operator[](int n) const { return board[n]; }
	int& operator[](int n) { return board[n]; }

	const int& move(int position);

	friend std::ostream& operator<<(std::ostream&, const Board&);

};  // class Board
}  // namespace mancala

#endif // GENETIC_MANCALA_H_
