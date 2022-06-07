//
// Created by sudta on 6/6/2022.
//

#ifndef ROCK_PAPER_SCISSORS_ENGINE_ENGINE_H_
#define ROCK_PAPER_SCISSORS_ENGINE_ENGINE_H_

#include <stdbool.h>

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

enum Team {
  RED, BLUE,
};
enum Type {
  ROCK, PAPER, SCISSORS, NONE, TRAP, TRAP_INVISIBLE
};
enum Screen {
  MAIN_MENU, PLACE_FLAG, PLACE_TRAP, GAME, PICK_TYPE, GAME_OVER,
};
enum FightResult {
  Win, Lose, Tie,
};
struct Location {
  int x;
  int y;
};
struct Piece {
  struct Location location;
  enum Team team;
  enum Type type;
  bool is_flag;
};
struct GameState {
  struct Piece pieces[BOARD_WIDTH * BOARD_HEIGHT];
  int pieces_length;
  enum Screen screen;
  bool red_trap_visible;
  bool blue_trap_visible;
};

bool remove_piece(struct GameState *game_state, int idx);
bool location_eq(struct Location l1, struct Location l2);

struct Location location_init(int x, int y);

enum FightResult fight(enum Type attacker, enum Type defender);

struct GameState game_state_init();

#endif //ROCK_PAPER_SCISSORS_ENGINE_ENGINE_H_
