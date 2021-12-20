#include <Windows.h>

#include <iostream>

#include "Chessboard.h"
#include "Chesspiece.h"
#include "Colors.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

constexpr bool USE_UTF8 = false;

#define DEBUG(exp) cout << std::boolalpha << (#exp) << " = " << (exp) << endl

static string get_player_color(Chessboard* board) {
  return board->is_whites_turn() ? "white" : "black";
}
static std::tuple<int, int> user_select_square(int size) {
  string move;
  char row = -1;
  int col = -1;
  bool valid_move = false;
  bool exit_game = false;
  while (!valid_move && !exit_game) {
    cout << "Please enter your move in algebraic notation eg 'C3' (or 'q' to quit): ";
    cin >> move;
    char first = std::toupper(move.at(0));
    if (move.length() == 1 && (first == 'Q')) {
      exit_game = true;
    }
    else if (move.length() >= 2) {
      row = first;
      col = move.at(1) - '0';
      // special handling of moves on boards > 9
      if (size > 9 && move.length() > 2) {
        col *= 10;
        col += move.at(2) - '0';
      }
      // sanity check
      if ((row >= 'A' || row < size + 'A') && (col >= 1 || col <= size)) {
        valid_move = true;
      }
    }
  }
  return { row, col };
}

static bool select_piece(Chessboard* board) {
  std::tuple<int, int> move = user_select_square(board->get_size());
  int row = std::get<0>(move);
  int col = std::get<1>(move);
  if (row <= 0 || col <= 0) {
    return false;
  }
  if (board->can_select_piece(row, col)) {
    board->select_piece(row, col);
    board->show();
    return true;
  }
  else {
    return select_piece(board);
  }
}

static bool move_piece(Chessboard* board) {
  std::tuple<int, int> move = user_select_square(board->get_size());
  int row = std::get<0>(move);
  int col = std::get<1>(move);
  if (row <= 0 || col <= 0) {
    return false;
  }
  if (board->can_move_selection_to(row, col)) {
    board->move_selection_to(row, col);
    board->show();
    return true;
  }
  else {
    return move_piece(board);
  }
}

static void print_game_over(const Chessboard& board, int number_of_moves) {
  cout << BOLD;
  if (board.is_game_over() == GameState::WHITE_LOST) {
    cout << "White";
  }
  else {
    cout << "Black";
  }
  cout << RESET << " has one in " << number_of_moves << " moves." << endl;
}

static int random(int min, int max) //range : [min, max]
{
  return min + rand() % ((max + 1) - min);
}

void play_automatic_game() {
  Chessboard board = Chessboard(USE_UTF8);
  int number_of_moves = 0;
  char rand_row = 'A';
  int rand_col = 1;
  int board_size = board.get_size();
  while (board.is_game_over() == GameState::PLAY_ON) {
    while (!board.can_select_piece(rand_row, rand_col)) {
      rand_row = random(0, board_size - 1) + 'A';
      rand_col = random(1, board_size);
    }
    board.select_piece(rand_row, rand_col);

    while (!board.can_move_selection_to(rand_row, rand_col)) {
      rand_row = random(0, board_size - 1) + 'A';
      rand_col = random(1, board_size);
    }
    board.move_selection_to(rand_row, rand_col);
    //board->show();
    number_of_moves++;
  }
  board.show();
  print_game_over(&board, number_of_moves);
}

void play_manual_game() {
  Chessboard board = Chessboard(USE_UTF8);
  board.show();
  bool continue_game = true;
  int number_of_moves = 0;
  while (continue_game) {
    cout << "Player " << BOLD << get_player_color(&board) << RESET << ' '
      << "is on turn." << endl;
    continue_game = select_piece(&board);
    if (continue_game) {
      continue_game = move_piece(&board);
      number_of_moves++;
    }
    if (board.is_game_over() != GameState::PLAY_ON) {
      continue_game = false;
      print_game_over(&board, number_of_moves);
    }
  }
}

void play_game_from_args(int argc, char* argv[]) {
  Chessboard board = Chessboard(USE_UTF8);
  int number_of_moves = 0;
  for (size_t i = 1; i < argc; i++)
  {
    string move = string(argv[i]);
    // check format
    if (move.size() != 5 || move[2] != '-') {
      cout << "Invalid format (" << move << ").Please enter moves in the format 'e2-e4 c7-c5 ...'" << endl;
      break;
    }

    // parse "select"
    char row = std::toupper(move[0]);
    char col = move[1] - '0';
    // sanity check
    if (((row < 'A' || row > 'H') && (col < 1 || col > 8)) ||
      !board.can_select_piece(row, col)) {
      cout << "Invalid select detected (" << move << ")." << endl;
      break;
    }
    board.select_piece(row, col);

    // parse "move"
    row = std::toupper(move[3]);
    col = move[4] - '0';
    // sanity check
    if (((row < 'A' || row > 'H') && (col < 1 || col > 8)) ||
      !board.can_move_selection_to(row, col)) {
      cout << "Invalid move detected (" << move << ")." << endl;
      break;
    }
    board.move_selection_to(row, col);
    number_of_moves++;
  }
  board.show();
  if (board.is_game_over() != GameState::PLAY_ON) {
    print_game_over(&board, number_of_moves);
  }
}

int main(int argc, char* argv[]) {
  if (USE_UTF8) {
    SetConsoleOutputCP(CP_UTF8);
  }

  // if gameplay is given via console
  if (argc > 1) {
    play_game_from_args(argc, argv);
    return 0;
  }

  // select game type
  char game_type;
  cout << "Select game-type 'a'utomatic or 'm'anual: ";
  cin >> game_type;

  if (game_type == 'a') { // automatic: the game is played till the end by the computer
    srand(time(0));
    play_automatic_game();
  }
  else if (game_type == 'm') { // manual: the moves are all selected by the user(s)
    play_manual_game();
  }
  return 0;
}
