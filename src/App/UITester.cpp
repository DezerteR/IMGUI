#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::LayoutVertical | UI::AlignLeft | UI::AlignTop | UI::Draw)
      .border(3)
      .overridePosition(0, 350);

    ui.button(400,20).color(color::white).text("Lorem ipsum dolor sit amet, consectetur adipiscing elit,", "ui_12").onLMB([]{log("A");})();
    ui.button(400,20).color(color::white).text("sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ", "ui_12").onLMB([]{log("B");})();
    ui.button(400,20).color(color::white).text("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris ", "ui_12").onLMB([]{log("B");})();
    ui.button(400,20).color(color::white).text("nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in ", "ui_12").onLMB([]{log("B");})();
    ui.button(400,20).color(color::white).text("reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.", "ui_12").onLMB([]{log("B");})();
    ui.button(400,20).color(color::white).text("", "ui_12").onLMB([]{log("B");})();
    ui.button(400,20).color(color::white).text("", "ui_12").onLMB([]{log("B");})();
    ui.button(400,20).color(color::white).text("f"+toString(f), "ui_12").onLMB([]{log("C");})();
    ui.rect(400, 20).slider(f, 0, 30)();
    ui.rect(400, 20).slider(i, 0, 30)();

    ui.endTable();
}
