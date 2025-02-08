#ifndef SM1370R_H
#define SM1370R_H

#include <Arduino.h>

class SM1370R {
  public:
    SM1370R(int pinCS1, int pinCS2, int pinCS3, int pinRSSI) {
        _pinCS1 = pinCS1;
        _pinCS2 = pinCS2;
        _pinCS3 = pinCS3;
        _pinRSSI = pinRSSI;
    }

    void begin() {
        pinMode(_pinCS1, OUTPUT);
        pinMode(_pinCS2, OUTPUT);
        pinMode(_pinCS3, OUTPUT);
        pinMode(_pinRSSI, INPUT);

        setChannel(0); // Встановлюємо початковий канал
    }

    void setChannel(uint8_t channel) {
        if (channel > 7) {
            channel = 0; // Якщо канал виходить за межі, повертаємо на 0
        }

        _currentChannel = channel;

        // Встановлюємо біти CS1, CS2, CS3 для перемикання каналів
        digitalWrite(_pinCS1, channel & 0x01);
        digitalWrite(_pinCS2, (channel >> 1) & 0x01);
        digitalWrite(_pinCS3, (channel >> 2) & 0x01);

        delay(10); // Затримка для стабілізації
    }

    uint8_t getChannel() {
        return _currentChannel;
    }

    void nextChannel() {
        _currentChannel = (_currentChannel + 1) % 8;
        setChannel(_currentChannel);
    }

    int getRSSI() {
        return map(analogRead(_pinRSSI), 0, 1023, 0, 100); // Повертає рівень сигналу у відсотках
    }

    bool isVideoPresent(int threshold = 50) {
        int rssi = getRSSI();
        return rssi > threshold; // Перевіряємо, чи перевищує RSSI поріг
    }

  private:
    int _pinCS1, _pinCS2, _pinCS3, _pinRSSI;
    uint8_t _currentChannel = 0;

    const float frequencyTable[8] = {1080, 1120, 1160, 1200, 1240, 1280, 1320, 1360};
};

#endif