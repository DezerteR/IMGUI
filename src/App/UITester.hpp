#pragma once

namespace UI {
    class IMGUI;
};

class UITester
{
private:
    UI::IMGUI &ui;
public:
    UITester(UI::IMGUI &ui) : ui(ui){}
};
