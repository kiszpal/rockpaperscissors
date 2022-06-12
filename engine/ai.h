//
// Created by palkiszel on 2022.06.12..
//
#include "engine.h"

#ifndef ROCKPAPERSCISSORS_AI_H
#define ROCKPAPERSCISSORS_AI_H

struct Piece random_piece(struct GameState *gs, enum Team player_team);

bool ai_can_attack(struct GameState *gs, struct Piece aipiece, enum Team player_team);

int ai_number_of_enemies(struct GameState *gs, struct Piece aipiece, enum Team player_team);

void ai_attack(struct GameState *gs, struct Piece aipiece, enum Team player_team, struct Location cursor, bool game_over_win,int aiID);

void ai_fight(struct GameState *gs, struct Piece aipiece, struct Location cursor, bool game_over_win, int aiID, int k);

#endif //ROCKPAPERSCISSORS_AI_H
