#include "Context.hpp"
#include "Logging.hpp"
#include "Assets.hpp"

void Context::reset(){
    resetTextures();
    resetFbo();
    resetBuffers();
    resetShapes();

    { /// catch error
        int err = gl::GetError();
        if(err != gl::NO_ERROR_){
            error("FBO fail:", err);
        }
        else
            log("[ OK ] initialisation of FBOs");
    }
}

void Context::resetTextures(){
    tex.gbuffer.color = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
    tex.gbuffer.normals = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.gbuffer.depth = Texture(gl::TEXTURE_2D, gl::DEPTH_COMPONENT32F, window.size.x, window.size.y, 1, gl::DEPTH_COMPONENT, gl::FLOAT, gl::LINEAR, 0);

    tex.light.color = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);
    tex.light.specular = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 0);

    tex.util.full.a = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
    tex.util.full.b = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
    tex.util.half.a = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x/2, window.size.y/2, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
    tex.util.half.b = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x/2, window.size.y/2, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
    tex.util.quarter.a = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x/4, window.size.y/4, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
    tex.util.quarter.b = Texture(gl::TEXTURE_2D, gl::RGBA16F, window.size.x/4, window.size.y/4, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);

    tex.blurredScene = Texture(gl::TEXTURE_2D, gl::RGBA8, window.size.x, window.size.y, 1, gl::RGBA, gl::HALF_FLOAT, gl::LINEAR, 4);
}
void Context::resetFbo(){
    fbo.drawBuffers[0] = gl::COLOR_ATTACHMENT0;
    fbo.drawBuffers[1] = gl::COLOR_ATTACHMENT1;
    fbo.drawBuffers[2] = gl::COLOR_ATTACHMENT2;

    gl::GenFramebuffers(1, &fbo.full);
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo.full);
        gl::Viewport(0, 0, window.size.x, window.size.y);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.gbuffer.color.ID, 0);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT1, gl::TEXTURE_2D, tex.gbuffer.normals.ID, 0);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::TEXTURE_2D, tex.gbuffer.depth.ID, 0);
        gl::DrawBuffers(2, fbo.drawBuffers);
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    gl::GenFramebuffers(1, &fbo._12);
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo._12);
        gl::Viewport(0, 0, window.size.x/2.f, window.size.y/2.f);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.util.half.a.ID, 0);
        gl::DrawBuffers(1, fbo.drawBuffers);
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    // gl::GenFramebuffers(1, &fbo._12_wide);
    //     gl::BindFramebuffer(gl::FRAMEBUFFER, fbo._12_wide);
    //     gl::Viewport(0, 0, window.size.x, window.size.y/2.f);
    //     gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.util.half.wide.ID, 0);
    //     gl::DrawBuffers(1, fbo.drawBuffers);
    //     gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    gl::GenFramebuffers(1, &fbo._14);
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo._14);
        gl::Viewport(0, 0, window.size.x/4.f, window.size.y/4.f);
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.util.quarter.a.ID, 0);
        gl::DrawBuffers(1, fbo.drawBuffers);
        gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    // gl::GenFramebuffers(1, &fbo._18);
    //     gl::BindFramebuffer(gl::FRAMEBUFFER, fbo._18);
    //     gl::Viewport(0, 0, window.size.x/8.f, window.size.y/8.f);
    //     gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, Textures::RGBA8_1_12.ID, 0);
    //     gl::DrawBuffers(1, fbo.drawBuffers);
    //     gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}
void Context::resetBuffers(){
    defaultVAO.setup()();
    for(auto &it : randomBuffers){
        it.setup(32000, true)();
    }
}
void Context::resetShapes(){ /// TODO: zwalidować użycia tego
    float point[] = {0.0f, 0.0f, 0.5f, 1.f};
    float quadCentered[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
          0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, 1.0f, 1.0f,
    };
    float quadCorner[] = {
        0.f, 0.f, 0.0f, 0.0f,
        0.f, 1.f, 0.0f, 1.f,
        1.f, 0.f, 1.0f, 0.f,
        1.f, 1.f, 1.0f, 1.f,
    };
    float screen[] = {
        -1.f, -1.f, 0.f, 0.f,
        -1.f,  1.f, 0.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
         1.f,  1.f, 1.f, 1.f,
    };
    float cube[] = {
        /// columns
        -0.5, -0.5, -0.5, 1,
        -0.5, -0.5, 0.5, 1,
        0.5, -0.5, -0.5, 1,
        0.5, -0.5, 0.5, 1,
        -0.5, 0.5, -0.5, 1,
        -0.5, 0.5, 0.5, 1,
        0.5, 0.5, -0.5, 1,
        0.5, 0.5, 0.5, 1,
        /// bottom
        -0.5, -0.5, -0.5, 1,
        0.5, -0.5, -0.5, 1,
        -0.5, -0.5, -0.5, 1,
        -0.5, 0.5, -0.5, 1,
        0.5, 0.5, -0.5, 1,
        -0.5, 0.5, -0.5, 1,
        0.5, 0.5, -0.5, 1,
        0.5, -0.5, -0.5, 1,
        /// top
        -0.5, -0.5, 0.5, 1,
        0.5, -0.5, 0.5, 1,
        -0.5, -0.5, 0.5, 1,
        -0.5, 0.5, 0.5, 1,
        0.5, 0.5, 0.5, 1,
        -0.5, 0.5, 0.5, 1,
        0.5, 0.5, 0.5, 1,
        0.5, -0.5, 0.5, 1,
    };
    shape.point.setup(point, 4, false)();
    shape.quadCentered.setup(quadCentered, 16, false)();
    shape.quadCorner.setup(quadCorner, 16, false)();
    shape.screen.setup(screen, 16, false)();
    shape.cube.setup(cube, 24*4, false)();

    std::vector<glm::vec4> vertices = {
        /// point
        glm::vec4(0,0,0,1),
        /// quad center, xyzw
        glm::vec4(-0.5, -0.5, 0, 1),
        glm::vec4(-0.5, 0.5, 0, 1),
        glm::vec4(0.5, -0.5, 0, 1),
        glm::vec4(0.5, 0.5, 0, 1),
        /// quad corner, xyzw
        glm::vec4(0,0,0,1),
        glm::vec4(0,1,0,1),
        glm::vec4(1,0,0,1),
        glm::vec4(1,1,0,1),
        /// quad center, xyuv
        glm::vec4(-0.5, -0.5, 0, 0),
        glm::vec4(-0.5, 0.5, 0, 1),
        glm::vec4(0.5, -0.5, 1, 0),
        glm::vec4(0.5, 0.5, 1, 1),
        /// quad corner, xyuv
        glm::vec4(0,0,0,0),
        glm::vec4(0,1,0,1),
        glm::vec4(1,0,1,0),
        glm::vec4(1,1,1,1),
        /// screen, xyuv
        glm::vec4(0,0,0,0),
        glm::vec4(0,1,0,1),
        glm::vec4(1,0,1,0),
        glm::vec4(1,1,1,1)
    };
    std::vector<glm::vec2> texcoords = {
        glm::vec2(0,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,0),
        glm::vec2(1,1)
    };

    shape.vao.setup()
          .addBuffer(vertices)
          .addBuffer(texcoords)
          ();
}

bool Context::_errors(const std::string &text, const std::string &file, int line, const std::string &fun){
    int err = gl::GetError();
    if(err != gl::NO_ERROR_){
        error("GLError:", err, file+"#", line, ":", fun, "::", text);
        return true;
    }
    return false;
}

void Context::setupFBO_11(const Texture &t0){
    // if(currentFbo != fbo.full){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo.full);
    //     currentFbo = fbo.full;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y);
}
void Context::setupFBO_11_depth(const Texture &t0){
    // if(currentFbo != fbo.full){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo.full);
    //     currentFbo = fbo.full;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::TEXTURE_2D, tex.gbuffer.depth.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y);
}
void Context::setupFBO_11_depth(const Texture &t0, const Texture &t1){
    // if(currentFbo != fbo.full){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo.full);
    //     currentFbo = fbo.full;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT1, t1.ID, 0);
    gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::TEXTURE_2D, tex.gbuffer.depth.ID, 0);
    gl::DrawBuffers(2, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y);
}
void Context::setupFBO_11(const Texture &t0, const Texture &t1){
    // if(currentFbo != fbo.full){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo.full);
    //     currentFbo = fbo.full;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT1, t1.ID, 0);
    gl::DrawBuffers(2, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y);
}
void Context::setupFBO_12(const Texture &t0){
    // if(currentFbo != fbo._12){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._12);
    //     currentFbo = fbo._12;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x*0.5f, window.size.y*0.5f);
}
void Context::setupFBO_12(const Texture &t0, const Texture &t1){
    // if(currentFbo != fbo._12){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._12);
    //     currentFbo = fbo._12;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT1, t1.ID, 0);
    gl::DrawBuffers(2, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x*0.5f, window.size.y*0.5f);
}
void Context::setupFBO_12_wide(const Texture &t0){
    // if(currentFbo != fbo._12_wide){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._12_wide);
    //     currentFbo = fbo._12_wide;
    // }
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y*0.5f);
}
void Context::setupFBO_14(const Texture &t0){
    // if(currentFbo != fbo._14){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._14);
    //     currentFbo = fbo._14;
    // }
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._14);
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x*0.25f, window.size.y*0.25f);
}
void Context::setupFBO_18(const Texture &t0){
    // if(currentFbo != fbo._18){
        gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._18);
    //     currentFbo = fbo._18;
    // }
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo._18);
    gl::FramebufferTexture(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, t0.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x*0.125f, window.size.y*0.125f);
}
void Context::unbindFBO(){
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
    currentFbo = 0xffffff;
}

void Context::drawScreen(){
    defaultVAO.bind();
    shape.screen.bind().attrib(0).pointer_float(4).divisor(0);
    gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
    gl::BindVertexArray(0);
}

void Context::beginFrame(){
    gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, fbo.full);
    gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, tex.gbuffer.color.ID, 0);
    // gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT1, gl::TEXTURE_2D, tex.gbuffer.normals.ID, 0);
    gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::TEXTURE_2D, tex.gbuffer.depth.ID, 0);
    gl::DrawBuffers(1, &fbo.drawBuffers[0]);
    // gl::DrawBuffers(2, &fbo.drawBuffers[0]);
    gl::Viewport(0, 0, window.size.x, window.size.y);

    gl::DepthRange(0.0f, 1.0f);
    gl::DepthFunc(gl::LEQUAL);
    gl::FrontFace(gl::CCW);

    gl::Disable(gl::BLEND);
    gl::DepthMask(gl::TRUE_);
    // gl::Enable(gl::CULL_FACE);
    gl::Disable(gl::CULL_FACE);
    gl::Enable(gl::DEPTH_TEST);
    gl::DisableVertexAttribArray(0);
    gl::BindTexture(gl::TEXTURE_2D, 0);

    gl::ClearColor(0.25f, 0.25f, 0.2f, 1.f);
    gl::ClearDepth(1);
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

    defaultVAO.bind();

    errors();
}
void Context::endFrame(){
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
    shader.texture("uTexture", tex.gbuffer.color.ID, 0);
    drawScreen();

    gl::UseProgram(0);
    gl::BindVertexArray(0);
    gl::DisableVertexAttribArray(0);
}
