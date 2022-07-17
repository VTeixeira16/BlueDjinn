#ifdef __cplusplus
extern "C" {
#endif

#include <bingo.h>

Bingo_Config_ST bingoConfig;
Bingo_Card_ST *bingoCards;

void printBingoConfiguration()
{
    printf("@%s LINE:%i\n", __func__, __LINE__);
    printf("Qtt Cards:%i\n", bingoConfig.qttCards);
    printf("Qtt Cells:%i\n", bingoConfig.qttCells);;
    printf("Qtt Lines:%i\n", bingoConfig.qttLines);
    printf("Qtt Rows:%i\n", bingoConfig.qttRows);
    printf("Max Numbers:%i\n", bingoConfig.maxNumbers);

}

void initializeBingoConfiguration()
{
    printf("@%s LINE:%i\n", __func__, __LINE__);
    bingoConfig.qttCards = BINGO_QTT_CARDS_DEFAULT;
    bingoConfig.qttLines = BINGO_CARD_LINES_DEFAULT;
    bingoConfig.qttRows = BINGO_CARD_ROWS_DEFAULT;
    bingoConfig.maxNumbers = BINGO_MAX_NUMBERS_DEFAULT;
    printBingoConfiguration();
}

void initBingoGame()
{
    int i = 0;
    printf("@%s LINE:%i\n", __func__, __LINE__);

    bingoCards = (Bingo_Card_ST*)malloc(bingoConfig.qttCards * sizeof(Bingo_Card_ST*));
    for(i = 0; i < bingoConfig.qttCards; i++)
    {
        bingoCards[i].cell = (short*)malloc(bingoConfig.qttCells * sizeof(short*));
    }

    bingoCards[0].cell[0] = 0;
    bingoCards[0].cell[1] = 1;
    bingoCards[0].cell[2] = 2;
    bingoCards[0].cell[3] = 3;
    printf("bingoCards:%i\n", bingoCards[0].cell[0]);
    printf("bingoCards:%i\n", bingoCards[0].cell[1]);
    printf("bingoCards:%i\n", bingoCards[0].cell[2]);
    printf("bingoCards:%i\n", bingoCards[0].cell[3]);
    printf("bingoCards:%i\n", bingoCards[0].cell[4]);
    printf("bingoCards:%i\n", bingoCards[0].cell[5]);

//    free();
}

#ifdef __cplusplus
}
#endif
