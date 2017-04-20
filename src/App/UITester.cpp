#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::LayoutVertical | UI::AlignLeft | UI::AlignTop);

    ui.button(150,20).color(color::white).text("AAA", "ui_12")().onLMB([]{log("A");});
    ui.button(150,20).color(color::white)().onLMB([]{log("B");});
    ui.button(150,20).color(color::white)().onLMB([]{log("C");});

    ui.endTable();
}
