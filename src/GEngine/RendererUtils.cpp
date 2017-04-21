#include "RendererUtils.hpp"
#include "Context.hpp"
#include "Assets.hpp"

void RendererUtils::drawBackground(const std::string &image){
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    gl::DisableVertexAttribArray(0);
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
    gl::BindFramebuffer(gl::READ_FRAMEBUFFER, 0);

    gl::Disable(gl::DEPTH_TEST);
    gl::Disable(gl::BLEND);
    gl::Disable(gl::CULL_FACE);
    gl::DepthMask(gl::FALSE_);

    auto shader = assets::getShader("ApplyFBO");
    shader.bind();
    shader.texture("uTexture", assets::getImage(image).ID, 0);
    drawScreen();

    gl::UseProgram(0);
    gl::BindVertexArray(0);
    gl::DisableVertexAttribArray(0);
}
