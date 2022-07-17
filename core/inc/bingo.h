#ifdef __cplusplus
extern "C" {
#endif
#ifndef BINGO_H
#define BINGO_H


#include <gambling.h>

#include <stdlib.h>

#define BINGO_QTT_CARDS_DEFAULT         4
#define BINGO_CARD_LINES_DEFAULT        3
#define BINGO_CARD_ROWS_DEFAULT         5

#define BINGO_MAX_NUMBERS_DEFAULT       60


typedef struct {
    short qttCards;
    short qttCells;
    short qttLines;
    short qttRows;
    short maxNumbers;

}Bingo_Config_ST;

typedef struct{
    short *cell;
} Bingo_Card_ST;



void initializeBingoConfiguration();
void printBingoConfiguration();
void initBingoGame();

#endif //BINGO_H

#ifdef __cplusplus
}
#endif
