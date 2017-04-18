#pragma once
#include "Common.hpp"
#include "Utils.hpp"
#include "Style.hpp"

namespace UI {
    enum DrawFlags : int;
    class IMGUI;
};

struct BoxColor {
    std::vector <HexColor>  m_color;
    std::vector <glm::vec4> m_box;
    void push(glm::vec4 box, HexColor color){
        m_box.push_back(box);
        m_color.push_back(color);
    }
};

struct BoxGraphic
{
    glm::vec4 positionSize;
    HexColor color;
    HexColor borderColor;
};

class UIGraphicComponent
{
private:
    UI::Style &style;
public:
    UIGraphicComponent(UI::Style &style) : style(style){
        boxes[0].reserve(100);
        boxes[1].reserve(10);
    }
    void push(const UI::DrawFlags drawFlags, const HexColor color, const glm::vec4 &box, const u32 layer);
    void push(const u32 drawFlags, const UI::Style &style, HexColor color, const glm::vec4 &box, const u32 layer = 0);
    void push(const UI::DrawFlags drawFlags, const Icon icon);
    void draw(UI::IMGUI &gui, u32 layer){}
    void change(const UI::DrawFlags drawFlags, const int idx, const HexColor color, const glm::vec4 &box, const u32 layer);
    int size(const UI::DrawFlags drawFlags);
    bool empty(u32 layer){
        return false;
    }

    std::vector<BoxGraphic> boxes[3];
    std::vector<Icon> images[3];

    std::pair<std::vector <Icon>[3], std::string> m_images;
    std::pair<BoxColor[3], std::string> m_backgroundBox;
    std::pair<BoxColor[3], std::string> m_editBox;
    std::pair<BoxColor[3], std::string> m_label;
    std::pair<BoxColor[3], std::string> m_boxes;
};
