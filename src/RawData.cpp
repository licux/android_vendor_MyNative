#include "RawData.h"

void* RawData::getReadHeader(){
    return (void *)read_head;
}

int RawData::getAvailableBytes(){
    return length - ((int)read_head - (int)((uint8_t*)head));
}

void RawData::offsetHeader(int offsetBytes){
    read_head += offsetBytes;
}

int8_t RawData::read8(){
    int8_t ret = *read_head;
    read_head++;
    return ret;
}

int16_t RawData::readBE16(){
    int16_t h = (int16_t)(read_head)[0] & 0xff;
    int16_t l = (int16_t)(read_head)[1] & 0xff;

    read_head += 2;

    return (h << 8) | l;
}

int32_t RawData::readLE32(){
    int32_t ml = (int16_t)(read_head)[0] & 0xff;
    int32_t  l = (int16_t)(read_head)[1] & 0xff;
    int32_t  h = (int16_t)(read_head)[2] & 0xff;
    int32_t mh = (int16_t)(read_head)[3] & 0xff;

    read_head += 4;

    return (mh << 24) | (h << 16) | (l << 8) | ml;
}