#ifndef GRAPHICS_CORE_HPP
#define GRAPHICS_CORE_HPP

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void BlueDjinn_GraphicsCore_Init();
int BlueDjinn_GraphicsCore_CreateWindow();
int BlueDjinn_GraphicsCore_RenderWindow(void (*BlueDjinn_GraphicCore_Render)());

#endif // GRAPHICS_CORE_HPP
