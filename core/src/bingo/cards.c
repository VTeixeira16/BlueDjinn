#ifdef __cplusplus
extern "C" {
#endif

#include <cards.h>

//TODO - REMOVER QUANDO CELULAS PARAREM DE TER NUMEROS ALEATORIOS.
#include <time.h>

void GenerateCellNumbers(Bingo_Card_ST *bingoCards, ushort qttCards, ushort qttCells){
    //TODO - Deve puxar de um Json ou Xml com tabela de prêmios.
    ushort i = 0, j = 0;

    srand(time(NULL));
    for(i = 0; i < qttCards; i++){
        for(j = 0; j < qttCells; j++){
            do{
                bingoCards[i].cell[j] = rand()% (getBingoConfig().maxNumbers + 1);
            }while(bingoCards[i].cell[j] == 0);

        }
    }
}

void PrintCards(Bingo_Card_ST *bingoCards, Bingo_Config_ST bingoConfig){
    ushort i, j, l, r;

    printf("bingoConfig.qttCards:%i\n", bingoConfig.qttCards);
    printf("bingoConfig.qttCells:%i\n", bingoConfig.qttCells);
    printf("bingoConfig.qttLines:%i\n", bingoConfig.qttLines);
    printf("bingoConfig.qttRows:%i\n", bingoConfig.qttRows);

    printf("_________________\n");
    for(i = 0; i < bingoConfig.qttCards; i++){
        for(j = 0; j < bingoConfig.qttCells; j++){
            if(j%bingoConfig.qttRows == 0){
                printf("| ");
            }
            if(bingoCards[i].cell[j] < 10)
                printf(" ");
            printf("%i ", bingoCards[i].cell[j]);
            if(j%bingoConfig.qttRows == bingoConfig.qttRows - 1){
                printf("|\n");
            }
        }
        printf("_________________\n");
    }
}

#ifdef __cplusplus
}
#endif

/* 3 x 5
0  1   2  3  4
5  6   7  8  9
10 11 12 13 14

o resto da divisão de cell / ROW for igual a ROW - 1
*/
