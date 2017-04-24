#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::LayoutVertical | UI::AlignLeft | UI::AlignTop | UI::Draw)
      .border(3)
      .overridePosition(100, 350);

    ui.button(200,20).color(color::white).text("AAA", "ui_12").onLMB([]{log("A");})();
    ui.button(200,20).color(color::white).onLMB([]{log("B");})();
    ui.button(200,20).color(color::white).onLMB([]{log("B");})();
    ui.button(200,20).color(color::white).onLMB([]{log("B");})();
    ui.button(200,20).color(color::white).onLMB([]{log("B");})();
    ui.button(200,20).color(color::white).onLMB([]{log("B");})();
    ui.button(200,20).color(color::white).onLMB([]{log("B");})();
    ui.button(200,20).color(color::white).text("f"+toString(f), "ui_12").onLMB([]{log("C");})();
    ui.rect(200, 20).slider(f, 0, 30)();
    ui.rect(200, 20).slider(i, 0, 30)();

    ui.endTable();
}
