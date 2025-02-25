#ifndef FT3500_H
#define FT3500_H

#include <Arduino.h>

class FT3500 {
public:
    FT3500(uint8_t s1, uint8_t s2, uint8_t s3, uint8_t cs1, uint8_t cs2, uint8_t cs3);
    
    void setChannel(uint8_t fr, uint8_t ch);
    void setFrequency(uint16_t freq);
    uint16_t getFrequency(uint8_t fr, uint8_t ch);
    int findNearestChannel(uint16_t freq);
    void scanChannels();

private:
    uint8_t _s1, _s2, _s3;
    uint8_t _cs1, _cs2, _cs3;

    void setPins(uint8_t s1, uint8_t s2, uint8_t s3, uint8_t cs1, uint8_t cs2, uint8_t cs3);
};

#endif