//--------------------------------------------------------------
//
//  KV Connecter for Keyence PLC series
//
//  Auther: Yusuke Shimizu - Workshpop Asahi
//  https://www.ws-asahi.net
//
//--------------------------------------------------------------
//Headers
#pragma once
#ifndef KV_CONNECTER_H
#define KV_CONNECTER_H
#include <Arduino.h>
#include <Ethernet.h>
#include "KVDevice.h"

#define KV_PLC_KV3000 "KV-3000"
#define KV_PLC_KV5000 "KV-5000"
#define KV_PLC_KV5500 "KV-5500"
#define KV_PLC_KV7300 "KV-7300"
#define KV_PLC_KV7500 "KV-7500"
#define KV_PLC_KV8000 "KV-8000"
#define KV_PLC_KV8000A "KV-8000A"
#define KV_PLC_NC32T "KV-NC32T"
#define KV_PLC_N60XX "KV-N60 Series"
#define KV_PLC_N40XX "KV-N40 Series"
#define KV_PLC_N24XX "KV-N24 Series"

//-----------------------------------------------------------------
//Enumerator
enum KVAnswer : uint16_t {
  KA_UNKNOWN_FAILED = 0x0000,
  KA_MODEL_FAILED = 0x0001,
  KA_COMMAND_NOT_SUPPORTED = 0x0002,
  KA_COMMAND_NOT_IMOLEMENTED = 0x0003,
  KA_COMMAND_UNMATCH_DEVICE = 0x0004,

  KA_COMMAND_PARAMETER_ABNORMAL = 0x1000,
  KA_COMMAND_PARAMETER1_ABNORMAL = 0x1100,
  KA_COMMAND_PARAMETER2_ABNORMAL = 0x1200,
  KA_COMMAND_PARAMETER3_ABNORMAL = 0x1300,
  KA_COMMAND_PARAMETER4_ABNORMAL = 0x1400,
  KA_COMMAND_PARAMETER5_ABNORMAL = 0x1500,
  KA_COMMAND_PARAMETER6_ABNORMAL = 0x1600,
  KA_COMMAND_PARAMETER7_ABNORMAL = 0x1700,
  KA_COMMAND_PARAMETER8_ABNORMAL = 0x1800,
  KA_COMMAND_PARAMETER9_ABNORMAL = 0x1900,
  KA_COMMAND_PARAMETER10_ABNORMAL = 0x1A00,

  KA_TCP_GENERAL_FAILED = 0x2000,
  KA_TCP_CONNECTION_LOST = 0x2001,
  KA_TCP_TIMEOUT = 0x2002,
  KA_TCP_RESPONSE_ERROR = 0x2003,
  KA_TCP_RESPONSE_BUFFER_OVERFLOW = 0x2004,
  KA_TCP_RESPONSE_EMPTY = 0x2005,

  KA_RESPONSE_ERROR_E0 = 2100,
  KA_RESPONSE_ERROR_E1 = 2101,
  KA_RESPONSE_ERROR_E2 = 2102,
  KA_RESPONSE_ERROR_E4 = 2104,
  KA_RESPONSE_ERROR_E5 = 2105,
  KA_RESPONSE_ERROR_E6 = 2106,

  KA_ANSWER_DATA_ABNORMAL = 0x6000,
  KA_ANSWER_DATA1_ABNORMAL = 0x6100,
  KA_ANSWER_DATA2_ABNORMAL = 0x6200,
  KA_ANSWER_DATA3_ABNORMAL = 0x6300,
  KA_ANSWER_DATA4_ABNORMAL = 0x6400,
  KA_ANSWER_DATA5_ABNORMAL = 0x6500,
  KA_ANSWER_DATA6_ABNORMAL = 0x6600,
  KA_ANSWER_DATA7_ABNORMAL = 0x6700,
  KA_ANSWER_DATA8_ABNORMAL = 0x6800,
  KA_ANSWER_DATA9_ABNORMAL = 0x6900,
  KA_ANSWER_DATA10_ABNORMAL = 0x6A00,

  KA_SUCCESS = 0xFFFF,
};

enum KVPLCState : uint8_t {
  KPS_PROGRAM = 0,
  KPS_RUN = 1,
};

enum KVPLCError : uint8_t {
  KPE_NONE = 0,

  KPE_SUBROUTINE_NEST_LIMIT_OVER = 10,
  KPE_FOR_NEXT_NEST_LIMIT_OVER = 11,
  KPE_INT_NEXT_NEST_LIMIT_OVER = 12,
  KPE_MPS_NEXT_NEST_LIMIT_OVER = 13,
  KPE_BREAK_NEXT_NEST_LIMIT_OVER = 14,
  KPE_COVERT_ERROR = 15,
  KPE_OBJECT_SIZE_ERROR = 19,
  KPE_CODE_SIZE_ERROR = 20,
  KPE_MACRO_NEST_LMIIT_OVER = 22,
  KPE_ABNORMAL_INTERRUPTED = 23,
  KPE_ABNORMAL_MACRO = 24,
  KPE_INDIRECT_REFERENCE_FORMAT_ERROR = 25,
  KPE_ABNORMAL_ADDRESS = 26,
  KPE_ABNORMAL_OBJECT = 27,
  KPE_LADDER_STACK_ERROR = 29,
  KPE_SCAN_TIMER_OVER = 30,
  KPE_LADDER_STACK_OVERFLOW = 31,
  KPE_LADDER_PROGRAM_NOTHING = 40,
  KPE_UNIT_SETTING_CHECKSUM_FAULT = 50,
  KPE_UNIT_SETTING_NOT_RESIGTERED = 51,
  KPE_UNIT_NOT_CONNECTED = 52,
  KPE_EXTEND_BUS_UNIT_TIMEOUT = 53,
  KPE_UNIT_COUNT_UNMATCHED = 54,
  KPE_UNIT_MODEL_UNMATCHED = 55,
  KPE_EXTEND_BUS_COMMUNICATION_ERROR = 56,
  KPE_END_UNIT_NOT_CONNECTED = 57,
  KPE_OUT_OF_EXTEND_BUS_RANGE = 58,
  KPE_UNIT_VERSION_ERROR = 59,
  KPE_OUT_OF_IO_RANGE = 60,
  KPE_OVERLAP_ALLOCATION_DEVICE = 61,
  KPE_MUST_RESET_POWER = 63,
  KPE_FLASH_ROM_FAULT = 80,
  KPE_SRAM_FAULT = 81,
  KPE_AW_DATA_FAULT = 82,
  KPE_BATTERY_VOLTAGE_LOW = 83,
  KPE_MUST_CLEAR_SRAM = 84,
  KPE_LOST_CALLENDER_TIMER = 85,
  KPE_REAL_TIME_CLOCK_FAULT = 86,
  KPE_MEMORY_CARD_FAULT = 87,
  KPE_MEMORY_CARD_EJECTED = 88,
  KPE_POWER_OFF_WHILE_ACCESSING_STORAGE = 89,
  KPE_WAIT_TO_COMPLETE_ACCESSING_STORAGE = 90,
  KPE_ETHERNET_CONTROLER_FAULT = 91,
  KPE_ACCESS_WINDOW_FAULT = 92,
  KPE_FUNCTION_EXTEND_UNIT_FAULT = 93,
  KPE_FUNCTION_EXTEND_UNIT_ID = 94,
  KPE_WHILE_ACCESSING_FILE = 95,
  KPE_EXPANSION_FAULT = 96,
  KPE_EXPANSION_TYPE_FAULT = 97,
  KPE_EXPANSION_NOT_COMPATIBLE = 98,
  KPE_EXPANSION_CHANGE_WHILE_POWER_ON = 99,
  KPE_EXPANSION_VERSION_FAULT = 101,
  KPE_POWER_OFF_ERROR = 102,
  KPE_SHUTDOWN_PROCESS_OVER = 103,
  KPE_CONSUMPTION_CURRENT_LIMIT_OVER = 104,
  KPE_POWER_OFF_HOLD_FORMAT_FAULT = 105,
  KPE_RESET_FAULT = 106,
  KPE_SYSTEM_ERROR1 = 107,
  KPE_CONVERT_WARNING = 108,
  KPE_ABNORMAL_DATA = 118,
  KPE_WATCHDOG_TIMER = 120,
  KPE_STACK_OVERFLOW = 121,
  KPE_SYSTEM_ERROR2 = 122,
  KPE_OUT_OF_DEVICE_RANGE = 123,
  KPE_RESOURCE_FAULT = 124,
  KPE_ERROR_BUFFER_OVERFLOW = 125,
  KPE_ERROR_BUFFER_FAULT = 126,
  KPE_FAILED_TO_AUTO_LOADING = 127,
  KPE_LADDER_CALCULATION_ERROR = 128,
  KPE_UNIT_ERROR = 129,
  KPE_DIRECT_IO_FAULT = 130,
  KPE_FLASH_ROM_FORMAT = 131,
  KPE_LADDER_FILE_ABNORMAL = 132,
  KPE_KV_BT1_FAULT = 133,
  KPE_AUTO_LOAD_DIRECTORY = 134,
  KPE_MEMORY_CARD_COVER_OPENED = 135,
  KPE_MEMORY_CARD_LOCKED = 136,
  KPE_NOT_COMPATIBLE_MEMORY = 137,
  KPE_PLC_MEMORY_WRITE_FREQUENCY_OVER = 141,
  KPE_PLC_MEMORY_TOTAL_WRITE_COUNT_WARNING = 142,
  KPE_PLC_MEMORY_TOTAL_WRITE_COUNT_OVER = 143,
  KPE_STARTUP_ERROR = 250,
  KPE_AW_TIMEOUT_ERROR = 251,
};

enum KVPLCModel : uint8_t {
  KPM_UNKONOWN = 0,

  KPM_KV3000 = 51,
  KPM_KV5000 = 52,
  KPM_KV5500 = 53,
  KPM_KV7300 = 54,
  KPM_KV7500 = 55,
  KPM_KV8000 = 57,
  KPM_KV8000A = 58,

  KPM_NC32T = 128,
  KPM_N60XX = 132,
  KPM_N40XX = 133,
  KPM_N24XX = 134,
};

enum KVSetMode {
  KSM_SET,
  KSM_RESET,
};

enum KVCommand : uint8_t {
  KC_UKNOWN = 0x00,

  KC_CHANGE_MODE,
  KC_CLEAR_ERROR,
  KC_GET_ERROR,
  KC_GET_MODEL,
  KC_GET_RUN_MODE,
  KC_SET_CURRENDER_TIMER,
  KC_FORCE_SET_DEVICE,
  KC_FORCE_SET_DEVICE_MULTI,
  KC_READ_DEVICE,
  KC_READ_DEVICE_MULTI,
  KC_WRITE_DEVICE,
  KC_WRITE_DEVICE_MULTI,
  KC_WRITE_SETTING_DEVICE,
  KC_WRITE_SETTING_DEVICE_MULTI,
  KC_SET_BIT_MONITOR,
  KC_SET_WORD_MONITOR,
  KC_READ_BIT_MONITOR,
  KC_READ_WORD_MONITOR,
  KC_READ_COMMENT,
  KC_CHANGE_BANK,
  KC_READ_EXUNIT_BUFFER,
  KC_WRITE_EXUNIT_BUFFER,
};

//-----------------------------------------------------------------
// Define structs
struct KVParameters {
  bool enable = true;
  IPAddress ipAddress = { 0, 0, 0, 0 };
  uint16_t upperLinkPort = 8501;
};

//-----------------------------------------------------------------
// Define class
class KVConnecter {
  //-----------------------------------------------------------------
public:
  static const uint16_t DEFAULT_UPPER_LINK_PORT = 8501;
  static const uint16_t DEFAULT_TIMEOUT = 500;

private:
  static const uint16_t PLC_UPPER_LINK_PORT_MIN = 1;
  static const uint16_t PLC_UPPER_LINK_PORT_MAX = 65535;


  //-----------------------------------------------------------------
private:
  EthernetClient _ethernetClient;
  IPAddress _ipAddress = { 192, 168, 0, 10 };
  uint16_t _upperLinkPort = DEFAULT_UPPER_LINK_PORT;
  uint16_t _timeout = DEFAULT_TIMEOUT;
  bool _alreadyBegin = false;

  KVPLCModel _model = KPM_UNKONOWN;

  //-----------------------------------------------------------------
  // Constracters
public:
  KVConnecter(const uint16_t timeout = DEFAULT_TIMEOUT);

  //-----------------------------------------------------------------
  // Connect TOA device by TCP
public:
  bool begin();
  bool begin(const KVParameters parameters);
  bool begin(const IPAddress plcIP, const uint16_t port);
  void stop();

  //-----------------------------------------------------------------
  // Getter & Setter
public:
  bool setParameters(const KVParameters parameters);
  bool setParameters(const IPAddress plcIP, const uint16_t port);

  IPAddress getPLCIP() const;
  uint16_t getUpperLinkPort() const;

  KVPLCModel getPLCModel() const;
  String getPLCModelStr() const;
  static String getPLCModelStr(KVPLCModel model);
  int getPLCModelCharArray(char* modelArr, const uint16_t maxLen) const;
  static int getPLCModelCharArray(KVPLCModel model, char* modelArr, const uint16_t maxLen);


  //-----------------------------------------------------------------
  // TCP request
private:
  KVAnswer requestCommand(const char* command, char* response, const uint16_t maxLen);
  KVAnswer sendTCP(const char* command);
  KVAnswer receiveTCP(char* response, const uint16_t maxLen);

  static time_t spanMilliSec(const time_t current, const time_t oldTime);
  static time_t spanMilliSec(const time_t current);

  //static uint16_t splitData(const String& data, String* );

  //-----------------------------------------------------------------
  // Communication command
public:
  KVAnswer comChangeMode(const KVPLCState mode);
  KVAnswer comClearError();
  KVAnswer comGetError(KVPLCError& error);
  KVAnswer comGetModel(KVPLCModel& model);
  KVAnswer comGetRunMode(KVPLCState& mode);
  KVAnswer comSetCurrenderTimer(uint16_t year, uint16_t month, uint16_t day, uint16_t time, uint16_t minute, uint16_t second);
  KVAnswer comSetCurrenderTimer(uint16_t year, uint16_t month, uint16_t day, uint16_t time, uint16_t minute, uint16_t second, uint8_t dow);

  KVAnswer comForceSetDevice(const KVAddress addr, const KVSetMode mode, const uint16_t range = 1);

  KVAnswer comReadDevice(KVDevice<KV_BIT>& device);
  KVAnswer comReadDevice(KVDevice<KV_SSWORD>& device);
  KVAnswer comReadDevice(KVDevice<KV_USWORD>& device);
  KVAnswer comReadDevice(KVDevice<KV_SDWORD>& device);
  KVAnswer comReadDevice(KVDevice<KV_UDWORD>& device);
  KVAnswer comReadDevice(KVDeviceArray<KV_BIT>& deviceArr);
  KVAnswer comReadDevice(KVDeviceArray<KV_SSWORD>& deviceArr);
  KVAnswer comReadDevice(KVDeviceArray<KV_USWORD>& deviceArr);
  KVAnswer comReadDevice(KVDeviceArray<KV_SDWORD>& deviceArr);
  KVAnswer comReadDevice(KVDeviceArray<KV_UDWORD>& deviceArr);

  KVAnswer comWriteDevice(const KVDevice<KV_BIT>& device);
  KVAnswer comWriteDevice(const KVDevice<KV_SSWORD>& device);
  KVAnswer comWriteDevice(const KVDevice<KV_USWORD>& device);
  KVAnswer comWriteDevice(const KVDevice<KV_SDWORD>& device);
  KVAnswer comWriteDevice(const KVDevice<KV_UDWORD>& device);
  KVAnswer comWriteDevice(const KVDeviceArray<KV_BIT>& deviceArr);
  KVAnswer comWriteDevice(const KVDeviceArray<KV_SSWORD>& deviceArr);
  KVAnswer comWriteDevice(const KVDeviceArray<KV_USWORD>& deviceArr);
  KVAnswer comWriteDevice(const KVDeviceArray<KV_SDWORD>& deviceArr);
  KVAnswer comWriteDevice(const KVDeviceArray<KV_UDWORD>& deviceArr);

  KVAnswer comWriteSettingDevice(const KVDevice<KV_BIT>& device);
  KVAnswer comWriteSettingDevice(const KVDevice<KV_SSWORD>& device);
  KVAnswer comWriteSettingDevice(const KVDevice<KV_USWORD>& device);
  KVAnswer comWriteSettingDevice(const KVDevice<KV_SDWORD>& device);
  KVAnswer comWriteSettingDevice(const KVDevice<KV_UDWORD>& device);
  KVAnswer comWriteSettingDevice(const KVDeviceArray<KV_BIT>& deviceArr);
  KVAnswer comWriteSettingDevice(const KVDeviceArray<KV_SSWORD>& deviceArr);
  KVAnswer comWriteSettingDevice(const KVDeviceArray<KV_USWORD>& deviceArr);
  KVAnswer comWriteSettingDevice(const KVDeviceArray<KV_SDWORD>& deviceArr);
  KVAnswer comWriteSettingDevice(const KVDeviceArray<KV_UDWORD>& deviceArr);

  template<typename T, typename... Args>
  KVAnswer comSetMonitor(T first, Args... args);
  template<typename T, typename... Args>
  KVAnswer comReadMonitor(T first, Args... args);

  KVAnswer comReadComment(String& comment, const KVAddress& addr);
  KVAnswer comChangeBank();
  KVAnswer comReadExUnitBuffer();
  KVAnswer comWriteExUnitBuffer();

  //-----------------------------------------------------------------
  // Verify state
public:
  bool isAlreadyBegin() const;
  bool isSafeUpperLinkPort() const;
  static bool isSafeUpperLinkPort(const int port);
  static bool isTCPSuccessAnswer(const KVAnswer ans);
  static bool isTimerOrCounterDevice(const KVAddressType type);

};


#endif