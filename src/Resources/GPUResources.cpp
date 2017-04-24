#include "gl_core_4_5.hpp"
#include "Logging.hpp"
#include "GPUResources.hpp"
#include "Texture.hpp"
#include "Common.hpp"
#include "Utils.hpp"
#include <regex>
#include <fstream>

void UBO::bind(u32 shader, u32 index, u32 bindingIndex){
    gl::UniformBlockBinding(shader, index, bindingIndex);
    gl::BindBufferRange(gl::UNIFORM_BUFFER, bindingIndex, ID, 0, sizeof(float) * maxSize);
}
void UBO::bind(u32 shader, const std::string &name, u32 bindingIndex){
    u32 index = gl::GetUniformBlockIndex(shader, name.c_str());
    gl::UniformBlockBinding(shader, index, bindingIndex);
    gl::BindBufferRange(gl::UNIFORM_BUFFER, bindingIndex, ID, 0, sizeof(float) * maxSize);
}
void UBO::unbind(){
    gl::BindBuffer(gl::UNIFORM_BUFFER, 0);
}

/// ---------------------- UBO ----------------------
void UBO::setup(u32 size){
    maxSize = size;
    gl::GenBuffers(1, &ID);
    gl::BindBuffer(gl::UNIFORM_BUFFER, ID);
    gl::BufferData(gl::UNIFORM_BUFFER, sizeof(float)*maxSize, nullptr, gl::DYNAMIC_DRAW);
    gl::BindBuffer(gl::UNIFORM_BUFFER, 0);
}
void UBO::update(float *data, u32 size){
    gl::BindBuffer(gl::UNIFORM_BUFFER, ID);
    gl::BufferSubData(gl::UNIFORM_BUFFER, 0, sizeof(float)*size, data);
    gl::BindBuffer(gl::UNIFORM_BUFFER, 0);
}

/// ---------------------- VBO ----------------------
VBO& VBO::bind(){
    gl::BindBuffer(gl::ARRAY_BUFFER, ID);
    return *this;
}
VBO& VBO::setup(u32 size, bool dynamic){
    maxSize = size;
    drawMode = gl::STATIC_DRAW;
    if(dynamic) drawMode = gl::DYNAMIC_DRAW;

    gl::GenBuffers(1, &ID);
    gl::BindBuffer(gl::ARRAY_BUFFER, ID);
    gl::BufferData(gl::ARRAY_BUFFER, sizeof(float)*maxSize, nullptr, drawMode);

    return *this;
}
VBO& VBO::setup(void *data, u32 count, bool dynamic){
    drawMode = gl::STATIC_DRAW;
    if(dynamic) drawMode = gl::DYNAMIC_DRAW;

    gl::GenBuffers(1, &ID);
    gl::BindBuffer(gl::ARRAY_BUFFER, ID);
    gl::BufferData(gl::ARRAY_BUFFER, sizeof(float)*count, data, drawMode);

    return *this;
}
VBO& VBO::update(void *data, u32 count){
    gl::BindBuffer(gl::ARRAY_BUFFER, ID);
    gl::BufferData(gl::ARRAY_BUFFER, sizeof(float)*maxSize, nullptr, drawMode);
    gl::BufferData(gl::ARRAY_BUFFER, count*sizeof(float), data, drawMode);

    return *this;
}
void VBO::clear(){
    gl::DeleteBuffers(1, &ID);
}


VBO& VBO::attrib(u32 n){
    numBuffer = n;
    gl::EnableVertexAttribArray(numBuffer);

    return *this;
}
VBO& VBO::pointer(u32 e, u32 dataType_, u32 stride, void *pointer_){
    elements = e;
    dataType = dataType_;
    gl::VertexAttribPointer(numBuffer, elements, dataType, false, stride, pointer_);

    return *this;
}
VBO& VBO::pointer_float(u32 e, u32 stride, void *pointer_){
    elements = e;
    dataType = gl::FLOAT;
    gl::VertexAttribPointer(numBuffer, elements, dataType, false, stride, pointer_);

    return *this;
}
VBO& VBO::pointer_color(u32 stride, void *pointer_){
    elements = 4;
    dataType = gl::UNSIGNED_BYTE;
    gl::VertexAttribPointer(numBuffer, elements, dataType, true, stride, pointer_);

    return *this;
}
VBO& VBO::divisor(u32 d){
    gl::VertexAttribDivisor(numBuffer, d);

    return *this;
}
void VBO::operator ()(){
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
}

/// ---------------------- IBO ----------------------
void IBO::setup(std::vector<u32> &indices){
    gl::GenBuffers(1, &ID);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ID);
    gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(u32)*indices.size(), indices.data(), gl::STATIC_DRAW);
}
void IBO::clear(){
    gl::DeleteBuffers(1, &ID);
}
void IBO::bind(){
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ID);
}
/// ---------------------- VAO ----------------------
VAO& VAO::setup(){
    gl::GenVertexArrays(1, &ID);
    gl::BindVertexArray(ID);
    return *this;
}
void VAO::clear(){
    if(not ID) return;
    for(u32 i=0; i<vboCount; i++)
        vbo[i].clear();
    ibo.clear();
    gl::DeleteVertexArrays(1, &ID);
}
void VAO::operator()(){
    gl::BindVertexArray(0);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
}
void VAO::bind(){
    gl::BindVertexArray(ID);
}
void VAO::unbind(){
    gl::BindVertexArray(0);
}
VAO& VAO::addBuffer(std::vector<float> &buffer, u32 numOfElements){
    vbo[vboCount].setup(buffer).attrib(vboCount).pointer_float(numOfElements).divisor(0);
    vboCount++;
    return *this;
}
VAO& VAO::addBuffer(std::vector<glm::vec2> &buffer){
    vbo[vboCount].setup(buffer).attrib(vboCount).pointer_float(2).divisor(0);
    vboCount++;
    return *this;
}
VAO& VAO::addBuffer(std::vector<glm::vec3> &buffer){
    vbo[vboCount].setup(buffer).attrib(vboCount).pointer_float(3).divisor(0);
    vboCount++;
    return *this;
}
VAO& VAO::addBuffer(std::vector<glm::vec4> &buffer){
    vbo[vboCount].setup(buffer).attrib(vboCount).pointer_float(4).divisor(0);
    vboCount++;
    return *this;
}
VAO& VAO::addBuffer(std::vector<u32> &buffer){
    ibo.setup(buffer);
    triangleCount = buffer.size()/3;
    info("VAO "+std::to_string(ID), "triangles:", triangleCount);
    return *this;
}

/// ---------------------- Shader ----------------------
Shader::~Shader(){
    if(binded){
        gl::BindTexture(gl::TEXTURE_2D, 0);
        // gl::BindVertexArray(0);
        gl::UseProgram(0);
    }
}
Shader& Shader::bind(){
    gl::UseProgram(ID);
    binded = true;
    return *this;
}
Shader& Shader::texture(const GLint uniformId, u32 texture, u32 id){
    gl::Uniform1i(uniformId, id);
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(gl::TEXTURE_2D, texture);
    return *this;
}
Shader& Shader::texture(const std::string &name, u32 texture, u32 id){
    gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), id);
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(gl::TEXTURE_2D, texture);
    return *this;
}
Shader& Shader::texture(const std::string &name, Texture &texture, u32 id){
    gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), id);
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(texture.type, texture.ID);
    return *this;
}
Shader& Shader::atlas(const std::string &name, u32 texture, u32 id){
    gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), id);
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(gl::TEXTURE_2D_ARRAY, texture);
    return *this;
}
Shader& Shader::cubeMap(const std::string &name, u32 texture, u32 id){
    gl::Uniform1i(gl::GetUniformLocation(ID, name.c_str()), id);
    gl::ActiveTexture(gl::TEXTURE0 + id);
    gl::BindTexture(gl::TEXTURE_CUBE_MAP, texture);
    return *this;
}
i32 Shader::location(const std::string &name){
    return gl::GetUniformLocation(ID, name.c_str());
}
i32 Shader::uboLocation(const std::string &name){
    return gl::GetUniformBlockIndex(ID, name.c_str());
}

Shader& Shader::ubo(const char *name, u32 u, u32 bindingIndex, u32 maxSize){
    u32 index = gl::GetUniformBlockIndex(ID, name);
    gl::UniformBlockBinding(ID, index, bindingIndex);
    gl::BindBufferRange(gl::UNIFORM_BUFFER, bindingIndex, u, 0, sizeof(float) * maxSize);
    return *this;
}

Shader& Shader::uniform(const GLint name, const std::vector<glm::mat4> &m){
    gl::UniformMatrix4fv(name, m.size(), false, (float*)m.data());
    return *this;
}
Shader& Shader::uniform(const GLint name, const std::vector<float> &m){
    gl::Uniform1fv(name, m.size(),  m.data());
    return *this;
}
Shader& Shader::uniform(const GLint name, const glm::mat4 &m){
    gl::UniformMatrix4fv(name, 1, false, glm::value_ptr(m));
    return *this;
}
Shader& Shader::uniform(const GLint name, const glm::vec4 &v){
    gl::Uniform4fv(name, 1, glm::value_ptr(v));
    return *this;
}
Shader& Shader::uniform(const GLint name, const glm::vec3 &v){
    gl::Uniform3fv(name, 1, glm::value_ptr(v));
    return *this;
}
Shader& Shader::uniform(const GLint name, const glm::vec2 &v){
    gl::Uniform2fv(name, 1, glm::value_ptr(v));
    return *this;
}
Shader& Shader::uniform(const GLint name, float v){
    gl::Uniform1f(name, v);
    return *this;
}
Shader& Shader::uniform(const GLint name, int v){
    gl::Uniform1i(name, v);
    return *this;
}
Shader& Shader::uniform(const GLint name, uint32_t v){
    gl::Uniform1ui(name, v);
    return *this;
}
Shader& Shader::uniform(const GLint name, uint16_t v){
    gl::Uniform1ui(name, v);
    return *this;
}

Shader& Shader::uniform(const char *name, const std::vector<glm::mat4> &m){
    gl::UniformMatrix4fv(gl::GetUniformLocation(ID,name), m.size(), false, (float*)m.data());
    return *this;
}
Shader& Shader::uniform(const char *name, const std::vector<float> &m){
    gl::Uniform1fv(gl::GetUniformLocation(ID,name), m.size(), m.data());
    return *this;
}
Shader& Shader::uniform(const char *name, const glm::mat4 &m){
    gl::UniformMatrix4fv(gl::GetUniformLocation(ID,name), 1, false, glm::value_ptr(m));
    return *this;
}
Shader& Shader::uniform(const char *name, const glm::vec4 &v){
    gl::Uniform4fv(gl::GetUniformLocation(ID, name), 1, glm::value_ptr(v));
    return *this;
}
Shader& Shader::uniform(const char *name, const glm::vec3 &v){
    gl::Uniform3fv(gl::GetUniformLocation(ID, name), 1, glm::value_ptr(v));
    return *this;
}
Shader& Shader::uniform(const char *name, const glm::vec2 &v){
    gl::Uniform2fv(gl::GetUniformLocation(ID, name), 1, glm::value_ptr(v));
    return *this;
}
Shader& Shader::uniform(const char *name, float v){
    gl::Uniform1f(gl::GetUniformLocation(ID, name), v);
    return *this;
}
Shader& Shader::uniform(const char *name, int v){
    gl::Uniform1i(gl::GetUniformLocation(ID, name), v);
    return *this;
}
Shader& Shader::uniform(const char *name, uint32_t v){
    gl::Uniform1i(gl::GetUniformLocation(ID, name), v);
    return *this;
}

namespace {
    std::string loadFile(const std::string &fileName){
        std::ifstream in(fileName, std::ios::in | std::ios::binary);
        if(in) {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
         }
        else {
            error("Unable to open file",fileName);
            hardPause();
            return "";
            // exit(1);
        }
    }

    void printShaderInfoLog(GLint shader){
        int infoLogLen = 0;
        int charsWritten = 0;
        char *infoLog;

        gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &infoLogLen);

        if (infoLogLen > 0){
            infoLog = new GLchar[infoLogLen];
            gl::GetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
            error("Shader compilation error:\n", infoLog);
            delete [] infoLog;
        }

    }

    std::string defines(const std::string &shader){
        return loadFile("../shaders/Defines.hpp");
        std::string out;
        // for(auto &define : Global::settings.settings["Defines"][shader]){
        //     out += "#define "s + define.key + " "s + define.string() + "\n"s;
        // }
        return out;
    }

    bool findGeometryShader(const std::string &shader){
        return shader.find("GEOMETRY_SHADER") != std::string::npos;
    }

    void resolveIncludes(std::string &source){
        auto pos = source.find("#include");

        while(pos != std::string::npos){
            auto endpos = pos;
            for(auto i=pos; i<pos+60; i++){
                if(source[i] != '\n') endpos++;
                else break;
            }
            auto include = source.substr(pos, endpos-pos);
            auto f = include.find("\"")+1;
            auto fileToInclude = include.substr(f, include.rfind("\"")-f);

            std::string pathTo;
            if(not findFile("../shaders/", fileToInclude, "-r", pathTo)){
                error("Cannot resolve include:", fileToInclude);
            }

            source.replace(pos, endpos-pos, loadFile(pathTo));

            pos = source.find("#include", endpos);
        }
    }
}

void Shader::loadFromFile(const std::string pathTo, const std::string name){
    if(ID){
        gl::DeleteProgram(ID);
    }

    GLuint vertexS, geometryS, fragmentS;

    std::string shaderSource = loadFile(pathTo);
    resolveIncludes(shaderSource);

    /// TODO: maybe make defines global? then we can put functions there
    /// TODO: add #line
    std::string vertexSource = "#version 330\n" + defines(name) + "#define VERTEX_SHADER\n" + shaderSource;
    std::string geometrySource = "#version 330\n" + defines(name) + "#define GEOMETRY_SHADER\n" + shaderSource;
    std::string fragmentSource = "#version 330\n" + defines(name) + "#define FRAGMENT_SHADER\n" + shaderSource;

    vertexS = gl::CreateShader(gl::VERTEX_SHADER);
    geometryS = findGeometryShader(shaderSource) ? gl::CreateShader(gl::GEOMETRY_SHADER) : 0;
    fragmentS = gl::CreateShader(gl::FRAGMENT_SHADER);


    GLint compiled;
    if(vertexS){
        const char *vs_str = vertexSource.c_str();
        gl::ShaderSource(vertexS, 1, &vs_str, nullptr);

        gl::CompileShader(vertexS);

        gl::GetShaderiv(vertexS, gl::COMPILE_STATUS, &compiled);

        if (!compiled){
            error("Vertex shader in", name, "not compiled.");
            printShaderInfoLog(vertexS);
            std::cin.ignore();

            return loadFromFile(pathTo, name);
        }
    }
    if(geometryS){
        const char *gs_str = geometrySource.c_str();
        gl::ShaderSource(geometryS, 1, &gs_str, nullptr);

        gl::CompileShader(geometryS);

        gl::GetShaderiv(geometryS, gl::COMPILE_STATUS, &compiled);

        if (!compiled){
            error("Geometry shader in", name, "not compiled.");
            printShaderInfoLog(geometryS);
            std::cin.ignore();

            return loadFromFile(pathTo, name);
        }
    }
    if(fragmentS){
        const char *fs_str = fragmentSource.c_str();
        gl::ShaderSource(fragmentS, 1, &fs_str, nullptr);

        gl::CompileShader(fragmentS);

        gl::GetShaderiv(fragmentS, gl::COMPILE_STATUS, &compiled);

        if (!compiled){
            error("Fragment shader in", name, "not compiled.");
            printShaderInfoLog(fragmentS);
            std::cin.ignore();

            return loadFromFile(pathTo,name);
        }
    }
    ID = gl::CreateProgram();

    gl::AttachShader(ID, vertexS);
    if(geometryS) gl::AttachShader(ID, geometryS);
    gl::AttachShader(ID, fragmentS);

    gl::LinkProgram(ID);
    /// if fuckup happen: https://www.opengl.org/discussion_boards/showthread.php/181432-Correct-way-to-delete-shader-programs
    gl::DeleteShader(vertexS);
    if(geometryS) gl::DeleteShader(geometryS);
    gl::DeleteShader(fragmentS);

    gl::DetachShader(ID, vertexS);
    if(geometryS) gl::DetachShader(ID, geometryS);
    gl::DetachShader(ID, fragmentS);
}

void Mesh::render(){
    if(mode == 0)
        gl::DrawElements(gl::TRIANGLES, count, gl::UNSIGNED_INT, offset());
    if(mode == gl::TRIANGLE_STRIP)
        gl::DrawArrays(gl::TRIANGLE_STRIP, begin, count);

}

State& State::blendFunc(u32 src, u32 dst){
    if(not m_blend) return *this;
    m_blendSRC = src;
    m_blendDST = dst;
    gl::BlendFunc(m_blendSRC, m_blendDST);
    return *this;
}
State& State::depthTest(bool b){
    if(b == m_depthTest) return *this;
    m_depthTest = b;
    if(b) gl::Enable(gl::DEPTH_TEST);
    else gl::Disable(gl::DEPTH_TEST);
    return *this;
}
State& State::depthMask(bool b){
    if(b == m_depthMask) return *this;
    m_depthMask = b;
    if(m_depthMask) gl::DepthMask(gl::FALSE_);
    else gl::DepthMask(gl::TRUE_);
    return *this;
}
State& State::culling(bool b, bool ccw){
    if(b == m_cullFace and ccw == m_frontFace) return *this;
    m_cullFace = b;
    m_frontFace = ccw;
    if(m_cullFace){
        gl::Enable(gl::CULL_FACE);
        m_frontFace ? gl::FrontFace(gl::CCW) : gl::FrontFace(gl::CCW);
    }
    else gl::Disable(gl::CULL_FACE);
    return *this;
}
State& State::depthFunc(u32 func){
    if(func == m_depthFunc or not m_depthTest) return *this;
    m_depthFunc = func;
    gl::DepthFunc(m_depthFunc);
    return *this;
}
