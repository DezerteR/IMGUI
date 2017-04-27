#pragma once

namespace UI {
    class IMGUI;
};

class UITester
{
private:
    UI::IMGUI &ui;
    float f {0};
    int i {220};
    int caretPosition {-2};
public:
    UITester(UI::IMGUI &ui) : ui(ui){}
    void run();
};
