#include <conio.h>
#ifdef _WIN32
#include <Windows.h>
#include <assert.h>
#else
#include <unistd.h>
#endif
#include "stdio.h"
#include "engine.h"

#define FIVE_LINES "\n\n\n\n\n"
#define FRAMES_PER_SECOND 2

void clear_screen() {
  puts("\n\n\n\n\n");
#ifdef _WIN32
  // system("cls");
#else
  system("clear");
#endif
}

void vertical_border(int len) {
  assert(len > 0);
  char buf[len + 2 + 1];
  memset(buf, '#', len + 2);
  buf[len + 2] = 0;
  printf("%s\n", buf);
}

void print_board(struct GameState *gs, enum Team player_team) {
  vertical_border(BOARD_WIDTH);
  for (int yy = 0; yy < BOARD_HEIGHT; yy++) {
    printf("#");
    for (int x = 0; x < BOARD_WIDTH; x++) {
      // flip board
      int y = player_team == RED ? yy : BOARD_HEIGHT - yy;
      bool found_piece = false;
      for (int i = 0; i < gs->pieces_length && !found_piece; i++) {
        struct Piece p = gs->pieces[i];
        if (p.location.x == x && p.location.y == y) {
          found_piece = true;
          // TODO: set color
          switch (p.type) {
            case ROCK: printf("R");
              break;
            case PAPER: printf("P");
              break;
            case SCISSORS: printf("S");
              break;
            case TRAP: printf("X");
              break;
            case TRAP_INVISIBLE:
            case NONE:printf(".");
              break;
          }
          // TODO: reset color
        }
      }
      if (!found_piece) printf(" ");
    }
    printf("#\n");
  }
  vertical_border(BOARD_WIDTH);
}

bool move_cursor(struct Location *loc, int control) {
  switch (control) {
    case 'a':
    case 'A':loc->x = max(loc->x - 1, 0);
      return true;
    case 'w':
    case 'W':loc->y = max(loc->y - 1, 0);
      return true;
    case 's':
    case 'S':loc->y = min(loc->y + 1, BOARD_HEIGHT - 1);
      return true;
    case 'd':
    case 'D':loc->x = min(loc->x + 1, BOARD_WIDTH - 1);
      return true;
    default:return false;
  }
}

int main(void) {
  struct GameState gs = game_state_init();
  struct Location cursor = {0, 0};
  enum Team player_team;
  int c;
  do {
    switch (gs.screen) {
      case MAIN_MENU:printf("MAIN_MENU\n");
        printf("Press R or B to select team\n");
        if (c == 'r' || c == 'b') {
          player_team = c == 'r' ? RED : BLUE;
          gs.screen = PLACE_FLAG;
        }
        break;
      case PLACE_FLAG:puts("PLACE_FLAG\n\n");
        print_board(&gs, player_team);
        break;
      case PLACE_TRAP:printf("PLACE_TRAP\n\n");
        break;
      case GAME:printf("GAME\n\n");
        break;
      case PICK_TYPE:printf("PICK_TYPE\n\n");
        break;
      case GAME_OVER:printf("GAME_OVER\n\n");
        break;
    };
    while (!kbhit()) usleep(1000000 / FRAMES_PER_SECOND);
    c = getch();
    clear_screen();
  } while (c != 'q');
  printf("\nBye!\n");
  return 0;
}