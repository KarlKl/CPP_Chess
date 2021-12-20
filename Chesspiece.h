#pragma once

#include <locale>  // for tolower
#include <map>

#include "Chessboard.h"

class Chessboard;

class Chesspiece {
 private:
  bool white;
  char symbol;

 public:
  Chesspiece(char symbol, bool is_white) : symbol(symbol), white(is_white) {}
  virtual ~Chesspiece() { /* nothing to do here */ }

  const char *get_symbol(bool use_utf8) const;
  char get_color() const { return is_white() ? 'W' : 'B'; }
  bool is_white() const { return white; }
  virtual bool is_essential() const { return false; }
  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard &cb) const = 0;
};

class King : public Chesspiece {
 public:
  King(bool is_white) : Chesspiece('K', is_white) {}

  virtual bool is_essential() const override { return true; } // override to return true

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard &cb) const override;
};

class Queen : public Chesspiece {
 public:
  Queen(bool is_white) : Chesspiece('Q', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard &cb) const override;
};

class Bishop : public Chesspiece {
 public:
  Bishop(bool is_white) : Chesspiece('B', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard &cb) const override;
};

class Rook : public Chesspiece {
 public:
  Rook(bool is_white) : Chesspiece('R', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard &cb) const override;
};

class Knight : public Chesspiece {
 public:
  Knight(bool is_white) : Chesspiece('N', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard &cb) const override;
};

class Pawn : public Chesspiece {
public:
  Pawn(bool is_white) : Chesspiece('P', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard& cb) const override;
};

/* --------- SPECIAL CHESSPIECES --------- */
class Hopper : public Chesspiece {
public:
  Hopper(bool is_white) : Chesspiece('H', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard& cb) const override;
};

class Quadrilateral : public Chesspiece {
public:
  Quadrilateral(bool is_white) : Chesspiece('U', is_white) {}

  virtual bool can_move(int from_row, int from_col,  //
                        int to_row, int to_col,      //
                        const Chessboard& cb) const override;
};
