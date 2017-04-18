#include "Common.hpp"
#include "BaseStructs.hpp"
#include "Style.hpp"
#include "IMGUI.hpp"
#include "UIGraphicComponent.hpp"

namespace UI {
    extern int g_UILayer;
}
void UIGraphicComponent::push(const UI::DrawFlags drawFlags, const HexColor color, const glm::vec4 &box, const u32 layer){
    boxes[layer].push_back({box, color});
}

void UIGraphicComponent::push(const u32 drawFlags, const UI::Style &style, HexColor color, const glm::vec4 &box, const u32 layer){
    boxes[layer].push_back({box, color});
}


void UIGraphicComponent::push(const UI::DrawFlags drawFlags, const Icon icon){
    // m_images.first[UI::g_UILayer].push_back(icon);
    m_images.first[0].push_back(icon);
}
int UIGraphicComponent::size(const UI::DrawFlags drawFlags){
    return boxes[0].size();
}

void UIGraphicComponent::change(const UI::DrawFlags drawFlags, const int idx, const HexColor color, const glm::vec4 &box, const u32 layer){
        boxes[layer][idx] = BoxGraphic {box, color};
}
