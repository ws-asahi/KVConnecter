//--------------------------------------------------------------
//
//  KV Device data for Keyence PLC series
//
//  Auther: Yusuke Shimizu - Workshpop Asahi
//  https://www.ws-asahi.net
//
//--------------------------------------------------------------
// Headers
#pragma once
#include "KVDevice.h"

//-----------------------------------------------------------------
// Class define (Single Device)
template<typename T>
KVDevice<T>::KVDevice()
  : _address() {
  clear();
}

template<typename T>
KVDevice<T>::KVDevice(KVAddress addr)
  : _address(addr) {
  clear();
}

template<typename T>
KVDevice<T>::KVDevice(KVAddress addr, T value)
  : _address(addr), _value(value) {
}

template<typename T>
KVDevice<T>::KVDevice(const KVAddressType type, const uint32_t addr)
  : _address(type, addr) {
  clear();
}

template<typename T>
KVDevice<T>::KVDevice(const KVAddressType type, const uint32_t addr, T value)
  : _address(type, addr), _value(value) {
}

template<typename T>
KVDevice<T>::KVDevice(const String& addr)
  : _address(addr) {
  clear();
}

template<typename T>
KVDevice<T>::KVDevice(const String& addr, T value)
  : _address(addr), _value(value) {
}

template<typename T>
KVDevice<T>::KVDevice(const char* addr)
  : _address(addr) {
  clear();
}

template<typename T>
KVDevice<T>::KVDevice(const char* addr, T value)
  : _address(addr), _value(value) {
}

template<typename T>
KVAddress KVDevice<T>::getAddress() const {
  return _address;
}

template<typename T>
void KVDevice<T>::setAddress(KVAddress addr) {
  _address = addr;
}

template<typename T>
void KVDevice<T>::setAddress(const uint32_t addr) {
  _address.setAddress(addr);
}

template<typename T>
T KVDevice<T>::getValue() const {
  return _value;
}

template<typename T>
void KVDevice<T>::setValue(T value) {
  _value = value;
}

template<typename T>
KVAddressType KVDevice<T>::getType() const {
  return _address.getType();
}

template<typename T>
bool KVDevice<T>::operator==(const KVDevice<T>& device) const {
  return equals(device);
}

template<typename T>
bool KVDevice<T>::operator==(T value) const {
  return equals(value);
}

template<typename T>
bool KVDevice<T>::operator!=(const KVDevice<T>& device) const {
  return !equals(device);
}

template<typename T>
bool KVDevice<T>::operator!=(T value) const {
  return !equals(value);
}

template<typename T>
KVDevice<T>& KVDevice<T>::operator=(const KVDevice<T>& device){
  _address = device._address;
  _value = device._value;
  return *this;
}

template<typename T>
bool KVDevice<T>::equals(const KVDevice<T>& device) const {
  return _value == device._value;
}

template<typename T>
bool KVDevice<T>::equals(const T value) const {
  return _value == value;
}

template<typename T>
void KVDevice<T>::clear() {
  _value = (T)0;
}


//-----------------------------------------------------------------
// Class define (Multi Device)
template<typename T>
KVDeviceArray<T>::KVDeviceArray(const KVAddressType type, uint32_t startAddr, const int length)
  : _startAddress(type, startAddr), _length(length) {
  _array = new T[length];
}

template<typename T>
KVDeviceArray<T>::KVDeviceArray(const KVDevice<T>& startDevice, const int length)
  : _startAddress(startDevice.getAddress()), _length(length) {
  _array = new T[length];
  _array[0] = startDevice.getValue();
}

template<typename T>
KVDeviceArray<T>::KVDeviceArray(const String& startAddr, const int length)
  : _startAddress(startAddr), _length(length) {
  _array = new T[length];
}

template<typename T>
KVDeviceArray<T>::KVDeviceArray(const char* startAddr, const int length)
  : _startAddress(startAddr), _length(length) {
  _array = new T[length];
}

template<typename T>
KVDeviceArray<T>::~KVDeviceArray() {
  delete[] _array;
}

template<typename T>
KVAddress KVDeviceArray<T>::getAddress() const {
  return _startAddress;
}

template<typename T>
void KVDeviceArray<T>::setAddress(KVAddress addr) {
  _startAddress = addr;
}

template<typename T>
T KVDeviceArray<T>::getValue(int index) const {
  return _array[index];
}

template<typename T>
void KVDeviceArray<T>::setValue(int index, T value) {
  _array[index] = value;
}

template<typename T>
KVAddressType KVDeviceArray<T>::getType() const {
  return _startAddress.getType();
}

template<typename T>
int KVDeviceArray<T>::length() const {
  return _length;
}

template<typename T>
KVDeviceArray<T>::operator T*() const {
  return _array;
}

template<typename T>
KVDevice<T> KVDeviceArray<T>::operator[](int index) const {
  return getDevice(index);
}

template<typename T>
KVDevice<T> KVDeviceArray<T>::getDevice(int index) const {
  KVDevice<T> device;
  int width = (sizeof(T) + 1) / 2;

  device.setAddress(_startAddress.getAddress() + index * width);
  device.setValue(_array[index]);
  return device;
}



template class KVDevice<KV_BIT>;
template class KVDevice<KV_SSWORD>;
template class KVDevice<KV_USWORD>;
template class KVDevice<KV_SDWORD>;
template class KVDevice<KV_UDWORD>;

template class KVDeviceArray<KV_BIT>;
template class KVDeviceArray<KV_SSWORD>;
template class KVDeviceArray<KV_USWORD>;
template class KVDeviceArray<KV_SDWORD>;
template class KVDeviceArray<KV_UDWORD>;
