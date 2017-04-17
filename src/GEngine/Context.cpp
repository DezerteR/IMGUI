#include "Context.hpp"
#include "Logging.hpp"

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

    // gl::GenFramebuffers(1, &fbo.fbo_18);
    //     gl::BindFramebuffer(gl::FRAMEBUFFER, fbo.fbo_18);
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
