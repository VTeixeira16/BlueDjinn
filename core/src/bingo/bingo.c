#ifdef __cplusplus
extern "C" {
#endif

#include <bingo.h>
#include <cards.h>

Bingo_Config_ST bingoConfig;
Bingo_Card_ST *bingoCards;

void printBingoConfiguration()
{
    printf("@%s LINE:%i\n", __func__, __LINE__);
    printf("Qtt Cards:%i\n", bingoConfig.qttCards);
    printf("Qtt Cells:%i\n", bingoConfig.qttLines * bingoConfig.qttRows);;
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
    bingoConfig.qttCells = bingoConfig.qttLines * bingoConfig.qttRows;
    bingoConfig.maxNumbers = BINGO_MAX_NUMBERS_DEFAULT;
    printBingoConfiguration();
}

void initBingoGame()
{
    int i = 0, j = 0;
    printf("@%s LINE:%i\n", __func__, __LINE__);

    bingoCards = (Bingo_Card_ST*)malloc(bingoConfig.qttCards * sizeof(Bingo_Card_ST*));

    for(i = 0; i < bingoConfig.qttCards; i++)
    {
        bingoCards[i].cell = (short*)malloc((bingoConfig.qttLines * bingoConfig.qttRows)* sizeof(short*));
    }

    GenerateCellNumbers(bingoCards, bingoConfig.qttCards, bingoConfig.qttCells);
    PrintCards(bingoCards, bingoConfig);

}

Bingo_Config_ST getBingoConfig(){
    return bingoConfig;
}

#ifdef __cplusplus
}
#endif
