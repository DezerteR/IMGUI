#include "Common.hpp"
#include "BaseStructs.hpp"
#include "Style.hpp"
#include "IMGUI.hpp"
#include "UIContainer.hpp"

namespace UI {
    extern int g_UILayer;
}
void UIContainer::push(const UI::DrawFlags drawFlags, const HexColor color, const glm::vec4 &box, const u32 layer){
        m_label.first[layer].push(box, color);
}

void UIContainer::push(const u32 drawFlags, const UI::Style &style, HexColor color, const glm::vec4 &box, const u32 layer){
    // color = 0;
    if(drawFlags & UI::BigBox){
        m_backgroundBox.first[layer].push(box, color ? color : style.background);
        return;
    }
    if(drawFlags & UI::Hover){
        color = style.hover;
    }

    if(drawFlags & UI::Label){
        m_label.first[layer].push(box, color ? color : style.label);
    }
    else if((drawFlags & UI::Button) == UI::Button){
        m_boxes.first[layer].push(box, color ? color : style.button);
    }
    else if((drawFlags & UI::EditBox) == UI::EditBox){
        m_editBox.first[layer].push(box, color ? color : style.editBox);
    }
    else if(drawFlags != 0){
        m_label.first[layer].push(box, color ? color : style.background);
    }
}


void UIContainer::push(const UI::DrawFlags drawFlags, const Icon icon){
    // m_images.first[UI::g_UILayer].push_back(icon);
    m_images.first[0].push_back(icon);
}
int UIContainer::size(const UI::DrawFlags drawFlags){
    return m_backgroundBox.first[0].m_box.size();
}

void UIContainer::change(const UI::DrawFlags drawFlags, const int idx, const HexColor color, const glm::vec4 &box){
        m_backgroundBox.first[0].m_box[idx] = box;
        m_backgroundBox.first[0].m_color[idx] = color;
}
