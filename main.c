#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "Functions.h"

#ifndef WIN
#define WIN 1000
#endif

#ifndef LOSE
#define LOSE -1000
#endif


int main() {
    _setmode(_fileno(stdout), 0x00020000);    // Enable Unicode UTF-16 output to console
    int result, board[8][8];

    defaultBoard(board);
    printBoard(board);
    while (1) {
        userMove(board);
        printBoard(board);

        result = enterAIMove();
        if (abs(result) >= WIN) break;
        enterAIMove();
        printBoard(board);
    }
    if (result >= WIN)
        wprintf(L"You lost.\n");
    if (result <= LOSE)
        wprintf(L"You won!\n");
}
