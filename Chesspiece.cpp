#include "Chesspiece.h"

#include <iostream>

#define DEBUG(X) cout << std::boolalpha << (#X) << " = " << (X) << endl

// if we want to display unicode characters, we need a mapping
std::map<std::string, const char*> utf8_symbol_map = {
  {
      "BQ",
      u8"\u2655",
  },
  {
      "BK",
      u8"\u2654",
  },
  {
      "BB",
      u8"\u2656",
  },
  {
      "BN",
      u8"\u2657",
  },
  {
      "BR",
      u8"\u2658",
  },
  {
      "BP",
      u8"\u265F",
  },
  {
      "WQ",
      u8"\u265B",
  },
  {
      "WK",
      u8"\u265A",
  },
  {
      "WB",
      u8"\u265C",
  },
  {
      "WN",
      u8"\u265D",
  },
  {
      "WR",
      u8"\u265E",
  },
  {
      "WP",
      u8"\u2659",
  }
};

#define DEBUG(exp) std::cout << std::boolalpha << (#exp) << " = " << (exp) << std::endl

#pragma region static_function_declarations

static void swap(int& first, int& second);

static bool rook_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb);

static bool bishop_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb);

static bool knight_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb);

static bool hopper_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb);

#pragma endregion static_function_declarations

const char* Chesspiece::get_symbol(bool use_utf8) const {
  // a special implementation to support utf8 characters
  if (use_utf8) {
    std::string symbol_color_id = get_color() + std::string(1, symbol);
    auto a = utf8_symbol_map.find(symbol_color_id);
    if (a != utf8_symbol_map.end()) {
      return utf8_symbol_map.at(symbol_color_id);
    }
  }

  // if no utf8 char found or if it is disabled -> show default ascii one
  char* symbol_arr = new char[2];
  symbol_arr[0] = is_white() ? symbol : std::tolower(symbol);
  symbol_arr[1] = '\0';
  return symbol_arr;
}

bool King::can_move(int from_row, int from_col, int to_row, int to_col,
  const Chessboard& cb) const {
  /*
   * One hop in all directions
   * [.][.][.]
   * [.] K [.]
   * [.][.][.]
   */
   // check final field
  if (!cb.can_land_on(to_row, to_col, is_white())) {
    return false;
  }

  int row_hop_diff = std::abs(from_row - to_row);
  int col_hop_diff = std::abs(from_col - to_col);
  // if hopping a row, col diff must be 0 or 1
  // if hopping in col, row diff must be 0
  if ((row_hop_diff == 1 && (col_hop_diff == 0 || col_hop_diff == 1)) ||
    (row_hop_diff == 0 && col_hop_diff == 1)) {
    return true;
  }
  return false;
}

bool Queen::can_move(int from_row, int from_col, int to_row, int to_col,
  const Chessboard& cb) const {
  /*
   * All directions
   * [\][|][/]
   * [-] Q [-]
   * [/][|][\]
   */
   // queen can make same moves as bishop and rook combined
  return bishop_can_move(from_row, from_col, to_row, to_col, is_white(), cb) ||
    rook_can_move(from_row, from_col, to_row, to_col, is_white(), cb);
}

bool Bishop::can_move(int from_row, int from_col, int to_row, int to_col,
  const Chessboard& cb) const {
  /*
   * Only diagonal
   * [\] . [/]
   *  .  B  .
   * [/] . [\]
   */
  return bishop_can_move(from_row, from_col, to_row, to_col, is_white(), cb);
}

bool Rook::can_move(int from_row, int from_col, int to_row, int to_col,
  const Chessboard& cb) const {
  /*
   * Horizontal and diagonal
   *  . [|] .
   * [-] R [-]
   *  . [|] .
   */
  return rook_can_move(from_row, from_col, to_row, to_col, is_white(), cb);
}

bool Knight::can_move(int from_row, int from_col, int to_row, int to_col,
  const Chessboard& cb) const {
  /*
   * Only in L(2x1) formations
   *  . [.] . [.] .
   * [.] .  .  . [.]
   *  .  .  N  .  .
   * [.] .  .  . [.]
   *  . [.] . [.] .
   */
  return knight_can_move(from_row, from_col, to_row, to_col, is_white(), cb);
}

bool Pawn::can_move(int from_row, int from_col, int to_row, int to_col,
  const Chessboard& cb) const {
  /*
   * Only forward; when at its initial position it can move two fields
   * Special care needs the capture part of the Pawn (as he can't capture in move direction)
   *  . [.] .
   *  . [.] .
   *  .  P  .
   */
   // check for move in row
  if (from_row == to_row) {
    // check ending field (no capture)
    if (!cb.can_pass_over(to_row, to_col)) {
      return false;
    }

    int size = cb.get_size();
    int diff = is_white() ? (int)-1 : 1;
    int initial_col = is_white() ? size - 2 : 1;
    int hop_diff = to_col - from_col;

    // check if one hop OR
    // if two hops AND initial pos AND pass over possible
    if (hop_diff != diff && !(hop_diff == diff * 2 && from_col == initial_col &&
      cb.can_pass_over(from_row, from_col + diff))) {
      return false;
    }
    return true;
  }
  else if (abs(from_row - to_row) == 1 && abs(from_col - to_col) == 1 &&
    cb.can_capture_on(to_row, to_col, is_white()))
  {
    // check vertical capture
    return true;
  }
  return false;
}

/* --------- SPECIAL CHESSPIECES --------- */

bool Hopper::can_move(int from_row, int from_col, int to_row, int to_col, const Chessboard& cb) const
{
  /*
   * Two hops horizontal or vertical
   *  .  . [.] .  .
   *  .  .  .  .  .
   * [.] .  H  . [.]
   *  .  .  .  .  .
   *  .  . [.] .  .
   */
  return hopper_can_move(from_row, from_col, to_row, to_col, is_white(), cb);
}

bool Quadrilateral::can_move(int from_row, int from_col, int to_row, int to_col, const Chessboard& cb) const
{
  /*
   * Hop like the king, but one wider
   *  . [.][.][.] .
   * [.] .  .  . [.]
   * [.] .  U  . [.]
   * [.] .  .  . [.]
   *  . [.][.][.] .
   */
   // quadrilateral can make same moves as knight and hopper combined
  return knight_can_move(from_row, from_col, to_row, to_col, is_white(), cb) ||
    hopper_can_move(from_row, from_col, to_row, to_col, is_white(), cb);
}

#pragma region static_function_definitions

static void swap(int& first, int& second) {
  int temp = first;
  first = second;
  second = temp;
}

static bool rook_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb) {
  // check diagonal move
  if (from_row != to_row && from_col != to_col) {
    return false;
  }

  // check final field
  if (!cb.can_land_on(to_row, to_col, is_white)) {
    return false;
  }

  // check waypoints
  if (from_row != to_row) {
    if (from_row > to_row) {
      swap(from_row, to_row);
    }
    // skip first square (where the figure stands)
    from_row++;
    for (int row = from_row; row < to_row; row++) {
      if (!cb.can_pass_over(row, from_col)) {
        return false;
      }
    }
  }
  else {
    if (from_col > to_col) {
      swap(from_col, to_col);
    }
    // skip first square (where the figure stands)
    from_col++;
    for (int col = from_col; col < to_col; col++) {
      if (!cb.can_pass_over(from_row, col)) {
        return false;
      }
    }
  }

  return true;
}

static bool bishop_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb) {
  // check final field
  if (!cb.can_land_on(to_row, to_col, is_white)) {
    return false;
  }

  int row_hop_diff = std::abs(from_row - to_row);
  int col_hop_diff = std::abs(from_col - to_col);
  // check if diagonal
  if (row_hop_diff != col_hop_diff) {
    return false;
  }
  int row = from_row, col = from_col;
  int row_step = from_row < to_row ? 1 : -1;
  int col_step = from_col < to_col ? 1 : -1;
  // do while because we want to skip first square (where the figure stands)
  do {
    row += row_step;
    col += col_step;
    // check if we are before the destination square and can pass over it
    if (row != to_row && !cb.can_pass_over(row, col)) {
      return false;
    }
  } while (row != to_row); // we only need to check one dimension
  return true;
}

static bool knight_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb) {
  // check final field
  if (!cb.can_land_on(to_row, to_col, is_white)) {
    return false;
  }

  int row_hop_diff = std::abs(from_row - to_row);
  int col_hop_diff = std::abs(from_col - to_col);
  // check if two hops in row/col and one hop in col/row
  if (row_hop_diff == 2 && col_hop_diff == 1 ||
    row_hop_diff == 1 && col_hop_diff == 2) {
    return true;
  }

  return false;
}

static bool hopper_can_move(int from_row, int from_col, int to_row, int to_col,
  bool is_white, const Chessboard& cb) {
  // check final field
  if (!cb.can_land_on(to_row, to_col, is_white)) {
    return false;
  }
  if (abs(from_row - to_row) == 2 && from_col == to_col) { // horizontal move
    return true;
  }
  else if (abs(from_col - to_col) == 2 && from_row == to_row) { // vertical move
    return true;
  }
  return false;
}

#pragma endregion static_function_definitions
