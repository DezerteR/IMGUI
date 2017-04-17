#pragma once
#include "Logging.hpp"

struct Texture
{
    Texture() = default;
    Texture(const Texture&) = default;
    Texture& operator = (const Texture&) = default;
    Texture(u32 type, GLint internalFormat, u32 width, u32 height, u32 layers, GLenum format, GLenum dataType, GLint filtering, u32 mipmapLevel) :
        type(type), internalFormat(internalFormat), width(width), height(height), layers(layers), format(format), dataType(dataType), filtering(filtering), mipmapLevel(mipmapLevel)
    {
        gl::GenTextures(1, &ID);
        gl::BindTexture(type, ID);

        gl::TexParameteri(type, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
        gl::TexParameteri(type, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
        gl::TexParameteri(type, gl::TEXTURE_MAG_FILTER, filtering);
        gl::TexParameteri(type, gl::TEXTURE_MIN_FILTER, filtering);

        if(format == gl::DEPTH_COMPONENT and type == gl::TEXTURE_2D_ARRAY){
            gl::TexParameteri(type, gl::TEXTURE_COMPARE_MODE, gl::COMPARE_REF_TO_TEXTURE);
            gl::TexParameteri(type, gl::TEXTURE_COMPARE_FUNC, gl::LEQUAL);
        }

        if(mipmapLevel){
            gl::TexParameteri(type, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
        }
        gl::TexParameteri(type, gl::TEXTURE_MAX_LEVEL, mipmapLevel);

        switch(type){
            case gl::TEXTURE_2D : {
                gl::TexImage2D(type, 0, internalFormat, width, height, 0, format, dataType, nullptr);
                break;
            }
            case gl::TEXTURE_2D_ARRAY : {
                gl::TexImage3D(type, 0, internalFormat, width, height, layers, 0, format, dataType, nullptr);
                break;
            }
            case gl::TEXTURE_3D : {
                gl::TexImage3D(type, 0, internalFormat, width, height, layers, 0, format, dataType, nullptr);
                break;
            }
            default : error("Unknown Texture type:", type);
        }

        genMipmaps();
        gl::BindTexture(type, 0);
        int err = gl::GetError();
        if(err != gl::NO_ERROR_){
            error("Texture initializatioin failure:", __FILE__, err);
        }
    }

    u32 type;
    GLint internalFormat;     /// gl::R8, gl::RGBA8, gl::RGBA16F, gl::R32F, gl::DEPTH24_STENCIL8, ...
    u32 width;
    u32 height;
    u32 layers;
    GLenum format;            /// gl::RGBA, gl::RED, gl::DEPTH_STENCIL, ...
    GLenum dataType;          /// gl::UNSIGNED_BYTE, gl::HALF_FLOAT, gl::FLOAT, gl::UNSIGNED_INT_24_8, ...
    GLint filtering;          /// gl::NEAREST, gl::LINEAR
    u32 mipmapLevel;
    GLuint ID {0};

    u32 mipmapLevelToAttach;
    GLuint genMipmaps(){
        if(mipmapLevel){
            gl::BindTexture(type, ID);
            gl::GenerateMipmap(type);
            gl::BindTexture(type, 0);
        }
        return 0;
    }
    void bind(u32 sampler = 0){
        gl::ActiveTexture(gl::TEXTURE0+sampler);
        gl::BindTexture(type, ID);
    }
    void bind(GLenum sampler, u32 uniform){
        gl::ActiveTexture(sampler);
        gl::Uniform1i(uniform, sampler-gl::TEXTURE0);
        gl::BindTexture(type, ID);
    }
    u32 noOfMipLevels(){
        return 1 + floor(log2(std::max(width, height)));
    }
    void swap(Texture &tex){
        std::swap(ID, tex.ID);
    }
    void remove(){
        gl::DeleteTextures(1, &ID);
    }
};

inline Texture genRGB16F(u32 width, u32 height){
    return Texture(gl::TEXTURE_2D, gl::RGB16F, width, height, 1, gl::RGB, gl::FLOAT, gl::LINEAR, 0);
}
inline Texture genRGB16F_3D(u32 width, u32 height, u32 layers){
    return Texture(gl::TEXTURE_3D, gl::RGB16F, width, height, layers, gl::RGB, gl::FLOAT, gl::LINEAR, 0);
}
