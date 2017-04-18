#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"

void UITester::run(){
    ui.table(UI::LayoutVertical | UI::AlignLeft | UI::AlignTop);

    ui.button(150,20).color(color::white)();
    ui.button(150,20).color(color::white)();
    ui.button(150,20).color(color::white)();

    ui.endTable();
}
