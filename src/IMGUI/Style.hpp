#pragma once
#include "Common.hpp"

namespace UI{

struct Style
{
    std::unordered_map<int, u32> colors;
    Style();

    enum hash_key {
        /// Items
        Button =  0b1111'00000'0001,
        Label =   0b1101'00000'0010,
        Editbox = 0b1111'00000'0100,
        Box =     0b0001'00000'1000,
        Background = 0b0001'00001'0000,
        Slider =  0b1111'00010'0000,
        Slide =   0b1111'00100'0000,
        Image =   0b1111'01000'0000,
        Font =    0b0001'10000'0000,
        /// Actions
        None =    0b0000'11111'1111,
        Hover =   0b0010'11111'1111,
        Press =   0b0100'11111'1111,
        Active =  0b1000'11111'1111,
    };

    int padding;
    struct {
        std::string name;
        HexColor color;
    } font;
    std::string shader;
};

enum DrawFlags : int
{
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
