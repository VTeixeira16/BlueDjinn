#ifndef CARDS_H
#define CARDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bingo.h>

void GenerateCellNumbers(Bingo_Card_ST *bingoCards, ushort qttCards, ushort qttCells);
void PrintCards(Bingo_Card_ST *bingoCards, Bingo_Config_ST bingoConfig);









#ifdef __cplusplus
}
#endif
#endif //BINGO_H
