#pragma once
#include "Common.hpp"

class Context;
class Window;
class UIDrawer;

class Renderer
{
private:
    Window &window;
    std::shared_ptr<Context> context;
    void initTextures();
    void initFBOs();
public:
    Renderer(Window &window);
    void reset();
    void beginFrame();
    void endFrame();

    // EnvironmentRenderer; -> GBuffer
    // ShadowRenderer; -> shadowMap
    // LightRenderer; -> light
    // Atmosphererenderer; composedScene -> composedScene(GBuffer.color + light)
    // HdrPostprocessing; composedScene -> composedScene
    // Tonemapping; composedScene -> composedSceneLdr, blurredScene
    std::shared_ptr<UIDrawer> uiDrawer;
};
