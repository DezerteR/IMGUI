#pragma once
#include "Common.hpp"

struct UBO
{
    u32 ID {0};
    u32 maxSize;

    void bind(u32 shader, u32 index, u32 bindingIndex);
    void bind(u32 shader, const std::string &name, u32 bindingIndex);
    static void unbind();

    void setup(u32 size);
    void update(float *data, u32 size);
};

struct VBO
{
    u32 ID {0};
    u32 numBuffer;
    u32 maxSize;

    u32 dataType; /// u32 float
    u32 elements;

    u32 drawMode;
    VBO& setup(u32 size, bool dynamic = false);
    VBO& bind();
    void clear();

    template<typename T>
    VBO& setup(std::vector<T> &data, bool dynamic = false){
        elements = sizeof(T)/sizeof(float);
        maxSize = data.size()*elements;

        setup(data.data(), data.size()*elements, dynamic);

        return *this;
    }
    /// private :D
    VBO& setup(void *data, u32 count, bool dynamic); /// count is number of floats
    template<typename T>
    VBO& update(std::vector<T> &data){
        update(data.data(), data.size()*sizeof(T)/sizeof(float));
        return *this;
    }
    /// private :D
    VBO& update(void *data, u32 count);

    VBO& attrib(u32 n);
    VBO& pointer(u32 e, u32 dataType_=0x1406/*gl::FLOAT*/, u32 stride=0, void *pointer_=nullptr);
    VBO& pointer_float(u32 e, u32 stride=0, void *pointer_=nullptr);
    VBO& pointer_color(u32 stride=0, void *pointer_=nullptr);
    VBO& divisor(u32 d = 0);
    void operator ()();
};

struct IBO
{
    u32 ID {0};
    void setup(std::vector<u32> &indices);
    void clear();
    void bind();
};

/**
 *  VAO wrapper
 *  usage:
 *  vbo.setup().addBuffer(vertices, 4).addBuffer(uvs, 2).addBuffer(indices)();
 *  ...
 *  vbo.bind()
 *  vbo::unbind();
 */
struct VAO
{
    u32 ID {0};
    VBO vbo[6] = {};
    IBO ibo { 0 };

    u32 triangleCount;
    u8 vboCount { 0 };

    VAO& setup();
    void clear();
    void operator()();
    void bind();
    static void unbind();

    VAO& addBuffer(std::vector<float> &buffer, u32 numOfElements = 1);
    VAO& addBuffer(std::vector<glm::vec2> &buffer);
    VAO& addBuffer(std::vector<glm::vec3> &buffer);
    VAO& addBuffer(std::vector<glm::vec4> &buffer);
    VAO& addBuffer(std::vector<u32> &buffer);
};

struct Mesh
{
    u32 begin;
    u32 end;
    u32 count;
    u32 mode {0};
    void render();
    void* offset(){
        return (void*)(sizeof(u32)*begin);
    }
};

class Texture;
struct Shader
{
    ~Shader();
    u32 ID {0};
    bool binded {false};
    Shader& bind();
    /// ../shaders/XX/name.glsl, name
    void loadFromFile(const std::string pathTo, const std::string name);
    Shader& texture(const i32 uniformId, u32 texture, u32 id = 0);
    Shader& texture(const std::string &name, u32 texture, u32 id = 0);
    Shader& texture(const std::string &name, Texture &texture, u32 id = 0);
    Shader& atlas(const std::string &name, u32 texture, u32 id = 0);
    Shader& cubeMap(const std::string &name, u32 texture, u32 id = 0);
    i32 location(const std::string &name);
    i32 uboLocation(const std::string &name);

    Shader& ubo(const char *name, u32 u, u32 bindingIndex, u32 maxSize);

    Shader& uniform(const i32 name, const std::vector<glm::mat4> &m);
    Shader& uniform(const i32 name, const std::vector<float> &m);
    Shader& uniform(const i32 name, const glm::mat4 &m);
    Shader& uniform(const i32 name, const glm::vec4 &v);
    Shader& uniform(const i32 name, const glm::vec3 &v);
    Shader& uniform(const i32 name, const glm::vec2 &v);
    Shader& uniform(const i32 name, float v);
    Shader& uniform(const i32 name, int v);
    Shader& uniform(const i32 name, uint32_t v);
    Shader& uniform(const i32 name, uint16_t v);

    Shader& uniform(const char *name, const std::vector<glm::mat4> &m);
    Shader& uniform(const char *name, const std::vector<float> &m);
    Shader& uniform(const char *name, const glm::mat4 &m);
    Shader& uniform(const char *name, const glm::vec4 &v);
    Shader& uniform(const char *name, const glm::vec3 &v);
    Shader& uniform(const char *name, const glm::vec2 &v);
    Shader& uniform(const char *name, float v);
    Shader& uniform(const char *name, int v);
    Shader& uniform(const char *name, uint32_t v);
};

struct State
{
    bool m_blend { 0 };
    u32 m_blendSRC { 0 };
    u32 m_blendDST { 0 };
    bool m_cullFace { 0 };
    bool m_frontFace { 0 };
    bool m_depthTest { 0 };
    bool m_depthMask { 0 };
    u32 m_depthFunc { 0 };

    void save(){}
    void restore(){
    }
    State& blending(bool b);
    State& blendFunc(u32 src, u32 dst);
    State& depthTest(bool b);
    State& depthMask(bool b);
    State& culling(bool b, bool ccw = true);
    State& depthFunc(u32 func);



};
