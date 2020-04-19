#ifndef ___PKGIMAGE_H__
#define ___PKGIMAGE_H__

#include <string>
#include <android/asset_manager.h>
#include "RawData.h"

class PkgImage{
public:
    PkgImage(AAssetManager *mgr, const std::string file_name);
    ~PkgImage();
    int getWidth();
    int getHeight();
    void* getData();
    int getDataBytes();
    
private:
    std::string file_name;
    int width;
    int height;
    int origin_width;
    int origin_height;
    int16_t data_type;
    RawData rawdata;
    void *data;
    int data_bytes;
};


#endif