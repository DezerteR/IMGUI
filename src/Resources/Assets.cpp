#include "Assets.hpp"
#include "BaseStructs.hpp"
#include "GpuResources.hpp"
#include "ResourceLoader.hpp"
#include "FontRenderer.hpp"

namespace assets {

u32 idGenerator;
u32 nextId(){
    return idGenerator++;
}

template<typename A>
struct UserID {
    UserID(){}
    UserID(u32 id, A &a)  :id(id), a(a){}
    u32 id {};
    A a {};
    A& get(){ return a; };
    const A& get() const { return a; };
};

std::unordered_map<std::string, ImageSet> imageSets;

std::unordered_map<std::string, UserID<Image>> images;

std::unordered_map<std::string, Shader> shaders;
std::unordered_map<int, UI::Font> fonts;

std::unordered_map<std::string, UserID<TextureArray>> albedoArrays;
std::unordered_map<std::string, UserID<TextureArray>> normalArrays;
std::unordered_map<std::string, UserID<TextureArray>> metallicArrays;
std::unordered_map<std::string, UserID<TextureArray>> roughnessArrays;
/// http://stackoverflow.com/questions/462721/rendering-to-cube-map
std::unordered_map<std::string, UserID<TextureArray>> cubeMaps;

std::unordered_map<std::string, VAO> vaos;
std::unordered_map<std::string, UserID<Mesh>> meshes;

/// TODO: make them const
Image getImage(const std::string &name){
    auto it = images.find(name);
    if(it == images.end()) return {};
    return it->second.get();
}
TextureArray& getAlbedoArray(const std::string &name){
    return albedoArrays[name].get();
}
TextureArray& getNormalArray(const std::string &name){
    return normalArrays[name].get();
}
TextureArray& getMetalic(const std::string &name){
    return metallicArrays[name].get();
}
TextureArray& getRoughnessArray(const std::string &name){
    return roughnessArrays[name].get();
}
TextureArray& getCubeMap(const std::string &name){
    return cubeMaps[name].get();
}
Shader& getShader(const std::string &name){
    return shaders[name];
}
UI::Font& getFont(int id){
    return fonts[id];
}

std::string findArrayWithTextureName(const std::string &name){
    for(auto &array : albedoArrays){
        for(auto tex : array.second.get().content)
            if(tex == name) return array.first;
    }
    return "";
}

std::function<float(const std::string&)> layerSearch(TextureArray &array){
    return [&](const std::string &name){
        float i = 0.f;
        for(const auto &it : array.content){
            if(name == it) return i;
            i += 1.f;
        }
        return 0.f;
    };
}

ResourceLoader getLoader(u32 id){
    return ResourceLoader(id);
}
ResourceLoader getNewLoader(){
    return ResourceLoader(nextId());
}

void relaseResources(u32 id){}

void addImageSet(ImageSet &imageSet, const std::string &name){
    imageSets[name] = imageSet;
}
ImageSet& getImageSet(const std::string &name){
    return imageSets[name];
}
void addShader(Shader &shader, const std::string &name){
    shaders[name] = shader;
}

void addImage(u32 id, Image &image, const std::string &name){
    images[name] = UserID<Image>(id, image);
}
void addAlbedoArray(u32 id, TextureArray &array, const std::string &name){
    albedoArrays[name] = UserID<TextureArray>(id, array);
}
void addNormalArray(u32 id, TextureArray &array, const std::string &name){
    normalArrays[name] = UserID<TextureArray>(id, array);
}
void addMetallicArray(u32 id, TextureArray &array, const std::string &name){
    metallicArrays[name] = UserID<TextureArray>(id, array);
}
void addRoughnessArray(u32 id, TextureArray &array, const std::string &name){
    roughnessArrays[name] = UserID<TextureArray>(id, array);
}
void addCubeMap(u32 id, TextureArray &array, const std::string &name){
    cubeMaps[name] = UserID<TextureArray>(id, array);
}

void addVao(VAO vao, const std::string &name){
    vaos[name] = vao;
}
VAO& getVao(const std::string &name){
    return vaos[name];
}

void addMesh(Mesh &mesh, const std::string &name){
    meshes[name] = UserID<Mesh>(0, mesh);
}
Mesh& getMesh(const std::string &name){
    return meshes[name].get();
}

};
