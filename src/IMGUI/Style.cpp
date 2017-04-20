#include "Style.hpp"
namespace UI
{

Style::Style(){
    colors[Button & None] = 0x22'7f'bb'ff;
    colors[Button & Hover] = 0x2c'96'dd'ff;
    colors[Button & Press] = 0xec'f0'f1'ff;
    colors[Label & None] = 0xec'4c'37'ff;
    colors[Label & Hover] = 0x07'07'07'ff;
    colors[Editbox & None] = 0x07'07'07'ff;
    colors[Editbox & Hover] = 0x07'07'07'ff;
    colors[Editbox & Press] = 0x07'07'07'ff;
    colors[Editbox & Active] = 0x07'07'07'ff;
    colors[Box & None] = 0x07'07'07'ff;
    colors[Background] = 0x07'07'07'ff;
    colors[Slider & None] = 0x07'07'07'ff;
    colors[Slider & Hover] = 0x07'07'07'ff;
    colors[Slider & Press] = 0x07'07'07'ff;
    colors[Slide & None] = 0x07'07'07'ff;
    colors[Slide & Hover] = 0x07'07'07'ff;
    colors[Slide & Press] = 0x07'07'07'ff;
    colors[Image & None] = 0x07'07'07'ff;
    colors[Image & Hover] = 0x07'07'07'ff;
    colors[Image & Press] = 0x07'07'07'ff;
    colors[Font] = 0x10'10'10'ff;
}
}
