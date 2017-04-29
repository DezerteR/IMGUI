#include "Style.hpp"
namespace UI
{

Style::Style(){
    colors[Button & None] = 0x22'7f'bb'40;
    colors[Button & Hover] = 0x2c'96'dd'40;
    colors[Button & Press] = 0xec'f0'f1'a0;
    colors[Label & None] = 0xec'4c'37'40;
    colors[Label & Hover] = 0x90'90'90'40;
    colors[Editbox & None] = 0x90'90'90'40;
    colors[Editbox & Hover] = 0x90'90'90'40;
    colors[Editbox & Press] = 0x90'90'90'40;
    colors[Editbox & Active] = 0x90'90'90'40;
    colors[Box & None] = 0x01'30'01'40;
    colors[Background] = 0x0'0'0'40;
    colors[Slider & None] = 0x81'87'87'a0;
    colors[Slider & Hover] = 0xa7'a7'a7'40;
    colors[Slider & Press] = 0xf7'f7'f7'40;
    colors[Slide & None] = 0x90'90'90'f0;
    colors[Slide & Hover] = 0x90'90'90'40;
    colors[Slide & Press] = 0x90'90'90'40;
    colors[Image & None] = 0x90'90'90'40;
    colors[Image & Hover] = 0x90'90'90'40;
    colors[Image & Press] = 0x90'90'90'40;
    colors[Font] = 0xff'ff'ff'ff;
}
}
