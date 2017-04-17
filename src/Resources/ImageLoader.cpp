#include "gl_core_4_5.hpp"
#include "ImageLoader.hpp"
#include "Logging.hpp"
#include <FreeImage.h>
#include <FreeImagePlus.h>
// #include "PerfTimers.hpp"

namespace ImageUtils
{
bool checkErrors(const std::string &text, const std::string &file, int line, const std::string &fun){
    int err = gl::GetError();
    if(err != gl::NO_ERROR_){
        error("GLError:", err, file+"#", line, ":", fun, "::", text);
        return true;
    }
    return false;
}
#define CHECK_FOR_ERRORS(x) checkErrors(x, __FILE__, __LINE__, __FUNCTION__);
#define logLine log(__LINE__, __FUNCTION__);

/// TODO: add hint for mipmaps
/// TODO: add validation of loadToMemory
ImageParams loadImageToGpu(const std::string &filePath){
    // CPU_SCOPE_TIMER("Loading image");

    auto data = loadToMemory(filePath, ImageDataType::Any8);
    if(data.size() == 0) return {};

    gl::GenTextures(1, &data.id);
    gl::BindTexture(gl::TEXTURE_2D, data.id);
    /// TODO: set from hints
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);

    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAX_LEVEL, 15);

    gl::TexImage2D(gl::TEXTURE_2D,
                    0,
                    data.internalFormat,
                    data.width,
                    data.height,
                    0,
                    data.format,
                    data.type,
                    data.data);

    gl::GenerateMipmap(gl::TEXTURE_2D);

    gl::BindTexture(gl::TEXTURE_2D, 0);

    CHECK_FOR_ERRORS("filePath")

    delete [] (u8*)data.data;
    data.data = nullptr;
    return data;
}
ImageParams loadArrayToGpu(const std::vector<std::string> &filePathes){
    // CPU_SCOPE_TIMER("Loading image array");
    ImageParams data {};
    u32 layers = filePathes.size();
    ImageParams tmp[layers];

    for(u32 i=0; i<layers; i++){
        tmp[i] = loadToMemory(filePathes[i], ImageDataType::RGBA8);
    }

    data = tmp[0];
    data.data = new u8[data.dataSize*layers];
    u8 *ptr = (u8*)data.data;

    for(u32 i=0; i<layers; i++){
        if(data.dataSize != tmp[i].dataSize){
            error(filePathes[i], "wrong image size:", tmp[i].dataSize, "expected:", data.dataSize);
            delete [] (u8*)tmp[i].data;
            tmp[i].data = nullptr;
            continue;
        }
        ptr = std::copy((u8*)tmp[i].data, (u8*)tmp[i].data + data.dataSize, ptr);
        delete [] (u8*)tmp[i].data;
        tmp[i].data = nullptr;
        ++data.layers;
    }

    gl::GenTextures(1, &data.id);
    gl::BindTexture(gl::TEXTURE_2D_ARRAY, data.id);

    gl::TexParameteri(gl::TEXTURE_2D_ARRAY, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D_ARRAY, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TexParameteri(gl::TEXTURE_2D_ARRAY, gl::TEXTURE_WRAP_S, gl::REPEAT);
    gl::TexParameteri(gl::TEXTURE_2D_ARRAY, gl::TEXTURE_WRAP_T, gl::REPEAT);

    gl::TexImage3D(gl::TEXTURE_2D_ARRAY, 0, data.internalFormat, data.width, data.height, data.layers, 0, data.format, data.type, data.data);

    gl::GenerateMipmap(gl::TEXTURE_2D_ARRAY);
    gl::BindTexture(gl::TEXTURE_2D_ARRAY, 0);

    delete [] (u8*)data.data;
    data.data = nullptr;

    CHECK_FOR_ERRORS(filePathes[0])

    return data;
}
ImageParams loadCubemapToGpu(const std::vector<std::string> &filePathes){
    // CPU_SCOPE_TIMER("Loading cubemap");
    if(filePathes.size() != 6){
        error("wrong number of cubemap faces");
        return {};
    }
    ImageParams data {};
    u32 count = filePathes.size();
    ImageParams cubemapFaces[count];

    for(u32 i=0; i<count; i++){
        cubemapFaces[i] = loadToMemory(filePathes[i], ImageDataType::RGBA8);
    }
    data = cubemapFaces[0];
    data.data = nullptr;

    gl::Enable(gl::TEXTURE_CUBE_MAP_SEAMLESS);
    gl::GenTextures(1, &data.id);
    gl::BindTexture(gl::TEXTURE_CUBE_MAP, data.id);

    gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
    gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
    gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
    gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
    gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);
    gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAX_LEVEL, 15);

    for(u32 face=0; face < 6; face++)
        gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X+face, 0, gl::RGBA8, cubemapFaces[face].width, cubemapFaces[face].height, 0, gl::BGRA, gl::UNSIGNED_BYTE, cubemapFaces[face].data);


    gl::GenerateMipmap(gl::TEXTURE_CUBE_MAP);
    gl::BindTexture(gl::TEXTURE_CUBE_MAP, 0);

    for(u32 i=0; i<count; i++){
        delete [] (u8*)cubemapFaces[i].data;
    }

    CHECK_FOR_ERRORS(filePathes[0])

    return data;
}

    // R8, R16, R16F, R32, R32F, RGBA8, RGBA16, RGBA32F
/// http://freeimage.sourceforge.net/fip/classfipImage.html
/// by default fi is storing in bgra format
/// http://freeimage.sourceforge.net/fnet/html/7E93CA66.htm
ImageParams loadToMemory(const std::string &filePath, ImageDataType targetType){
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    fif = FreeImage_GetFileType(filePath.c_str(), 0);
    if(fif == FIF_UNKNOWN){
        error("unknown file type: " + filePath);
        return {};
    }

    FIBITMAP *dib = FreeImage_Load(fif, filePath.c_str());
    if(dib == nullptr){
        error("cant open file:" + filePath);
        return {};
    }
    auto bitsPerPixel = FreeImage_GetBPP(dib);
    auto imageType = FreeImage_GetImageType(dib);

    ImageParams data {};

    FIBITMAP *bitmap = dib;

    switch(imageType){
        case FIT_BITMAP : {
            if(bitsPerPixel == 8){
                if(targetType == Any8){
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::RED;
                    data.type = gl::UNSIGNED_BYTE;
                }
                else if(targetType == R8){
                    bitmap = FreeImage_ConvertTo8Bits(dib);
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::RED;
                    data.type = gl::UNSIGNED_BYTE;
                }
                else if(targetType == RGBA8){
                    bitmap = FreeImage_ConvertTo32Bits(dib);
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::BGRA;
                    data.type = gl::UNSIGNED_BYTE;
                }
            }
            else if(bitsPerPixel == 24){
                if(targetType == Any8){
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::BGR;
                    data.type = gl::UNSIGNED_BYTE;
                }
                else if(targetType == R8){
                    bitmap = FreeImage_ConvertTo8Bits(dib);
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::RED;
                    data.type = gl::UNSIGNED_BYTE;
                }
                else if(targetType == RGBA8){
                    bitmap = FreeImage_ConvertTo32Bits(dib);
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::BGRA;
                    data.type = gl::UNSIGNED_BYTE;
                }
            }
            else if(bitsPerPixel == 32){
                if(targetType == Any8){
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::BGRA;
                    data.type = gl::UNSIGNED_BYTE;
                }
                else if(targetType == R8){
                    bitmap = FreeImage_ConvertTo8Bits(dib);
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::RED;
                    data.type = gl::UNSIGNED_BYTE;
                }
                else if(targetType == RGBA8){
                    data.internalFormat = gl::RGBA8;
                    data.format = gl::BGRA;
                    data.type = gl::UNSIGNED_BYTE;
                }
            }
            break;
        }
        case FIT_UINT16 : {
            log("FIT_UINT16");
            if(targetType == R16){
                bitsPerPixel = 16;
                data.internalFormat = gl::R16;
                data.format = gl::RED;
                data.type = gl::UNSIGNED_SHORT;
            }
            break;
        }
        case FIT_INT16 : {}
        case FIT_UINT32 : {}
        case FIT_INT32 : {}
        case FIT_FLOAT : {}
        case FIT_RGB16 : {}
        case FIT_RGBA16 : {}
        case FIT_RGBF : {}
        case FIT_RGBAF : {}
        default: {
            error(filePath, "has not supported type:", imageType);
            return {};
        }
    }
    if(dib != bitmap) FreeImage_Unload(dib);
    dib = nullptr;

    data.width = FreeImage_GetWidth(bitmap);
    data.height = FreeImage_GetHeight(bitmap);

    bitsPerPixel = FreeImage_GetBPP(bitmap);
    data.dataSize = data.width*data.height * (bitsPerPixel/8);
    data.data = new u8[data.dataSize];

    size_t scanlineWidth = data.width*(bitsPerPixel/8);
    for(auto y=0; y<data.height; y++){
        auto *bits = (u8*)FreeImage_GetScanLine(bitmap, y);
        std::copy(bits, bits + scanlineWidth, (u8*)data.data + y*scanlineWidth);
    }

    FreeImage_Unload(bitmap);

    return data;
}

bool saveFromMemory(const std::string &filePath, ImageDataType targetType, ImageParams image){
    /// TODO: best way is to spawn another thread to save file
    FIBITMAP *dib;
    log("saving:", filePath);

    /// for floating point types see: https://sourceforge.net/p/freeimage/discussion/36111/thread/dce7f4a3/
    if(image.dataType != targetType){
        /// convert
    }
    switch(image.dataType){
        case R8: {
            dib = FreeImage_AllocateT(FIT_BITMAP, image.width, image.height, 8);
            for(auto y=0; y<image.height; y++){
                auto *bits = (u8*)FreeImage_GetScanLine(dib, y);
                for(auto x=0; x<image.width; x++){
                    bits[x] = *((u8*)image.data + x + y*image.width);
                }
            }
            return FreeImage_Save(FIF_PNG, dib, filePath.c_str(), PNG_DEFAULT);
        }
        case R16: {
            dib = FreeImage_AllocateT(FIT_UINT16, image.width, image.height, 16);
            for(auto y=0; y<image.height; y++){
                auto *bits = (u16*)FreeImage_GetScanLine(dib, y);
                for(auto x=0; x<image.width; x++){
                    bits[x] = *((u16*)image.data + x + y*image.width);
                }
            }
            return FreeImage_Save(FIF_PNG, dib, filePath.c_str(), PNG_DEFAULT);
        }
        case R32F: {
            dib = FreeImage_AllocateT(FIT_FLOAT, image.width, image.height, 16);
            for(auto y=0; y<image.height; y++){
                auto *bits = (float*)FreeImage_GetScanLine(dib, y);
                for(auto x=0; x<image.width; x++){
                    bits[x] = *((float*)image.data + x + y*image.width);
                }
            }
            return FreeImage_Save(FIF_EXR, dib, filePath.c_str(), EXR_FLOAT);
        }
        case RGB8: {
            dib = FreeImage_AllocateT(FIT_BITMAP, image.width, image.height, 24);
            for(auto y=0; y<image.height; y++){
                auto *bits = (u8*)FreeImage_GetScanLine(dib, y);
                for(auto x=0; x<image.width*3; x+=3){
                    bits[x+0] = *((u8*)image.data + x + 0 + y*image.width*3);
                    bits[x+1] = *((u8*)image.data + x + 1 + y*image.width*3);
                    bits[x+2] = *((u8*)image.data + x + 2 + y*image.width*3);
                }
            }
            return FreeImage_Save(FIF_JPEG, dib, filePath.c_str(), JPEG_QUALITYSUPERB);
        }
        case RGBA8: {
            dib = FreeImage_AllocateT(FIT_BITMAP, image.width, image.height, 32);
            for(auto y=0; y<image.height; y++){
                auto *bits = (u8*)FreeImage_GetScanLine(dib, y);
                for(auto x=0; x<image.width*4; x+=4){
                    bits[x+0] = *((u8*)image.data + x + 0 + y*image.width*4);
                    bits[x+1] = *((u8*)image.data + x + 1 + y*image.width*4);
                    bits[x+2] = *((u8*)image.data + x + 2 + y*image.width*4);
                    bits[x+3] = *((u8*)image.data + x + 3 + y*image.width*4);
                }
            }
            return FreeImage_Save(FIF_PNG, dib, filePath.c_str(), PNG_DEFAULT);
        }
        default: error("unsupported file format:", image.dataType);
    }

    return false;
}
}
