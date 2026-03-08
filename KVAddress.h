//--------------------------------------------------------------
//
//  KV Device address for Keyence PLC series
//
//  Auther: Yusuke Shimizu - Workshpop Asahi
//  https://www.ws-asahi.net
//
//--------------------------------------------------------------
//Headers
#pragma once
#ifndef KV_ADDRESS_H
#define KV_ADDRESS_H
#include <Arduino.h>
#include <Printable.h>
#include <Regexp.h>

//-----------------------------------------------------------------
// Constants

#define KV_DEVICE_STRING_LENGTH_MIN 2
#define KV_DEVICE_STRING_LENGTH_MAX 32
#define KV_DEVICE_DEVICE_ADDRESS_MIN 0
#define KV_DEVICE_DEVICE_ADDRESS_MAX 99999999

//-----------------------------------------------------------------
//Enumerator
enum KVAddressType : uint8_t {
  KAT_UNKNOWN = 0x00,

  KAT_BIT = 0x20,
  KAT_R = 0x21,
  KAT_B = 0x22,
  KAT_MR = 0x23,
  KAT_LR = 0x24,
  KAT_CR = 0x25,
  KAT_VB = 0x26,
  KAT_T = 0x27,
  KAT_CTH = 0x28,
  KAT_C = 0x29,
  KAT_CTC = 0x2A,
  KAT_BIT_END = 0x3F,

  KAT_WORD = 0x40,
  KAT_DM = 0x41,
  KAT_EM = 0x42,
  KAT_FM = 0x43,
  KAT_ZF = 0x44,
  KAT_W = 0x45,
  KAT_TM = 0x46,
  KAT_Z = 0x47,
  KAT_TC = 0x48,
  KAT_TS = 0x49,
  KAT_CC = 0x4A,
  KAT_CS = 0x4B,
  KAT_AT = 0x4C,
  KAT_CM = 0x4D,
  KAT_VM = 0x4E,
  KAT_WORD_END = 0x7F,
};


//-----------------------------------------------------------------
// Class define
class KVAddress : public Printable {
private:
  KVAddressType _addressType = KAT_UNKNOWN;
  uint32_t _address = 0;

public:
  KVAddress();
  KVAddress(KVAddressType type, uint32_t addr = 0);
  KVAddress(const KVAddress& addr);
  KVAddress(const String& addr);
  KVAddress(char* addr);

  KVAddressType getType() const {
    return _addressType;
  }
  void setType(const KVAddressType type) {
    _addressType = type;
  }

  uint32_t getAddress() const {
    return _address;
  }
  void setAddress(const uint32_t address) {
    _address = address;
  }

  bool operator==(const KVAddress& addr) const;
  bool operator==(char* addr) const;
  bool operator==(const String& addr) const;
  bool operator!=(const KVAddress& addr) const;
  bool operator!=(char* addr) const;
  bool operator!=(const String& addr) const;
  KVAddress& operator=(const KVAddress& addr);
  KVAddress& operator=(char* addr);
  KVAddress& operator=(const String& addr);
  KVAddress& operator+=(const int32_t addr);
  KVAddress& operator-=(const int32_t addr);
  KVAddress& operator+(const int32_t addr);
  KVAddress& operator-(const int32_t addr);
  operator String() const;

  bool equals(const KVAddress& addr) const;
  bool equals(char* addr) const;
  bool equals(const String& addr) const;
  bool fromString(const char* addr);
  bool fromString(const char* addr, const uint16_t len);
  bool fromString(const String& addr);
  int toCharArray(char* arr, const int length) const;
  String toString() const;

  virtual size_t printTo(Print& p) const;


public:
  static bool isSafePLCDevice(const String& device);
  static bool isSafePLCDevice(char* device);

  static bool isSafePLCWordDevice(const String& device);
  static bool isSafePLCWordDeviceDM(const String& device);
  static bool isSafePLCWordDeviceW(const String& device);
  static bool isSafePLCWordDeviceEM(const String& device);
  static bool isSafePLCWordDeviceFM(const String& device);
  static bool isSafePLCWordDeviceZF(const String& device);
  static bool isSafePLCWordDeviceCM(const String& device);
  static bool isSafePLCWordDeviceTM(const String& device);
  static bool isSafePLCWordDeviceZ(const String& device);
  static bool isSafePLCWordDeviceTC(const String& device);
  static bool isSafePLCWordDeviceTS(const String& device);
  static bool isSafePLCWordDeviceCC(const String& device);
  static bool isSafePLCWordDeviceCS(const String& device);
  static bool isSafePLCWordDeviceAT(const String& device);
  static bool isSafePLCWordDeviceVM(const String& device);

  static bool isSafePLCWordDevice(char* device);
  static bool isSafePLCWordDeviceDM(char* device);
  static bool isSafePLCWordDeviceW(char* device);
  static bool isSafePLCWordDeviceEM(char* device);
  static bool isSafePLCWordDeviceFM(char* device);
  static bool isSafePLCWordDeviceZF(char* device);
  static bool isSafePLCWordDeviceCM(char* device);
  static bool isSafePLCWordDeviceTM(char* device);
  static bool isSafePLCWordDeviceZ(char* device);
  static bool isSafePLCWordDeviceTC(char* device);
  static bool isSafePLCWordDeviceTS(char* device);
  static bool isSafePLCWordDeviceCC(char* device);
  static bool isSafePLCWordDeviceCS(char* device);
  static bool isSafePLCWordDeviceAT(char* device);
  static bool isSafePLCWordDeviceVM(char* device);


  static bool isSafePLCBitDevice(const String& device);
  static bool isSafePLCBitDeviceR(const String& device);
  static bool isSafePLCBitDeviceB(const String& device);
  static bool isSafePLCBitDeviceMR(const String& device);
  static bool isSafePLCBitDeviceLR(const String& device);
  static bool isSafePLCBitDeviceCR(const String& device);
  static bool isSafePLCBitDeviceVB(const String& device);
  static bool isSafePLCBitDeviceT(const String& device);
  static bool isSafePLCBitDeviceCTH(const String& device);
  static bool isSafePLCBitDeviceC(const String& device);
  static bool isSafePLCBitDeviceCTC(const String& device);

  static bool isSafePLCBitDevice(char* device);
  static bool isSafePLCBitDeviceR(char* device);
  static bool isSafePLCBitDeviceB(char* device);
  static bool isSafePLCBitDeviceMR(char* device);
  static bool isSafePLCBitDeviceLR(char* device);
  static bool isSafePLCBitDeviceCR(char* device);
  static bool isSafePLCBitDeviceVB(char* device);
  static bool isSafePLCBitDeviceT(char* device);
  static bool isSafePLCBitDeviceCTH(char* device);
  static bool isSafePLCBitDeviceC(char* device);
  static bool isSafePLCBitDeviceCTC(char* device);

private:
  static bool isMatchFormat(char* target, const char* format);
};


#endif