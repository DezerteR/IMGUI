#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::Vertical | UI::ToLeft | UI::ToTop | UI::Draw)
      .border(1)
      .pxSize(500, -300);
    //   .overridePosition(100, 550);

    ui.button(i,30).color(color::white).text("Lorem ipsum dolor sit amet, consectetur adipiscing elit,").onLMB([]{log("A");})();
    ui.button(i,30).color(color::white).text("sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris ").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in ").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("}{bdS9fyHZ", font::Default).onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("S9608qgh3kpi", font::Default, font::Center, caretPosition).onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("f"+toString(f), font::Default, font::Center).onLMB([]{log("C");})();
    ui.rect(250, 30).slider(f, 0, 30)();
    ui.rect(250, 30).slider(i, 200, 400)();
    ui.rect(250, 30).slider(caretPosition, -2, 15).text(toString(caretPosition, font::Default, font::Center))();

    ui.endTable();
}
