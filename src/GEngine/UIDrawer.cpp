#include "UIDrawer.hpp"
#include "Context.hpp"
#include "IMGUI.hpp"
#include "UIGraphicComponent.hpp"
#include "Assets.hpp"

void UIDrawer::renderUis(std::vector<std::shared_ptr<UI::IMGUI>> &uis){
    gl::DepthMask(gl::FALSE_);
    gl::Disable(gl::DEPTH_TEST);
    gl::Disable(gl::CULL_FACE);

    gl::Enable(gl::BLEND);
    gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

    for(auto &ui : uis){
        // for(auto i=0; i<ui.layers; i++)
        u32 i = 0;
        renderBoxes(*ui, i);
        renderImages(*ui, i);
        renderFonts(*ui, i);
    }
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);

    gl::DisableVertexAttribArray(1);
    gl::DisableVertexAttribArray(2);
    gl::DisableVertexAttribArray(3);
    gl::DisableVertexAttribArray(4);
    gl::DepthMask(1);
    gl::Disable(gl::BLEND);
    gl::Enable(gl::CULL_FACE);
    context.errors();
}

void UIDrawer::renderBoxes(UI::IMGUI &ui, u32 layer){
    auto shader = assets::getShader("DrawBox");
    shader.bind();
    shader.uniform("uWidth", window.size.x);
    shader.uniform("uHeight", window.size.y);

    clog(shader.ID, "Render", ui.m_uiGraphic.boxes[layer].size(), "boxes, at ", ui.m_uiGraphic.boxes[layer][0].positionSize);

    context.shape.quadCorner.bind().attrib(0).pointer_float(4).divisor(0);
    context.getRandomBuffer().update(ui.m_uiGraphic.boxes[layer])
        .attrib(1).pointer_float(4, sizeof(BoxGraphic), (void*)offsetof(BoxGraphic, positionSize)).divisor(1)
        .attrib(2).pointer_color(sizeof(BoxGraphic), (void*)offsetof(BoxGraphic, color)).divisor(1)
        .attrib(3).pointer_color(sizeof(BoxGraphic), (void*)offsetof(BoxGraphic, borderColor)).divisor(1);

    gl::DrawArraysInstanced(gl::TRIANGLE_STRIP, 0, 4, ui.m_uiGraphic.boxes[layer].size()); /// TODO: check if simple draw arrays wouldn't be faster

    ui.m_uiGraphic.boxes[layer].clear();

    context.errors();
}

void UIDrawer::renderImages(UI::IMGUI &ui, u32 layer){
    // auto shader = assets::getShader("DrawIcon");
    // shader.bind();
    // shader.texture("uTexture", ui.m_imageSet->ID);
    // shader.uniform("uWidth", window.size.x);
    // shader.uniform("uHeight", window.size.y);

    // context.shape.quadCorner.bind().attrib(0).pointer_float(4).divisor(0);

    // for(auto &it : cnt->m_images.first[layer]){
    //     shader.uniform("(uColor)", (colorHex(it.color)));
    //     shader.uniform("uRect", it.rect);
    //     shader.uniform("uUVs", it.uvs);
    //     gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
    // }

    // context.errors();
}

void UIDrawer::renderFonts(UI::IMGUI &ui, u32 layer){
    auto shader = assets::getShader("UIText");
    shader.bind();
    shader.uniform("uWidth", window.size.x);
    shader.uniform("uHeight", window.size.y);

    context.shape.quadCorner.bind().attrib(0).pointer_float(4).divisor(0);
    for (auto &it : ui.fonts){
        if(it.second.renderedFonts[layer].m_size){
            shader.texture("uTexture", it.second.fontInfo->texWithLetters);

            auto &renderData = it.second.renderedFonts[layer];
            context.getRandomBuffer().update(renderData.positions).attrib(1).pointer_float(2).divisor(1);
            context.getRandomBuffer().update(renderData.sizes).attrib(2).pointer_float(2).divisor(1);
            context.getRandomBuffer().update(renderData.uvs).attrib(3).pointer_float(4).divisor(1);
            context.getRandomBuffer().update(renderData.colors).attrib(4).pointer_color().divisor(1);

            gl::DrawArraysInstanced(gl::TRIANGLE_STRIP, 0, 4, renderData.m_size);
        }
        if(it.second.renderedSymbols[layer].m_size){
            shader.texture("uTexture", it.second.fontInfo->texWithSymbols);

            auto &renderData = it.second.renderedSymbols[layer];

            context.getRandomBuffer().update(renderData.positions).attrib(1).pointer_float(2).divisor(1);
            context.getRandomBuffer().update(renderData.sizes).attrib(2).pointer_float(2).divisor(1);
            context.getRandomBuffer().update(renderData.uvs).attrib(3).pointer_float(4).divisor(1);
            context.getRandomBuffer().update(renderData.colors).attrib(4).pointer_color().divisor(1);

            gl::DrawArraysInstanced(gl::TRIANGLE_STRIP, 0, 4, renderData.m_size);
        }

        it.second.clear(layer);
    }
    context.errors();
}
