#pragma once
#include "Common.hpp"

class Window;

namespace UI
{

class IMGUI;

class Updater
{
private:
    std::vector<std::shared_ptr<IMGUI>> uis;
    Window &window;
public:
    struct {
        bool lmbPress;
        bool mmbPress;
        bool rmbPress;
        bool lmbRepeat;
        bool mmbRepeat;
        bool rmbRepeat;
        u32 button;
        u32 action;
        glm::vec2 mousePosition; /// upper left corner is zero
        glm::vec2 relativeMousePosition; /// upper left corner is zero
        glm::vec2 mouseTranslation;
        glm::vec2 mouseTranslationNormalized;
    } mb {};

    u32 lastDt {};

    Updater(Window &window) : window(window){}
    void update(u32 dt);
    void setMouseAction(int key, int action);
    void setMousePosition(int x, int y);
    std::vector<std::shared_ptr<IMGUI>>& getUis(){
        return uis;
    }
    std::shared_ptr<IMGUI> createUi();
};


}
