#include "RendererUtils.hpp"
#include "Context.hpp"
#include "Assets.hpp"

void RendererUtils::drawBackground(const std::string &image){
    gl::Disable(gl::DEPTH_TEST);
    gl::Disable(gl::BLEND);
    gl::Disable(gl::CULL_FACE);
    gl::DepthMask(gl::FALSE_);

    auto shader = assets::getShader("ApplyFBO");
    shader.bind();
    shader.texture("uTexture", assets::getImage(image).ID, 0);
    context.drawScreen();
}

void RendererUtils::blurBuffer(){
    gl::Disable(gl::DEPTH_TEST);
    gl::Disable(gl::BLEND);
    gl::Disable(gl::CULL_FACE);
    gl::DepthMask(gl::FALSE_);

    context.tex.gbuffer.color.genMipmaps();

    /// first pass
    context.setupFBO_12(context.tex.util.half.a);

    auto shader = assets::getShader("BlurHorizontal");
    shader.bind();
    shader.uniform("uPixelSize", window.pixelSize/2.f);
    shader.texture("uTexture", context.tex.gbuffer.color, 0);
    context.drawScreen();

    /// second pass
    context.setupFBO_12(context.tex.util.half.b);

    shader = assets::getShader("BlurVertical");
    shader.bind();
    shader.uniform("uPixelSize", window.pixelSize/2.f);
    shader.texture("uTexture", context.tex.util.half.a, 0);
    context.drawScreen();

    context.tex.blurredScene = context.tex.util.half.b;
    context.setupFBO_11(context.tex.gbuffer.color);
}

void RendererUtils::renderBlurred(){
    auto shader = assets::getShader("ApplyFBO");
    shader.bind();
    shader.texture("uTexture", context.tex.blurredScene, 0);
    context.drawScreen();
}
