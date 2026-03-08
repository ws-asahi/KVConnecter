# KVConnecter
- Arudino + イーサネットシールドを使用してキーエンス製PLCと上位リンク接続を行うためのライブラリです。
- Keyence PLC upper link library for arduino with ethernet shield.

## サポートしているPLC / Supported PLC
 - KV-3000
 - KV-5000
 - KV-5500
 - KV-7300
 - KV-7500
 - KV-8000
 - KV-8000A

 - KV-NC32T
 - KV-N60xx
 - KV-N40xx
 - KV-N24xx
 
## サポートしている接続方法 / Supported communication type
 - イーサネット接続のみ
 - Ethernet only

## クラス概要 / Classes
### KVConnecter
  - PLCとの接続を管理するためのクラス。
  - Supervision of plc connection.

### KVDevice
  - PLC内の単一デバイス指定とその値を保存するためのクラス。
  - Specify single PLC device and storage value.

### KVDeviceArray
  - PLC内の連続する複数デバイスの先頭指定と幅、各デバイスの値を保存するためのクラス。
  - Specify first device of multiple PLC device, width, and storage each values.

### KVAddress
  - PLC内のデバイスアドレスを管理するクラス。
  - Device address on PLC memory.

## 使用方法 / How to use
### 0. PLC側の設定 / Setup PLC
 - イーサネットポートに任意のIPアドレスと上位リンクのポートを設定してください。
 - Set IPAddress and upper link port on Ethernet port on PLC
### 1. 参照を追加 / add include
 - #include <KVConnecter.h>を追加してください。
 - Add #include <KVConnecter.h>.
### 2. Arduino Ethernetを有効化 / begin Arduino Ethernet
 - Arduinoの標準Ethernetライブラリを使用してEthernet.begin()でイーサネット接続を有効化してください。
 - Execute Ethernet.begin() on arduino with standard ethernet library.
### 3. KVConnecterインスタンスを作成 / Create KVConnecter instance
 - KVConnecterクラスのインスタンスを作成します。オプションでタイムアウトが指定可能です。
 - Create KVConnecter instance. The constracter has timeout option.
### 4. begin()を使用して通信を開始 / Execute begin() for start communication
 - begin実行時には接続先の指定が必要になるので、先にsetParameterで接続先PLCを指定するか、begin実行時に接続先を指定します。
 - You have to specify communication target. Execute setParameter() before begin, or specify target by option of begin().

 - 正常に接続できたらbegin()はPLC型式を取得しtrueを返します。PLC型式はgetPLCModel()で取得可能です。
 - If success to connect, begin () returns true and read PLC model information. You can get model by getPLCModel().
### 5. comから始まる関数で通信します / You can communication by each comXXXXX functions
 - 例えばPLCデバイスを読み取る場合はcomReadDevice()、PLCデバイスに値を書き込む時はcomcomWriteDevice()を使用します。
 - For example, if you want to read device value, execute comReadDevice. And write device value, execute comWriteDevice.

 - デバイスの読み書きの際には対象のデバイスアドレスと種別（ビット、符号付きシングルワードなど）を指定してKVDeviceもしくはKVDeviceArrayのインスタンスを作成し、各通信用関数に引き渡します。
 - For reading or writing PLC device, Create instance KVDevice or KVDeviceArray with first address and type(about bit, signed single word, etc.), and pass over each function. 

 - 各通信は同期処理されるので通信完了かタイムアウトまで待つ必要があります。
 - Each communication is not asyncronous.You must wait communication end or time out.


## 未実装機能 / not implementation
 - comSetCurrenderTimer()
 - comForceSetDevice()
 - comSetMonitor()
 - comReadMonitor()
 - comReadComment()
 - comChangeBank()
 - comReadExUnitBuffer()
 - comWriteExUnitBuffer()
