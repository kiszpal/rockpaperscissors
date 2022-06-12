//
// Created by sudta on 6/6/2022.
//

#ifndef ROCK_PAPER_SCISSORS_ENGINE_ENGINE_H_
#define ROCK_PAPER_SCISSORS_ENGINE_ENGINE_H_

#include <stdbool.h>
#include <stdlib.h>

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

enum Team
{
  RED,
  BLUE,
};
enum Type
{
  ROCK,
  PAPER,
  SCISSORS,
  NONE,
  TRAP,
  TRAP_INVISIBLE
};
enum Screen
{
  MAIN_MENU,
  PLACE_FLAG,
  PLACE_TRAP,
  GAME,
  MOVE_PIECE,
  PICK_TYPE,
  GAME_OVER,
};
enum FightResult
{
  ATTACKER_WIN,
  DEFENDER_WIN,
  DRAW,
};
struct Location
{
  int x;
  int y;
};
struct Piece
{
  struct Location location;
  enum Team team;
  enum Type type;
  bool is_flag;
  bool visible;
};
struct GameState
{
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

bool is_trap(enum Type type);

enum Type random_type();

void place_ai_flag(struct GameState *gs, enum Team team);

void place_ai_trap(struct GameState *gs, enum Team team);

#endif // ROCK_PAPER_SCISSORS_ENGINE_ENGINE_H_
