/*
#include <iostream>
#include <array>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
//#include <glm/gtx/string_cast.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLFW_DLL


//TODO - Adicionar função que retira Screen Tearing

using namespace std;


//TODO - se tornará parâmetro- Variaveis de tela
int width = 800;
int height = 600;
const char titleName[] = "Blue Marble";


std::string ReadFile(const char *FilePath)
{
    std::string FileContents;
    if(std::ifstream FileStream{ FilePath, std::ios::in})
    {
        //Lê conteudo dentro do FilePath e salva em FileContents
        FileContents.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
    }
    return FileContents;
}

void CheckShader(GLuint ShaderId)
{
    // Verificar se o shader foi compilado
    GLint Result = GL_TRUE;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

    if (Result == GL_FALSE)
    {
        // Erro ao compilar o shader, imprimir o log para saber o que est� errado
        GLint InfoLogLength = 0;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

        std::string ShaderInfoLog(InfoLogLength, '\0');
        glGetShaderInfoLog(ShaderId, InfoLogLength, nullptr, &ShaderInfoLog[0]);

        if (InfoLogLength > 0)
        {
            std::cout << "Erro no Vertex Shader: " << std::endl;
            std::cout << ShaderInfoLog << std::endl;

            assert(false);
        }
    }
}

GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile)
{
    // Criar os identificadores de cada um dos shaders
    GLuint VertShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    std::string VertexShaderSource = ReadFile(VertexShaderFile);
    std::string FragmentShaderSource = ReadFile(FragmentShaderFile);

    assert(!VertexShaderSource.empty());
    assert(!FragmentShaderSource.empty());

    std::cout << "Compilando " << VertexShaderFile << std::endl;
    const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
    glShaderSource(VertShaderId, 1, &VertexShaderSourcePtr, nullptr);
    glCompileShader(VertShaderId);
    CheckShader(VertShaderId);

    std::cout << "Compilando " << FragmentShaderFile << std::endl;
    const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
    glShaderSource(FragShaderId, 1, &FragmentShaderSourcePtr, nullptr);
    glCompileShader(FragShaderId);
    CheckShader(FragShaderId);

    std::cout << "Linkando Programa" << std::endl;
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertShaderId);
    glAttachShader(ProgramId, FragShaderId);
    glLinkProgram(ProgramId);

    // Verificar o programa
    GLint Result = GL_TRUE;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);

    if (Result == GL_FALSE)
    {
        GLint InfoLogLength = 0;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);

        if (InfoLogLength > 0)
        {
            std::string ProgramInfoLog(InfoLogLength, '\0');
            glGetProgramInfoLog(ProgramId, InfoLogLength, nullptr, &ProgramInfoLog[0]);

            std::cout << "Erro ao linkar programa" << std::endl;
            std::cout << ProgramInfoLog << std::endl;

            assert(false);
        }
    }

    glDetachShader(ProgramId, VertShaderId);
    glDetachShader(ProgramId, FragShaderId);

    glDeleteShader(VertShaderId);
    glDeleteShader(FragShaderId);

    return ProgramId;
}

GLuint LoadTexture(const char *TextureFile)
{
    std::cout << "Carregando Textura " << TextureFile << std::endl;

    stbi_set_flip_vertically_on_load(true);

    int TextureWidth = 0;
    int TextureHeight = 0;
    int NumberOfComponents = 0;
    unsigned char* TextureData = stbi_load(TextureFile, &TextureWidth, &TextureHeight, &NumberOfComponents, 3);

    assert(TextureData);

    //Gera o identificador da textura
    GLuint TextureId;
    glGenTextures(1, &TextureId);

    //Habilita a textura para ser modificada
    glBindTexture(GL_TEXTURE_2D, TextureId);

    //Copiar a textura para a memoria de video (GPU)
    //Pesquisar sobre glTexImage2D
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);

    //Define tipo de filtragem (magnificação e minificação)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);

    //Configurar o Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Gera o mipmap a partir da textura
    glGenerateMipmap(GL_TEXTURE_2D);

    //Desliga a textura pois ela ja foi copiada para a GPU
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, 1);

    //Libera memoria da imagem
    stbi_image_free(TextureData);

    return TextureId;

}

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
    glm::vec2 UV;
};

struct DirectionLight
{
    glm::vec3 Direction;
    GLfloat Intensity;
};

GLuint LoadGeometry()
{
    //TRIANGULO - 2D (com coordenadas baseadas na tela.
        //Define um triangulo em coordenadas normalizadas
        // std::array possui tamanho fixo, ao contrario de std::vector

        //Vertex é uma struct criada que possui 2 vetores, um de posição e outro de cor.
    //    std::array<Vertex, 3> Triangle = {
    //        Vertex{ glm::vec3{-1.0f, -1.0f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
    //        Vertex{ glm::vec3{ 1.0f, -1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
    //        Vertex{ glm::vec3{-1.0f,  1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{0.0f, 1.0f}}
    //    };

    std::array<Vertex, 6> Quad = {
        Vertex{ glm::vec3{-1.0f, -1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
        Vertex{ glm::vec3{ 1.0f, -1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
        Vertex{ glm::vec3{ 1.0f,  1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
        Vertex{ glm::vec3{-1.0f,  1.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{0.0f, 1.0f}}
    };


    //Definir a lista de elementos que formam os Triangulos
    std::array<glm::ivec3, 2 > Indices = {
        glm::ivec3{0, 1, 3},
        glm::ivec3{3, 1, 2}
    };

    //Copia vertices do triangulo para a memoria da GPU
    GLuint VertexBuffer;

    //Pede para a OpenGL gerar o identificador do VertexBuffer (VBO)
    glGenBuffers(1, &VertexBuffer);

    //Pedir para OpenGl gerar o identificador do EBO
    GLuint ElementBuffer = 0;
    glGenBuffers(1, &ElementBuffer);


    //Ativar o VertexBuffer como sendo o buffer para onde vamos copiar os dados do triangulo
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

    //Copiar os dados para a memoria de video
    glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad.data(), GL_STATIC_DRAW);

    //Copia os dados do Element Buffer para a GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices.data(), GL_STATIC_DRAW);

    //Fim - TRIANGULO

    //Gerar o Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    //Habilitar o VAO
    glBindVertexArray(VAO);


        //TRIANGULO
    //Atributo 0 está definido no Triangle_vert como posição
    //Atributo 1 está definido no Triangle_vert como cor
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    //Diz para a OpenGL que o VertexBuffer vai ser o buffer ativo no momento
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

    //Informa ao OpenGL onde, dentro do VertexBuffer, os vertices estão.
    // No caso, o array Triangles é contíguo em memória, logo basta dizer quantos vértices vamos usar para desenhar o triângulo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

    //Desabilita o VAO
    glBindVertexArray(0);

    return VAO;
}

void GenerateSphereMesh(GLuint Resolution, std::vector<Vertex> &Vertices, std::vector<glm::ivec3> &Indices)
{
    Vertices.clear();
    Indices.clear();
    //Guarda dados em constantes
    constexpr float Pi = glm::pi<float>();
    constexpr float TwoPi = glm::two_pi<float>();
    const float InvResolution = 1.0f / static_cast<float>(Resolution - 1);

    for(GLuint uIndex = 0; uIndex < Resolution; ++uIndex)
    {
        const float u = uIndex * InvResolution;
        const float phi = glm::mix(0.0f, TwoPi, u);

        for(GLuint vIndex = 0; vIndex < Resolution; ++vIndex)
        {
            const float v = vIndex * InvResolution;
            const float theta = glm::mix(0.0f, Pi, v);

            glm::vec3 vertexPosition =
            {
                glm::sin(theta) * glm::cos(phi),
                glm::sin(theta) * glm::sin(phi),
                glm::cos(theta)
            };

            Vertex vertex
            {
                vertexPosition,
                glm::normalize(vertexPosition),
                glm::vec3{ 1.0f, 1.0f, 1.0f},
                glm::vec2{u * -1, v}

            };

            Vertices.push_back(vertex);
        }
    }

    for(GLuint u = 0; u < Resolution - 1; ++u)
    {
        for(GLuint v = 0; v < Resolution -1; ++v)
        {
            GLuint p0 = u + v * Resolution;
            GLuint p1 = (u + 1) + v * Resolution;
            GLuint p2 = (u + 1) + (v + 1) * Resolution;
            GLuint p3 = u + ( v + 1) * Resolution;

            Indices.push_back(glm::ivec3{p0, p1, p3});
            Indices.push_back(glm::ivec3{p3, p1, p2});
        }

    }
}

GLuint LoadSphere(GLuint &numVertices, GLuint &numIndices)
{
    std::vector<Vertex> Vertices;
    std::vector<glm::ivec3> Triangles;

    GenerateSphereMesh(100, Vertices, Triangles);

    numVertices = Vertices.size();
    numIndices = Triangles.size() * 3;

    GLuint VertexBuffer;
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

    GLuint ElementBuffer;
    glGenBuffers(1, &ElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), Triangles.data(), GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));

    glBindVertexArray(0);

    return VAO;
}

class FlyCamera
{
public:

    void MoveFoward(float Amount)
    {
        Location += glm::normalize(Direction) * Amount * Speed;
    }

    void MoveRight(float Amount)
    {
        glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));
        Location += Right * Amount * Speed;
    }

    void Look(float Yaw, float Pitch)
    {
        Yaw *= Sensitivity;
        Pitch *= Sensitivity;

        glm::vec3 Right = glm::normalize(glm::cross(Direction, Up));

        static const glm::mat4 I = glm::identity<glm::mat4>();
        glm::mat4 YawRotation = glm::rotate(I, glm::radians(Yaw), Up);
        glm::mat4 PitchRotation = glm::rotate(I, glm::radians(Pitch), Right);

        Up = PitchRotation * glm::vec4{Up, 0.0f};
        Direction = YawRotation * PitchRotation * glm::vec4{ Direction, 0.0f};
    }

    glm::mat4 GetView() const
    {
         return glm::lookAt(Location, Location + Direction, Up);
    }

    glm::mat4 GetViewProjection() const
    {
        glm::mat4 View = glm::lookAt(Location, Location + Direction, Up);
        //TODO - Se fosse ortográfica, usaria glm::ortho
        glm::mat4 Projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
        return Projection * GetView();
    }

    //Parametros de Interatividade
    float Speed = 3.0f;
    float Sensitivity = 0.1f;

    //Definição da Matriz de View
    glm::vec3 Location{ 0.0f, 0.0f, 5.0f};
    glm::vec3 Direction{ 0.0f, 0.0f, -1.0f};
    glm::vec3 Up{ 0.0f, 1.0f, 0.0f};

    //TODO - Para projeção ortográfica, os parâmetros devem ser mudados
    //Definição da Matriz Projection
    float FieldOfView = glm::radians(45.0f);
    float AspectRatio = width / height;
    float Near = 0.01f;
    float Far = 1000.0f;

};

//TODO - No tutorial o objeto é do tipo FlyCamera.


FlyCamera Camera;
bool boolEnableMouseMovement = false;
glm::vec2 PreviousCursor{0.0, 0.0};

void MouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Modifiers)
{
    std::cout << "Button: " << Button
              << "Action: " << Action
              << "Modifiers: " << Modifiers << std::endl;
    if (Button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(Action == GLFW_PRESS)
        {
            //Some com o cursor
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double x, y;
            glfwGetCursorPos(Window, &x, &y);

            PreviousCursor = glm::vec2{x, y};
            boolEnableMouseMovement = true;
        }

        if(Action == GLFW_RELEASE)
        {
            //Habilita o cursor
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            boolEnableMouseMovement = false;
        }
    }
}

void MouseMotionCallback(GLFWwindow *Window, double x, double y)
{
    if(boolEnableMouseMovement){
        //deltaCursor é usado para medir posição anterior
        glm::vec2 currentCursor{x, y};
        glm::vec2 deltaCursor = currentCursor - PreviousCursor;

//        std::cout << glm::to_string(deltaCursor) << std::endl;
        Camera.Look(deltaCursor.x, deltaCursor.y);

        PreviousCursor = currentCursor;

    }
}

void Resize(GLFWwindow* Window, int NewWidth, int newHeight)
{
    width = NewWidth;
    height = newHeight;

    Camera.AspectRatio = static_cast<float>(width) / height;
    glViewport(0, 0, width, height);


}
/*
int main()
{

	std::cout << "Inicio Projeto" << std::endl;
	
    if(glfwInit() == GLFW_FALSE){
        std::cout << "ERRO GLFW" << endl;
    }



    //Cria Janela
    GLFWwindow *Window = glfwCreateWindow(width, height, titleName, nullptr, nullptr);
    //TODO - Verificar se deu erro
    assert(Window);

    // Cadastrar as callbacks no GLFW
    glfwSetMouseButtonCallback(Window, MouseButtonCallback);
    glfwSetCursorPosCallback(Window, MouseMotionCallback);
    glfwSetFramebufferSizeCallback(Window, Resize);

    glfwMakeContextCurrent(Window);

    //0 desliga limitação de FPS.
    //0 é útil para testar performance.
    glfwSwapInterval(1);



    //Inicializa biblioteca Glew (inclui funções modernas da OpenGL)
    //TODO - Verificar se deu erro
    assert(glewInit() == GLEW_OK);

    //Verificar versão da OpenGL que estamos usando
    GLint GLMajorVersion = 0;
    GLint GLMinorVersion = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

    cout << "OpenGL Version: " << GLMajorVersion << "." << GLMinorVersion << endl;
    cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    Resize(Window, width, height);

    //Carrega Shaders
    GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

    GLuint TextureId = LoadTexture("textures/earth_2k.jpg");
    GLuint CloudTextureId = LoadTexture("textures/earth_clouds_2k.jpg");

    GLuint QuadVao = LoadGeometry();

    GLuint SphereNumVertices = 0;
    GLuint SphereNumIndices = 0;
    GLuint SphereVAO = LoadSphere(SphereNumVertices, SphereNumIndices);

    std::cout << "Numero de Vertices da Esfera: " << SphereNumVertices << std::endl;
    std::cout << "Numero de Indices da Esfera: " << SphereNumIndices << std::endl;

    //Model
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 ModelMatrix = glm::rotate(identity, glm::radians(90.f), glm::vec3{1, 0, 0});
//    glm::mat4 ModelMatrix2 = glm::translate(identity, glm::vec3{10, 0, 0});
//    glm::mat4 ModelMatrix = glm::identity<glm::mat4>();


    //Define cor de fundo RGBA
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Guarda o tempo do frame anterior
    double PreviousTime = glfwGetTime();

    //Habilita o Backface Culling - Faz com que o verso nao seja renderizado
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Habilita teste de profundizade (z buffer)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Criar uma fonte de luz direcional
    DirectionLight Light;
    Light.Direction = glm::vec3{0.0f, 0.0f, -1.0};
    Light.Intensity = 1.0f;

    //entra no loop de eventos da aplicação
    while(!glfwWindowShouldClose(Window))
    {
        double CurrentTime = glfwGetTime();
        double DeltaTime = CurrentTime - PreviousTime;
        if(DeltaTime > 0.0)
        {
            PreviousTime = CurrentTime;
        }

        //Limpar cor, ou seja, preencher com a cor definida em ClearColor
        //glClear vai limpar o framebuffer. GL_COLOR_BUFFER_BIT diz para limpar o buffer de cor.
        // Apos limpar, preenche com a cor configurada em glClearColor.
        // Quando formos desenhar geometrias 3D, vamos voltar ao glClear porque temos que limpar o buffer de profundidade (depth buffer).
//        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        //Ativa o programa de shaders
        glUseProgram(ProgramId);


        glm::mat4 NormalMatrix = glm::inverse(glm::transpose(Camera.GetView() * ModelMatrix));
        glm::mat4 ViewProjectionMatrix = Camera.GetViewProjection();
        glm::mat4 ModelViewProjection = ViewProjectionMatrix * ModelMatrix;

        GLint TimeLoc = glGetUniformLocation(ProgramId, "Time");
        glUniform1f(TimeLoc, CurrentTime);

        //Pega o "ModelViewProjection" do vert/frag shader vinculado ao ProgramId.
        GLint ModelViewProjectionLoc = glGetUniformLocation(ProgramId, "ModelViewProjection");
        glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

        GLint NormalMatrixLoc = glGetUniformLocation(ProgramId, "NormalMatrix");
        glUniformMatrix4fv(NormalMatrixLoc, 1, GL_FALSE, glm::value_ptr(NormalMatrix));


//Ativa Textura
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureId);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, CloudTextureId);

        GLint TextureSamplerLoc = glGetUniformLocation(ProgramId, "TextureSampler");
        glUniform1i(TextureSamplerLoc, 0);

        GLint CloudTextureLoc = glGetUniformLocation(ProgramId, "CloudsTexture");
        glUniform1i(CloudTextureLoc, 1);


        GLint LightDirectionLoc = glGetUniformLocation(ProgramId, "LightDirection");
        glUniform3fv(LightDirectionLoc, 1, glm::value_ptr(Camera.GetView() * glm::vec4{Light.Direction, 0.0f}));

        GLint LightIntensityLoc = glGetUniformLocation(ProgramId, "LightIntensity");
        glUniform1f(LightIntensityLoc, Light.Intensity);

//        glBindVertexArray(QuadVao);
        glBindVertexArray(SphereVAO);


        // Teste para verificação de triangulos.
        glPointSize(10.0f);
        glLineWidth(10.0f);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        //Finalmente diz para a OpenGL desenhar o triângulo com os dados armazenados no VertexBuffer
        //3 é o número de vértices
//        glDrawArrays(GL_TRIANGLES, 0, Quad.size());
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//        glDrawArrays(GL_POINTS, 0, SphereNumVertices);
        glDrawElements(GL_TRIANGLES, SphereNumIndices, GL_UNSIGNED_INT, nullptr);
        //Reverte o estado que nós criamos
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glDisableVertexAttribArray(0);
//        glDisableVertexAttribArray(1);
//        glDisableVertexAttribArray(2);


//        glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(ViewProjectionMatrix * ModelMatrix2));
//        glDrawElements(GL_TRIANGLES, SphereNumIndices, GL_UNSIGNED_INT, nullptr);


        //Como boa prática, será desabilitado o programa de shaders
        glUseProgram(0);

//Fim - TRIANGULO

        //Processa todos os eventos da fila de eventos do GLFW
        // Eventos podem ser eventos do teclado, mouse, gamepad...
        glfwPollEvents();

        //Envia o conteúdo do framebuffer da janela para ser desehado na tela
        glfwSwapBuffers(Window);

        //Processa os inputs do teclado
        if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Camera.MoveFoward(1.0f * DeltaTime);
        }
        if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Camera.MoveFoward(-1.0f * DeltaTime);
        }
        if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Camera.MoveRight(-1.0f * DeltaTime);
        }
        if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Camera.MoveRight(1.0f * DeltaTime);
        }
    }

//TRIANGULO
    //Desaloca o VertexBuffer
//    glDeleteBuffers(1, &VertexBuffer);
//Fim - TRIANGULO

    glDeleteVertexArrays(1, &QuadVao);

    //encerra biblioteca GLFW
    glfwTerminate();
	
	return 0;
}

*/
