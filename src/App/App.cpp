#include <GLFW/glfw3.h>

#include "Common.hpp"
#include "Window.hpp"
#include "Logging.hpp"
#include "Timer.hpp"
#include "UIUpdater.hpp"
#include "Renderer.hpp"
#include "UIDrawer.hpp"
#include "RendererUtils.hpp"
#include "UITester.hpp"
#include "IMGUI.hpp"
#include "YAML.hpp"
#include "ResourceLoader.hpp"

bool CLOG_SPECIAL_VALUE = false;

class App
{
public:
    Window window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<UI::Updater> uiUpdater;
    bool quit;

    Yaml settings;
    Yaml resources;

    App() : window(1600, 900, "IMGUI"){}

    void initialize(){
        uiUpdater = std::make_unique<UI::Updater>(window);
        renderer = std::make_unique<Renderer>(window);

        glfwSetWindowUserPointer(window.window, this);
    }
    void loadResources(){
        settings = loadYaml("../Settings.yml");
        resources = loadYaml("../GameResources.yml");

        ResourceLoader loader;
        loader.loadShaders();
        loader.loadResources(resources);
        loader.loadFonts(resources);
    }

    void run(){
        window.showWindow();
        log("starting main loop");
        Timer<u32, 1000, 1> msecLoopDeltaTime;
        auto ui = uiUpdater->createUi();
        UITester tester(*ui);
        while(not quit){
            auto dt = msecLoopDeltaTime();
            uiUpdater->update(dt);
            ui->begin();

            glfwPollEvents();

            tester.run();

            ui->end();

            render();
            CLOG_SPECIAL_VALUE = false;
        }
    }
    void render(){
        renderer->beginFrame();
        renderer->utils->drawBackground("pmk");
        // renderer->utils->drawBackground("city");
        renderer->utils->blurBuffer();
        // renderer->utils->renderBlurred();
        renderer->uiDrawer->renderUis(uiUpdater->getUis());
        renderer->endFrame();
        glfwSwapBuffers(window.window);
    }
    void keyCallback(int key, int scancode, int action, int mods){
        if(key == GLFW_KEY_ESCAPE) quit = true;
        if(key == 'R' && action == GLFW_PRESS){
            ResourceLoader loader;
            log("Reloading shaders");
            auto &&shadersToReload = loadYaml("../ShadersToReload.yml");
            for(auto &it : shadersToReload){
                loader.reloadShader(it.string());
            }
        }
    }
    void mouseButtonCallback(int button, int action, int mods){
        uiUpdater->setMouseAction(button, action);
    }
    void cursorPosCallback(double xpos, double ypos){
        uiUpdater->setMousePosition(xpos, ypos);
    }
};

void scrollCallback(GLFWwindow *window, double dx, double dy){
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    ((App*)glfwGetWindowUserPointer(window))->keyCallback(key, scancode, action, mods);
    	if(key == GLFW_KEY_F12 && action == GLFW_PRESS){
		CLOG_SPECIAL_VALUE = true;
    }
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    ((App*)glfwGetWindowUserPointer(window))->mouseButtonCallback(button, action, mods);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
    ((App*)glfwGetWindowUserPointer(window))->cursorPosCallback(xpos, ypos);
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
    app.loadResources();
    app.run();

    return 0;
}
