#include "FT3500.h"

// Таблица частот (8 групп по 8 каналов)
const uint16_t FT3500_FREQUENCIES[8][8] = {
    {3360, 3380, 3400, 3420, 3440, 3460, 3480, 3500}, // FR1
    {3200, 3220, 3240, 3260, 3280, 3300, 3320, 3340}, // FR2
    {3330, 3350, 3370, 3390, 3410, 3430, 3450, 3470}, // FR3
    {3170, 3190, 3210, 3230, 3250, 3270, 3290, 3310}, // FR4
    {3320, 3345, 3370, 3395, 3420, 3445, 3470, 3495}, // FR5
    {3310, 3330, 3355, 3380, 3405, 3430, 3455, 3480}, // FR6
    {3220, 3240, 3260, 3280, 3300, 3320, 3340, 3360}, // FR7
    {3060, 3080, 3100, 3120, 3140, 3160, 3180, 3200}  // FR8
};

FT3500::FT3500(uint8_t s1, uint8_t s2, uint8_t s3, uint8_t cs1, uint8_t cs2, uint8_t cs3) {
    _s1 = s1; _s2 = s2; _s3 = s3;
    _cs1 = cs1; _cs2 = cs2; _cs3 = cs3;
    
    pinMode(_s1, OUTPUT);
    pinMode(_s2, OUTPUT);
    pinMode(_s3, OUTPUT);
    pinMode(_cs1, OUTPUT);
    pinMode(_cs2, OUTPUT);
    pinMode(_cs3, OUTPUT);
}

void FT3500::setChannel(uint8_t fr, uint8_t ch) {
    if (fr > 7 || ch > 7) return;
    setPins(fr & 0b001, fr & 0b010, fr & 0b100, ch & 0b001, ch & 0b010, ch & 0b100);
}

void FT3500::setFrequency(uint16_t freq) {
    int chIndex = findNearestChannel(freq);
    if (chIndex != -1) {
        setChannel(chIndex / 8, chIndex % 8);
    }
}

uint16_t FT3500::getFrequency(uint8_t fr, uint8_t ch) {
    if (fr > 7 || ch > 7) return 0;
    return FT3500_FREQUENCIES[fr][ch];
}

int FT3500::findNearestChannel(uint16_t freq) {
    int bestMatch = -1;
    uint16_t bestDiff = 65535; // Максимальная разница

    for (uint8_t fr = 0; fr < 8; fr++) {
        for (uint8_t ch = 0; ch < 8; ch++) {
            uint16_t f = FT3500_FREQUENCIES[fr][ch];
            uint16_t diff = abs(int(f) - int(freq));

            if (diff < bestDiff) {
                bestDiff = diff;
                bestMatch = fr * 8 + ch;
            }
        }
    }
    return bestMatch;
}

void FT3500::scanChannels() {
    for (uint8_t fr = 0; fr < 8; fr++) {
        for (uint8_t ch = 0; ch < 8; ch++) {
            uint16_t freq = getFrequency(fr, ch);
            setChannel(fr, ch);
            Serial.print("Scanning: ");
            Serial.print(freq);
            Serial.println(" MHz");
            delay(500);
        }
    }
}

void FT3500::setPins(uint8_t s1, uint8_t s2, uint8_t s3, uint8_t cs1, uint8_t cs2, uint8_t cs3) {
    digitalWrite(_s1, s1 ? HIGH : LOW);
    digitalWrite(_s2, s2 ? HIGH : LOW);
    digitalWrite(_s3, s3 ? HIGH : LOW);
    digitalWrite(_cs1, cs1 ? HIGH : LOW);
    digitalWrite(_cs2, cs2 ? HIGH : LOW);
    digitalWrite(_cs3, cs3 ? HIGH : LOW);
}