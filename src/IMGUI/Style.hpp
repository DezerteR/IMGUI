#pragma once
#include <string>
namespace UI{

struct Style
{
    std::unordered_map<int, u32> colors;
    Style(){
        // colors[Button & None] =
        // colors[Button & Hover] =
        // colors[Button & Press] =
        // colors[Label & None] =
        // colors[Label & Hover] =
        // colors[Editbox & None] =
        // colors[Editbox & Hover] =
        // colors[Editbox & Press] =
        // colors[Editbox & Active] =
        // colors[Box & None] =
    }

    enum type {
        Background = 0b0001,
        Label = 0b0010,
        Button = 0b0111,
        Editbox = 0b1111,
    };
    enum action {
        None, Hover, Pressed
    };

    // enum hash_key {
    //     /// Items
    //     Button =  0b11110001,
    //     Label =   0b11010010,
    //     Editbox = 0b11110100,
    //     Box =     0b00011000,
    //     /// Actions
    //     None =    0b00011111,
    //     Hover =   0b00101111,
    //     Press =   0b01001111,
    //     Active =  0b10001111,

    // };

    int padding;
    struct {
        HexColor color;
    } background;
    struct {
        HexColor color;
        HexColor hovered;
        HexColor pressed;
    } button;
    struct {
        HexColor color;
        HexColor hovered;
    } label;
    struct {
        HexColor color;
        HexColor hovered;
        HexColor pressed;
        HexColor active;
    } editbox;
    struct {
        HexColor color;
    } image;
    struct {
        std::string name;
        HexColor color;
    } font;
    std::string shader;
};
enum class StyleID : int32_t
{
    Basic = 0,
    Light = 1,
    Dark = 2,
    Label = 3,
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
