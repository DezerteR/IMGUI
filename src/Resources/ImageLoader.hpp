#pragma once
/// TODO: remove deps
#include "Common.hpp"

struct ImageData
{
    ImageData() = default;
    ImageData(ImageData&&) = default;

    enum Datatype {
        Bitmap, U16, I16, U32, I32, Float
    };

    u32 ID;
    u32 width;
    u32 height;

    u32 numOfChannels;
    u32 channels;
    u32 channelSize;
    u32 bitDepth; /// image depth in bytes: 1,2,4:(u8, u16, u32/foat); bytes per channel

    u32 originalFormat;
    u32 format; /// format of loaded data
    u32 internalFormat; /// format in GPU
    u32 type; /// data type of image, u8, hFloat, float

    u32 imageMagFilter;
    u32 imageMinFilter;

    u32 count;
    bool useMipmaps;
    Datatype datatype;
    const void *data {nullptr};
};

enum ImageDataType
{
    R8, R16, R16F, R32, R32F, RGB8, RGBA8, RGBA16, RGBA32F, Any8, Any16
};

namespace ImageUtils
{

struct ImageParams
{
    ImageDataType dataType;
    u32 id;
    u32 width;
    u32 height;
    u32 layers;

    u32 internalFormat; /// format in GPU
    u32 format; /// format of loaded data
    u32 type; /// data type of image, u8, hFloat, float

    const void *data {nullptr};
    size_t dataSize;
    u32 size(){
        // if(not data) return 0;
        /// TODO: improve it
        // else
        return width*height;
    }
    void clear(){
        delete [] (u8*)data;
    }
};

ImageParams loadImageToGpu(const std::string &filePath);
ImageParams loadArrayToGpu(const std::vector<std::string> &filePathes);
ImageParams loadCubemapToGpu(const std::vector<std::string> &filePathes);
ImageParams loadToMemory(const std::string &filePath, ImageDataType targetType);
bool saveFromMemory(const std::string &filePath, ImageDataType targetType, ImageParams image);

template<typename T>
bool save(const std::string &filePath, ImageDataType sourceType, ImageDataType targetType, std::vector<T> &data){
    return saveFromMemory(filePath, sourceType, targetType, (u8*)data.data(), data.size()*sizeof(T));
}

}
