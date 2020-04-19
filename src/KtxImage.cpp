#include <android/log.h>
#include <cassert>
#include "KtxImage.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "KtxImage", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "KtxImage", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "KtxImage", __VA_ARGS__))

KtxImage::KtxImage(AAssetManager *mgr, const std::string file_name)
 : file_name(file_name)
 , rawdata()
{
    AAsset* asset = AAssetManager_open(mgr, file_name.c_str(), AASSET_MODE_STREAMING);
    if(!asset){
        LOGE("Error open: %s", file_name.c_str());
        return;
    }

    const uint8_t KTXID[12] = {0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};

    rawdata.length = AAsset_getLength(asset);
    rawdata.head = (void*)(new uint8_t[rawdata.length]);
    memcpy(rawdata.head, AAsset_getBuffer(asset), rawdata.length);
    rawdata.read_head = (uint8_t*)rawdata.head;

    AAsset_close(asset);

    for(int index = 0; index < 12; index++){
        if(rawdata.read_head[index] != KTXID[index]){
            LOGE("Header Error(%s)", file_name.c_str());
            delete[] (uint8_t*)rawdata.head;
        }
    }
    rawdata.offsetHeader(12);

    int32_t check_endian = rawdata.readLE32();
    if(check_endian == 0x01020304){
        // Supposed to be Little Endian;
        // BUt this is Big Endian;
        LOGE("endian erorr(%s)", file_name.c_str());
        delete[] (uint8_t*)rawdata.head;
        return;
    }

    typedef struct KTXImageHeader{
        uint32_t glType;
        uint32_t glTypeSize;
        uint32_t glFormat;
        uint32_t glInternalFormat;
        uint32_t glBaseInternalFormat;
        uint32_t pixelWidth;
        uint32_t pixelHeight;
        uint32_t pixelDepth;
        uint32_t numberOfArrayElements;
        uint32_t numberOfFaces;
        uint32_t numberOfMipmapLevels;
        uint32_t bytesOfKeyValueData;
    } KTXImageHeader;

    KTXImageHeader *imageHeader = (KTXImageHeader*)rawdata.getReadHeader();

    LOGI("image format(%x)", imageHeader->glBaseInternalFormat);
    LOGI("image size(%dx%d", imageHeader->pixelWidth, imageHeader->pixelHeight);
    LOGI("image mipmaps(%d)", imageHeader->pixelHeight);
    LOGI("image key value data (%d bytes)", imageHeader->bytesOfKeyValueData);

    format = imageHeader->glBaseInternalFormat;
    width = imageHeader->pixelWidth;
    height = imageHeader->pixelHeight;
    mipmaps = imageHeader->numberOfMipmapLevels;

    data_bytes_table = new int[imageHeader->numberOfMipmapLevels];
    data_table = (void**) new void*[imageHeader->numberOfMipmapLevels];

    rawdata.offsetHeader(sizeof(KTXImageHeader));
    rawdata.offsetHeader(imageHeader->bytesOfKeyValueData);

    for(int mip_level = 0; mip_level < imageHeader->numberOfMipmapLevels; mip_level++){
        uint32_t image_size = rawdata.readLE32();
        LOGI("level(%d) image size(%d bytes", mip_level, image_size);
        data_bytes_table[mip_level] = image_size;
        data_table[mip_level] = rawdata.getReadHeader();
        rawdata.offsetHeader(image_size);
    }
}

KtxImage::~KtxImage(){
    delete[] data_bytes_table;
    delete[] data_table;
    delete[] (uint8_t*)rawdata.head;
}

int KtxImage::getWidth(){
    return width;
}

int KtxImage::getHeight(){
    return height;
}

void **KtxImage:: getDataTable(){
    return data_table;
}

int *KtxImage::getDataBytesTable(){
    return data_bytes_table;
}

GLenum KtxImage::getFormat(){
    return format;
}

int KtxImage::getMipsLevel(){
    return mipmaps;
}