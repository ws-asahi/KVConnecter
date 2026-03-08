//-----------------------------------------------------------------
//
//  KV Device address for Keyence PLC series
//
//  Auther: Yusuke Shimizu - Workshpop Asahi
//  https://www.ws-asahi.net
//
//-----------------------------------------------------------------
// Headers
#pragma once
#include "KVAddress.h"
//#define _KV_ADDRESS_DEBUG_


#ifdef _KV_ADDRESS_DEBUG_
#define debugPrint(...) \
  { \
    if (Serial) Serial.print(__VA_ARGS__); \
  }
#define debugPrintln(...) \
  { \
    if (Serial) Serial.println(__VA_ARGS__); \
  }
#else
#define debugPrint(...)
#define debugPrintln(...)
#endif

#define KV_ADDRESS_FORMAT_DM "^DM[0-9][0-9]?[0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_W "^W[0-9A-Fa-f][0-9A-Fa-f]?[0-9A-Fa-f]?[0-9A-Fa-f]?$"
#define KV_ADDRESS_FORMAT_EM "^EM[0-9][0-9]?[0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_FM "^FM[0-9][0-9]?[0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_ZF "^ZF[0-9][0-9]?[0-9]?[0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_CM "^CM[0-9][0-9]?[0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_TM "^TM[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_Z "^Z[0-9][0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_T "^T[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_TC "^TC[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_TS "^TS[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_CTH "^CTH[0-9]$"
#define KV_ADDRESS_FORMAT_C "^C[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_CC "^CC[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_CS "^CS[0-9][0-9]?[0-9]?[0-9]?$"
#define KV_ADDRESS_FORMAT_CTS "^CTC[0-9]$"
#define KV_ADDRESS_FORMAT_AT "^AT[0-9]$"
#define KV_ADDRESS_FORMAT_VM "^VM[0-9][0-9]?[0-9]?[0-9]?[0-9]?$"

#define KV_ADDRESS_FORMAT_R1 "^R[0-9][0-9]?[0-9]?[0-9]?0[0-9]$"
#define KV_ADDRESS_FORMAT_R2 "^R[0-9][0-9]?[0-9]?[0-9]?1[0-5]$"
#define KV_ADDRESS_FORMAT_B "^B[0-9A-Fa-f][0-9A-Fa-f]?[0-9A-Fa-f]?[0-9A-Fa-f]?$"
#define KV_ADDRESS_FORMAT_MR1 "^MR[0-9][0-9]?[0-9]?[0-9]?0[0-9]$"
#define KV_ADDRESS_FORMAT_MR2 "^MR[0-9][0-9]?[0-9]?[0-9]?1[0-5]$"
#define KV_ADDRESS_FORMAT_LR1 "^LR[0-9][0-9]?[0-9]?0[0-9]$"
#define KV_ADDRESS_FORMAT_LR2 "^LR[0-9][0-9]?[0-9]?1[0-5]$"
#define KV_ADDRESS_FORMAT_CR1 "^CR[0-9][0-9]?0[0-9]$"
#define KV_ADDRESS_FORMAT_CR2 "^CR[0-9][0-9]?1[0-5]$"
#define KV_ADDRESS_FORMAT_VB "^VB[0-9A-Fa-f][0-9A-Fa-f]?[0-9A-Fa-f]?[0-9A-Fa-f]?$"

//-----------------------------------------------------------------
//Constracter
KVAddress::KVAddress() {
}

KVAddress::KVAddress(KVAddressType type, uint32_t addr) {
  _addressType = type;
  _address = addr;
}

KVAddress::KVAddress(const KVAddress& addr) {
  _addressType = addr._addressType;
  _address = addr._address;
}

KVAddress::KVAddress(const String& addr) {
  fromString(addr);
}

KVAddress::KVAddress(char* addr) {
  fromString(addr);
}


//-----------------------------------------------------------------
// Operater
bool KVAddress::operator==(const KVAddress& addr) const {
  return equals(addr);
}
bool KVAddress::operator==(char* addr) const {
  return equals(addr);
}
bool KVAddress::operator==(const String& addr) const {
  return equals(addr);
}
bool KVAddress::operator!=(const KVAddress& addr) const {
  return !equals(addr);
}
bool KVAddress::operator!=(char* addr) const {
  return !equals(addr);
}
bool KVAddress::operator!=(const String& addr) const {
  return !equals(addr);
}

KVAddress& KVAddress::operator=(const KVAddress& addr) {
  _address = addr._address;
  _addressType = addr._addressType;
  return *this;
}
KVAddress& KVAddress::operator=(char* addr) {
  fromString(addr);
  return *this;
}
KVAddress& KVAddress::operator=(const String& addr) {
  fromString(addr);
  return *this;
}

KVAddress& KVAddress::operator+=(const int32_t addr) {
  if ((_address + addr) < KV_DEVICE_DEVICE_ADDRESS_MIN) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MIN;
  } else if ((_address + addr) > KV_DEVICE_DEVICE_ADDRESS_MAX) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MAX;
  } else {
    _address = _address + addr;
  }
  return *this;
}
KVAddress& KVAddress::operator-=(const int32_t addr) {
  if ((_address - addr) < KV_DEVICE_DEVICE_ADDRESS_MIN) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MIN;
  } else if ((_address - addr) > KV_DEVICE_DEVICE_ADDRESS_MAX) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MAX;
  } else {
    _address = _address - addr;
  }
  return *this;
}
KVAddress& KVAddress::operator+(const int32_t addr) {
  if ((_address - addr) < KV_DEVICE_DEVICE_ADDRESS_MIN) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MIN;
  } else if ((_address - addr) > KV_DEVICE_DEVICE_ADDRESS_MAX) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MAX;
  } else {
    _address = _address - addr;
  }
  return *this;
}
KVAddress& KVAddress::operator-(const int32_t addr) {
  if ((_address - addr) < KV_DEVICE_DEVICE_ADDRESS_MIN) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MIN;
  } else if ((_address - addr) > KV_DEVICE_DEVICE_ADDRESS_MAX) {
    _address = KV_DEVICE_DEVICE_ADDRESS_MAX;
  } else {
    _address = _address - addr;
  }
  return *this;
}

KVAddress::operator String() const {
  return toString();
}

size_t KVAddress::printTo(Print& p) const {
  debugPrintln(F("KVAddress - printTo(Print&)"));
  String str = toString();
  debugPrint(F("KVAddress - printStr : "));
  debugPrintln(str);
  p.print(str);

  return str.length();
}


//-----------------------------------------------------------------
// Common operation
bool KVAddress::equals(const KVAddress& addr) const {
  debugPrintln(F("KVAddress - equals(KVAddress&)"));
  return (_addressType == addr._addressType) && (_address == addr._address);
}

bool KVAddress::equals(char* addr) const {
  debugPrintln(F("KVAddress - equals(char*)"));
  KVAddress matchAddress;
  if (!matchAddress.fromString(addr)) return false;
  return equals(matchAddress);
}

bool KVAddress::equals(const String& addr) const {
  debugPrintln(F("KVAddress - equals(String&)"));
  KVAddress matchAddress;
  if (!matchAddress.fromString(addr)) return false;
  return equals(matchAddress);
}

bool KVAddress::fromString(const String& addr) {
  debugPrintln(F("KVAddress - fromString(String&)"));
  return fromString(addr.c_str());
}

bool KVAddress::fromString(const char* addr) {
  return fromString(addr, strlen(addr));
}

bool KVAddress::fromString(const char* addr, const uint16_t len) {
  debugPrintln(F("KVAddress - fromString(char*)"));

  if (len <= 1 || len > KV_DEVICE_STRING_LENGTH_MAX) {
    return false;
  }

  char addrStr[KV_DEVICE_STRING_LENGTH_MAX];
  memset(addrStr, 0, sizeof(char) * KV_DEVICE_STRING_LENGTH_MAX);

  for (int i = 0; i < len && addr[i] != '\0'; i++) {
    addrStr[i] = toupper(addr[i]);
  }

  uint32_t addrVal = 0;
  uint32_t addrSub = 0;

  KVAddressType type = KAT_UNKNOWN;
  uint32_t newAddr = 0;

  //find debice type
  if (isSafePLCBitDeviceR(addrStr)) {
    type = KAT_R;
    sscanf(addrStr, "R%u%2u", &addrVal, &addrSub);
    newAddr = addrVal * 16 + addrSub;
  } else if (isSafePLCBitDeviceB(addrStr)) {
    type = KAT_B;
    sscanf(addrStr, "B%x", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCBitDeviceMR(addrStr)) {
    type = KAT_MR;
    sscanf(addrStr, "MR%u%2u", &addrVal, &addrSub);
    newAddr = addrVal * 16 + addrSub;
  } else if (isSafePLCBitDeviceLR(addrStr)) {
    type = KAT_LR;
    sscanf(addrStr, "LR%u%2u", &addrVal, &addrSub);
    newAddr = addrVal * 16 + addrSub;
  } else if (isSafePLCBitDeviceCR(addrStr)) {
    type = KAT_CR;
    sscanf(addrStr, "CR%u%2u", &addrVal, &addrSub);
    newAddr = addrVal * 16 + addrSub;
  } else if (isSafePLCBitDeviceVB(addrStr)) {
    type = KAT_VB;
    sscanf(addrStr, "VB%x", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCBitDeviceT(addrStr)) {
    type = KAT_T;
    sscanf(addrStr, "T%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCBitDeviceCTH(addrStr)) {
    type = KAT_CTH;
    sscanf(addrStr, "CTH%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCBitDeviceC(addrStr)) {
    type = KAT_C;
    sscanf(addrStr, "C%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCBitDeviceCTC(addrStr)) {
    type = KAT_CTC;
    sscanf(addrStr, "CTC%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceDM(addrStr)) {
    type = KAT_DM;
    sscanf(addrStr, "DM%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceEM(addrStr)) {
    type = KAT_EM;
    sscanf(addrStr, "EM%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceFM(addrStr)) {
    type = KAT_FM;
    sscanf(addrStr, "FM%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceZF(addrStr)) {
    type = KAT_ZF;
    sscanf(addrStr, "ZF%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceW(addrStr)) {
    type = KAT_W;
    sscanf(addrStr, "W%x", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceTM(addrStr)) {
    type = KAT_TM;
    sscanf(addrStr, "TM%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceZ(addrStr)) {
    type = KAT_Z;
    sscanf(addrStr, "Z%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceTC(addrStr)) {
    type = KAT_TC;
    sscanf(addrStr, "TC%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceTS(addrStr)) {
    type = KAT_TS;
    sscanf(addrStr, "TS%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceCC(addrStr)) {
    type = KAT_CC;
    sscanf(addrStr, "CC%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceCS(addrStr)) {
    type = KAT_CS;
    sscanf(addrStr, "CS%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceAT(addrStr)) {
    type = KAT_AT;
    sscanf(addrStr, "AT%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceCM(addrStr)) {
    type = KAT_CM;
    sscanf(addrStr, "CM%u", &addrVal);
    newAddr = addrVal;
  } else if (isSafePLCWordDeviceVM(addrStr)) {
    type = KAT_VM;
    sscanf(addrStr, "VM%u", &addrVal);
    newAddr = addrVal;
  } else {
    return false;
  }

  _address = newAddr;
  _addressType = type;

  return true;
}

int KVAddress::toCharArray(char* addr, const int maxLen) const {
  debugPrintln(F("KVAddress - toCharArray(char*, int)"));
  switch (_addressType) {
    case KAT_R:
      return snprintf(addr, maxLen, "R%d%2d", _address / 16, _address % 16);
    case KAT_B:
      return snprintf(addr, maxLen, "B%X", _address);
    case KAT_MR:
      return snprintf(addr, maxLen, "MR%d%2d", _address / 16, _address % 16);
    case KAT_LR:
      return snprintf(addr, maxLen, "LR%d%2d", _address / 16, _address % 16);
    case KAT_CR:
      return snprintf(addr, maxLen, "CR%d%2d", _address / 16, _address % 16);
    case KAT_VB:
      return snprintf(addr, maxLen, "VB%X", _address);

    case KAT_DM:
      return snprintf(addr, maxLen, "DM%d", _address);
    case KAT_EM:
      return snprintf(addr, maxLen, "EM%d", _address);
    case KAT_FM:
      return snprintf(addr, maxLen, "FM%d", _address);
    case KAT_ZF:
      return snprintf(addr, maxLen, "ZF%d", _address);
    case KAT_W:
      return snprintf(addr, maxLen, "W%X", _address);
    case KAT_TM:
      return snprintf(addr, maxLen, "TM%d", _address);
    case KAT_Z:
      return snprintf(addr, maxLen, "Z%d", _address);
    case KAT_T:
      return snprintf(addr, maxLen, "T%d", _address);
    case KAT_TC:
      return snprintf(addr, maxLen, "TC%d", _address);
    case KAT_TS:
      return snprintf(addr, maxLen, "TS%d", _address);
    case KAT_CTH:
      return snprintf(addr, maxLen, "CTH%d", _address);
    case KAT_C:
      return snprintf(addr, maxLen, "C%d", _address);
    case KAT_CC:
      return snprintf(addr, maxLen, "CC%d", _address);
    case KAT_CS:
      return snprintf(addr, maxLen, "CS%d", _address);
    case KAT_CTC:
      return snprintf(addr, maxLen, "CTC%d", _address);
    case KAT_AT:
      return snprintf(addr, maxLen, "AT%d", _address);
    case KAT_CM:
      return snprintf(addr, maxLen, "CM%d", _address);
    case KAT_VM:
      return snprintf(addr, maxLen, "VM%d", _address);

    default:
      return snprintf(addr, maxLen, "");
  }
}

String KVAddress::toString() const {
  debugPrintln(F("KVAddress - toString()"));
  char buf[KV_DEVICE_STRING_LENGTH_MAX];
  toCharArray(buf, KV_DEVICE_STRING_LENGTH_MAX);
  return String(buf);
}


//-----------------------------------------------------------------
//Word / bit device string safety
bool KVAddress::isSafePLCDevice(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCDevice(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCDevice(buffer);
}

bool KVAddress::isSafePLCDevice(char* device) {
  debugPrintln(F("KVAddress - isSafePLCDevice(char*)"));
  return isSafePLCWordDevice(device) || isSafePLCBitDevice(device);
}

//-----------------------------------------------------------------
//Word device string safety
bool KVAddress::isSafePLCWordDevice(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDevice(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDevice(buffer);
}
bool KVAddress::isSafePLCWordDeviceDM(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceDM(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceDM(buffer);
}
bool KVAddress::isSafePLCWordDeviceW(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceW(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceW(buffer);
}
bool KVAddress::isSafePLCWordDeviceEM(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceEM(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceEM(buffer);
}
bool KVAddress::isSafePLCWordDeviceFM(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceFM(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceFM(buffer);
}
bool KVAddress::isSafePLCWordDeviceZF(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceZF(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceZF(buffer);
}
bool KVAddress::isSafePLCWordDeviceCM(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceCM(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceCM(buffer);
}
bool KVAddress::isSafePLCWordDeviceTM(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceTM(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceTM(buffer);
}
bool KVAddress::isSafePLCWordDeviceZ(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceZ(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceZ(buffer);
}
bool KVAddress::isSafePLCWordDeviceTC(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceTC(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceTC(buffer);
}
bool KVAddress::isSafePLCWordDeviceTS(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceTS(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceTS(buffer);
}
bool KVAddress::isSafePLCWordDeviceCC(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceCC(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceCC(buffer);
}
bool KVAddress::isSafePLCWordDeviceCS(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceCS(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceCS(buffer);
}
bool KVAddress::isSafePLCWordDeviceAT(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceAT(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceAT(buffer);
}
bool KVAddress::isSafePLCWordDeviceVM(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceVM(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDeviceVM(buffer);
}

bool KVAddress::isSafePLCWordDevice(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDevice(char*)"));
  return isSafePLCWordDeviceDM(device) || isSafePLCWordDeviceW(device) || isSafePLCWordDeviceEM(device) || isSafePLCWordDeviceFM(device) || isSafePLCWordDeviceZF(device) || isSafePLCWordDeviceCM(device) || isSafePLCWordDeviceTM(device) || isSafePLCWordDeviceZ(device) || isSafePLCWordDeviceTC(device) || isSafePLCWordDeviceTS(device) || isSafePLCWordDeviceCC(device) || isSafePLCWordDeviceCS(device) || isSafePLCWordDeviceAT(device) || isSafePLCWordDeviceVM(device);
}
bool KVAddress::isSafePLCWordDeviceDM(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceDM(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_DM);
}
bool KVAddress::isSafePLCWordDeviceW(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceW(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_W);
}
bool KVAddress::isSafePLCWordDeviceEM(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceEM(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_EM);
}
bool KVAddress::isSafePLCWordDeviceFM(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceFM(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_FM);
}
bool KVAddress::isSafePLCWordDeviceZF(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceZF(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_ZF);
}
bool KVAddress::isSafePLCWordDeviceCM(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceCM(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_CM);
}
bool KVAddress::isSafePLCWordDeviceTM(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceTM(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_TM);
}
bool KVAddress::isSafePLCWordDeviceZ(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceZ(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_Z);
}
bool KVAddress::isSafePLCWordDeviceTC(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceTC(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_TC);
}
bool KVAddress::isSafePLCWordDeviceTS(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceTS(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_TS);
}
bool KVAddress::isSafePLCWordDeviceCC(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceCC(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_CC);
}
bool KVAddress::isSafePLCWordDeviceCS(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceCS(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_CS);
}
bool KVAddress::isSafePLCWordDeviceAT(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceAT(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_AT);
}
bool KVAddress::isSafePLCWordDeviceVM(char* device) {
  debugPrintln(F("KVAddress - isSafePLCWordDeviceVM(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_VM);
}


//-----------------------------------------------------------------
//Bit device string safety
bool KVAddress::isSafePLCBitDevice(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDevice(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCWordDevice(buffer);
}
bool KVAddress::isSafePLCBitDeviceR(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceR(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceR(buffer);
}
bool KVAddress::isSafePLCBitDeviceB(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceB(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceB(buffer);
}
bool KVAddress::isSafePLCBitDeviceMR(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceMR(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceMR(buffer);
}
bool KVAddress::isSafePLCBitDeviceLR(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceLR(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceLR(buffer);
}
bool KVAddress::isSafePLCBitDeviceCR(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceCR(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceCR(buffer);
}
bool KVAddress::isSafePLCBitDeviceVB(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceVB(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceVB(buffer);
}
bool KVAddress::isSafePLCBitDeviceT(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceT(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceT(buffer);
}
bool KVAddress::isSafePLCBitDeviceCTH(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceCTH(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceCTH(buffer);
}
bool KVAddress::isSafePLCBitDeviceC(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceC(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceC(buffer);
}
bool KVAddress::isSafePLCBitDeviceCTC(const String& device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceCTC(String&)"));
  if (device.length() < KV_DEVICE_STRING_LENGTH_MIN || device.length() > KV_DEVICE_STRING_LENGTH_MAX) return false;
  char buffer[KV_DEVICE_STRING_LENGTH_MAX];
  device.toCharArray(buffer, KV_DEVICE_STRING_LENGTH_MAX);
  return isSafePLCBitDeviceCTC(buffer);
}

bool KVAddress::isSafePLCBitDevice(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDevice(char*)"));
  return isSafePLCBitDeviceR(device) || isSafePLCBitDeviceB(device) || isSafePLCBitDeviceMR(device) || isSafePLCBitDeviceLR(device) || isSafePLCBitDeviceCR(device) || isSafePLCBitDeviceVB(device) || isSafePLCBitDeviceT(device) || isSafePLCBitDeviceCTH(device) || isSafePLCBitDeviceC(device) || isSafePLCBitDeviceCTC(device);
}
bool KVAddress::isSafePLCBitDeviceR(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceR(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_R1) || isMatchFormat(device, KV_ADDRESS_FORMAT_R2);
}
bool KVAddress::isSafePLCBitDeviceB(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceB(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_B);
}
bool KVAddress::isSafePLCBitDeviceMR(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceMR(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_MR1) || isMatchFormat(device, KV_ADDRESS_FORMAT_MR2);
}
bool KVAddress::isSafePLCBitDeviceLR(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceLR(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_LR1) || isMatchFormat(device, KV_ADDRESS_FORMAT_LR2);
}
bool KVAddress::isSafePLCBitDeviceCR(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceCR(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_CR1) || isMatchFormat(device, KV_ADDRESS_FORMAT_CR2);
}
bool KVAddress::isSafePLCBitDeviceVB(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceVB(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_VB);
}
bool KVAddress::isSafePLCBitDeviceT(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceT(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_T);
}
bool KVAddress::isSafePLCBitDeviceCTH(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceCTH(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_CTH);
}
bool KVAddress::isSafePLCBitDeviceC(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceC(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_C);
}
bool KVAddress::isSafePLCBitDeviceCTC(char* device) {
  debugPrintln(F("KVAddress - isSafePLCBitDeviceCTC(char*)"));
  return isMatchFormat(device, KV_ADDRESS_FORMAT_CTS);
}

//-----------------------------------------------------------------
//Regular matching
bool KVAddress::isMatchFormat(char* target, const char* format) {
  debugPrintln(F("KVAddress - isMatchFormat(char*, char*)"));
  debugPrint(F("Target : "));
  debugPrintln(target);
  debugPrint(F("Format : "));
  debugPrintln(format);
  MatchState ms;
  ms.Target(target);
  bool result = (ms.Match(format) == REGEXP_MATCHED);
  debugPrint(F("Is match : "));
  debugPrintln(result ? "true" : "false");
  return result;
}


#undef debugPrint
#undef debugPrintln
#undef _KV_ADDRESS_DEBUG_
