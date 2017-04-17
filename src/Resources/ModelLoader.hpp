#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Common.hpp"
#include "Logging.hpp"
#include "GPUResources.hpp"
/// TODO: zaoszczedzic na includach!
// #include "PhysicsWorld.hpp"

struct InternalMeshInfo
{
    u32 vID;
    u32 uvID;
    u32 nID;
    u32 tID;
    u32 iID;

    u32 vertexCount;
    u32 count;

    Mesh toMesh(){
        return {iID, iID+count, count};
    }
};
using floats = std::vector<float>;
using ints = std::vector<i32>;
using uints = std::vector<u32>;
struct InternalMesh
{
    InternalMesh() = default;
    InternalMesh(InternalMesh&&) = default;
    floats vertex;
    floats texcoord;
    floats normal;
    floats tangent;
    uints indices;

    void join(InternalMesh&);
};
/**
  *  next lod levels has suffix _lod<level[1:3]>
  *  loading inserts in collection, position in collecion is returned
  *  for loadAll models are loaded in order of LOD
  *
  *
*/
class ModelLoader
{
public:
    u32 m_uvSize {2};
    float m_vertexW { 1.f };
    bool good { false };
    bool loadTangents { false };
    bool loadUV { true };
    bool debug { false };

    ModelLoader& open(const std::string &filename, std::function<float(const std::string&)> getLayer);
    ModelLoader& open(const std::string &filename);
    void close();
    ~ModelLoader(){
        if(scene) close();
    }

    Mesh beginMesh(){
        Mesh mesh {};
        mesh.begin = indices.size();
        return mesh;
    }
    void endMesh(Mesh &m){
        m.end = indices.size();
        m.count = m.end - m.begin;
    }

    InternalMeshInfo load(const std::string &name);
    InternalMesh getInternalMesh(const std::string &name);
    InternalMeshInfo insert(InternalMesh &intMesh);
    // std::vector<ConvexMesh> loadCompoundMesh(const std::string &name);
    std::vector<std::string> findInstances(const std::string &pattern);
    void setTextureLayer(InternalMeshInfo info, float layer);
    void setIndex(InternalMeshInfo info, float index);
    VAO build();

    std::pair<std::vector<double>, std::vector<u32>> loadStatic3DMesh(const std::string &name);

    std::vector<std::string> getNames();
private:
    floats vertex;
    floats texcoord;
    floats normal;
    floats tangent;
    uints indices;

    Assimp::Importer importer;
    const aiScene* scene { nullptr };
    void copyVertices(aiMesh *mesh, floats &target);
    void copyTexcoords(aiMesh *mesh, floats &target);
    void copyNormals(aiMesh *mesh, floats &target);
    void copyTangents(aiMesh *mesh, floats &target);
    void copyIndices(aiMesh *mesh, uints &target, u32 offset);

    std::function<float(const std::string&)> getLayer;
    std::string getTextureName(aiMesh *mesh);

    std::vector<aiMesh*> find(const std::string &name);
};
