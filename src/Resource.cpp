//
// Created by masaki on 20/04/02.
//

#include "Resource.h"

Resource::Resource(android_app* app)
 : assetManager(app->activity->assetManager)
 , asset(NULL)
{

}

int Resource::open(const char *path) {
    asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
    return (asset != NULL) ? 0 : -1;
}

void Resource::close(){
    if(asset != NULL){
        AAsset_close(asset);
        asset = NULL;
    }
}

int Resource::read(void *buffer, size_t size) {
    int count = AAsset_read(asset, buffer, size);
    return (count == size) ? 0 : -1;
}