#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::LayoutVertical | UI::AlignLeft | UI::AlignTop | UI::Draw)
      .border(1)
      .size(500, -300)
      .overridePosition(100, 550);

    // ui.button(400,20).color(color::white).text("Lorem ipsum dolor sit amet, consectetur adipiscing elit,").onLMB([]{log("A");})();
    // ui.button(400,20).color(color::white).text("sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ").onLMB([]{log("B");})();
    // ui.button(400,20).color(color::white).text("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris ").onLMB([]{log("B");})();
    // ui.button(400,20).color(color::white).text("nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in ").onLMB([]{log("B");})();
    // ui.button(400,20).color(color::white).text("reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.").onLMB([]{log("B");})();
    ui.button(400,80).color(color::white).text("}{bdS9fyHZ", font::HUD+56).onLMB([]{log("B");})();
    ui.button(400,80).color(color::white).text("No i chuj by to strzelil", font::HUD+56).onLMB([]{log("B");})();
    ui.button(400,80).color(color::white).text("S9608qgh3kpi", font::HUD+56).onLMB([]{log("B");})();
    ui.button(400,80).color(color::white).text("f"+toString(f), font::HUD+56).onLMB([]{log("C");})();
    ui.rect(400, 80).slider(f, 0, 30)();
    ui.rect(400, 80).slider(i, 0, 30)();

    ui.endTable();
}
