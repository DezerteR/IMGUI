#include "Renderer.hpp"
#include "Context.hpp"
#include "UIDrawer.hpp"
#include "Window.hpp"


Renderer::Renderer(Window &window) : window(window), context(std::make_shared<Context>(window)), uiDrawer(std::make_shared<UIDrawer>(window, *context)){
    context->reset();
}

void Renderer::beginFrame(){context->beginFrame();}
void Renderer::endFrame(){context->endFrame();}
