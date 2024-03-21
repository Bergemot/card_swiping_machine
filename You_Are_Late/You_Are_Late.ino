#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <RTClib.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <ArduinoJson.h> 

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define SS_PIN 16
#define RST_PIN 5
#define BEEP_PIN 15

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

RTC_DS3231 rtc;

MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,D3,D2,U8X8_PIN_NONE);

byte nuidPICC[4]; 

const char* ssid = "Cu的iPhone 12";
const char* password = "87654321";
const char* serverAddress = "10.1.186.72";
const int serverPort = 3000;

const char* get_time_path = "/api/current-time"; // 服务器获取时间的路径
const char* get_room_path = "/api/get_roomnum"; // 服务器获取房间的路径
const char* check_in_path = "/api"; // 服务器签到的路径
const char* duplicate_check_path = "/api/duplicate_check"; // 服务器检查是否重复打卡路径
const char* stuid_check_path = "/api/stuid_check"; // 服务器获取学生id的路径

String macaddress;
String roomnum;

//———————————————————————————————————————setup—————————————————————————————————————————————————————
void setup() {
  init_all();
  roomnum = getRoom(macaddress);
  u8g2.clearBuffer(); // 清空内部缓存
}
//—————————————————————————————————————自定义函数———————————————————————————————————————————————————
// 初始化所有
void init_all(){

  // OLED显示器初始化
  u8g2.begin(); 
  u8g2.enableUTF8Print(); 
  displayStringOnOLED_cn("开机中",3,40);
  delay(2000);
  u8g2.clearBuffer();
  // 语音模块初始化
  Serial1.begin(9600); // 设置串口波特率为 9600
  Serial1.println("AF:20");
  delay(150);
  // RFID模块初始化
  Serial.begin(9600);
  // 语音
  Serial1.println("A7:00011");
  delay (5000);
  // 刷卡相关初始化
  SPI.begin(); 
  rfid.PCD_Init(); 
  // 设置默认密钥（MIFARE Classic卡片的密钥）
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; 
  }
  // 输出一些信息到串口（Serial Monitor）
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE); 

  //连接WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    displayStringOnOLED("Connecting to WiFi...",3);
  }
  Serial.println("Connected to WiFi");
  u8g2.clearBuffer();
  displayStringOnOLED_cn("已开机",3,40);
  Serial1.println("A7:00021");
  delay (5000);
  Serial.print("IP address:    ");           
  Serial.println(WiFi.localIP());             
  Serial.print("MAC address:   ");
  macaddress = WiFi.macAddress();
  Serial.println(WiFi.macAddress());
  u8g2.clearBuffer();
} 

// OLED显示（支持一句话和对应的行数）
void displayStringOnOLED(String text, int lineNumber) {
  //u8g2.clearBuffer(); // 清空内部缓存
  u8g2.setFont(u8g2_font_ncenB08_tr);
  int textWidth = u8g2.getStrWidth(text.c_str()); // 获取字符串宽度

  // 计算字符串在屏幕上居中时的起始位置
  int startX = (u8g2.getWidth() - textWidth) / 2;

  // 计算字符串在指定行数上的纵坐标位置
  int lineHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
  int startY = lineHeight * lineNumber;

  // 在计算好的位置上绘制文本
  u8g2.setCursor(startX, startY);
  u8g2.print(text);

  u8g2.sendBuffer(); // 将缓存中的内容发送到显示器
}
// OLED显示_中文版（支持一句话和对应的行数列数）
void displayStringOnOLED_cn(String text, int startLine, int startColumn) {
  //u8g2.clearBuffer(); // 清空内部缓存
  u8g2.setFont(u8g2_font_unifont_t_chinese2); // 使用中文Unicode字体

  // 计算文本在指定行数上的纵坐标位置
  int lineHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
  int startY = lineHeight * startLine;

  // 在计算好的位置上绘制文本
  u8g2.setCursor(startColumn, startY);
  u8g2.print(text);

  u8g2.sendBuffer(); // 将缓存中的内容发送到显示器
}
// 获取当前时间 
String getTime() {
  /*
    连接到服务器
    发送gettime请求
    返回当前时间的字符串
  */
  String currentTime;

  WiFiClient client;

  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    return currentTime;
  }

  client.print("GET ");
  client.print(get_time_path);
  client.print(" HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  // 读取和解析JSON数据
  DynamicJsonDocument jsonDoc(512); // 可根据JSON数据大小调整缓冲区大小
  DeserializationError jsonError = deserializeJson(jsonDoc, client);

  if (jsonError) {
    Serial.print("JSON parsing failed: ");
    Serial.println(jsonError.c_str());
    return currentTime;
  }
  
  // 从JSON数据中提取"currentTime"字段的值
  currentTime = jsonDoc["currentTime"].as<String>();
  client.stop();

  return currentTime;
}
// 获取当前mac对应的房间号
String getRoom(String mac) {
  String roomNum;

  WiFiClient client;

  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    Serial1.println("A7:00018");
    delay(5000);
    return roomNum;
  }

  String postData = "mac=" + mac;
  Serial.print("Sending POST request to server: ");
  Serial.println(postData);
  // 发送请求
  client.print("POST ");
  client.print(get_room_path);
  client.print(" HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(postData.length());
  client.print("\r\n\r\n");
  client.print(postData);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  // 读取和解析JSON数据
  DynamicJsonDocument jsonDoc(512); // 可根据JSON数据大小调整缓冲区大小
  DeserializationError jsonError = deserializeJson(jsonDoc, client);
  Serial.print("Received JSON response: ");
  Serial.println(jsonDoc.as<String>());


  if (jsonError) {
    Serial.print("JSON parsing failed: ");
    Serial.println(jsonError.c_str());
    Serial1.println("A7:00017");
    delay(5000);
    displayStringOnOLED_cn("未找到教室",3,24);
    return roomNum;
  }
  
  // 从JSON数据中提取"num"字段的值
  roomNum = jsonDoc["num"].as<String>();
  client.stop();

  // 显示
  u8g2.clearBuffer();
  displayStringOnOLED_cn("当前地址",3,32);
  displayStringOnOLED(macaddress,5);
  delay(3000);
  u8g2.clearBuffer();
  displayStringOnOLED_cn("当前教室",3,32);
  displayStringOnOLED(roomNum,5);
  if(roomNum == "330"){
    Serial1.println("A7:00013");
  }else if(roomNum == "313"){
    Serial1.println("A7:00012");
  }else{
    Serial1.println("A7:00017");
  }
  delay(5000);
  u8g2.clearBuffer();

  return roomNum;
}
// 读卡 返回NUID
String readNUID(){
  // 检测重复刷卡
  if ( ! rfid.PICC_IsNewCardPresent())
    return "please swipe card";

  if ( ! rfid.PICC_ReadCardSerial())
    return "please swipe card";
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return "Your tag is not of type MIFARE Classic.";
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // 存储NUID到数组中
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else {
    Serial.println(F("Card read previously."));
    return "Card read previously.";
    }
    // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  char decString[4 * rfid.uid.size + 1];
  byteToDecString(rfid.uid.uidByte, rfid.uid.size, decString);
  return decString;
}
// 以十六进制格式打印字节数组的内容
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
// 以十进制格式打印字节数组的内容
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);

  }
}
// 数组转字符串
void byteToDecString(byte* byteArray, byte length, char* decString) {
  for (byte i = 0; i < length; i++) {
    sprintf(decString + 3 * i, "%03d", byteArray[i]);
  }
  decString[3 * length] = '\0';
}
// 获取学生id
String get_stuid (String cardid){
  String stuid;
  WiFiClient client;

  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    u8g2.clearBuffer();
    displayStringOnOLED("Connection failed.",3);
    delay(3000);
    return "failed";
  }

  String postData = "&id=" + cardid;
  Serial.print("get_stuid Sending POST request to server: ");
  Serial.println(postData);

  client.print("POST ");
  client.print(stuid_check_path);
  client.print(" HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(postData.length());
  client.print("\r\n\r\n");
  client.print(postData);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  // 读取和解析JSON数据
  DynamicJsonDocument jsonDoc(512); // 可根据JSON数据大小调整缓冲区大小
  DeserializationError jsonError = deserializeJson(jsonDoc, client);
  Serial.print("Received JSON response: ");
  Serial.println(jsonDoc.as<String>());


  if (jsonError) {
    Serial.print("JSON parsing failed: ");
    Serial.println(jsonError.c_str());
    displayStringOnOLED("not found",1);
    drawsadface();
    Serial1.println("A7:00014");
    delay (3000);
    return "failed";
  }
  


  // 从JSON数据中提取"result"字段的值
  String status = jsonDoc["status"].as<String>();
  
  if (status == "failed"){
    return "failed";
  }
  else{
    stuid = jsonDoc["stuid"].as<String>();
    return stuid;
  }

}
// 查找stuid并检查五分钟内是否打过卡
int duplicate_check(String cardid){
  WiFiClient client;
  String stuid= get_stuid (cardid);

  if (stuid == "failed"){
    return 3;
  }
  else{
    if (!client.connect(serverAddress, serverPort)) {
      Serial.println("Connection failed.");
      u8g2.clearBuffer();
      displayStringOnOLED("Connection failed.",3);
      delay(3000);
      return 0;
    }

    // 发送请求
    String postData = "&id=" + stuid;
    Serial.print("duplicate_checkn Sending POST request to server: ");
    Serial.println(postData);
    client.print("POST ");
    client.print(duplicate_check_path);
    client.print(" HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(serverAddress);
    client.print("\r\n");
    client.print("Connection: close\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: ");
    client.print(postData.length());
    client.print("\r\n\r\n");
    client.print(postData);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received");
        break;
      }
    }
    // 读取和解析JSON数据
    DynamicJsonDocument jsonDoc(512); // 可根据JSON数据大小调整缓冲区大小
    DeserializationError jsonError = deserializeJson(jsonDoc, client);
    Serial.print("Received JSON response: ");
    Serial.println(jsonDoc.as<String>());
    if (jsonError) {
      Serial.print("JSON parsing failed: ");
      Serial.println(jsonError.c_str());
      return 0;
    }
    // 从JSON数据中提取"status"字段的值
    String status = jsonDoc["status"].as<String>();
    if (status == "failed"){
     return 0;
    }
    else{
      return 1;
    }
  }
}


// 刷卡签到请求并解析响应
void check_in(String mac,String cardid){
  String result;
  String idcard;
  String name;
  String reason;

  WiFiClient client;
  // 没检测到刷卡
  if (cardid == "please swipe card"){
    return;
  }
  //没连接到服务器
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed.");
    u8g2.clearBuffer();
    displayStringOnOLED("Connection failed.",3);
    delay(3000);
    return;
  }
  // 发送请求
  String postData = "mac=" + mac + "&id=" + cardid;
  Serial.print("Sending POST request to server: ");
  Serial.println(postData);

  client.print("POST ");
  client.print(check_in_path);
  client.print(" HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(serverAddress);
  client.print("\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(postData.length());
  client.print("\r\n\r\n");
  client.print(postData);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  // 读取和解析JSON数据
  DynamicJsonDocument jsonDoc(512); // 可根据JSON数据大小调整缓冲区大小
  DeserializationError jsonError = deserializeJson(jsonDoc, client);
  Serial.print("Received JSON response: ");
  Serial.println(jsonDoc.as<String>());
  
  if (jsonError) {
    Serial.print("JSON parsing failed: ");
    Serial.println(jsonError.c_str());
    // 未找到学生
    Serial1.println("A7:00014");
    delay(5000);
    return;
  }
  
  // 从JSON数据中提取"result"字段的值
  result = jsonDoc["result"].as<String>();

  if (result == "success" ){
    idcard = jsonDoc["idcard"].as<String>();
    name = jsonDoc["name"].as<String>();
    u8g2.clearBuffer();
    displayStringOnOLED("success",1);
    displayStringOnOLED(idcard,3);
    drawhappyface();
    Serial1.println("A7:00019");
    delay(3000);
    u8g2.clearBuffer();
    return;
  }
  else{
    reason = jsonDoc["reason"].as<String>();
    u8g2.clearBuffer();
    displayStringOnOLED(reason,1);
    Serial1.println("A7:00019");
    delay(3000);
    if (reason == "wrong room"){
      drawsadface();
      Serial1.println("A7:00017");
      delay(3000);
    }else if(reason == "duplicate"){
      drawsadface();
      Serial1.println("A7:00015");
      delay(3000);
    }
  }

  client.stop();
  u8g2.clearBuffer();
  return ;
}
// 报错图案
void drawsadface() {
  // 缩小比例
  float scale = 0.5;

  // 绘制圆，使用scale来缩小圆的半径
  u8g2.drawCircle(64,40,20);

  // 绘制左眼
  u8g2.drawCircle(60, 30, 2);

  // 绘制右眼
  u8g2.drawCircle(68, 30, 2);

  u8g2.drawCircle(64, 50, 8, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(64, 50, 8, U8G2_DRAW_UPPER_LEFT);
  u8g2.sendBuffer(); // 显示图形
}
// 成功图案
void drawhappyface() {
  float scale = 0.5;

  // 绘制圆，使用scale来缩小圆的半径
  u8g2.drawCircle(64,47,13);

  // 绘制左眼
  u8g2.drawCircle(61, 43, 2);

  // 绘制右眼
  u8g2.drawCircle(67, 43, 2);

  u8g2.drawCircle(64, 50, 6, U8G2_DRAW_LOWER_RIGHT);
  u8g2.drawCircle(64, 50, 6, U8G2_DRAW_LOWER_LEFT);
  u8g2.sendBuffer(); // 显示图形
}

//—————————————————————————————————————跑起来了———————————————————————————————————————————————————————
void loop() {
  // 检测刷卡
  String cardid = readNUID();
  if(cardid == "Your tag is not of type MIFARE Classic."){
    // 错误卡
    tone(BEEP_PIN,1000,100);
    drawsadface();
    displayStringOnOLED("type error ",1);
    Serial1.println("A7:00022");
    delay (3000);
  }else if(cardid == "Card read previously."){
    // 重复刷卡
    tone(BEEP_PIN,1000,100);
    drawsadface();
    displayStringOnOLED("duplicate",1);
    Serial1.println("A7:00015");
    delay (3000);
  }else if(cardid == "please swipe card"){
    // 等待刷卡 展示时间
    u8g2.clearBuffer();
    String now_time=getTime();
    displayStringOnOLED(now_time,3);
    u8g2.clearBuffer();
  }else{
    // 打卡检测
    tone(BEEP_PIN,1000,100);
    int result = duplicate_check(cardid); // 五分钟内打卡检测
    if(result == 1){
      // 成功打卡
      check_in(macaddress,cardid);
    }else if(result == 3){
      // 未找到学生
      Serial.println("404!!");
      displayStringOnOLED("not found",1);
      drawsadface();
      Serial1.println("A7:00014");
      delay (3000);
    }
    else{
      // 重复打卡
      displayStringOnOLED("duplicate",1);
      drawsadface();
      Serial1.println("A7:00015");
      delay (3000);
    }
  }
  u8g2.clearBuffer();
}

