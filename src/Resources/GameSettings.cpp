#include "GameSettings.hpp"
#include "IMGUI.hpp"
#include "Yaml.hpp"

GameSettings g_gameSettings;

GameSettings& gameSettings(){
    return g_gameSettings;
}

void GameSettings::load(){
    auto &&yaml = loadYaml("../Settings.yml");

    video.fullscreen = yaml["Video"]["Fullscreen"].boolean();
    video.size = yaml["Video"]["Size"].vec2();
    video.gamma = yaml["Video"]["Gamma"].number();
    video.camera.fov = yaml["Video"]["Camera"]["Fov"].number();
    log("video.fullscreen", video.fullscreen);
}

void GameSettings::save(){
    Yaml yaml;

    yaml["Video"]["Fullscreen"] = video.fullscreen;
    yaml["Video"]["Size"] = video.size;
    yaml["Video"]["Gamma"] = video.gamma;
    yaml["Video"]["Camera"]["Fov"] = video.camera.fov;

    saveYaml("../Settings.yml", yaml);
}
