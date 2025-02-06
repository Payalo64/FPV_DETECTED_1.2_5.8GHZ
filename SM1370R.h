#ifndef SM1370R_H
#define SM1370R_H

#include <Arduino.h>

class SM1370R {
public:
    SM1370R(uint8_t signalPin);  // Конструктор
    int getSignalLevel();        // Чтение уровня сигнала
private:
    uint8_t _signalPin;
};

#endif