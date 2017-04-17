#pragma once
namespace UI{

struct Style {
    HexColor background;
    HexColor button;
    HexColor hover; // tyczy siê jeno buttona i edita
    HexColor editBox;
    HexColor fontColor;
    HexColor imageColor;
    HexColor label;
    HexColor fancy[4];
    int padding;
    std::string font;

};
enum class StyleID : int32_t{
    Basic = 0,
    Light = 1,
    Dark = 2,
    Label = 3,
};

enum DrawFlags : int {
    Label = 0x1,
    Hover = 0x2,
    Editable = 0x4,
    CaptureMouse = 0x8,
    Image = 0x10,
    BigBox = 0x20,

    Hoverable = CaptureMouse | Hover,
    EditBox = Editable | CaptureMouse,
    Button = Label | Hover | CaptureMouse,
};
}
