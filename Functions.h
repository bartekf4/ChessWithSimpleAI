#ifndef CHESSWITHSIMPLEAI_FUNCTIONS_H
#define CHESSWITHSIMPLEAI_FUNCTIONS_H
#ifndef MAIN_C_FUNCTIONS_H
#define MAIN_C_FUNCTIONS_H

void defaultBoard(int board[8][8]);

int evaluate(int board[8][8]);

int changeToUTF(int field);

int bestPossibleMove(int board[8][8], int mode, int *x, int *y, int *k, int *o);

void printBoard(int board[8][8]);

void userMove(int board[8][8]);

int enterAIMove(int board[8][8]);


#endif

#endif //CHESSWITHSIMPLEAI_FUNCTIONS_H
