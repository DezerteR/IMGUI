#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::Vertical | UI::ToLeft | UI::ToBottom | UI::Draw)
      .border(5)
      .size(UI::Px(500));
        ui.rect(UI::Px(30)).slider(f, -500, 500).text("f" + toString(f), font::Default, font::Center)();
        ui.box(UI::Horizontal).size(UI::Rel(1));
            ui.box(UI::Vertical).size(UI::Rel(0.5));
                ui.label(UI::Px(30)).text("absa")();
                ui.label(UI::Px(30)).text("iltut")();
            ui.endBox();
            ui.box(UI::Vertical).size(UI::Rel(0.5));
                ui.label(UI::Px(30)).text("agrds")();
                ui.label(UI::Px(30)).text("hdf")();
                ui.label(UI::Px(30)).text("hdf")();
            ui.endBox();
        ui.endBox();
        ui.label(UI::Px(30)).text("blagga")();
    ui.endTable();

    ui.table(UI::Vertical | UI::ToLeft | UI::ToTop | UI::Draw)
      .border(1)
      .pos((int)f, -1)
      .size(500, -300);

    ui.button(i,30).color(color::white).text("Lorem ipsum dolor sit amet, consectetur adipiscing elit,").onLMB([]{log("A");})();
    ui.button(i,30).color(color::white).text("sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris ").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in ").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.").onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("}{bdS9fyHZ", font::Default).onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("S9608qgh3kpi", font::Default, font::Center, caretPosition).onLMB([]{log("B");})();
    ui.button(i,30).color(color::white).text("f"+toString(f), font::Default, font::Center).onLMB([]{log("C");})();
    // ui.rect(250, 30).slider(f, -500, 500)();
    // ui.rect(250, 30).slider(i, 200, 400)();
    // ui.rect(250, 30).slider(caretPosition, -2, 15).text(toString(caretPosition), font::Default, font::Center)();

    ui.endTable();
}
