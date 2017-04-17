#pragma once
#include "GPUResources.hpp"
/// TODO: split on more informative files
struct Image
{
    uint32_t ID;
    u32 width;
    u32 height;
};

struct Icon
{
    glm::vec4 uvs;
    glm::vec4 rect;
    uint32_t color;
};

struct ImageSet
{
    uint32_t ID;
    int w;
    int h;
    std::map <std::string, Icon> set;
};

struct ArrayData
{
    std::vector <unsigned char> data;
    int size;
    float operator ()(int x, int y){
        return data[x+y*size]/256.f;
    }
    float operator ()(glm::vec2 uv){
        return data[(int)uv.x+(int)(uv.y*size)]/256.f;
    }
};

struct Material
{
    glm::vec4 color;
    float glossyEnergy;
};

struct Entity
{
    Mesh mesh;
    Material material;
    glm::vec4 position;
    glm::quat quat;
};

struct SampleResult
{
    glm::vec4 position;
    glm::vec4 normal;
    bool succes;
    explicit  operator bool() const {
        return succes;
    }
};
