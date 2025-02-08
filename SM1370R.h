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
    setChannel(0);  // Устанавливаем начальный канал
  }

  void setChannel(uint8_t channel) {
    if (channel > 7) {
      channel = 0;  // Если канал выходит за границы, сбрасываем на 0
    }
    _currentChannel = channel;
    digitalWrite(_pinCS1, channel & 0x01);
    digitalWrite(_pinCS2, (channel >> 1) & 0x01);
    digitalWrite(_pinCS3, (channel >> 2) & 0x01);
    delay(10);  // Короткая задержка для стабилизации
  }

  uint8_t getChannel() {
    return _currentChannel;
  }

  void nextChannel() {
    _currentChannel = (_currentChannel + 1) % 8;
    setChannel(_currentChannel);
  }

  int getRSSI() {
    return analogRead(_pinRSSI);
  }

  // Фильтрация RSSI (медианный фильтр)
  int getFilteredRSSI() {
    int samples[5];
    for (int i = 0; i < 5; i++) {
      samples[i] = getRSSI();
      delay(5);
    }
    std::sort(samples, samples + 5);
    return samples[2];  // Берем среднее значение из 5 измерений
  }

  // Автоматический поиск канала с максимальным сигналом
  void autoScan() {
    int maxRSSI = 0;
    uint8_t bestChannel = 0;
    for (uint8_t ch = 0; ch < 8; ch++) {
      setChannel(ch);
      delay(50);
      int rssi = getFilteredRSSI();
      if (rssi > maxRSSI) {
        maxRSSI = rssi;
        bestChannel = ch;
      }
    }
    setChannel(bestChannel);
  }

  // Автосканирование с учетом порога (Threshold Scan)
  void autoScanWithThreshold(int threshold) {
    int maxRSSI = threshold; // Минимальный порог
    uint8_t bestChannel = _currentChannel; // Если ничего не найдет, останется на текущем

    for (uint8_t ch = 0; ch < 8; ch++) {
      setChannel(ch);
      delay(50);
      int rssi = getFilteredRSSI();
      if (rssi > maxRSSI) {
        maxRSSI = rssi;
        bestChannel = ch;
      }
    }

    if (maxRSSI > threshold) {
      setChannel(bestChannel);
    }
  }

 private:
  int _pinCS1;
  int _pinCS2;
  int _pinCS3;
  int _pinRSSI;
  uint8_t _currentChannel;
};

#endif