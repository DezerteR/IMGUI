#include <GLFW/glfw3.h>
#include "UIUpdater.hpp"
#include "Window.hpp"
#include "IMGUI.hpp"

namespace UI
{

void Updater::update(u32 dt){
    mb.lmbPress = false;
    mb.mmbPress = false;
    mb.rmbPress = false;
    lastDt = dt;
    for(auto &ui : uis){
        // ui->update();
    }
}
void Updater::setMouseAction(int key, int action){
    mb.button = key;
    mb.action = action;
    if(action == GLFW_PRESS){
        if(key == GLFW_MOUSE_BUTTON_LEFT){
            mb.lmbPress = true;
            mb.lmbRepeat = true;
        }
        else if(key == GLFW_MOUSE_BUTTON_MIDDLE){
            mb.mmbPress = true;
            mb.mmbRepeat = true;
        }
        else if(key ==  GLFW_MOUSE_BUTTON_RIGHT){
            mb.rmbPress = true;
            mb.rmbRepeat = true;
        }
    }
    else if(action == GLFW_RELEASE){
        if(key == GLFW_MOUSE_BUTTON_LEFT) mb.lmbRepeat = false;
        else if(key == GLFW_MOUSE_BUTTON_MIDDLE) mb.mmbRepeat = false;
        else if(key ==  GLFW_MOUSE_BUTTON_RIGHT) mb.rmbRepeat = false;
    }
}
void Updater::setMousePosition(int x, int y){
    mb.mouseTranslation = glm::vec2(x, y) - mb.mousePosition;
    mb.mouseTranslationNormalized = mb.mouseTranslation / window.size * 0.5f;

    mb.mousePosition = glm::vec2(x, window.size.y - y);
    mb.relativeMousePosition = glm::vec2(x, y)/window.size;
}

std::shared_ptr<IMGUI> Updater::createUi(){
    uis.emplace_back(std::make_shared<IMGUI>(glm::vec4(0,0, window.size), *this));
    return uis.back();
}

}
