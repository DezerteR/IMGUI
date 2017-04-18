#include "gl_core_3_3.hpp"
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Logging.hpp"

Window::Window(u32 w, u32 h, const std::string &name) : size(w, h), name(name){}
bool Window::init(){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (glfwInit() != 1){
        error("glfw init fail");
        return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, gl::FALSE_);
    // glfwWindowHint(GLFW_DECORATED, gl::FALSE_);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenSize.x = mode->width;
    screenSize.y = mode->height;

    size.x = std::min((int)size.x, mode->width);
    size.y = std::min((int)size.y, mode->height);

    window = glfwCreateWindow(size.x, size.y, "", nullptr, nullptr);
    glfwSetWindowPos(window, screenSize.x/2 - size.x/2, screenSize.y/2 - size.y/2);
    glfwHideWindow(window);

    if(!window){
        error("creating window fail");
        glfwTerminate();
        return false;
    }
    glfwSetWindowTitle(window, name.c_str());
    glfwMakeContextCurrent(window);

    pixelSize = 1.f/size;
    center = size * 0.5f;
    aspect = size.x/size.y;

    viewport = glm::vec4(0, 0, size.x, size.y);

    gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
    if(!didLoad){
        error("GL init fail");
        return false;
    }
    return true;

    return true;
}

void Window::showWindow(){
    glfwSetWindowSize(window, size.x, size.y);
    glfwSetWindowPos(window, screenSize.x/2 - size.x/2, screenSize.y/2 - size.y/2);
    glfwShowWindow(window);
}
