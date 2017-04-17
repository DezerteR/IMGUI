#include "Renderer.hpp"
#include "Context.hpp"
#include "UIDrawer.hpp"
#include "Window.hpp"


Renderer::Renderer(Window &window) : window(window), context(std::make_shared<Context>(window)), uiDrawer(std::make_shared<UIDrawer>(*context)){
    context->reset();
}

void Renderer::beginFrame(){}
void Renderer::endFrame(){}
