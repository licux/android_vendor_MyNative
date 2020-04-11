#ifndef ___PNGIMAGE_H__
#define ___PNGIMAGE_H__

#include <string>

#include "png.h"
#include <android/asset_manager.h>

class PngImage{
public:
    PngImage(AAssetManager *mgr, const std::string file_name);
    ~PngImage();
    int getWidth();
    int getHeight();
    bool hasAlpha();
    unsigned char* getData();

private:
    std::string file_name;
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int interlace_type;
    int compression_type;
    int filter_method;
    unsigned char* data;
};

#endif // ___PNGIMAGE_H__