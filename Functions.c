#include "Functions.h"
#include <io.h>
#include <stdio.h>

#define WIN 1000
#define LOSE -1000

/*
    0 - king
    1 - queen
    2 - rook
    3 - bishop
    4 - knight
    5 - pawn
    6 - king_k
    7 - queen_k
    8 - rook_k
    9 - bishop_k
    10 - knight_k
    11 - pawn_k
    12 - field BLANK
*/

int MAX_DIRECTION[] = {8, 8, 4, 4, 8, 3, 8, 8, 4, 4, 8, 3, 0};
int MAX_DISTANCE[] = {2, 8, 8, 8, 2, 2, 2, 8, 8, 8, 2, 2, 0};
int board[8][8];
int WX[12][8] = {{0,  1, 1,  1, 0,  -1, -1, -1},
                 {0,  1, 1,  1, 0,  -1, -1, -1,},
                 {0,  1, 0,  -1},
                 {1,  1, -1, -1},
                 {1,  2, 2,  1, -1, -2, -2, -1},
                 {-1, 0, 1},
                 {0,  1, 1,  1, 0,  -1, -1, -1},
                 {0,  1, 1,  1, 0,  -1, -1, -1,},
                 {0,  1, 0,  -1},
                 {1,  1, -1, -1},
                 {1,  2, 2,  1, -1, -2, -2, -1},
                 {-1, 0, 1}};

int WY[12][8] = {{-1, -1, 0, 1, 1, 1, 0,  -1},
                 {-1, -1, 0, 1, 1, 1, 0,  -1},
                 {-1, 0,  1, 0},
                 {-1, 1,  1, -1},
                 {-2, -1, 1, 2, 2, 1, -1, -2},
                 {-1, -1, -1},
                 {-1, -1, 0, 1, 1, 1, 0,  -1},
                 {-1, -1, 0, 1, 1, 1, 0,  -1},
                 {-1, 0,  1, 0},
                 {-1, 1,  1, -1},
                 {-2, -1, 1, 2, 2, 1, -1, -2},
                 {1,  1,  1}};


void defaultBoard(int board[8][8]) {
    //black figures
    board[0][0] = 8;
    board[0][1] = 10;
    board[0][2] = 9;
    board[0][3] = 7;
    board[0][4] = 6;
    board[0][5] = 9;
    board[0][6] = 10;
    board[0][7] = 8;

    for (int i = 0; i < 8; ++i) {
        board[1][i] = 11; //black pawns
        board[2][i] = 12; //blank fields
        board[3][i] = 12; //blank fields
        board[4][i] = 12; //blank fields
        board[5][i] = 12; //blank fields
        board[6][i] = 5;  //white pawns
    }

    //white figures
    board[7][0] = 2;
    board[7][1] = 4;
    board[7][2] = 3;
    board[7][3] = 1;
    board[7][4] = 0;
    board[7][5] = 3;
    board[7][6] = 4;
    board[7][7] = 2;
}

int evaluate(int board[8][8]) {
    int ev[] = {LOSE, -9, -5, -4, -3, -2, -1, WIN, 9, 5, 4, 3, 2, 1, 0};
    int temp_score = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            temp_score += ev[board[i][j]];
        }
    }
    return temp_score;
}

int changeToUTF(int field) {
    if (field == 0) return 0x2654;
    if (field == 1) return 0x2655;
    if (field == 2) return 0x2656;
    if (field == 3) return 0x2657;
    if (field == 4) return 0x2658;
    if (field == 5) return 0x2659;
    if (field == 6) return 0x265A;
    if (field == 7) return 0x265B;
    if (field == 8) return 0x265C;
    if (field == 9) return 0x265D;
    if (field == 10) return 0x265E;
    if (field == 11) return 0x265F;
    if (field == 12) return 0x2001;
}

int bestPossibleMove(int board[8][8], int mode, int *x, int *y, int *k, int *o) {
    int px_add, py_add, k_add, o_add, px, py, dx, dy, direction, distance;
    int score, smax, smin, move_piece, taken_piece;

    score = evaluate(board);
    if (mode == 0 || 2 * score > WIN || 2 * score < LOSE) return score;
    if (mode % 2 == 0) {
        for (px = 0, smax = 100 * LOSE; px < 8; ++px) {
            for (py = 0; py < 8; ++py) {
                if (board[px][py] >= 6 && board[px][py] < 12) {
                    for (direction = 0; direction < MAX_DIRECTION[board[px][py]]; ++direction) {
                        for (distance = 1; distance < MAX_DISTANCE[board[px][py]]; ++distance) {
                            dx = (distance - 1) * WX[board[px][py]][direction];
                            dy = (distance - 1) * WY[board[px][py]][direction];
                            if (distance >= 2 && board[px + dx][py + dy] != 12)
                                break;// if there are figures on the way, which arent ours, break
                            dx = distance * WX[board[px][py]][direction];
                            dy = distance * WY[board[px][py]][direction];
                            if (px + dx >= 0 && px + dx < 8 && py + dy >= 0 && py + dy < 8) {
                                if (board[px + dx][py + dy] == 12 || board[px + dx][py + dy] <= 5) {
                                    if (board[px][py] != 11 ||
                                        (board[px + dx][py + dy] == 12 && dy == 0) ||
                                        (board[px + dx][py + dy] != 12 && dy != 0)) {
                                        move_piece = board[px][py];
                                        taken_piece = board[px + dx][py + dy];
                                        board[px + dx][py + dy] = board[px][py];
                                        board[px][py] = 12;
                                        if (board[px + dx][py + dy] == 11 && py + dy == 7) board[px + dx][py + dy] = 7;
                                        score = bestPossibleMove(board, mode - 1, &px_add, &py_add, &k_add, &o_add);
                                        board[px][py] = move_piece;
                                        board[px + dx][py + dy] = taken_piece;
                                        if (score >= smax) {
                                            smax = score;
                                            *x = px;
                                            *y = py;
                                            *k = direction;
                                            *o = distance;
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
        return smax;
    } else {
        for (px = 0, smin = 100 * WIN; px < 8; ++px) {
            for (py = 0; py < 8; ++py) {
                if (board[px][py] <= 5) {
                    for (direction = 0; direction < MAX_DIRECTION[board[px][py]]; ++direction) {
                        for (distance = 1; distance < MAX_DISTANCE[board[px][py]]; ++distance) {
                            dx = (distance - 1) * WX[board[px][py]][direction];
                            dy = (distance - 1) * WY[board[px][py]][direction];
                            if (distance >= 2 && board[px + dx][py + dy] != 12)
                                break;// if there are figures on the way, which arent ours, break
                            dx = distance * WX[board[px][py]][direction];
                            dy = distance * WY[board[px][py]][direction];
                            if (px + dx >= 0 && px + dx < 8 && py + dy >= 0 && py + dy < 8) {
                                if (board[px + dx][py + dy] >= 6) {
                                    if (board[px][py] != 5 ||
                                        (board[px + dx][py + dy] == 12 && dy == 0) ||
                                        (board[px + dx][py + dy] != 12 && dy != 0)) {
                                        move_piece = board[px][py];
                                        taken_piece = board[px + dx][py + dy];
                                        board[px + dx][py + dy] = board[px][py];
                                        board[px][py] = 12;
                                        if (board[px + dx][py + dy] == 5 && py + dy == 0) board[px + dx][py + dy] = 1;
                                        score = bestPossibleMove(board, mode - 1, &px_add, &py_add, &k_add, &o_add);
                                        board[px][py] = move_piece;
                                        board[px + dx][py + dy] = taken_piece;
                                        if (score <= smin) {
                                            smin = score;
                                            *x = px;
                                            *y = py;
                                            *k = direction;
                                            *o = distance;
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
        return smin;
    }
}


void printBoard(int board[8][8]) {
    _setmode(_fileno(stdout), 0x00020000);
    wprintf(L"  %cA%cB%cC%cD%cE%cF%cG%cH\n", 0x2009, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001, 0x2001);
    wprintf(L"  ― ― ― ― ― ― ― ― \n");
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (j == 0) wprintf(L"%d|%c|", i + 1, changeToUTF(board[i][j]));
            else wprintf(L"%c|", changeToUTF(board[i][j]));
        }
        wprintf(L"\n  ― ― ― ― ― ― ― ― \n");
    }
    wprintf(L"%c", 0x000A);
}

void userMove(int board[8][8]) {
    char sourceColumn = 'c', destinationColumn = 'c';
    int sourceLine = 0, destinationLine = 0, tmp, tmpS, tmpD;


    wprintf(L"It's your move. Enter source%c", 0x000A);
    scanf("%s %d", &sourceColumn, &sourceLine);
    tmpS = (int) sourceColumn;
    tmpS -= 97;
    if (sourceLine < 0 || sourceLine >= 8 || tmpS < 0 || tmpS >= 8 || board[sourceLine][tmp] >= 6)
    {
        wprintf(L"Invalid value! Try again!\n");
        userMove(board);
    }

    wprintf(L"Now enter destination%c%c", 0x000A, 0x000D);
    scanf("%s %d", &destinationColumn, &destinationLine);
    tmpD = (int) destinationColumn;
    tmpD -= 97;
    if (destinationLine < 0 || destinationLine >= 8 || tmpD < 0 || tmpD >= 8)
    {
        wprintf(L"Invalid value! Try again!\n");
        userMove(board);
    }


    tmp = board[sourceLine - 1][tmpS];
    board[(destinationLine - 1)][tmpD] = tmp;
    board[sourceLine - 1][tmpS] = 12;

}

int enterAIMove(int board[8][8]) {
    int x = 0, y = 0, k = 0, o = 0, result = 0;
    result = bestPossibleMove(board, 6, &x, &y, &k, &o);
    int px = WX[board[x][y]][k] * o;
    int py = WY[board[x][y]][k] * o;
    board[x + px][y + py] = board[x][y];
    board[x][y] = 12;
    return result;


}


