//-----------------------------------------------------------------
//
//  KV Connecter for Keyence PLC series
//
//  Auther: Yusuke Shimizu - Workshpop Asahi
//  https://www.ws-asahi.net
//
//-----------------------------------------------------------------
// Headers
#pragma once
#include "KVConnecter.h"
//#define _KV_CONNECTER_DEBUG_


#ifdef _KV_CONNECTER_DEBUG_
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

//-----------------------------------------------------------------
// Constants
#define KV_COMMAND_CHANGE_MODE "M"
#define KV_COMMAND_CLEAR_ERROR "ER"
#define KV_COMMAND_GET_ERROR "?E"
#define KV_COMMAND_GET_MODEL "?K"
#define KV_COMMAND_GET_RUN_MODE "?M"
#define KV_COMMAND_SET_CURRENDER_TIMER "WRT"
#define KV_COMMAND_FORCE_SET_DEVICE "ST"
#define KV_COMMAND_FORCE_RESET_DEVICE "RS"
#define KV_COMMAND_FORCE_SET_DEVICE_MULTI "STS"
#define KV_COMMAND_FORCE_RESET_DEVICE_MULTI "RSS"
#define KV_COMMAND_READ_DEVICE "RD"
#define KV_COMMAND_READ_DEVICE_MULTI "RDE"
#define KV_COMMAND_WRITE_DEVICE "WR"
#define KV_COMMAND_WRITE_DEVICE_MULTI "WRE"
#define KV_COMMAND_WRITE_SETTING_DEVICE "WS"
#define KV_COMMAND_WRITE_SETTING_DEVICE_MULTI "WSS"
#define KV_COMMAND_SET_BIT_MONITOR "MBS"
#define KV_COMMAND_SET_WORD_MONITOR "MWS"
#define KV_COMMAND_READ_BIT_MONITOR "MBR"
#define KV_COMMAND_READ_WORD_MONITOR "MWR"
#define KV_COMMAND_READ_COMMENT "RDC"
#define KV_COMMAND_CHANGE_BANK "BE"
#define KV_COMMAND_READ_EXUNIT_BUFFER "URD"
#define KV_COMMAND_WRITE_EXUNIT_BUFFER "UWR"

#define KV_SUFFIX_SIGNED_SWORD ".S"
#define KV_SUFFIX_UNSIGNED_SWORD ".U"
#define KV_SUFFIX_SIGNED_DWORD ".L"
#define KV_SUFFIX_UNSIGNED_DWORD ".D"

#define KV_SKIP_FOR_TIMER_AND_COUNTER 3

#define KV_SEND_MAX_LENGTH 256
#define KV_RECIEVE_MAX_LENGTH 256

//-----------------------------------------------------------------
// Constracters
KVConnecter::KVConnecter(uint16_t timeout) {
  _timeout = timeout;
  _ethernetClient.setTimeout(_timeout);
}

//-----------------------------------------------------------------
// Connect PLC device by TCP
bool KVConnecter::begin() {
  debugPrintln(F("KVConnecter - begin()"));
  Serial.println(F("KVConnecter - begin()"));
  _model = KPM_UNKONOWN;
  _alreadyBegin = false;

  KVAnswer answer;
  debugPrintln(F("KVConnecter - Get PLC Model"));
  answer = comGetModel(_model);
  if (answer == KA_SUCCESS) {
    debugPrintln(F("KVConnecter - Success to get model"));
    debugPrint(F("KVConnecter - Model : "));
    debugPrintln(getPLCModelStr());
    _alreadyBegin = true;
  } else if (answer == KA_RESPONSE_ERROR_E1) {
    debugPrintln(F("KVConnecter - Failed to get PLC Model"));
    _model = KPM_UNKONOWN;
    _alreadyBegin = true;
  } else {
    debugPrintln(F("KVConnecter - Failed to connected"));
    _model = KPM_UNKONOWN;
  }

  return _alreadyBegin;
}

bool KVConnecter::begin(const KVParameters parameters) {
  debugPrintln(F("KVConnecter - begin(KVParameters)"));
  setParameters(parameters);

  return begin();
}

bool KVConnecter::begin(const IPAddress plcIP, const uint16_t port = DEFAULT_UPPER_LINK_PORT) {
  debugPrintln(F("KVConnecter - begin(IPAddress, uint16_t)"));

  return begin();
}

void KVConnecter::stop(){
  debugPrintln(F("KVConnecter - stop()"));
  _ethernetClient.stop();
  _alreadyBegin = false;
  return;
}

//-----------------------------------------------------------------
// Getter & Setter
bool KVConnecter::setParameters(const KVParameters parameters) {
  return setParameters(parameters.ipAddress, parameters.upperLinkPort);
}

bool KVConnecter::setParameters(const IPAddress plcIP, const uint16_t port = DEFAULT_UPPER_LINK_PORT) {
  if (!isSafeUpperLinkPort(port)) {
    return false;
  }

  _ipAddress = plcIP;
  _upperLinkPort = port;
}

IPAddress KVConnecter::getPLCIP() const {
  return _ipAddress;
}

uint16_t KVConnecter::getUpperLinkPort() const {
  return _upperLinkPort;
}

KVPLCModel KVConnecter::getPLCModel() const {
  return _model;
}

String KVConnecter::getPLCModelStr() const {
  return getPLCModelStr(_model);
}

String KVConnecter::getPLCModelStr(KVPLCModel model) {
  char modelStr[16];
  getPLCModelCharArray(model, modelStr, 16);
  return String(modelStr);
}

int KVConnecter::getPLCModelCharArray(char* modelArr, const uint16_t maxLen) const {
  return getPLCModelCharArray(_model, modelArr, maxLen);
}

int KVConnecter::getPLCModelCharArray(KVPLCModel model, char* modelArr, const uint16_t maxLen) {
  switch (model) {
    case KPM_KV3000:
      return snprintf(modelArr, maxLen, KV_PLC_KV3000);
    case KPM_KV5000:
      return snprintf(modelArr, maxLen, KV_PLC_KV5000);
    case KPM_KV5500:
      return snprintf(modelArr, maxLen, KV_PLC_KV5500);
    case KPM_KV7300:
      return snprintf(modelArr, maxLen, KV_PLC_KV7300);
    case KPM_KV7500:
      return snprintf(modelArr, maxLen, KV_PLC_KV7500);
    case KPM_KV8000:
      return snprintf(modelArr, maxLen, KV_PLC_KV8000);
    case KPM_KV8000A:
      return snprintf(modelArr, maxLen, KV_PLC_KV8000A);

    case KPM_NC32T:
      return snprintf(modelArr, maxLen, KV_PLC_NC32T);
    case KPM_N60XX:
      return snprintf(modelArr, maxLen, KV_PLC_N60XX);
    case KPM_N40XX:
      return snprintf(modelArr, maxLen, KV_PLC_N40XX);
    case KPM_N24XX:
      return snprintf(modelArr, maxLen, KV_PLC_N24XX);

    default:
      return 0;
  }
}

//-----------------------------------------------------------------
// TCP request
KVAnswer KVConnecter::requestCommand(const char* commnad, char* response, const uint16_t maxLen) {
  debugPrintln(F("KVConnecter - requestCommand(char*, char*, uint16_t)"));

  KVAnswer answer = KA_SUCCESS;
  //request
  if (!_ethernetClient.connect(_ipAddress, _upperLinkPort)) {
    debugPrintln(F("KVConnecter - tcp cannot connected"));
    return KA_TCP_CONNECTION_LOST;
  }
  answer = sendTCP(commnad);
  if (answer != KA_SUCCESS) {
    debugPrintln(F("KVConnecter - tcp request send failed"));
    _ethernetClient.stop();
    return answer;
  }

  answer = receiveTCP(response, maxLen);
  _ethernetClient.stop();
  if (answer != KA_SUCCESS) {
    debugPrintln(F("KVConnecter - tcp request recieve failed"));
    return answer;
  } else {
    debugPrintln(F("KVConnecter - tcp request recieve success"));
    if (!strcmp(response, "E0")) {
      debugPrintln(F("KVConnecter - command error : E0"));
      return KA_RESPONSE_ERROR_E0;
    } else if (!strcmp(response, "E1")) {
      debugPrintln(F("KVConnecter - command error : E1"));
      return KA_RESPONSE_ERROR_E1;
    } else if (!strcmp(response, "E2")) {
      debugPrintln(F("KVConnecter - command error : E2"));
      return KA_RESPONSE_ERROR_E2;
    } else if (!strcmp(response, "E4")) {
      debugPrintln(F("KVConnecter - command error : E4"));
      return KA_RESPONSE_ERROR_E4;
    } else if (!strcmp(response, "E5")) {
      debugPrintln(F("KVConnecter - command error : E5"));
      return KA_RESPONSE_ERROR_E5;
    } else if (!strcmp(response, "E6")) {
      debugPrintln(F("KVConnecter - command error : E6"));
      return KA_RESPONSE_ERROR_E6;
    } else {
      return KA_SUCCESS;
    }
  }
}

KVAnswer KVConnecter::sendTCP(const char* commnad) {
  debugPrintln(F("KVConnecter - sendTCP(char*)"));

  if (strlen(commnad) <= 0) {
    return KA_COMMAND_PARAMETER_ABNORMAL;
  }
  if(!_ethernetClient.connected()){
    debugPrintln(F("KVConnecter - connection lost"));
    stop();
    return KA_TCP_CONNECTION_LOST;
  }

  debugPrintln(F("KVConnecter - Send command"));
  debugPrintln(F("--------------------------"));
  debugPrintln(commnad);
  debugPrintln(F("--------------------------"));
  _ethernetClient.println(commnad);
  _ethernetClient.flush();
  return KA_SUCCESS;
}

KVAnswer KVConnecter::receiveTCP(char* response, const uint16_t maxLen) {
  debugPrintln(F("KVConnecter - receiveTCP(char*, uint16_t)"));

  if (strlen(response) <= 0) {
    return KA_COMMAND_PARAMETER1_ABNORMAL;
  }
  if (maxLen <= 0) {
    return KA_COMMAND_PARAMETER2_ABNORMAL;
  }

  // read response
  int readCnt = 0;
  while (true) {
    if (readCnt >= maxLen) {
      debugPrintln(F("KVConnecter - read buffer overflow"));
      response[readCnt] = '\0';
      return KA_TCP_RESPONSE_BUFFER_OVERFLOW;
    }
    time_t startTime = millis();
    while (!_ethernetClient.available()) {
      if (!_ethernetClient.connected()) {
        debugPrintln(F("KVConnecter - connection lost"));
        stop();
        return KA_TCP_CONNECTION_LOST;
      }
      if (spanMilliSec(startTime) >= _timeout) {
        debugPrintln(F("KVConnecter - connection time out"));
        stop();
        return KA_TCP_TIMEOUT;
      }
      delay(1);
    }
    response[readCnt] = (char)_ethernetClient.read();
    if (readCnt >= 1 && response[readCnt - 1] == '\r' && response[readCnt] == '\n') {
      response[readCnt - 1] = '\0';
      response[readCnt] = '\0';
      break;
    }
    readCnt++;
  }
  _ethernetClient.flush();

  if (strlen(response) <= 0) {
    debugPrintln(F("KVConnecter - response empty"));
    return KA_TCP_RESPONSE_EMPTY;
  }

  debugPrintln(F("KVConnecter - recieved response"));
  debugPrintln(F("----------------------------"));
  debugPrintln(response);
  debugPrintln(F("----------------------------"));

  return KA_SUCCESS;
}

time_t KVConnecter::spanMilliSec(const time_t current, const time_t oldTime) {
  time_t progressTime = 0;
  if (current < oldTime) {
    //overflow
    progressTime = (0xFFFFFFFF - oldTime) + current;
  } else {
    //normal
    progressTime = current - oldTime;
  }

  return progressTime;
}

time_t KVConnecter::spanMilliSec(const time_t oldTime) {
  return spanMilliSec(millis(), oldTime);
}

//-----------------------------------------------------------------
KVAnswer KVConnecter::comChangeMode(const KVPLCState mode) {
  debugPrintln(F("KVConnecter - comChangeMode(KVPLCState)"));

  char command[16];
  switch (mode) {
    case KPS_PROGRAM:
      strcpy(command, KV_COMMAND_CHANGE_MODE);
      strcat(command, "0");
      break;

    case KPS_RUN:
      strcpy(command, KV_COMMAND_CHANGE_MODE);
      strcat(command, "1");
      break;

    default:
      return KA_COMMAND_PARAMETER_ABNORMAL;
  }

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));
      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comClearError() {
  debugPrintln(F("KVConnecter - comClearError()"));

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(KV_COMMAND_CLEAR_ERROR, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));
      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comGetError(KVPLCError& error) {
  debugPrintln(F("KVConnecter - comGetError(KVPLCError&)"));

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(KV_COMMAND_GET_ERROR, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));
      int errVal;
      sscanf(response, "%d", &errVal);
      error = (KVPLCError)errVal;

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comGetModel(KVPLCModel& model) {
  debugPrintln(F("KVConnecter - comGetModel(KVPLCModel&)"));

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(KV_COMMAND_GET_MODEL, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));
      int modelVal;
      sscanf(response, "%d", &modelVal);
      model = (KVPLCModel)modelVal;

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comGetRunMode(KVPLCState& mode) {
  debugPrintln(F("KVConnecter - comGetRunMode(KVPLCState&)"));

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(KV_COMMAND_GET_RUN_MODE, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));
      int cpuModeVal;
      sscanf(response, "%d", &cpuModeVal);
      mode = (KVPLCState)cpuModeVal;

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDevice<KV_BIT>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDevice<KV_BIT>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s", KV_COMMAND_READ_DEVICE, addrStr);

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      int val;

      bool isTorC = isTimerOrCounterDevice(device.getType());
      if(!isTorC){
        sscanf(response, "%d", &val);
      }
      else{
        int tcSet1, tcSet2;
        sscanf(response, "%d %d %d", &val, &tcSet1, &tcSet2);
      }
      if (val == 0) {
        device.setValue(KB_OFF);
      } else {
        device.setValue(KB_ON);
      }

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDevice<KV_SSWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDevice<KV_SSWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s", KV_COMMAND_READ_DEVICE, addrStr, KV_SUFFIX_SIGNED_SWORD);

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      KV_SSWORD val;
      sscanf(response, "%d", &val);
      device.setValue(val);

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDevice<KV_USWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDevice<KV_USWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s", KV_COMMAND_READ_DEVICE, addrStr, KV_SUFFIX_UNSIGNED_SWORD);

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      KV_USWORD val;
      sscanf(response, "%u", &val);
      device.setValue(val);

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDevice<KV_SDWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDevice<KV_SDWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s", KV_COMMAND_READ_DEVICE, addrStr, KV_SUFFIX_SIGNED_DWORD);

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      KV_SDWORD val;
      sscanf(response, "%d", &val);
      device.setValue(val);

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDevice<KV_UDWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDevice<KV_UDWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s", KV_COMMAND_READ_DEVICE, addrStr, KV_SUFFIX_UNSIGNED_DWORD);

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      KV_UDWORD val;
      sscanf(response, "%u", &val);
      device.setValue(val);

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDeviceArray<KV_BIT>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDeviceArray<KV_BIT>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s %d", KV_COMMAND_READ_DEVICE_MULTI, addrStr, device.length());

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      int ignoreSP = 2;
      bool isTorC = isTimerOrCounterDevice(device.getType());

      char valStr[16];
      memset(valStr, 0, sizeof(char) * 16);
      int val;
      strcat(response , " ");
      int respLen = strlen(response);
      
      for(int i = 0, strIdx = 0, arrIdx = 0; i < respLen; i++){
        if(response[i] == ' '){
          if(ignoreSP > 0){
            ignoreSP--;
          } 
          if(!isTorC || ignoreSP <= 0){
            valStr[strIdx] = '\0';
            sscanf(valStr, "%d", &val);
            if (val == 0) {
              device.setValue(arrIdx++, KB_OFF);
            } else {
              device.setValue(arrIdx++, KB_ON);
            }
            if(isTorC){
              ignoreSP = KV_SKIP_FOR_TIMER_AND_COUNTER;
            } 
          }
          memset(valStr, 0, sizeof(char) * 16);
          strIdx = 0;
        } else {
          valStr[strIdx++] = response[i]; 
        }

        if(strIdx >= 16){
          return KA_ANSWER_DATA_ABNORMAL;
        }
      }

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDeviceArray<KV_SSWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDeviceArray<KV_SSWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %d", KV_COMMAND_READ_DEVICE_MULTI, addrStr, KV_SUFFIX_SIGNED_SWORD, device.length());

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      char valStr[16];
      memset(valStr, 0, sizeof(char) * 16);
      KV_SSWORD val;
      strcat(response , " ");
      int respLen = strlen(response);
      for(int i = 0, strIdx = 0, arrIdx = 0; i < respLen; i++){
        if(response[i] == ' '){
          valStr[strIdx] = '\0';
          sscanf(valStr, "%d", &val);
          debugPrint(F("KVConnecter - convert : "));
          debugPrint(valStr);
          debugPrint(F(" -> "));
          debugPrintln(val);
          device.setValue(arrIdx++, val);
          memset(valStr, 0, sizeof(char) * 16);
          strIdx = 0;
        } else {
          valStr[strIdx++] = response[i]; 
        }

        if(strIdx >= 16){
          return KA_ANSWER_DATA_ABNORMAL;
        }
      }
      
      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDeviceArray<KV_USWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDeviceArray<KV_USWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %d", KV_COMMAND_READ_DEVICE_MULTI, addrStr, KV_SUFFIX_UNSIGNED_SWORD, device.length());

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      char valStr[16];
      memset(valStr, 0, sizeof(char) * 16);
      KV_USWORD val;
      strcat(response , " ");
      int respLen = strlen(response);
      for(int i = 0, strIdx = 0, arrIdx = 0; i < respLen; i++){
        if(response[i] == ' '){
          valStr[strIdx] = '\0';
          sscanf(valStr, "%u", &val);
          debugPrint(F("KVConnecter - convert : "));
          debugPrint(valStr);
          debugPrint(F(" -> "));
          debugPrintln(val);
          device.setValue(arrIdx++, val);
          memset(valStr, 0, sizeof(char) * 16);
          strIdx = 0;
        } else {
          valStr[strIdx++] = response[i]; 
        }

        if(strIdx >= 16){
          return KA_ANSWER_DATA_ABNORMAL;
        }
      }

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDeviceArray<KV_SDWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDeviceArray<KV_SDWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %d", KV_COMMAND_READ_DEVICE_MULTI, addrStr, KV_SUFFIX_SIGNED_DWORD, device.length());

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      char valStr[16];
      memset(valStr, 0, sizeof(char) * 16);
      KV_SDWORD val;
      strcat(response , " ");
      int respLen = strlen(response);
      for(int i = 0, strIdx = 0, arrIdx = 0; i < respLen; i++){
        if(response[i] == ' '){
          valStr[strIdx] = '\0';
          sscanf(valStr, "%d", &val);
          device.setValue(arrIdx++, val);
          debugPrint(F("KVConnecter - convert : "));
          debugPrint(valStr);
          debugPrint(F(" -> "));
          debugPrintln(val);
          memset(valStr, 0, sizeof(char) * 16);
          strIdx = 0;
        } else {
          valStr[strIdx++] = response[i]; 
        }

        if(strIdx >= 16){
          return KA_ANSWER_DATA_ABNORMAL;
        }
      }

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comReadDevice(KVDeviceArray<KV_UDWORD>& device) {
  debugPrintln(F("KVConnecter - comReadDevice(KVDeviceArray<KV_UDWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %d", KV_COMMAND_READ_DEVICE_MULTI, addrStr, KV_SUFFIX_UNSIGNED_DWORD, device.length());

  char response[KV_RECIEVE_MAX_LENGTH];
  KVAnswer result = requestCommand(command, response, KV_RECIEVE_MAX_LENGTH);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      char valStr[16];
      memset(valStr, 0, sizeof(char) * 16);
      KV_UDWORD val;
      strcat(response , " ");
      int respLen = strlen(response);
      for(int i = 0, strIdx = 0, arrIdx = 0; i < respLen; i++){
        if(response[i] == ' '){
          valStr[strIdx] = '\0';
          sscanf(valStr, "%u", &val);
          device.setValue(arrIdx++, val);
          debugPrint(F("KVConnecter - convert : "));
          debugPrint(valStr);
          debugPrint(F(" -> "));
          debugPrintln(val);
          memset(valStr, 0, sizeof(char) * 16);
          strIdx = 0;
        } else {
          valStr[strIdx++] = response[i]; 
        }

        if(strIdx >= 16){
          return KA_ANSWER_DATA_ABNORMAL;
        }
      }

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDevice<KV_BIT>& device) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDevice<KV_BIT>&)"));

  if(isTimerOrCounterDevice(device.getType())){
    return KA_COMMAND_UNMATCH_DEVICE;
  }

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s %d", KV_COMMAND_WRITE_DEVICE, addrStr, device.getValue());

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDevice<KV_SSWORD>& device) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDevice<KV_SSWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %d", KV_COMMAND_WRITE_DEVICE, addrStr, KV_SUFFIX_SIGNED_SWORD, device.getValue());

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDevice<KV_USWORD>& device) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDevice<KV_USWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %u", KV_COMMAND_WRITE_DEVICE, addrStr, KV_SUFFIX_UNSIGNED_SWORD, device.getValue());

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDevice<KV_SDWORD>& device) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDevice<KV_SDWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %ld", KV_COMMAND_WRITE_DEVICE, addrStr, KV_SUFFIX_SIGNED_DWORD, device.getValue());

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDevice<KV_UDWORD>& device) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDevice<KV_UDWORD>&)"));

  char command[64];
  char addrStr[32];
  device.getAddress().toCharArray(addrStr, 32);
  snprintf(command, 64, "%s %s%s %lu", KV_COMMAND_WRITE_DEVICE, addrStr, KV_SUFFIX_UNSIGNED_DWORD, device.getValue());

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDeviceArray<KV_BIT>& deviceArr) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDeviceArray<KV_BIT>&)"));

  if(isTimerOrCounterDevice(deviceArr.getType())){
    return KA_COMMAND_UNMATCH_DEVICE;
  }

  char command[KV_SEND_MAX_LENGTH];
  char addrStr[32];
  deviceArr.getAddress().toCharArray(addrStr, 32);
  snprintf(command, KV_SEND_MAX_LENGTH, "%s %s %d", KV_COMMAND_WRITE_DEVICE_MULTI, addrStr, deviceArr.length());
  for(int i = 0; i < deviceArr.length(); i++){
    char valStr[16];
    snprintf(valStr, 16, " %d", deviceArr.getValue(i));
    strcat(command, valStr);
  }

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDeviceArray<KV_SSWORD>& deviceArr) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDeviceArray<KV_SSWORD>&)"));

  char command[KV_SEND_MAX_LENGTH];
  char addrStr[32];
  deviceArr.getAddress().toCharArray(addrStr, 32);
  snprintf(command, KV_SEND_MAX_LENGTH, "%s %s %d", KV_COMMAND_WRITE_DEVICE_MULTI, addrStr, deviceArr.length());
  for(int i = 0; i < deviceArr.length(); i++){
    char valStr[16];
    snprintf(valStr, 16, " %d", deviceArr.getValue(i));
    strcat(command, valStr);
  }

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDeviceArray<KV_USWORD>& deviceArr) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDeviceArray<KV_USWORD>&)"));

  char command[KV_SEND_MAX_LENGTH];
  char addrStr[32];
  deviceArr.getAddress().toCharArray(addrStr, 32);
  snprintf(command, KV_SEND_MAX_LENGTH, "%s %s %d", KV_COMMAND_WRITE_DEVICE_MULTI, addrStr, deviceArr.length());
  for(int i = 0; i < deviceArr.length(); i++){
    char valStr[16];
    snprintf(valStr, 16, " %u", deviceArr.getValue(i));
    strcat(command, valStr);
  }

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDeviceArray<KV_SDWORD>& deviceArr) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDeviceArray<KV_SDWORD>&)"));

  char command[KV_SEND_MAX_LENGTH];
  char addrStr[32];
  deviceArr.getAddress().toCharArray(addrStr, 32);
  snprintf(command, KV_SEND_MAX_LENGTH, "%s %s %d", KV_COMMAND_WRITE_DEVICE_MULTI, addrStr, deviceArr.length());
  for(int i = 0; i < deviceArr.length(); i++){
    char valStr[16];
    snprintf(valStr, 16, " %ld", deviceArr.getValue(i));
    strcat(command, valStr);
  }

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comWriteDevice(const KVDeviceArray<KV_UDWORD>& deviceArr) {
  debugPrintln(F("KVConnecter - comWriteDevice(KVDeviceArray<KV_UDWORD>&)"));

  char command[KV_SEND_MAX_LENGTH];
  char addrStr[32];
  deviceArr.getAddress().toCharArray(addrStr, 32);
  snprintf(command, KV_SEND_MAX_LENGTH, "%s %s %d", KV_COMMAND_WRITE_DEVICE_MULTI, addrStr, deviceArr.length());
  for(int i = 0; i < deviceArr.length(); i++){
    char valStr[16];
    snprintf(valStr, 16, " %lu", deviceArr.getValue(i));
    strcat(command, valStr);
  }

  char response[16];
  KVAnswer result = requestCommand(command, response, 16);

  if (isTCPSuccessAnswer(result)) {
    debugPrintln(F("KVConnecter - tcp success"));

    if (result != KA_SUCCESS) {
      debugPrintln(F("KVConnecter - tcp result error"));
      return result;
    } else {
      debugPrintln(F("KVConnecter - tcp result OK"));

      return result;
    }
  } else {
    debugPrintln(F("KVConnecter - tcp failed"));
    return result;
  }
}

KVAnswer KVConnecter::comSetCurrenderTimer(uint16_t year, uint16_t month, uint16_t day, uint16_t time, uint16_t minute, uint16_t second) {
  debugPrintln(F("KVConnecter - comSetCurrenderTimer(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)"));

  if (!(0 <= year && year <= 99)) {
    return KA_COMMAND_PARAMETER1_ABNORMAL;
  }
  if (!(1 <= month && month <= 12)) {
    return KA_COMMAND_PARAMETER2_ABNORMAL;
  }
  if (!(1 <= day && day <= 30)) {
    return KA_COMMAND_PARAMETER3_ABNORMAL;
  }
  if (!(0 <= time && time <= 11)) {
    return KA_COMMAND_PARAMETER4_ABNORMAL;
  }
  if (!(0 <= minute && minute <= 59)) {
    return KA_COMMAND_PARAMETER5_ABNORMAL;
  }
  if (!(0 <= second && second <= 59)) {
    return KA_COMMAND_PARAMETER6_ABNORMAL;
  }
  uint16_t y = year + 2000;
  uint16_t m = month;

  //generate day of week by Zeller's congruence
  if (m < 3) {
    m += 12;
    y--;
  }
  uint16_t k = y % 100;
  uint16_t j = y / 100;
  uint16_t h = (day + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
  return comSetCurrenderTimer(year, month, day, time, minute, second, (h + 6) % 7);
}

//Not implemented
KVAnswer KVConnecter::comSetCurrenderTimer(uint16_t year, uint16_t month, uint16_t day, uint16_t time, uint16_t minute, uint16_t second, uint8_t dow) {
  debugPrintln(F("KVConnecter - comSetCurrenderTimer(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t)"));

  return KA_COMMAND_NOT_IMOLEMENTED;
}


//-----------------------------------------------------------------
// Verify state
bool KVConnecter::isAlreadyBegin() const {
  return _alreadyBegin;
}

bool KVConnecter::isSafeUpperLinkPort() const {
  return isSafeUpperLinkPort(_upperLinkPort);
}

bool KVConnecter::isSafeUpperLinkPort(const int port) {
  return (port >= PLC_UPPER_LINK_PORT_MIN && port <= PLC_UPPER_LINK_PORT_MAX);
}

bool KVConnecter::isTCPSuccessAnswer(const KVAnswer ans) {
  return (ans == KA_SUCCESS || (KA_RESPONSE_ERROR_E0 <= ans && ans <= KA_RESPONSE_ERROR_E6));
}

bool KVConnecter::isTimerOrCounterDevice(const KVAddressType type){
  return (type == KAT_T || type == KAT_CTH || type == KAT_C || type == KAT_CTC);
}

#undef debugPrint
#undef debugPrintln
#undef _KV_CONNECTER_DEBUG_