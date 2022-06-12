//
// Created by palkiszel on 2022.06.12..
//

#include "engine.h"
#include "ai.h"
#include "assert.h"

struct Piece random_piece(struct GameState *gs, enum Team player_team){
    enum Team ai_team;
    struct Piece* ai_pieces;
    player_team == BLUE ? (ai_team = BLUE) : (ai_team = RED);
    int i = (rand() % (gs->pieces_length + 1));
    if(gs->pieces[i].team == ai_team){

    }

}

bool ai_can_attack(struct GameState *gs, struct Piece aipiece, enum Team player_team){
    if(aipiece.type == TRAP || aipiece.team == TRAP_INVISIBLE) return false;
    if(ai_number_of_enemies(gs,aipiece,player_team)>0) return true;
    return false;
}

int ai_number_of_enemies(struct GameState *gs, struct Piece aipiece, enum Team player_team){
    int numofenemies = 0;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i==0 && j==0) continue;
            if(aipiece.location.x < 0 || aipiece.location.x >= BOARD_WIDTH || aipiece.location.y < 0 || aipiece.location.y >= BOARD_HEIGHT) continue;
            for(int k = 0; k < gs->pieces_length; k++){
                if(gs->pieces[k].location.x == aipiece.location.x && gs->pieces[k].location.y == aipiece.location.y){
                    if(gs->pieces[k].team == player_team) {
                        numofenemies++;
                    }
                }
            }
        }
    }
    return numofenemies;
}

void ai_attack(struct GameState *gs, struct Piece aipiece, enum Team player_team, struct Location cursor, bool game_over_win,int aiID){
    if(ai_number_of_enemies(gs,aipiece,player_team) ==1 ){
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                if(i==0 && j==0) continue;
                if(aipiece.location.x < 0 || aipiece.location.x >= BOARD_WIDTH || aipiece.location.y < 0 || aipiece.location.y >= BOARD_HEIGHT) continue;
                for(int k = 0; k < gs->pieces_length; k++){
                    if(gs->pieces[k].location.x == aipiece.location.x && gs->pieces[k].location.y == aipiece.location.y){
                        if(gs->pieces[k].team == player_team) {
                            if(gs->pieces[k].visible_for_ai == false){
                                ai_fight(gs,aipiece,cursor,game_over_win,aiID,k);
                            } else{
                                switch (aipiece.type) {
                                    ROCK:
                                        if(gs->pieces[k].type == SCISSORS){
                                            ai_fight(gs,aipiece,cursor,game_over_win,aiID,k);
                                        }
                                        break;
                                    PAPER:
                                        break;
                                    SCISSORS:
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ai_fight(struct GameState *gs, struct Piece aipiece, struct Location cursor, bool game_over_win, int aiID, int k){
    enum FightResult result = fight(aipiece.type,gs->pieces[k].type);
    if (result == ATTACKER_WIN)
    {
        aipiece.location = cursor;
        if (gs->pieces[k].is_flag)
        {
            game_over_win = false;
            gs->screen = GAME_OVER;
        }
        else
            gs->screen = GAME;
        remove_piece(&gs, k);
    }
    else if (result == DEFENDER_WIN)
    {
        if (aipiece.is_flag)
        {
            game_over_win = true;
            gs->screen = GAME_OVER;
        }
        else
        {
            gs->screen = GAME;
        }
        remove_piece(&gs, aiID);
        gs->pieces[k].visible=true;
    }
    else
    {
        assert(result == DRAW);
        gs->screen = PICK_TYPE;
    }
}


