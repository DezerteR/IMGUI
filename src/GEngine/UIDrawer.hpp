#pragma once
#include "Common.hpp"

class Context;
class Window;
namespace UI {class IMGUI;}

class UIDrawer
{
    Window &window;
    Context &context;
public:
    UIDrawer(Window &window, Context &context) : window(window), context(context){}
    void renderUis(std::vector<std::shared_ptr<UI::IMGUI>> &uis);

    void renderBoxes(UI::IMGUI &ui, u32 layer);
    void renderImages(UI::IMGUI &ui, u32 layer);
    void renderFonts(UI::IMGUI &ui, u32 layer);
};
