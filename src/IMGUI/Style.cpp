#include "Style.hpp"
namespace UI
{

Style::Style(){
    colors[Button & None] = 0x22'7f'bb'70;
    colors[Button & Hover] = 0x2c'96'dd'70;
    colors[Button & Press] = 0xec'f0'f1'a0;
    colors[Label & None] = 0xec'4c'37'70;
    colors[Label & Hover] = 0x90'90'90'70;
    colors[Editbox & None] = 0x90'90'90'70;
    colors[Editbox & Hover] = 0x90'90'90'70;
    colors[Editbox & Press] = 0x90'90'90'70;
    colors[Editbox & Active] = 0x90'90'90'70;
    colors[Box & None] = 0x90'90'90'70;
    colors[Background] = 0x90'90'90'70;
    colors[Slider & None] = 0x81'87'87'a0;
    colors[Slider & Hover] = 0xa7'a7'a7'70;
    colors[Slider & Press] = 0xf7'f7'f7'70;
    colors[Slide & None] = 0x90'90'90'f0;
    colors[Slide & Hover] = 0x90'90'90'70;
    colors[Slide & Press] = 0x90'90'90'70;
    colors[Image & None] = 0x90'90'90'70;
    colors[Image & Hover] = 0x90'90'90'70;
    colors[Image & Press] = 0x90'90'90'70;
    colors[Font] = 0x10'10'10'aa;
}
}
