#include "ANSI_colors.h"
#include "engine.h"
#include "stdio.h"
#include <assert.h>
#include "string.h"

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>

#else

#include <unistd.h>

#endif

#define FIVE_LINES "\n\n\n\n\n"
#define FRAMES_PER_SECOND 2
#define clear() printf("\033[H\033[J")

void clear_screen() {
    puts(FIVE_LINES);
#ifdef _WIN32
    // system("cls");
#else
    clear();
#endif
}

void vertical_border(int len) {
    assert(len > 0);
    char buf[len + 2 + 1];
    memset(buf, '#', len + 2);
    buf[len + 2] = 0;
    printf("%s\n", buf);
}

void print_board(struct GameState *gs, enum Team player_team, struct Location cursor, bool c_visible) {
    vertical_border(BOARD_WIDTH);
    for (int yy = 0; yy < BOARD_HEIGHT; yy++) {
        printf("#");
        for (int x = 0; x < BOARD_WIDTH; x++) {
            // flip board
            int y = yy;
            bool found_piece = false;
            for (int i = 0; i < gs->pieces_length && !found_piece; i++) {
                struct Piece p = gs->pieces[i];
                if (c_visible && cursor.x == x && cursor.y == y) {
                    printf(YEL "o");
                    found_piece=true;
                } else {
                    if (p.location.x == x && p.location.y == y) {
                        found_piece = true;
                        p.team == RED ? printf(BRED) : printf(BBLU);
                        switch (p.type) {
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
            if (!found_piece) printf(" ");
        }
        printf("#\n");
    }
    vertical_border(BOARD_WIDTH);
}

bool move_cursor(struct Location *loc, int control) {
    switch (control) {
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

int main(void) {
    struct GameState gs = game_state_init();
    struct Location cursor = {0, 0};
    enum Team player_team;
    bool cursor_visible = false;
    int c;
    do {
        cursor_visible = !cursor_visible;
        switch (gs.screen) {
            case MAIN_MENU:
                puts("MAIN_MENU");
                puts("Press R or B to select team");
                if (c == 'r' || c == 'b') {
                    player_team = c == 'r' ? RED : BLUE;
                    gs.screen = PLACE_FLAG;
                }
                break;
            case PLACE_FLAG:
                puts("PLACE_FLAG\n");
                print_board(&gs, player_team, cursor, cursor_visible);
                puts("\nMove with WASD! Place flag with F!");
                if(!move_cursor(&cursor, c) && c == 'f'){
                    for(int i=0;i<gs.pieces_length;i++){
                        if(cursor.x==gs.pieces[i].location.x && cursor.y==gs.pieces[i].location.y && gs.pieces[i].team==player_team){
                            gs.pieces[i].is_flag=true;
                            gs.screen=PLACE_TRAP;
                            break;
                        }
                    }
                }
                break;
            case PLACE_TRAP:
                puts("PLACE_TRAP\n");
                print_board(&gs, player_team, cursor, cursor_visible);
                puts("\nMove with WASD! Place trap with F!");
                if(!move_cursor(&cursor, c) && c == 'f'){
                    for(int i=0;i<gs.pieces_length;i++){
                        if(cursor.x==gs.pieces[i].location.x && cursor.y==gs.pieces[i].location.y && gs.pieces[i].team==player_team && !gs.pieces[i].is_flag){
                            gs.pieces[i].type=TRAP_INVISIBLE;
                            gs.screen=GAME;
                            break;
                        }
                    }
                }
                break;
            case GAME:
                puts("GAME\n");
                break;
            case PICK_TYPE:
                puts("PICK_TYPE\n");
                break;
            case GAME_OVER:
                puts("GAME_OVER\n");
                break;
        };
        sleep(0.1);
        c = getchar();
        clear_screen();
    } while (c != 'q');
    puts("\nBye!\n");
    return 0;
}