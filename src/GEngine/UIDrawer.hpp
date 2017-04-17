#pragma once

class Context;

class UIDrawer
{
    Context &context;
public:
    UIDrawer(Context &context) : context(context){}
};
