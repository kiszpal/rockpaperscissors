//
// Created by sudta on 6/6/2022.
//

#include <assert.h>
#include <stdlib.h>
#include "engine.h"

struct Location location_init(int x, int y) {
  assert(x >= 0 && x < BOARD_WIDTH);
  assert(y >= 0 && y < BOARD_HEIGHT);
  struct Location location;
  location.x = x;
  location.y = y;
  return location;
}

enum FightResult fight(enum Type attacker, enum Type defender) {
  if (attacker == NONE || defender == NONE || attacker == defender)
    return Tie;
  else
    return (attacker == ROCK && defender == PAPER) || (attacker == PAPER && defender == SCISSORS)
               || (attacker == SCISSORS && defender == ROCK) ? Lose : Win;
}

bool remove_piece(struct GameState *game_state, int idx) {
  assert(idx >= 0 && idx < game_state->pieces_length);
  // replace [idx] with [last piece]
  game_state->pieces[idx] = game_state->pieces[game_state->pieces_length - 1];
  game_state->pieces_length--;
  return true;
}

struct Piece piece_init(struct Location location, enum Team team) {
  struct Piece piece;
  piece.location = location;
  piece.team = team;
  piece.type = NONE;
  piece.is_flag = false;
  return piece;
}

struct GameState game_state_init() {
  struct GameState game_state;
  const int PIECE_PER_PLAYER = BOARD_WIDTH * 2;
  game_state.pieces_length = PIECE_PER_PLAYER * 2;
  for (int i = 0; i < PIECE_PER_PLAYER; i++) {
    game_state.pieces[i] = piece_init(location_init(i % BOARD_WIDTH, i / BOARD_WIDTH), BLUE);
    game_state.pieces[PIECE_PER_PLAYER + i] =
        piece_init(location_init(i % BOARD_WIDTH, BOARD_HEIGHT - 1 - i / BOARD_WIDTH), RED);
  }
  game_state.screen = MAIN_MENU;
  game_state.red_trap_visible = false;
  game_state.blue_trap_visible = false;
  return game_state;
}

bool location_eq(struct Location l1, struct Location l2) {
  return l1.x == l2.x && l1.y == l2.y;
}