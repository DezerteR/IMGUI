#include <GLFW/glfw3.h>

#include "Common.hpp"
#include "Window.hpp"
#include "Logging.hpp"
#include "Timer.hpp"
#include "UIUpdater.hpp"
#include "Renderer.hpp"
#include "UIDrawer.hpp"

class App
{
public:
    Window window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<UI::Updater> uiUpdater;
    bool quit;
    App() : window(1200, 600, "IMGUI"){}

    void initialize(){
        uiUpdater = std::make_unique<UI::Updater>(window);
        renderer = std::make_unique<Renderer>(window);

        glfwSetWindowUserPointer(window.window, this);
    }

    void run(){
        window.showWindow();
        log("starting main loop");
        Timer<u32, 1000, 1> msecLoopDeltaTime;
        while(not quit){
            auto dt = msecLoopDeltaTime();
            uiUpdater->update(dt);

            glfwPollEvents();
            render();
        }
    }
    void render(){
        renderer->uiDrawer->renderUis(uiUpdater->getUis());
        glfwSwapBuffers(window.window);
    }
};

void scrollCallback(GLFWwindow *window, double dx, double dy){
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    // CTRL_MODE = mods &  GLFW_MOD_CONTROL;
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    ((App*)glfwGetWindowUserPointer(window))->uiUpdater->setMouseAction(button, mods);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
    ((App*)glfwGetWindowUserPointer(window))->uiUpdater->setMousePosition(xpos, ypos);
}
void exitCallback(GLFWwindow *window){
    ((App*)glfwGetWindowUserPointer(window))->quit = true;
    log("[ Exit ]");
}

int main(){
    App app;

    if(not app.window.init()) return 1;
    {
        glfwSetScrollCallback(app.window.window, scrollCallback);
        glfwSetKeyCallback(app.window.window, keyCallback);
        glfwSetMouseButtonCallback(app.window.window, mouseButtonCallback);
        glfwSetCursorPosCallback(app.window.window, cursorPosCallback);
        glfwSetWindowCloseCallback(app.window.window, exitCallback);
    }

    app.initialize();
    app.run();

    return 0;
}
