#ifndef RX5808_H
#define RX5808_H

#include <Arduino.h>
#include <SPI.h>

class Rx5808 {
  public:
    Rx5808(int pinCS, int pinRSSI) {
        _pinCS = pinCS;
        _pinRSSI = pinRSSI;
    }

    void begin() {
        pinMode(_pinCS, OUTPUT);
        pinMode(_pinRSSI, INPUT);
        digitalWrite(_pinCS, HIGH); // Деактивуємо SPI
        SPI.begin();
    }

    // Встановлення частоти
    void setFrequency(uint16_t frequency) {
        uint16_t regValue = (frequency - 479) / 2;
        writeRegister(0xA000 | regValue); // Регістр A
        writeRegister(0xB000 | regValue); // Регіст B
    }

    // Зчитування RSSI
    int getRSSI() {
        return map(analogRead(_pinRSSI), 0, 1023, 0, 100); // Повертає рівень сигналу у відсотках
    }

    // Зчитування каналу (з таблиці частот)
    uint16_t getChannelFrequency(uint8_t channelIndex) {
        if (channelIndex < 48) {
            return _frequencyTable[channelIndex];
        }
        return 0; // Невірний канал
    }

    // Встановлення каналу (з таблиці частот)
    void setChannel(uint8_t channelIndex) {
        if (channelIndex < 48) {
            setFrequency(_frequencyTable[channelIndex]);
        }
    }

  private:
    int _pinCS, _pinRSSI;

    const uint16_t _frequencyTable[48] = {
        5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725, // Band A
        5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866, // Band B
        5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945, // Band E
        5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880, // Band F (Airwave)
        5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917, // Raceband
        5333, 5373, 5413, 5453, 5493, 5533, 5573, 5613  // Low band
    };

    void writeRegister(uint16_t data) {
        digitalWrite(_pinCS, LOW); // Активуємо SPI
        delayMicroseconds(1);
        SPI.transfer16(data);
        delayMicroseconds(1);
        digitalWrite(_pinCS, HIGH); // Деактивуємо SPI
        delay(5); // Затримка для стабілізації
    }
};

#endif