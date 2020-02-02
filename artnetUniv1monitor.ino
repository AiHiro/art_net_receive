#include <ArtnetWifi.h>
#include <M5Stack.h>
#include <WiFi.h>

const char* ssid = "SSID"; //接続するアクセスポイントのSSIDに書き換える
const char* password = "password";//接続するアクセスポイントのパスワードに書き換える

//Art-Net setting
ArtnetWifi artnet;
const int startUniverse = 0;

bool sendFrame = 1;
int previousDataLength = 0;

//M5Stack Lcd setting
int pos_x = 40;
int pos_y = 140;
int height = 80;
int width = 80;

//Wi-Fiの接続確認
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0; 
  WiFi.begin(ssid,password); //Wi-fi APに接続する
  while(WiFi.status() != WL_CONNECTED){ //Wi-Fi AP接続待ち
    delay(500);
    M5.Lcd.print(".");
    if(i > 20){
      state = false;  
      break;
    }
    i++;
  }
  if(state){
    M5.Lcd.print("\r\nWiFi connected\r\nIP address:");
    M5.Lcd.println(WiFi.localIP());
  }else{
    M5.Lcd.println("");
    M5.Lcd.println("Conection failed.");
  }
  return state;
}

//M5Stackに描画する際にRGB値を使用するため
uint16_t getColor(uint8_t red, uint8_t green, uint8_t blue){
  return ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  if(universe == 1){
    for(int i = 0; i < 3; i++){
    　//DMX ch0 ~ 2,ch3 ~ 5, ch6~8の出力値をM5Stackで描画
      M5.Lcd.fillRect(pos_x + i*width, pos_y,height,width,getColor(data[0+i*3],data[1+i*3],data[2+i*3]));
    }
  }else{
    //受診がなければ黒に
    M5.Lcd.fillRect(pos_x,pos_y,height,width*3,BLACK);
  }
}

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setTextSize(2);
  ConnectWifi();
  artnet.begin();

  // onDmxFrame will execute every time a packet is received by M5Stack.
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop() {
  // put your main code here, to run repeatedly:
  artnet.read();
}
