#pragma once
#include "Common.hpp"
#include "Utils.hpp"
#include "Style.hpp"

namespace UI {
    enum DrawFlags : int;
    class IMGUI;
};
struct IUIContainer {
    virtual void push(const UI::DrawFlags drawFlags, const HexColor color, const glm::vec4 &box, const u32 layer) = 0;
    virtual void push(const u32 drawFlags, const UI::Style &style, HexColor color, const glm::vec4 &box, const u32 layer) = 0;
    virtual void push(const UI::DrawFlags drawFlags, const Icon icon) = 0;
    virtual void draw(UI::IMGUI &gui, u32 layer) = 0;
    virtual void change(const UI::DrawFlags drawFlags, const int idx, const HexColor color, const glm::vec4 &box) = 0;
    virtual int size(const UI::DrawFlags drawFlags) = 0;
    virtual bool empty(u32 layer) = 0;
    virtual ~IUIContainer(){}
};

struct BoxColor {
    std::vector <HexColor>  m_color;
    std::vector <glm::vec4> m_box;
    void push(glm::vec4 box, HexColor color){
        m_box.push_back(box);
        m_color.push_back(color);
    }
};
struct UIContainer : public IUIContainer {
    void push(const UI::DrawFlags drawFlags, const HexColor color, const glm::vec4 &box, const u32 layer);
    void push(const u32 drawFlags, const UI::Style &style, HexColor color, const glm::vec4 &box, const u32 layer = 0);
    void push(const UI::DrawFlags drawFlags, const Icon icon);
    void draw(UI::IMGUI &gui, u32 layer){}
    void change(const UI::DrawFlags drawFlags, const int idx, const HexColor color, const glm::vec4 &box);
    int size(const UI::DrawFlags drawFlags);
    bool empty(u32 layer){
        return false;
    }

    UIContainer(){
        m_images.second = "UIIcon2D";
        m_backgroundBox.second = "UIButton";
        m_editBox.second = "UIButton";
        m_label.second = "UIButton";
        m_boxes.second = "UIButton";
    }

    std::pair<std::vector <Icon>[3], std::string> m_images;
    std::pair<BoxColor[3], std::string> m_backgroundBox;
    std::pair<BoxColor[3], std::string> m_editBox;
    std::pair<BoxColor[3], std::string> m_label;
    std::pair<BoxColor[3], std::string> m_boxes;
};
