#include "Chessboard.h"

#include <iostream>
#include <iomanip> // for cout setw

using std::cout;
using std::endl;

//#define DEBUGOUTPUT true
#define DEBUG(X) cout << std::boolalpha << (#X) << " = " << (X) << endl

Chessboard::Chessboard(bool use_utf8, int size)
  : size(size),
  use_utf8(use_utf8),
  selected(nullptr),
  chesspieces(new Chesspiece* [size * size]()) {
  if (size < 8 || size > 26) {
    std::cerr << "Chessboard must have a size of at least 8 and maximum of 26."
      << std::endl;
    exit(-1);
  }

  place_figures();
}

Chessboard::~Chessboard() {
  if (chesspieces != nullptr) {
    for (size_t i = 0; i < size * size; i++)
    {
      Chesspiece* cp = chesspieces[i];
      if (cp != nullptr) {
        delete cp;
      }
    }
    delete[] chesspieces;
    chesspieces = nullptr;
  }
}
/// <summary>
/// maps a user inputed row (A-Z) to our internal representation (0-25)
/// </summary>
int Chessboard::mapUserRow(int row) const {
  if (row > get_size()) {  // if alphabetic selection
    row = std::toupper(row);
    row -= 'A';
    return row;
  }
  return row--;
}

/// <summary>
/// maps a user inputed column (26-1) to our internal representation (0-25)
/// </summary>
int Chessboard::mapUserCol(int col) const { return get_size() - col; }

GameState Chessboard::is_game_over() const {
  size_t black_essential = 0;
  size_t white_essential = 0;
  for (size_t i = 0; i < size * size; i++)
  {
    Chesspiece* cp = chesspieces[i];
    if (cp != nullptr) {
      if (cp->is_essential()) {
        if (cp->is_white()) {
          white_essential++;
        }
        else {
          black_essential++;
        }
      }
    }
  }
  if (black_essential == 0) {
    return GameState::BLACK_LOST;
  }
  else if (white_essential == 0) {
    return GameState::WHITE_LOST;
  }
  return GameState::PLAY_ON;
}

/// <summary>
/// gets the chesspiece on the given row/col square
/// </summary>
const Chesspiece* Chessboard::operator()(int row, int col) const {
  return chesspieces[at(row, col)];
}

bool Chessboard::can_pass_over(int row, int col) const {
  return (*this)(row, col) == nullptr;
}

bool Chessboard::can_land_on(int row, int col, bool is_white) const {
  return can_pass_over(row, col) || can_capture_on(row, col, is_white);
}

const Chesspiece* Chessboard::get_selected_chesspiece() const {
  if (selected == nullptr) {
    return nullptr;
  }
  return (*this)(selected->row, selected->col);
}

void Chessboard::place_figures() {
  int top_row = get_size();

  Rook* rook_l_white = new Rook{ true };
  Rook* rook_r_white = new Rook{ true };
  chesspieces[userAt('A', 1)] = rook_l_white;
  chesspieces[userAt('H', 1)] = rook_r_white;
  Rook* rook_l_black = new Rook{ false };
  Rook* rook_r_black = new Rook{ false };
  chesspieces[userAt('A', top_row)] = rook_l_black;
  chesspieces[userAt('H', top_row)] = rook_r_black;

  Knight* knight_l_white = new Knight{ true };
  Knight* knight_r_white = new Knight{ true };
  chesspieces[userAt('B', 1)] = knight_l_white;
  chesspieces[userAt('G', 1)] = knight_r_white;
  Knight* knight_l_black = new Knight{ false };
  Knight* knight_r_black = new Knight{ false };
  chesspieces[userAt('B', top_row)] = knight_l_black;
  chesspieces[userAt('G', top_row)] = knight_r_black;

  Bishop* bishop_l_white = new Bishop{ true };
  Bishop* bishop_r_white = new Bishop{ true };
  chesspieces[userAt('C', 1)] = bishop_l_white;
  chesspieces[userAt('F', 1)] = bishop_r_white;
  Bishop* bishop_l_black = new Bishop{ false };
  Bishop* bishop_r_black = new Bishop{ false };
  chesspieces[userAt('C', top_row)] = bishop_l_black;
  chesspieces[userAt('F', top_row)] = bishop_r_black;

  Queen* queen_white = new Queen{ true };
  chesspieces[userAt('D', 1)] = queen_white;
  Queen* queen_black = new Queen{ false };
  chesspieces[userAt('D', top_row)] = queen_black;

  King* king_white = new King{ true };
  chesspieces[userAt('E', 1)] = king_white;
  King* king_black = new King{ false };
  chesspieces[userAt('E', top_row)] = king_black;

  for (int i = 'A'; i < size + 'A'; i++) {
    Pawn* pawn_white = new Pawn{ true };
    chesspieces[userAt(i, 2)] = pawn_white;
    Pawn* pawn_black = new Pawn{ false };
    chesspieces[userAt(i, top_row - 1)] = pawn_black;
  }

  // SPECIAL FIGURES
  //Hopper* hopper_white = new Hopper{ true };
  //chesspieces[userAt('C', 5)] = hopper_white;
  //Quadrilateral* quadrilateral_white = new Quadrilateral{ true };
  //chesspieces[userAt('E', 5)] = quadrilateral_white;
}

bool Chessboard::can_capture_on(int row, int col, bool is_white) const {
  const Chesspiece* cp = (*this)(row, col);
  if (cp == nullptr || cp->is_white() == is_white) {
    return false;
  }
  return true;
}

bool Chessboard::can_select_piece(int row, int col) const {
  int user_row = mapUserRow(row);
  int user_col = mapUserCol(col);
  const Chesspiece* cp = (*this)(user_row, user_col);
  // check if there is a figure on this square
  if (cp == nullptr) {
    return false;
  }
  // check if color of figure on square is on turn
  if (is_whites_turn() != cp->is_white()) {
    return false;
  }

  // check if figure can move at all
  for (size_t to_row = 0; to_row < get_size(); to_row++) {
    for (size_t to_col = 0; to_col < get_size(); to_col++) {
      if (cp->can_move(user_row, user_col, to_row, to_col, *this)) {
        return true;
      }
    }
  }
  return false;
}

bool Chessboard::can_move_selection_to(int row, int col) const {
  if (selected == nullptr) {
    return false;
  }
  return can_move(selected->row, selected->col, row, col);
}

bool Chessboard::can_move(int from_row, int from_col, int to_row,
  int to_col) const {
  const Chesspiece* cp = get_selected_chesspiece();
  if (cp == nullptr) {
    return false;
  }
  return cp->can_move(from_row, from_col, mapUserRow(to_row), mapUserCol(to_col), *this);
}

void Chessboard::select_piece(int row, int col) {
  if (!can_select_piece(row, col)) {
    return;
  }
  if (selected != nullptr) {
    delete selected;
    selected = nullptr;
  }
  row = mapUserRow(row);
  col = mapUserCol(col);
  if ((*this)(row, col) != nullptr) {
    selected = new Position;
    selected->row = row;
    selected->col = col;
  }
}

void Chessboard::move_selection_to(int row, int col) {
  if (selected == nullptr) {
    return;
  }
  if (!can_move_selection_to(row, col)) {
    return;
  }

  // must implement it another time because () operator gets const value
  Chesspiece* sel_cp = chesspieces[at(selected->row, selected->col)];
  if (sel_cp == nullptr) {
    return;
  }
  // remove from current square
  chesspieces[at(selected->row, selected->col)] = nullptr;

  // check for figure that was previously there, delete it if applicable
  Chesspiece* previous = chesspieces[userAt(row, col)];
  if (previous != nullptr) {
    delete previous;
  }

  // place to new square
  chesspieces[userAt(row, col)] = sel_cp;

  whites_turn = !whites_turn;
  delete selected;
  selected = nullptr;
}

/// <summary>
/// helper function to draw the header (A B C ...)
/// </summary>
static void draw_header(int size) {
  cout << "    ";
  if (size > 9) { // if there is a number > 9 on the left, add some spacing
    cout << ' ';
  }
  for (short i = 'A'; i < 'A' + size; i++) {
#ifdef DEBUGOUTPUT
    cout << ' ' << i - 'A' << ' ';
#else
    cout << ' ' << char(i) << ' ';
#endif  // DEBUG
  }
  cout << endl;
}

/// <summary>
/// helper function to draw a horizontal line (for top and bottom of board)
/// </summary>
static void draw_hr(int size) {
  cout << "   ";
  if (size > 9) { // if there is a number > 9 on the left, add some spacing
    cout << ' ';
  }
  for (size_t i = 0; i < size; i++) {
    cout << "---";
  }
  cout << "--" << endl;
}

void Chessboard::show() const {
  draw_header(size);
  draw_hr(size);
  const Chesspiece* sel_cp = get_selected_chesspiece();
  // draw row number
  for (size_t col = 0; col < get_size(); col++) {
    std::streamsize width = 1;
    if (size > 9) { // if there is a number > 9 on the left, add some spacing
      width = 2;
    }
#ifdef DEBUGOUTPUT
    cout << ' ' << std::setw(width) << col << ' ' << '|';
#else
    cout << ' ' << std::setw(width) << mapUserCol(col) << ' ' << '|';
#endif  // DEBUG
    // draw row of board
    for (size_t row = 0; row < get_size(); row++) {
      char opening_char = ' ';
      char closing_char = ' ';
      if (sel_cp != nullptr) {
        if (row == selected->row && col == selected->col) { // selected figure gets "highlighted"
          opening_char = '(';
          closing_char = ')';
        }
        else if (sel_cp->can_move(selected->row, selected->col, row, col, *this)) { // selectable squares get "highlighted"
          opening_char = '[';
          closing_char = ']';
        }
      }
      const Chesspiece* p = (*this)(row, col);
      cout << opening_char;
      if (p != nullptr) {
        cout << p->get_symbol(use_utf8);
      }
      else {
        cout << ".";
      }
      cout << closing_char;
    }
    cout << '|' << endl;
  }
  draw_hr(size);
}
