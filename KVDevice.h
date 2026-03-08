//--------------------------------------------------------------
//
//  KV Device data for Keyence PLC series
//
//  Auther: Yusuke Shimizu - Workshpop Asahi
//  https://www.ws-asahi.net
//
//--------------------------------------------------------------
//Headers
#pragma once
#ifndef KV_DEVICE_H
#define KV_DEVICE_H
#include <Arduino.h>
#include "KVAddress.h"

enum KV_BIT : uint8_t {
  KB_OFF = 0,
  KB_ON = 1,
};

typedef int16_t KV_SSWORD;
typedef uint16_t KV_USWORD;
typedef int32_t KV_SDWORD;
typedef uint32_t KV_UDWORD;

//-----------------------------------------------------------------
// Class define (Single Device)
template<typename T>
class KVDevice {
private:
  KVAddress _address;
  T _value;

public:
  KVDevice();

  KVDevice(KVAddress addr);
  KVDevice(KVAddress addr, T value);
  KVDevice(const KVAddressType type, const uint32_t addr);
  KVDevice(const KVAddressType type, const uint32_t addr, T value);
  KVDevice(const String& addr);
  KVDevice(const String& addr, T value);
  KVDevice(const char* addr);
  KVDevice(const char* addr, T value);

  KVAddress getAddress() const;
  void setAddress(KVAddress addr);
  void setAddress(const uint32_t addr);
  T getValue() const;
  void setValue(T value);

  KVAddressType getType() const;

  bool operator==(const KVDevice<T>& device) const;
  bool operator==(T value) const;
  bool operator!=(const KVDevice<T>& device) const;
  bool operator!=(T value) const;
  KVDevice<T>& operator=(const KVDevice<T>& device);

  bool equals(const KVDevice<T>& device) const;
  bool equals(const T value) const;

private:
  void clear();
};


//-----------------------------------------------------------------
// Class define (Multi Device)
template<typename T>
class KVDeviceArray {

private:
  KVAddress _startAddress;
  T* _array;
  int _length;

public:
  KVDeviceArray(const KVAddressType type, uint32_t startAddr, const int length);
  KVDeviceArray(const KVDevice<T>& startDevice, const int length);
  KVDeviceArray(const String& startAddr, const int length);
  KVDeviceArray(const char* startAddr, const int length);

  ~KVDeviceArray();

  KVAddress getAddress() const;
  void setAddress(KVAddress addr);

  T getValue(int index) const;
  void setValue(int index, T value);

  KVAddressType getType() const;

  int length() const;

  operator T*() const;
  KVDevice<T> operator[](int index) const;

  int toCharArray(char* arr, const uint16_t maxLen) const;
  KVDevice<T> getDevice(int index) const;
};


template<>
inline int KVDeviceArray<KV_BIT>::toCharArray(char* arr, const uint16_t maxLen) const {
  return -1;
}

template<typename T>
inline int KVDeviceArray<T>::toCharArray(char* arr, const uint16_t maxLen) const {
  int len = 0;
  for (int i = 0; i < _length && len < maxLen - 1; i++) {
    if (len < maxLen) {
      arr[len++] = (char)(_array[i] >> 8);
    }
    if (len < maxLen) {
      arr[len++] = (char)(_array[i] & 0xFF);
    }
  }
  if (len < maxLen - 1) {
    arr[len] = '\0';
    return len;
  } else {
    arr[maxLen - 1] = '\0';
    return maxLen;
  }
}


#endif