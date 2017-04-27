#include "IMGUI.hpp"
#include "Yaml.hpp"
#include "Assets.hpp"
#include "Constants.hpp"

namespace UI {
void IMGUI::begin(){
    // draw = true;
    m_boxIndex = 0;
    layerToDraw = 0;
    tmpMaxLayer = 0;
    currentLayer = 0;
    m_force = false;
    m_boxStack[0].m_box = bounds;
    m_collRects.clear();
    this->captureMouse = false;
    if(updater.mb.lmbRepeat)
        accu = 0.f;
    if(accu > 1/frequency){
        accu = 0.f;
        counter = true;
    }
    if(accu2 > 0.5f){
        accu2 = 0.f;
        MS500OSCILATOR = !MS500OSCILATOR;
    }
    if(not updater.mb.lmbRepeat) item.currentSlider = nullptr;
    restoreDefaults();
}
void IMGUI::end(){
    if(m_boxIndex)
        error("box index error");
    m_key = -1;
    m_mouseKey = -1;
    m_force = false;
    drawLayer = layerToDraw;
    maxLayer = tmpMaxLayer;
    counter = false;
    m_collRects.clear();
}

void IMGUI::mouseKeyInput(int key, int action){
        if (action == GLFW_PRESS){
            m_mouseKey = key;
            m_mouseAction = action;
        }
    }
void IMGUI::keyInput(int key, int action, int mod){
    if (action == GLFW_PRESS){
        m_key = key;
        m_action = action;
        m_mod = mod;
    }
    if (textEditor.state())
        textEditor.input(key, action, mod);
}
bool IMGUI::keyAction(int key, bool *state){
    if (key == m_key && currentLayer >= drawLayer){
        *state = !*state;
        m_key = -1;
        return true;
    }
    return false;
}
bool IMGUI::keyAction(int key, std::function<void(void)>fun){
    // if(key == m_key && currentLayer >= drawLayer){
    if (key == m_key && currentLayer >= drawLayer){
        fun();
        m_key = -1;
        return true;
    }
    return false;
}
bool IMGUI::mouseKeyAction(int key, bool *state){
    if (key == m_mouseKey && currentLayer >= drawLayer){
        *state = !*state;
        m_mouseKey = -1;
        return true;
    }
    return false;
}

bool IMGUI::onRightClick(bool *state){
    return mouseKeyAction(GLFW_MOUSE_BUTTON_RIGHT, state);
}
bool IMGUI::onEnter(bool *state){
    return keyAction(GLFW_KEY_ENTER, state);
}
bool IMGUI::onESC(bool *state){
    return keyAction(GLFW_KEY_ESCAPE, state);
}
bool IMGUI::onESC(std::function<void(void)>fun){
    return keyAction(GLFW_KEY_ESCAPE, fun);
}
bool IMGUI::onEnter(std::function<void(void)>fun){
    return keyAction(GLFW_KEY_ENTER, fun);
}

bool IMGUI::tableHover(){
    return hasHover(fixRect(m_boxStack[m_boxIndex].m_box));
}

void IMGUI::onGroupHover(std::function<void(Box rect)>fun){
    if (hasHover(fixRect(m_boxStack[m_boxIndex].m_box)))
        fun(fixRect(m_boxStack[m_boxIndex].m_box));

}
void IMGUI::onGrouplClick(std::function<void(Box rect)>fun){
    if (hasHover(fixRect(m_boxStack[m_boxIndex].m_box))
        && updater.mb.lmbPress)
        fun(fixRect(m_boxStack[m_boxIndex].m_box));
}
bool IMGUI::onGroupGrab(std::function<void(Box rect)>fun){
    if (hasHover(fixRect(m_boxStack[m_boxIndex].m_box)) && updater.mb.lmbPress){
        fun(fixRect(m_boxStack[m_boxIndex].m_box));
        return true;
    }
    return false;
}
void IMGUI::setFont(const std::string &s){
    m_font = s;
}
void IMGUI::setfont(const std::string &s, int size){
    item.font = s;
    item.fontSize = size;
}

void IMGUI::forceClick(){
    m_force = true;
}

void IMGUI::switchDrawing(){
    // draw = !draw;
}

Box IMGUI::getBox(){
    return m_rects.back();
}

void IMGUI::drawOnlyTopLayer(){
    layerToDraw = std::max(currentLayer, layerToDraw);
}
void IMGUI::beginLayer(){
    currentLayer++;
    tmpMaxLayer = std::max(tmpMaxLayer, currentLayer);
}
void IMGUI::endLayer(){
    currentLayer--;
}

void IMGUI::border(int borderSize){
    // m_groupStack[m_groupIndex].m_border = borderSize;
    m_boxStack[m_boxIndex].m_border = borderSize;
}
void IMGUI::indentation(int indentationLen){
    // m_groupStack[m_groupIndex].m_border = borderSize;
    // item.indentation = indentationLen;
}
void IMGUI::updateCounter(float deltaTime){
    accu += deltaTime;
    accu2 += deltaTime;
}
void IMGUI::updateCounter(uint32_t deltaTime){
    timeFromstart += deltaTime;
}
void IMGUI::setDefaultFont(std::string font, int size){
    m_font = font;
    m_defaultFont = font;
    m_defaultFontSize = size;
}

bool IMGUI::hover(){
    return item.hover;
}
bool IMGUI::overlay(Box &rect, Box &point){
    return     point.x >= rect.x && point.x <= rect.x+rect.z &&
                    point.y >= rect.y && point.y <= rect.y+rect.w;
}
bool IMGUI::hasHover(Box rect){
    if(currentLayer == maxLayer)
        return     (updater.mb.mousePosition.x >= rect.x && updater.mb.mousePosition.x < rect.x+rect.z &&
                    updater.mb.mousePosition.y >= rect.y && updater.mb.mousePosition.y < rect.y+rect.w) && !captureMouse;
    return false;
}
bool IMGUI::outOfTable(){
    auto &&rect = fixRect(m_boxStack[m_boxIndex+1].m_box);
    // if(currentLayer == maxLayer)
        return this->updater.mb.lmbPress && !(updater.mb.mousePosition.x >= rect.x && updater.mb.mousePosition.x < rect.x+rect.z &&
                    updater.mb.mousePosition.y >= rect.y && updater.mb.mousePosition.y < rect.y+rect.w);


}
Box IMGUI::fixRect(Box rect){
    if(rect.z < 0.f)
        rect = Box(rect.x+rect.z, rect.y, -rect.z, rect.w);
    if(rect.w < 0.f)
        rect = Box(rect.x, rect.y+rect.w, rect.z, -rect.w);

    return rect;
}

}
