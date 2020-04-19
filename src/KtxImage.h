#ifndef ___KTXIMAGE_H__
#define ___KTXIMAGE_H__


#include <string>
#include <android/asset_manager.h>
#include <GLES2/gl2.h>
#include "RawData.h"

class KtxImage{
public:
    KtxImage(AAssetManager *mgr, const std::string file_name);
    ~KtxImage();
    int getWidth();
    int getHeight();
    void **getDataTable();
    int *getDataBytesTable();
    GLenum getFormat();
    int getMipsLevel();

private:
    std::string file_name;
    RawData rawdata;
    GLenum format;
    GLenum type;
    int width;
    int height;
    int mipmaps;
    void **data_table;
    int* data_bytes_table;
};

#endif 