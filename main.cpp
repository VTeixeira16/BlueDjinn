#include <iostream>

#include <bingo.h>


using namespace std;

//extern "C" void initializeBingoConfiguration();
//extern "C" void initBingoGame();

int main()
{
    cout << "MAIN" << endl;
    initializeBingoConfiguration();
    cout << "Bingo executado" << endl;
    initBingoGame();

    cout << "Aplica��o encerrada" << endl;

    return 0;
}
