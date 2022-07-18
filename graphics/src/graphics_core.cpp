#include <graphics_core.hpp>

using namespace std;


static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);

static GLFWwindow *window;


void BlueDjinn_GraphicsCore_Init(){

    //Inicia GLFW e configura
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

int BlueDjinn_GraphicsCore_CreateWindow(){
//TODO - Variáveis vão virar parâmetros da função.
    string screenName = "Blue Djinn";
    short screenWidth = 1024;
    short screenHeight = 768;

    //Cria janela GLFW
    window = glfwCreateWindow(screenWidth, screenHeight, screenName.c_str(), NULL, NULL);
    if(window == NULL){
        cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Glad: Carrega todos os ponteiros de função OpenGL
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glfwSetWindowShouldClose(window, 0);

    return 0;
}

int BlueDjinn_GraphicsCore_RenderWindow(void (*BlueDjinn_GraphicCore_Render)()){
    //TODO - Migrar pra funcao própria
        while(!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
            BlueDjinn_GraphicCore_Render();
        }

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
        return 0;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
