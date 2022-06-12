//
// Created by palkiszel on 2022.06.12..
//

#include "utest_engine.h"
#include "engine.h"


static void test_remove_piece(){
    struct GameState gs = game_state_init();
    struct Piece pieces[gs.pieces_length];
    for(int i =0 ; i < gs.pieces_length; i++){
        pieces[i]=gs.pieces[i];
    }
    int rnd_num = rand() % (gs.pieces_length + 1);
    remove_piece(&gs,rnd_num);
    CU_ASSERT_FALSE( (gs.pieces[rnd_num].location.x == pieces[rnd_num].location.x) && (gs.pieces[rnd_num].location.y == pieces[rnd_num].location.y));
}

static void test_location_eq(){
    struct Location l1,l2,l3;
    struct Piece p1,p2,p3;
    p1.location.x=0;
    p1.location.y=0;
    p2.location.x=3;
    p2.location.y=2;
    p3.location.x=2;
    p3.location.y=2;
    l1.x=0;
    l1.y=0;
    l2.x=3;
    l2.y=3;
    l3.x=3;
    l3.y=5;
    CU_ASSERT_TRUE(location_eq(p1.location,l1));
    CU_ASSERT_FALSE(location_eq(p2.location,l2));
    CU_ASSERT_FALSE(location_eq(p3.location,l3));
}

static void test_location_init(){
    struct Location l1,t1,l2,t2,l3,t3;
    l1 = location_init(2,2);
    l2 = location_init(3,5);
    l3 = location_init(6,3);
    t1.x = 2;
    t1.y = 2;
    t2.x = 3;
    t2.y = 4;
    t3.x = 5;
    t3.y = 2;
    CU_ASSERT_EQUAL(l1.x,t1.x);
    CU_ASSERT_EQUAL(l1.y,t1.y);
    CU_ASSERT_EQUAL(l2.x,t2.x);
    CU_ASSERT_NOT_EQUAL(l2.y,t2.y);
    CU_ASSERT_NOT_EQUAL(l3.x,t3.x);
    CU_ASSERT_NOT_EQUAL(l3.y,t3.y);
}

static void test_fight(){
    CU_ASSERT_EQUAL(fight(ROCK,ROCK),DRAW);
    CU_ASSERT_EQUAL(fight(ROCK,PAPER),DEFENDER_WIN);
    CU_ASSERT_EQUAL(fight(ROCK,SCISSORS),ATTACKER_WIN);
    CU_ASSERT_EQUAL(fight(PAPER,PAPER),DRAW);
    CU_ASSERT_EQUAL(fight(PAPER,ROCK),ATTACKER_WIN);
    CU_ASSERT_EQUAL(fight(PAPER,SCISSORS),DEFENDER_WIN);
    CU_ASSERT_EQUAL(fight(SCISSORS,SCISSORS),DRAW);
    CU_ASSERT_EQUAL(fight(SCISSORS,ROCK),DEFENDER_WIN);
    CU_ASSERT_EQUAL(fight(SCISSORS,PAPER),ATTACKER_WIN);
}

static void test_game_state_init(){
    struct GameState gs = game_state_init();
    CU_ASSERT_EQUAL(gs.pieces_length,28);
    CU_ASSERT_EQUAL(gs.screen,MAIN_MENU);
    CU_ASSERT_FALSE(gs.blue_trap_visible);
    CU_ASSERT_FALSE(gs.red_trap_visible);
}

static void test_is_trap(){
    struct Piece p1,p2,p3,p4,p5;
    p1.type=TRAP_INVISIBLE;
    p2.type=TRAP;
    p3.type=ROCK;
    p4.type=PAPER;
    p5.type=SCISSORS;
    CU_ASSERT_TRUE(is_trap(p1.type));
    CU_ASSERT_TRUE(is_trap(p2.type));
    CU_ASSERT_FALSE(is_trap(p3.type));
    CU_ASSERT_FALSE(is_trap(p4.type));
    CU_ASSERT_FALSE(is_trap(p5.type));
}

static void test_random_type(){
    for(int i =0; i<1000 ; i++){
        switch (random_type()) {
            case ROCK:
            case PAPER:
            case SCISSORS:
                break;
            default: CU_ASSERT_TRUE(false);
        }
    }
}

static void test_place_ai_flag(){
    struct GameState gs = game_state_init();
    place_ai_flag(&gs,BLUE);
    for(int i = 0; i< gs.pieces_length;i++){
        if(gs.pieces[i].is_flag==true){
            CU_ASSERT_TRUE(true);
            CU_ASSERT_EQUAL(gs.pieces[i].team,BLUE);
            return;
        }
    }
    struct GameState gs2 = game_state_init();
    place_ai_flag(&gs2,RED);
    for(int i = 0; i< gs2.pieces_length;i++){
        if(gs2.pieces[i].is_flag==true){
            CU_ASSERT_TRUE(true);
            CU_ASSERT_EQUAL(gs2.pieces[i].team,RED);
            return;
        }
    }
    CU_ASSERT_TRUE(false);
}

static void test_place_ai_trap(){
    struct GameState gs = game_state_init();
    place_ai_trap(&gs,BLUE);
    for(int i = 0; i< gs.pieces_length;i++){
        if(gs.pieces[i].type==TRAP_INVISIBLE){
            CU_ASSERT_TRUE(true);
            CU_ASSERT_EQUAL(gs.pieces[i].team,BLUE);
            return;
        }
    }
    struct GameState gs2 = game_state_init();
    place_ai_trap(&gs2,RED);
    for(int i = 0; i< gs2.pieces_length;i++){
        if(gs2.pieces[i].type==TRAP_INVISIBLE){
            CU_ASSERT_TRUE(true);
            CU_ASSERT_EQUAL(gs2.pieces[i].team,RED);
            return;
        }
    }
    CU_ASSERT_TRUE(false);
}

static void test_move_cursor(){
    struct Location cursor;
    cursor.x=0;
    cursor.y=0;
    move_cursor(&cursor,'a');
    CU_ASSERT_EQUAL(cursor.x,0);
    CU_ASSERT_EQUAL(cursor.y,0);
    move_cursor(&cursor,'w');
    CU_ASSERT_EQUAL(cursor.x,0);
    CU_ASSERT_EQUAL(cursor.y,0);
    move_cursor(&cursor,'s');
    CU_ASSERT_EQUAL(cursor.x,0);
    CU_ASSERT_EQUAL(cursor.y,1);
    move_cursor(&cursor,'d');
    CU_ASSERT_EQUAL(cursor.x,1);
    CU_ASSERT_EQUAL(cursor.y,1);
    cursor.x=5;
    cursor.y=5;
    move_cursor(&cursor,'a');
    CU_ASSERT_EQUAL(cursor.x,4);
    CU_ASSERT_EQUAL(cursor.y,5);
    move_cursor(&cursor,'w');
    CU_ASSERT_EQUAL(cursor.x,4);
    CU_ASSERT_EQUAL(cursor.y,4);
    move_cursor(&cursor,'s');
    CU_ASSERT_EQUAL(cursor.x,4);
    CU_ASSERT_EQUAL(cursor.y,5);
    move_cursor(&cursor,'d');
    CU_ASSERT_EQUAL(cursor.x,5);
    CU_ASSERT_EQUAL(cursor.y,5);
}

CU_TestInfo engine_tests[] = {
        {"piece removal", test_remove_piece},
        {"location equation", test_location_eq},
        {"location initialization", test_location_init},
        {"fight", test_fight},
        {"game state initialization", test_game_state_init},
        {"asserting trap", test_is_trap},
        {"random type", test_random_type},
        {"placing AI flag", test_place_ai_flag},
        {"placing AI trap", test_place_ai_trap},
        {"moving cursor", test_move_cursor},
        CU_TEST_INFO_NULL
};