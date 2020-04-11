//
// Created by masaki on 20/04/02.
//

#ifndef NATIVE_ACTIVITY_RESOURCE_H
#define NATIVE_ACTIVITY_RESOURCE_H

#include <android_native_app_glue.h>
#include <android/asset_manager.h>

class Resource {

    AAssetManager* assetManager;
    AAsset* asset;
public:
    Resource(android_app* app);
    int open(const char* path);
    void close();

    int read(void *buffer, size_t size);
};


#endif //NATIVE_ACTIVITY_RESOURCE_H
