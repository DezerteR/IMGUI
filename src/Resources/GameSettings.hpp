#pragma once
#include "Common.hpp"

class GameSettings
{
public:
    void load();
    void save();

    struct Video
    {
        bool fullscreen;
        glm::vec2 size;
        float gamma;
        struct {
            float fov;
            float inertia;
        } camera;
    } video;

    struct Audio
    {} audio;

    struct Controls
    {} controls;
};

GameSettings& gameSettings();
