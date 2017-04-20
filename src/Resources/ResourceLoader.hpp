#pragma once
#include "Common.hpp"
#include "Assets.hpp"

class Yaml;
struct LightSourceContainer;

class ResourceLoader
{
public:
    ResourceLoader(u32 id=0) : id(id){}

    std::string shaderPath { "../shaders/" };
    std::string imagePath { "../res/textures/" };
    std::string meshPath { "../res/models/" };
    std::string soundPath { "../res/sound/" };

    std::vector<float> model_vertices;
    std::vector<float> model_coords;
    std::vector<float> model_normals;
    std::vector<u32> model_indices;

    u32 id;

    bool loadTangents { false };

    void loadResources(const Yaml &cfg);
    bool loadShaders();
    bool reloadShader(const std::string &filename);

    /// TODO: remove
    Mesh loadMesh(std::string meshName);
    bool loadMesh(const Yaml &cfg);

    Image loadImage(const std::string &name);
    bool loadImage(const Yaml &cfg);
    bool loadFonts(const Yaml &cfg);
    bool loadFont(const Yaml &cfg);
    bool loadImageSet(const Yaml &cfg);
    bool loadObj(std::fstream &file);
    std::string  loadFile(std::string fname);
    void printShaderInfoLog(i32 shader);
    void fillBuffers();

    assets::TextureArray loadTextureArray(const std::string &folder);
    assets::TextureArray loadCubeMap(const std::string &folder);
};

namespace assets {
    ResourceLoader getLoader(u32 id);
    ResourceLoader getNewLoader();
}
