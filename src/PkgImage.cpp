#include <android/log.h>
#include <cassert>
#include "PkgImage.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "PkgImage", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "PkgImage", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "PkgImage", __VA_ARGS__))

PkgImage::PkgImage(AAssetManager *mgr, const std::string file_name)
 : file_name(file_name)
 , width(0)
 , height(0)
 , rawdata()
{
    AAsset* asset = AAssetManager_open(mgr, file_name.c_str(), AASSET_MODE_STREAMING);
    if(!asset){
        LOGE("Error open: %s", file_name.c_str());
        return;
    }

    rawdata.length = AAsset_getLength(asset);
    LOGI("Load file size : %d", rawdata.length);
    rawdata.head = (void*)(new uint8_t[rawdata.length]);
    memcpy(rawdata.head, AAsset_getBuffer(asset), rawdata.length);

    rawdata.read_head = (uint8_t*)rawdata.head;

    AAsset_close(asset);

    {
        char magic[4] = {0};
        magic[0] = rawdata.read8();
        magic[1] = rawdata.read8();
        magic[2] = rawdata.read8();
        magic[3] = rawdata.read8();

        LOGI("magic : [%c][%c][%c][%c]", magic[0], magic[1], magic[2], magic[3]);
        assert(magic[0] == 'P');
        assert(magic[1] == 'K');
        assert(magic[2] == 'M');
    }
    {
        char version[2] = {0};
        version[0] = rawdata.read8();
        version[1] = rawdata.read8();
        LOGI("version : [%d].[%d]", version[0], version[1]);
    }

    data_type = rawdata.readBE16();
    width = rawdata.readBE16();
    height = rawdata.readBE16();
    origin_width = rawdata.readBE16();
    origin_height = rawdata.readBE16();

    data = rawdata.getReadHeader();
    data_bytes = rawdata.getAvailableBytes();

    assert(width >= origin_width);
    assert(height >= origin_height);

    LOGI("ETC1 byte(%d) tex size(%d x %d) origin size(%d x %d)", data_bytes, width, height, origin_width, origin_height);

}

PkgImage::~PkgImage()
{
    delete[] (uint8_t*)rawdata.head;
}

int PkgImage::getWidth(){
    return width;
}

int PkgImage::getHeight(){
    return height;
}

void* PkgImage::getData(){
    return data;
}

int PkgImage::getDataBytes(){
    return data_bytes;
}