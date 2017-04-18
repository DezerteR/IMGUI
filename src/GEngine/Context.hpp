#pragma once
#include "gl_core_3_3.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "GPUResources.hpp"

class Context
{
private:
    Window &window;
    void resetTextures();
    void resetFbo();
    void resetBuffers();
    void resetShapes();
public:
    struct {
        struct {
            Texture color;
            Texture normals;
            Texture depth;
        } gbuffer;
        struct {
            Texture color;
            Texture specular;
        } light;

        struct {
            struct {
                Texture a;
                Texture b;
            } full;
            struct {
                Texture a;
                Texture b;
            } half;
            struct {
                Texture a;
                Texture b;
            } quarter;
        } util;

        Texture shadowMapCascade;
        Texture blurredScene;
    } tex;

    struct {
        GLenum drawBuffers[5];
        GLuint full;
        GLuint _12;
        GLuint _12_wide;
        GLuint _14;
        GLuint _18;
    } fbo;

    struct {
        VAO vao;
        VBO point {};
        VBO quadCentered {}; /// xyyv
        VBO quadCorner {}; /// xyuv
        VBO screen {}; /// xyuv
        VBO cube {};
    } shape;

    std::array<VBO,6> randomBuffers;
    u32 randomBufferId {0};
    VBO& getRandomBuffer(){
        randomBufferId = (randomBufferId+1)%6;
        return randomBuffers[randomBufferId];
    }
    VAO defaultVAO;

    Context(Window &window) : window(window){}
    void reset();

    void beginFrame();
    void endFrame();

    void setupFbo_11(const Texture &texture);
    void setupFbo_11_depth(const Texture &texture);
    void setupFbo_11_depth(const Texture &texture, const Texture &texture2);
    void setupFbo_12(const Texture &texture);
    void setupFbo_12(const Texture &texture, const Texture &texture2);
    void setupFbo_14(const Texture &texture);

    bool _errors(const std::string &text, const std::string &file, int line, const std::string &fun);
};
#define errors() _errors("", __FILE__, __LINE__, __FUNCTION__)
