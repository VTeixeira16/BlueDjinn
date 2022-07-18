#include <iostream>

#include <bingo.h>
#include <graphics_core.hpp>
#include <graphics_shaders.hpp>


using namespace std;

//extern "C" void initializeBingoConfiguration();
//extern "C" void initBingoGame();

void BlueDjinn_GraphicCore_Render(){
    cout << "Renderizando" << endl;
}

int main()
{
    cout << "MAIN" << endl;
    /*
     * //Init Bingo
    initializeBingoConfiguration();
    cout << "Bingo executado" << endl;
    initBingoGame();
*/
    //Init Graphic

    void* window = (void*)10;

    BlueDjinn_GraphicsCore_Init();
    cout << "Init executado" << endl;
    BlueDjinn_GraphicsCore_CreateWindow();
    cout << "Janela Criada" << endl;
    BlueDjinn_GraphicsShaders_Init();
    cout << "Shaders Criados" << endl;
    BlueDjinn_GraphicsCore_RenderWindow(BlueDjinn_GraphicCore_Render);

    cout << "Aplicação encerrada" << endl;
    return 0;
}
