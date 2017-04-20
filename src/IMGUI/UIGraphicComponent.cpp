#include "Common.hpp"
#include "BaseStructs.hpp"
#include "Style.hpp"
#include "IMGUI.hpp"
#include "UIGraphicComponent.hpp"

namespace UI {
    extern int g_UILayer;
}
void UIGraphicComponent::push(const int item, const HexColor color, const glm::vec4 &box, const u32 layer){
    if(not item) return;
    boxes[layer].push_back({box, style.colors[item]});
}
void UIGraphicComponent::push(const int item, const glm::vec4 &box, const u32 layer){
    if(not item) return;
    boxes[layer].push_back({box, style.colors[item]});
}

void UIGraphicComponent::push(const int item, const UI::Style &styl651e, HexColor color, const glm::vec4 &box, const u32 layer){
    if(not item) return;
    boxes[layer].push_back({box, style.colors[item]});
}


void UIGraphicComponent::push(const int item, const Icon icon){
    // m_images.first[UI::g_UILayer].push_back(icon);
    m_images.first[0].push_back(icon);
}
int UIGraphicComponent::size(const int item){
    return boxes[0].size();
}

void UIGraphicComponent::change(const int item, const int idx, const glm::vec4 &box, const u32 layer){
        boxes[layer][idx] = BoxGraphic {box, style.colors[item]};
}
