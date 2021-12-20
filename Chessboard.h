#pragma once

#include "Chesspiece.h";

class Chesspiece;

// used for game_over state
enum class GameState { PLAY_ON, BLACK_LOST, WHITE_LOST };

struct Position {
  int row;
  int col;
};

class Chessboard {
private:
  int size;
  bool whites_turn = true;
  bool use_utf8;
  Position* selected;
  Chesspiece** chesspieces;

  inline int mapUserRow(int row) const;
  inline int mapUserCol(int col) const;
  int at(int row, int col) const { return col * get_size() + row; }
  int userAt(int row, int col) const {
    return mapUserCol(col) * get_size() + mapUserRow(row);
  }
  const Chesspiece* get_selected_chesspiece() const;
  void place_figures();

public:
  Chessboard() = delete;
  Chessboard(bool use_utf8 = false, int size = 8);
  ~Chessboard();
  bool is_whites_turn() const { return whites_turn; };
  GameState is_game_over() const;
  int get_size() const { return size; }
  const Chesspiece* operator()(int row, int col) const;

  bool can_pass_over(int row, int col) const;
  bool can_land_on(int row, int col, bool is_white) const;
  bool can_capture_on(int row, int col, bool is_white) const;

  bool can_select_piece(int row, int col) const;
  bool can_move_selection_to(int row, int col) const;
  bool can_move(int from_row, int from_col, int to_row, int to_col) const;

  void select_piece(int row, int col);
  void move_selection_to(int row, int col);
  void show() const;
};
