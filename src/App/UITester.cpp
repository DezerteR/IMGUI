#include "UITester.hpp"
#include "IMGUI.hpp"
#include "Color.hpp"
#include "Logging.hpp"

void UITester::run(){
    ui.table(UI::LayoutVertical | UI::AlignLeft | UI::AlignTop | UI::Draw)
      .border(1)
      .size(500, -300)
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

// void dupa(){
//     // jaa, dało
//     auto table = ui.table(Floating, Left, Top, Vertical, from(20, 50));
//     table.button().text("dupa")();
//     auto box = table.box(Vertical or Horizontal); /// tu już nie ma więcej ustawień, no, może rozmiar jeszcze
//     /// będzie czytelniej, ale będzie trudniej zagłębienia robić
//     box.label("box 1")();
//     box.button().text("nuuda")();
//     box.button().text("nuuda")();
//     box.button().text("nuuda")();

//     box = table.box(Vertical);
//     box.label("box 2")();
//     box.button().text("zabawa")();
//     box.button().text("zabawa")();
//     box.button().text("zabawa")();


// }
