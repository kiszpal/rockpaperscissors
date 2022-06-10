#include "ANSI_colors.h"
#include "engine.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#else
#include <curses.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
#endif

void clear_screen()
{
  printf("\033[H\033[J");
  // clear();
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void piece_print(struct Piece p)
{
  printf("Piece { team: \"%s\", location: { %d, %d }, type: %d }",
         p.team == RED ? "RED" : "BLUE",
         p.location.x,
         p.location.y,
         p.type);
}

void vertical_border(int len)
{
  assert(len > 0);
  char buf[len + 2 + 1];
  memset(buf, '#', len + 2);
  buf[len + 2] = 0;
  printf("%s\n", buf);
}

void print_board(struct GameState *gs, struct Location cursor, bool c_visible, int selected_piece)
{
  vertical_border(BOARD_WIDTH);
  for (int y = 0; y < BOARD_HEIGHT; y++)
  {
    printf("#");
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
      bool found_piece = false;
      for (int i = 0; i < gs->pieces_length && !found_piece; i++)
      {
        struct Piece p = gs->pieces[i];
        if (c_visible && cursor.x == x && cursor.y == y)
        {
          printf(YEL "o");
          found_piece = true;
        }
        else
        {
          if (p.location.x == x && p.location.y == y)
          {
            found_piece = true;
            printf(i == selected_piece ? YEL : p.team == RED ? BRED
                                                             : BBLU);
            switch (p.type)
            {
            case ROCK:
              printf("R");
              break;
            case PAPER:
              printf("P");
              break;
            case SCISSORS:
              printf("S");
              break;
            case TRAP:
              printf("X");
              break;
            case TRAP_INVISIBLE:
            case NONE:
              printf(".");
              break;
            }
          }
        }
        printf(reset);
      }
      if (!found_piece)
        printf(" ");
    }
    printf("#\n");
  }
  vertical_border(BOARD_WIDTH);
}

bool move_cursor(struct Location *loc, int control)
{
  switch (control)
  {
  case 'a':
  case 'A':
    loc->x = max(loc->x - 1, 0);
    return true;
  case 'w':
  case 'W':
    loc->y = max(loc->y - 1, 0);
    return true;
  case 's':
  case 'S':
    loc->y = min(loc->y + 1, BOARD_HEIGHT - 1);
    return true;
  case 'd':
  case 'D':
    loc->x = min(loc->x + 1, BOARD_WIDTH - 1);
    return true;
  default:
    return false;
  }
}

int main(void)
{
  int type_picked = 0;
  /*
   - 0 when inactive,
   - 1 when picked in prev iter
   - 2 when picked in current iter
   */
  bool game_over_win = false;
  struct GameState gs = game_state_init();
  struct Location cursor = {0, 0};
  enum Team player_team;
  int selected_piece = -1;
  bool cursor_visible = false;
  int c;
  do
  {
    cursor_visible = !cursor_visible;
    if (type_picked != 0)
      type_picked -= 1;

    if (gs.screen != MAIN_MENU)
      puts(player_team == RED ? BRED "RED's turn" reset : BBLU "BLUE's turn" reset);
    switch (gs.screen)
    {
    case MAIN_MENU:
    {
      puts("MAIN_MENU");
      puts("Press R or B to select team");
      if (c == 'r' || c == 'b')
      {
        if (c == 'r')
        {
          player_team = RED;
          cursor.y = BOARD_HEIGHT - 1;
        }
        else
          player_team = BLUE;
        gs.screen = PLACE_FLAG;
      }
      break;
    }
    case PLACE_FLAG:
    {
      puts("PLACE_FLAG");
      puts("Move with WASD! Place flag with F!");
      print_board(&gs, cursor, cursor_visible, selected_piece);
      if (!move_cursor(&cursor, c) && c == 'f')
      {
        for (int i = 0; i < gs.pieces_length; i++)
        {
          if (location_eq(cursor, gs.pieces[i].location) && gs.pieces[i].team == player_team)
          {
            gs.pieces[i].is_flag = true;
            gs.screen = PLACE_TRAP;
            break;
          }
        }
      }
      break;
    }
    case PLACE_TRAP:
    {
      puts("PLACE_TRAP\n");
      puts("Move with WASD! Place trap with F!");
      print_board(&gs, cursor, cursor_visible, selected_piece);
      if (!move_cursor(&cursor, c) && c == 'f')
      {
        for (int i = 0; i < gs.pieces_length; i++)
        {
          if (location_eq(cursor, gs.pieces[i].location) && gs.pieces[i].team == player_team && !gs.pieces[i].is_flag)
          {
            gs.pieces[i].type = TRAP_INVISIBLE;
            gs.screen = GAME;
            break;
          }
        }
      }
      break;
    }
    case GAME:
    {
      puts("GAME\n");
      puts("Move with WASD! Select piece with F!");
      selected_piece = -1;

      print_board(&gs, cursor, cursor_visible, selected_piece);
      if (!move_cursor(&cursor, c) && c == 'f')
      {
        for (int i = 0; i < gs.pieces_length; i++)
        {
          if (location_eq(cursor, gs.pieces[i].location) && gs.pieces[i].team == player_team && !is_trap(gs.pieces[i].type))
          {
            selected_piece = i;
            gs.screen = MOVE_PIECE;
            break;
          }
        }
      }
      break;
    }
    case MOVE_PIECE:
    {
      assert(selected_piece != -1);
      puts("MOVE_PIECE\n");
      puts("Move with WASD! Move piece with F!");
      print_board(&gs, cursor, cursor_visible, selected_piece);
      if (!move_cursor(&cursor, c) && c == 'f' || type_picked == 1)
      {
        {
          int dx = abs(gs.pieces[selected_piece].location.x - cursor.x);
          int dy = abs(gs.pieces[selected_piece].location.y - cursor.y);
          if (dx > 1 || dy > 1 || (dx + dy == 0))
          {
            puts("Invalid move!");
            break;
          }
        } // move distance

        bool occupied = false;
        bool invalid = false;
        for (int i = 0; i < gs.pieces_length; i++)
        {
          if (i == selected_piece)
            continue;
          if (location_eq(gs.pieces[i].location, cursor))
          {
            occupied = true;
            invalid = gs.pieces[selected_piece].team == gs.pieces[i].team;
            break;
          }
        }

        if (invalid)
        {
          puts("Invalid move!");
          break;
        }

        if (!occupied)
        {
          // move
          gs.pieces[selected_piece].location = cursor;
        }
        else
        {
          // fight

          for (int def_id = 0; def_id < gs.pieces_length; def_id++)
          {
            if (!location_eq(cursor, gs.pieces[def_id].location))
              continue;
            // found the enemy
            piece_print(gs.pieces[def_id]);
            piece_print(gs.pieces[selected_piece]);
            puts("");

            if (gs.pieces[def_id].type == TRAP || gs.pieces[def_id].type == TRAP_INVISIBLE)
            {
              gs.pieces[def_id].type = TRAP;
              // def won, delete attacker
              remove_piece(&gs, selected_piece);
              selected_piece = -1;
              gs.screen = GAME;
              break;
            }
            else
            {
              if (gs.pieces[def_id].type == NONE || type_picked == 1)
                gs.pieces[def_id].type = random_type();
              enum FightResult result = fight(gs.pieces[selected_piece].type, gs.pieces[def_id].type);
              if (result == ATTACKER_WIN)
              {
                gs.pieces[selected_piece].location = cursor;
                selected_piece = -1;
                if (gs.pieces[def_id].is_flag)
                {
                  game_over_win = true;
                  gs.screen = GAME_OVER;
                }
                else
                  gs.screen = GAME;
                remove_piece(&gs, def_id);
                break;
              }
              else if (result == DEFENDER_WIN)
              {
                if (gs.pieces[selected_piece].is_flag)
                {
                  game_over_win = false;
                  gs.screen = GAME_OVER;
                }
                else
                {
                  gs.screen = GAME;
                }
                remove_piece(&gs, selected_piece);
                selected_piece = -1;
                break;
              }
              else
              {
                assert(result == DRAW);
                gs.screen = PICK_TYPE;
                break;
              }
            }
          }
        }
      }
      break;
    }
    case PICK_TYPE:
    {
      assert(selected_piece != -1);
      assert(!is_trap(gs.pieces[selected_piece].type));
      puts("PICK_TYPE");
      puts("PRESS A: rock, S: paper, D: scissors");
      switch (c)
      {
      case 'a':
      case 'A':
        gs.pieces[selected_piece].type = ROCK;
        type_picked = 2;
        gs.screen = MOVE_PIECE;
        break;
      case 's':
      case 'S':
        gs.pieces[selected_piece].type = PAPER;
        type_picked = 2;
        gs.screen = MOVE_PIECE;
      case 'd':
      case 'D':
        gs.pieces[selected_piece].type = SCISSORS;
        type_picked = 2;
        gs.screen = MOVE_PIECE;
      default:
        break;
      }

      break;
    }
    case GAME_OVER:
    {
      puts("GAME_OVER\n");
      break;
    }
    };
    puts("");
    usleep(100 * 1000);
    if (kbhit())
      c = getchar();
    else
      c = 0;
    clear_screen();
  } while (c != 'q');
  puts("\nBye!\n");
  return 0;
}