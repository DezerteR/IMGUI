#pragma once
#include "Common.hpp"

class Context;
class Window;

class RendererUtils
{
    Window &window;
    Context &context;
public:
    RendererUtils(Window &window, Context &context) : window(window), context(context){}
    void drawBackground(const std::string &image);
};
