#pragma once
#include "Common.hpp"

class GLFWwindow;

class Window
{
public:
    glm::vec2 size;
    glm::vec2 screenSize;
    glm::vec2 pixelSize;
    glm::vec2 center;
    glm::vec4 viewport;
    float aspect;

    bool quit {false};

    std::string name;
    GLFWwindow *window;

    Window(u32 w, u32 h, const std::string &name);
    bool init();
    void showWindow();
};
