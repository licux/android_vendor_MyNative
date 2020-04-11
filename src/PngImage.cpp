#include "PngImage.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "PngImage", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "PngImage", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "PngImage", __VA_ARGS__))

static AAsset* asset;

void png_asset_read(png_structp png, png_bytep data, png_size_t length){
    AAsset_read(asset, data, length);
}

// PngImage::PngImage(const std::string file_name)
//  : file_name(file_name)
//  , width(0)
//  , height(0)
//  , data(0)
//  {
//     FILE* file = fopen(file_name.c_str(), "rb");
//     if(file == nullptr){
//         LOGE("Image was not found:%s", file_name.c_str());
//         return;
//     }

//     png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
//     if(png == nullptr){
//         LOGE("png_create_read_struct error!");
//         fclose(file);
//         return;
//     }

//     png_infop info = png_create_info_struct(png);
//     if(info == nullptr){
//         LOGE("png_create_info_struct error!");
//         png_destroy_read_struct(&png, nullptr, nullptr);
//         fclose(file);
//         return;
//     }

//     if(setjmp(png_jmpbuf(png))){
//         LOGE("png_jmpbuf error!");
//         png_destroy_read_struct(&png, nullptr, nullptr);
//         fclose(file);
//         return;
//     }

//     png_init_io(png, file);
    
//     unsigned int sig_bytes = 0;
//     png_set_sig_bytes(png, sig_bytes);
//     png_read_png(png, info, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), nullptr);
//     png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);
    
//     unsigned int row_bytes = png_get_rowbytes(png, info);
//     data = new unsigned char[row_bytes * height];
//     png_bytepp rows = png_get_rows(png, info);
//     for(int i = 0; i < height; i++){
//         memcpy(data + (row_bytes * i), rows[i], row_bytes);
//     }

//     png_destroy_read_struct(&png, &info, nullptr);
// }

PngImage::PngImage(AAssetManager *mgr, const std::string file_name)
 : file_name(file_name)
 , width(0)
 , height(0)
 , data(0)
 {
    asset = AAssetManager_open(mgr, file_name.c_str(), AASSET_MODE_STREAMING);
    if(!asset){
        LOGE("Error open : %s", file_name.c_str());
        return;
    }

    png_byte header[8];

    // read header
    AAsset_read(asset, header, 8);

    int isPng = !png_sig_cmp(header, 0, 8);
    if(!isPng){
        AAsset_close(asset);
        LOGE("Not a png file!");
        return;
    }

    // crate png struct
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(png == nullptr){
        LOGE("png_create_read_struct error!");
        return;
    }

    // create png info struct 
    png_infop info = png_create_info_struct(png);
    if(info == nullptr){
        LOGE("png_create_info_struct error!");
        png_destroy_read_struct(&png, nullptr, nullptr);
        return;
    }

    if(setjmp(png_jmpbuf(png))){
        LOGE("png_jmpbuf error!");
        png_destroy_read_struct(&png, nullptr, nullptr);
        return;
    }

    // init png reading
    // png_init_io(png, file);
    png_set_read_fn(png, NULL, png_asset_read);
    
    unsigned int sig_bytes = 8;
    png_set_sig_bytes(png, sig_bytes);

    // read all info up to image data
    png_read_info(png, info);

    // get info about png
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);
    
    // Update the png info struct
    png_read_update_info(png, info);

    unsigned int row_bytes = png_get_rowbytes(png, info);

    data = new unsigned char[row_bytes * height];
    png_bytep* row_pointer = new png_bytep[height];

    for(int i = 0; i < height; i++){
        row_pointer[i] = data + i * row_bytes;
    }

    // read the png into data through row_pointer
    png_read_image(png, row_pointer);

    png_destroy_read_struct(&png, &info, nullptr);
    delete[] row_pointer;
    AAsset_close(asset);
}

PngImage::~PngImage(){
    if(data){
        delete[] data;
    }
}

int PngImage::getWidth(){
    return width;
}

int PngImage::getHeight(){
    return height;
}

bool PngImage::hasAlpha(){
    if(color_type == PNG_COLOR_TYPE_RGBA){
        return true;
    }
    return false;
}

unsigned char* PngImage::getData(){
    return data;
}
