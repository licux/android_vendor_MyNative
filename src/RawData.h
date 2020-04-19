#ifndef ___RAWDATA_H__
#define ___RAWDATA_H__

#include <stdint.h>
class RawData{
public:
    void *head;
    int length;
    uint8_t *read_head;

    void* getReadHeader();
    int getAvailableBytes();
    void offsetHeader(int offsetBytes);
    int8_t read8();
    int16_t readBE16();
    int32_t readLE32();

};


#endif