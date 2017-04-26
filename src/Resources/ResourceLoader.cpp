#include <boost/filesystem.hpp>
#include "Logging.hpp"
#include "GPUResources.hpp"
#include "Yaml.hpp"
#include "ResourceLoader.hpp"
#include "ModelLoader.hpp"
#include "ImageLoader.hpp"
#include "Renderer.hpp"
#include "BaseStructs.hpp"
#include "FontRenderer.hpp"
#include "Utils.hpp"
// #include "PerfTimers.hpp"
// #include "LightSource.hpp"
// #include "DecalsAndMarkers.hpp"
// #include "Global.hpp"

namespace {
    template<typename T>
    void insert(std::vector<T> &a, std::vector<T> &b){
        a.insert(a.end(), b.begin(), b.end());
    }
}

void ResourceLoader::loadResources(const Yaml &cfg){
    for(auto &image : cfg["Images"]){
        loadImage(image);
    }
    for(auto &set : cfg["ImageSets"]){
        loadImageSet(set);
    }
    if(isFile("../res/atlases/")){
        loadTextureArray("Decals");
        loadTextureArray("Materials");
        loadTextureArray("Terrain");
        loadTextureArray("Foliage");
        loadTextureArray("Cgi256");
        loadCubeMap("Park");
    }

    if(isFile("../res/models/")){
        ModelLoader modelLoader;
        modelLoader.loadTangents = true;
        modelLoader.open("../res/models/CommonModels.dae", std::move(assets::layerSearch(assets::getAlbedoArray("Materials"))));
        auto names = modelLoader.getNames();
        for(auto &name : names){
            auto mesh = modelLoader.beginMesh();
            modelLoader.load(name);
            modelLoader.endMesh(mesh);

            assets::addMesh(mesh, name);
        }
        assets::addVao(modelLoader.build(), "Common");
    }
    loadFonts();
}

/**
 *  Reads shaders from ../shaders directory and its subfolders
 *
 */
bool ResourceLoader::loadShaders(){
    using namespace boost::filesystem;
    try {
        path p(shaderPath);
        auto dir_it = recursive_directory_iterator(p);
        for(dir_it; dir_it != recursive_directory_iterator(); dir_it++){
            if ( is_directory(dir_it->status()) ) continue;

            std::string localShaderPath = (*dir_it).path().relative_path().string();

            auto shaderName = (*dir_it).path().stem().string();
            if((*dir_it).path().extension().string() != ".glsl") continue;
            assets::getShader(shaderName).loadFromFile(localShaderPath, shaderName);
        }
    return true;
    }
    catch (const filesystem_error& ex){
        error("boost::filesystem ex: ", ex.what());
    }
    return false;
}
bool ResourceLoader::reloadShader(const std::string &name){

    std::string pathToShader;
    findFile(shaderPath, name, "-r", pathToShader);

    assets::getShader(name).loadFromFile(pathToShader, name);
    log("[ SHADER ] reload:", name);

    return true;
}

Mesh ResourceLoader::loadMesh(std::string meshName){
    std::string fileName = meshPath+meshName+".obj";

    Mesh mesh;
    std::fstream file;
    file.open(fileName, std::ios::in);

    if(!file.is_open()){
        error(fileName, "not exist");
        return {};
    }

    mesh.begin = model_indices.size();
    loadObj(file);
    mesh.end = model_indices.size();
    mesh.count = mesh.end - mesh.begin;

    file.close();
    assets::addMesh(mesh, meshName);

    log("[ MESH ]", meshName);
    return mesh;
}
bool ResourceLoader::loadMesh(const Yaml &cfg){
    std::string name = cfg["Name"].string();
    loadMesh(name);
    return true;
}
bool ResourceLoader::loadObj(std::fstream &file){
    std::string tmp, mpt;

    char nop;
    while(tmp != "v" && !file.eof())
        file>>tmp;

    std::vector<float> vertex;
    std::vector<float> coord;
    std::vector<float> normal;
    vertex.reserve(800000);
    coord.reserve(400000);
    normal.reserve(800000);
    int u(0), k(0);
    // file>>tmp;
    float x,y,z;
    int a,b,c;
    while(tmp=="v"){
            file>>x>>y>>z;
            vertex[u++]=x;
            vertex[u++]=y;
            vertex[u++]=z;
            file>>tmp;
    }
    u=0;
    while(tmp=="vt"){
        file>>x>>y;
        coord[u++]=x;
        coord[u++]=y;
        file>>tmp;
    }
    u=0;
    while(tmp=="vn"){
        file>>x>>y>>z;
        normal[u++]=x;
        normal[u++]=y;
        normal[u++]=z;
        file>>tmp;
    }
    while(tmp != "f" && !file.eof())
        file>>tmp;
    int i(0);
    if(model_indices.size()>0)
        i = model_indices[model_indices.size()-1]+1;
    // i+=przebieg;

    u=0;//model_vertices.size();
    k=0;//model_coords.size();


    /*
    1______________2
    |            / |
    |         /    |
    |      /       |
    |   /          |
    |/             |
    0______________3
    012 023
    */
    while(tmp == "f" && !file.eof()){
            for(int t=0; t<3; t++){
                file >> a >> nop >> b >> nop >> c;
                a--;b--;c--;
                model_vertices.push_back(vertex[a*3]);
                model_vertices.push_back(vertex[a*3+1]);
                model_vertices.push_back(vertex[a*3+2]);
                model_vertices.push_back(1.f);
                model_coords.push_back(coord[b*2]);
                model_coords.push_back(coord[b*2+1]);
                model_normals.push_back(normal[c*3]);
                model_normals.push_back(normal[c*3+1]);
                model_normals.push_back(normal[c*3+2]);
                model_normals.push_back(0.f);
                u+=3;k+=2;
                model_indices.push_back(i++);
            }
            file>>tmp;
            if(tmp=="f");
            else { /// in case thera are 4 vertices(quad)
                model_indices.push_back(i-3);
                model_indices.push_back(i-1);
                file >> nop >> b >> nop >> c;
                a=atoi(tmp.c_str());a--;b--;c--;
                model_vertices.push_back(vertex[a*3]);
                model_vertices.push_back(vertex[a*3+1]);
                model_vertices.push_back(vertex[a*3+2]);
                model_coords.push_back(coord[b*2]);
                model_coords.push_back(coord[b*2+1]);
                model_normals.push_back(normal[c*3]);
                model_normals.push_back(normal[c*3+1]);
                model_normals.push_back(normal[c*3+2]);
                u+=3;k+=2;
                model_indices.push_back(i++);
                file>>tmp;
            }
        }

    return true;
}

bool ResourceLoader::loadFonts(){
    std::vector<std::string> imagesToLoad;
    auto fontsToLoad = listDirectory("../res/fonts/");
    for(auto &font : fontsToLoad){
        loadFont(font, imagesToLoad);
    }

    for(auto &it : imagesToLoad) it = "../res/textures/" + it;
    assets::TextureArray out;
    out.id = ImageUtils::loadArrayToGpu(imagesToLoad).id;
    assets::addAlbedoArray(id, out, "Fonts");

    return true;
}
bool ResourceLoader::loadFont(const std::string &fontFileName, std::vector<std::string> &imagesToLoad){
    UI::Font &font = assets::getFont(UI::convertFontFilenameToId(fontFileName));
    font.load(fontFileName, imagesToLoad);
    return true;
}

bool ResourceLoader::loadImage(const Yaml &cfg){
    std::string name = cfg.string();
    return loadImage(name).ID;
}
Image ResourceLoader::loadImage(const std::string &name){
    using namespace boost::filesystem;
    path p(name);

    std::string exactName = getName(name);
    Image image = assets::getImage(exactName);
    if(image.ID != 0){
        return image;
    }
    log("[ IMAGE ] ", exactName);

    std::string fileName = imagePath+name;

    auto imageData = ImageUtils::loadImageToGpu(fileName);

    image = Image { imageData.id, imageData.width, imageData.height };
    assets::addImage(id, image, exactName);
    return image;
}
bool ResourceLoader::loadImageSet(const Yaml &cfg){
    std::string name = cfg["Config"].string();
    std::string image = cfg["Image"].string();
    try {
        auto descr = loadYaml("../res/misc/"+name+".yml");
        auto modeStr = descr["Mode"].string();
        u32 mode = 10;
        if(modeStr == "fromBottom") mode = 0;
        else if(modeStr == "fromTop") mode = 1;
        else if(modeStr == "irfanView") mode = 2;

        loadImage(image);
        auto tmp = loadImage(image);

        ImageSet set;
        int w = set.w = tmp.width;
        int h = set.h = tmp.height;
        set.ID = tmp.ID;
        int count = descr.size();

        for(auto &it : descr["Root"]){
            std::string n = it["name"].string();
            glm::vec4 r = it["rect"].vec4();
            if(mode == 2)
                r.y += r.w;

            r.y = h - r.y;

            glm::vec4 d = glm::vec4(0,0,r.z, r.w);
            r = glm::vec4(r.x/w, r.y/h, r.z/w, r.w/h);
            set.set.emplace(n, Icon {r, d, 0xffffffff});
        }

        assets::addImageSet(set, name);
    }
    catch(...) {
        error("loading image set:", name);
        return false;
    }
    return true;
}

/// loads albedo, normal, metalic and roughness maps
assets::TextureArray ResourceLoader::loadTextureArray(const std::string &folder){
    bool found = false;
    auto extract = [&](std::vector<std::string> &files, std::string ext){
        std::vector<std::string> out;
        for(auto &file : files)
            if(file.find(ext) != std::string::npos){
                out.push_back(file);
                found = true;
            }

        return out;
    };
    auto cutExt = [](std::vector<std::string> &files){
        for(auto &it : files) it = getName(it);
    };
    std::vector<std::string> files;
    if(isFile("../res/atlases/" + folder + "/order.yml")){
        auto &&order = loadYaml("../res/atlases/" + folder + "/order.yml");
        auto allfiles = listDirectory("../res/atlases/" + folder);

        for(auto &it : order){
            for(auto &file : allfiles){
                if(file.find(it.string()) != std::string::npos)
                    files.push_back(file);
            }
        }
    }
    else {
        files = listDirectory("../res/atlases/" + folder);
    }
    for(auto &it : files){
        it = "../res/atlases/" + folder + "/" + it;
    }

    std::vector<std::string> albedo = extract(files, "_a.");
    std::vector<std::string> normal = extract(files, "_n.");
    std::vector<std::string> metallic = extract(files, "_m.");
    std::vector<std::string> roughness = extract(files, "_r.");

    if(not found) albedo = files;

    {
        assets::TextureArray out;
        out.id = ImageUtils::loadArrayToGpu(albedo).id;
        out.content = albedo;
        cutExt(out.content);
        assets::addAlbedoArray(id, out, folder);
    }
    if(not normal.empty()){
        assets::TextureArray out;
        out.id = ImageUtils::loadArrayToGpu(normal).id;
        out.content = normal;
        cutExt(out.content);
        assets::addNormalArray(id, out, folder);
    }
    if(not metallic.empty()){
        assets::TextureArray out;
        out.id = ImageUtils::loadArrayToGpu(metallic).id;
        out.content = metallic;
        cutExt(out.content);
        assets::addMetallicArray(id, out, folder);
    }
    if(not roughness.empty()){
        assets::TextureArray out;
        out.id = ImageUtils::loadArrayToGpu(roughness).id;
        out.content = roughness;
        cutExt(out.content);
        assets::addRoughnessArray(id, out, folder);
    }

    return assets::getAlbedoArray(folder);
}
assets::TextureArray ResourceLoader::loadCubeMap(const std::string &folder){
    bool found = false;
    auto extract = [&](std::vector<std::string> &files, std::string ext){
        std::vector<std::string> out;
        for(auto &file : files)
            if(file.find(ext) != std::string::npos){
                out.push_back(file);
                found = true;
            }

        return out;
    };
    auto cutExt = [](std::vector<std::string> &files){
        for(auto &it : files) it = getName(it);
    };

    std::vector<std::string> files = listDirectory("../res/atlases/" + folder);
    for(auto &it : files){
        it = "../res/atlases/" + folder + "/" + it;
    }

    assets::TextureArray out;
    out.id = ImageUtils::loadCubemapToGpu(files).id;
    out.content = files;
    cutExt(out.content);
    assets::addCubeMap(id, out, folder);

    return out;
}
