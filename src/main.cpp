// Copyright(c) 2023 Takao Akaki

#include <stdlib.h>
#include <M5Unified.h>
#include <Avatar.h>
#include "fft.hpp"

#define USE_MIC
// LEDレベルメータを使用する;
#define USE_FASTLED
// HEX LEDをレベルメータとして使用する;
//#define USE_HEX_LED

// 秋月	マイコン内蔵RGB LED 8LEDスティック 
// [AE-WS2812B-STICK8] を2つ使用する
//#define USE_STICK8X2

// マイク2つでステレオ対応にする
#define STEREO
// 2つ目のマイクとしてPDM UNITを利用する(M5GO Bottom2のマイクと併用でステレオ対応にできます)
#define PDM_UNIT

#ifdef USE_MIC
  // ---------- Mic sampling ----------

  int pin_clk  = -1;
  int pin_data = -1;
  //#define PIN_CLK     0
  //#define PIN_DATA    34
  #define READ_LEN    (2 * 256)
  #define GAIN_FACTOR 3
  uint8_t BUFFER[READ_LEN] = {0};

  //int16_t *adcBuffer = NULL;
  static fft_t fft;
  static constexpr size_t WAVE_SIZE = 320;
  static int16_t raw_data[WAVE_SIZE * 2];

  float lipsync_max = 300.0f;  // リップシンクの単位ここを増減すると口の開き方が変わります。

#endif

#ifdef USE_FASTLED
#include <FastLED.h>

  // How many leds in your strip?
#ifdef USE_HEX_LED
  #define NUM_LEDS 37
#else
  #ifdef USE_STICK8X2
    #define NUM_LEDS 16
  #else
    #define NUM_LEDS 10
  #endif
#endif
  // #if defined(ARDUINO_M5STACK_Core2)
  // #define DATA_PIN 25
  // #else
  // #define DATA_PIN 15
  // #endif

// Define the array of leds
CRGB leds[NUM_LEDS];

void turn_off_led() {
  // Now turn the LED off, then pause
  for(int i=0;i<NUM_LEDS;i++) leds[i] = CRGB::Black;
  FastLED.show();
}

void fill_led_buff(CRGB color) {
  // Now turn the LED off, then pause
  for(int i=0;i<NUM_LEDS;i++) leds[i] =  color;
}

void clear_led_buff() {
  // Now turn the LED off, then pause
  for(int i=0;i<NUM_LEDS;i++) leds[i] =  CRGB::Black;
}

#ifndef USE_HEX_LED
  #ifdef USE_STICK8X2
    CRGB led_table[NUM_LEDS/2] = {CRGB::Purple,CRGB::MediumPurple,CRGB::Blue,CRGB::Green,CRGB::LimeGreen,CRGB::Yellow,CRGB::Orange,CRGB::Red};
  #else
    CRGB led_table[NUM_LEDS/2] = {CRGB::Blue,CRGB::Green,CRGB::Yellow,CRGB::Orange,CRGB::Red};
  //CRGB color_table[5] = {CRGB::Blue,CRGB::Green,CRGB::Yellow,CRGB::Orange,CRGB::Red};
  #endif

void level_led(int level1, int level2) {
  if(level1>NUM_LEDS/2) level1 = NUM_LEDS/2;
  if(level2>NUM_LEDS/2) level2 = NUM_LEDS/2;
  clear_led_buff();
  // LEDの順番
  //
  // M5GO Bottom2
  //    下 ------------------ 上
  // 右 LED4 LED3 LED2 LED1 LED0
  // 左 LED5 LED6 LED7 LED8 LED9
  //
  // 秋月	マイコン内蔵RGB LED 8LEDスティック x2
  //    下 ---------------------------------------- 上
  // 右 LED0 LED 1 LED2  LED3  LED4  LED5  LED6  LED7
  // 左 LED8 LED 9 LED10 LED11 LED12 LED13 LED14 LED15
  
  // 右LED 
  for(int i=0;i<level1;i++){
#ifndef USE_STICK8X2
    leds[NUM_LEDS/2-1-i] = led_table[i];
#else
    leds[i] = led_table[i];
#endif
  }
  // 左LED
  for(int i=0;i<level2;i++){
    leds[i+NUM_LEDS/2] = led_table[i];
  }
  FastLED.show();
}
#else
CRGB hex_color_table[6][NUM_LEDS] = {
{//0
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
	CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
	CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black
},
{//1
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
	CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Blue,CRGB::Black,CRGB::Black,CRGB::Black,
	CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black
},
{//2
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
	CRGB::Black,CRGB::Black,CRGB::Green,CRGB::Green,CRGB::Black,CRGB::Black,
CRGB::Black,CRGB::Black,CRGB::Green,CRGB::Blue,CRGB::Green,CRGB::Black,CRGB::Black,
	CRGB::Black,CRGB::Black,CRGB::Green,CRGB::Green,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black
},
{//3
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black,
		CRGB::Black,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Black,
	CRGB::Black,CRGB::Yellow,CRGB::Green,CRGB::Green,CRGB::Yellow,CRGB::Black,
CRGB::Black,CRGB::Yellow,CRGB::Green,CRGB::Blue,CRGB::Green,CRGB::Yellow,CRGB::Black,
	CRGB::Black,CRGB::Yellow,CRGB::Green,CRGB::Green,CRGB::Yellow,CRGB::Black,
		CRGB::Black,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Black,
			CRGB::Black,CRGB::Black,CRGB::Black,CRGB::Black
},
// {//4
// 			CRGB::Orange  ,CRGB::Orange,CRGB::Orange,CRGB::Orange,
// 		CRGB::Orange,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Orange,
// 	CRGB::Orange,CRGB::Yellow,CRGB::Green,CRGB::Green,CRGB::Yellow,CRGB::Orange,
// CRGB::Orange,CRGB::Yellow,CRGB::Green,CRGB::Blue,CRGB::Green,CRGB::Yellow,CRGB::Orange,
// 	CRGB::Orange,CRGB::Yellow,CRGB::Green,CRGB::Green,CRGB::Yellow,CRGB::Orange,
// 		CRGB::Orange,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Orange,
// 			CRGB::Orange,CRGB::Orange,CRGB::Orange,CRGB::Orange
// },
{//4
			CRGB::Magenta  ,CRGB::Magenta,CRGB::Magenta,CRGB::Magenta,
		CRGB::Magenta,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Magenta,
	CRGB::Magenta,CRGB::Yellow,CRGB::Green,CRGB::Green,CRGB::Yellow,CRGB::Magenta,
CRGB::Magenta,CRGB::Yellow,CRGB::Green,CRGB::Blue,CRGB::Green,CRGB::Yellow,CRGB::Magenta,
	CRGB::Magenta,CRGB::Yellow,CRGB::Green,CRGB::Green,CRGB::Yellow,CRGB::Magenta,
		CRGB::Magenta,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Magenta,
			CRGB::Magenta,CRGB::Magenta,CRGB::Magenta,CRGB::Magenta
},
{//5
			CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,
		CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,
	CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,
CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,
	CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,
		CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red,
			CRGB::Red,CRGB::Red,CRGB::Red,CRGB::Red
}
};
void hex_led(int level) {
  if(level>5) level = 5;
  for(int i=0;i<NUM_LEDS;i++) leds[i] =  hex_color_table[level][i];  
  FastLED.show();
}
// void hex_led(int level) {
//   if(level>5) level = 5;
//   clear_led_buff();
//   if(level>0) fill_led_buff(color_table[level-1]);
//   FastLED.show();
// }
#endif
#else
void turn_off_led() {}
// void fill_led_buff(CRGB color) {}
void clear_led_buff() {}
void level_led(int level1, int level2) {}
#endif

using namespace m5avatar;

Avatar avatar;
ColorPalette *cps[6];
uint8_t palette_index = 0;

uint32_t last_rotation_msec = 0;

void initI2S(const int8_t s = 0)  // Init I2S.  初始化I2S
{
  i2s_config_t i2s_config;
  i2s_pin_config_t pin_config;
  switch (s)
  {
  case 0: // I2S init with pin config for M5GO Bottom2 or Core2 Ext.
    Serial.printf("init 0\n");
    i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX |
                           I2S_MODE_PDM), // Set the I2S operating mode.
                                          // 设置I2S工作模式
      .sample_rate = 44100,               // Set the I2S sampling rate.  设置I2S采样率
      .bits_per_sample =
          I2S_BITS_PER_SAMPLE_16BIT, // Fixed 12-bit stereo MSB.
                                     // 固定为12位立体声MSB
      .channel_format =
#if defined(STEREO) && not defined(PDM_UNIT)
          I2S_CHANNEL_FMT_RIGHT_LEFT,
#else
          I2S_CHANNEL_FMT_ALL_RIGHT, // Set the channel format.  设置频道格式
#endif
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
      .communication_format =
          I2S_COMM_FORMAT_STAND_I2S, // Set the format of the communication.
#else                                // 设置通讯格式
      .communication_format = I2S_COMM_FORMAT_I2S,
#endif
      .intr_alloc_flags =
          ESP_INTR_FLAG_LEVEL1, // Set the interrupt flag.  设置中断的标志
      .dma_buf_count = 2,       // DMA buffer count.  DMA缓冲区计数
      .dma_buf_len = 128,       // DMA buffer length.  DMA缓冲区长度
    };
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

 case 1: // pin config only  for M5GO Bottom2 or Core2 Ext.
#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = pin_clk;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = pin_data;
    i2s_set_pin(I2S_NUM_0, &pin_config);
    break; 
 case 2: // pin config only  for PDM_UNIT(PortA)
#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = 33;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = 32;
    i2s_set_pin(I2S_NUM_0, &pin_config);
    break;
  }
}

//int8_t i2s_readraw_buff[1024];
int16_t *i2s_readraw_buff = (int16_t *)calloc(1, 1024);
#ifdef PDM_UNIT
int16_t *i2s_readraw_buff2 = (int16_t *)calloc(1, 1024);
#endif
uint8_t fft_dis_buff[161][80] = {0};
uint16_t posData              = 160;

void lipsync() {
  
  size_t bytesread;
  uint64_t level = 0;
  #ifdef PDM_UNIT
  //pin configを切り替え　その後暫くデータが乱れるのでダミーで少し読み込んでから再度読み込み
  initI2S(1);
  i2s_read(I2S_NUM_0, i2s_readraw_buff, 512, &bytesread,
                (100 / portTICK_RATE_MS));
  #endif
  i2s_read(I2S_NUM_0, i2s_readraw_buff, 1024, &bytesread,
                (100 / portTICK_RATE_MS));
  //Serial.printf("M5GO Bottom2    %6d  %6d\n",i2s_readraw_buff[0], i2s_readraw_buff[1]);
  
#ifdef PDM_UNIT
  uint64_t level2 = 0;
  //pin configを切り替え　その後暫くデータが乱れるのでダミーで少し読み込んでから再度読み込み
  initI2S(2);
  i2s_read(I2S_NUM_0, i2s_readraw_buff2, 512, &bytesread,
              (100 / portTICK_RATE_MS));
  i2s_read(I2S_NUM_0, i2s_readraw_buff2, 1024, &bytesread,
              (100 / portTICK_RATE_MS));
  //Serial.printf("PDM_UNIT(PortA) %6d  %6d\n",i2s_readraw_buff2[0], i2s_readraw_buff2[1]);
#endif
  fft.exec(i2s_readraw_buff);
  for (size_t bx=5;bx<=60;++bx) {
    int32_t f = fft.get(bx);
    level += abs(f);
  }
#ifdef PDM_UNIT
  fft.exec(i2s_readraw_buff2);
  for (size_t bx=5;bx<=60;++bx) {
    int32_t f = fft.get(bx);
    level2 += abs(f);
  }
#endif

#ifdef STEREO
  uint64_t level_R = 0;
  uint64_t level_L = 0;
  #ifndef PDM_UNIT
  fft.exec(i2s_readraw_buff, 1);
  for (size_t bx=5;bx<=60;++bx) {
    int32_t f = fft.get(bx);
    level_R += abs(f);
  }
  fft.exec(i2s_readraw_buff, 2);
  for (size_t bx=5;bx<=60;++bx) {
    int32_t f = fft.get(bx);
    level_L += abs(f);
  }
  #else
  level_L = level2;
  level_R = level;
  #endif
#endif
#ifdef PDM_UNIT
  if (level < level2) level=level2;
#endif
  //Serial.printf("LEVEL  MONO:%6ld", level); 
  //Serial.printf("  R:%6ld", level_R);
  //Serial.printf("  L:%6ld\n", level_L);

  //Serial.printf("level:%d\n", level) ;         // lipsync_maxを調整するときはこの行をコメントアウトしてください。
  float ratio = (float)((level >> 9)/lipsync_max);
  // Serial.printf("ratio:%f\n", ratio);
  if (ratio <= 0.01f) {
    ratio = 0.0f;
  } else if (ratio >= 1.3f) {
    ratio = 1.3f;
  }
  if ((millis() - last_rotation_msec) > 350) {
#ifdef STEREO
// ステレオ対応の場合はランダムではなく左右の音量差が一定以上ある場合に顔を傾ける
    int direction = 0;
    if (level_L > level_R * 1.1 ) {
      if (level_L > level_R * 1.2 ) {
        direction = -2; 
      } else {
        direction = -1; 
      }
    } else {
      if (level_R > level_L * 1.1 ){
        if (level_R > level_L * 1.2 ){
          direction = 2;
        } else {
          direction = 1;
        }
      }  
    } 
#else
    int direction = random(-2, 2);
#endif
    avatar.setRotation(direction * 10 * ratio);
    last_rotation_msec = millis();
  }
  avatar.setMouthOpenRatio(ratio);
#ifdef USE_FASTLED
   int led_level = (int)(ratio*(NUM_LEDS/2));
  #ifdef USE_HEX_LED
   hex_led(led_level);
  #else
    #ifndef STEREO
   level_led(led_level, led_level);
    #else
   float ratio_L = (float)((level_L >> 9)/lipsync_max);
   float ratio_R = (float)((level_R >> 9)/lipsync_max);
   int led_level_L = (int)(ratio_L*(NUM_LEDS/2));
   int led_level_R = (int)(ratio_R*(NUM_LEDS/2));
   level_led(led_level_R,led_level_L);
    #endif 
  #endif
#endif    
}


void setup()
{
  auto cfg = M5.config();
  cfg.internal_mic = false;
  M5.begin(cfg);
  float scale = 0.0f;
  int8_t position_x = 0;
  int8_t position_y = 0;
  uint8_t display_rotation = 3; // ディスプレイの向き(0〜3)
  uint8_t first_cps = 0;
  switch (M5.getBoard()) {
    case m5::board_t::board_M5AtomS3:
      first_cps = 4;
      scale = 0.5f;
      position_x = 5;
      position_y = -15;
      pin_clk  = 1;
      pin_data = 2;
      break;

    case m5::board_t::board_M5StickC:
#ifdef ARDUINO_M5Stick_C    
      M5.Power.Axp192.setLDO0(2800); // 一部これを実行しないとマイクが動かない機種がある。
#endif
      first_cps = 1;
      scale = 0.6f;
      position_x = -30;
      position_y = -15;
      pin_clk  = 0;
      pin_data = 34;
      break;

    case m5::board_t::board_M5StickCPlus:
#ifdef ARDUINO_M5Stick_C    
      M5.Power.Axp192.setLDO0(2800); // 一部これを実行しないとマイクが動かない機種がある。
#endif
      first_cps = 2;
      scale = 0.7f;
      position_x = -15;
      position_y = 5;
      pin_clk  = 0;
      pin_data = 34;
      break;
    
    case m5::board_t::board_M5StackCore2:
      scale = 1.0f;
      position_x = 0;
      position_y = 0;
      display_rotation = 1;
      pin_clk  = 0;
      pin_data = 34;
      break;

    case m5::board_t::board_M5Stack:
      scale = 1.0f;
      position_x = 0;
      position_y = 0;
      display_rotation = 1;
      pin_clk  = 0;
      pin_data = 34;  
      break;
      
    defalut:
      Serial.println("Invalid board.");
      break;
  }
  initI2S();
  M5.Lcd.setRotation(display_rotation);
  avatar.setScale(scale);
  avatar.setPosition(position_x, position_y);
  avatar.init(); // start drawing
  cps[0] = new ColorPalette();
  cps[0]->set(COLOR_PRIMARY, TFT_BLACK);
  cps[0]->set(COLOR_BACKGROUND, TFT_YELLOW);
  cps[1] = new ColorPalette();
  cps[1]->set(COLOR_PRIMARY, TFT_BLACK);
  cps[1]->set(COLOR_BACKGROUND, TFT_ORANGE);
  cps[2] = new ColorPalette();
  cps[2]->set(COLOR_PRIMARY, (uint16_t)0x00ff00);
  cps[2]->set(COLOR_BACKGROUND, (uint16_t)0x303303);
  cps[3] = new ColorPalette();
  cps[3]->set(COLOR_PRIMARY, TFT_WHITE);
  cps[3]->set(COLOR_BACKGROUND, TFT_BLACK);
  cps[4] = new ColorPalette();
  cps[4]->set(COLOR_PRIMARY, TFT_BLACK);
  cps[4]->set(COLOR_BACKGROUND, TFT_WHITE);
  cps[5] = new ColorPalette();
  cps[5]->set(COLOR_PRIMARY, 0x303303);
  cps[5]->set(COLOR_BACKGROUND, 0x00ff00);
  avatar.setColorPalette(*cps[first_cps]);
  //avatar.addTask(lipsync, "lipsync");
  last_rotation_msec = millis();

#ifdef USE_FASTLED
  if (M5.getBoard() == m5::board_t::board_M5Stack)
  {
    FastLED.addLeds<SK6812, GPIO_NUM_15, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  }
  else
  if (M5.getBoard() == m5::board_t::board_M5StackCore2)
  {
#ifdef USE_HEX_LED
    FastLED.addLeds<SK6812, GPIO_NUM_32, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
#else
    FastLED.addLeds<SK6812, GPIO_NUM_25, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
#endif
  }
  FastLED.setBrightness(10);
#ifdef USE_HEX_LED
  hex_led(4);
#else
//level_led(5, 5);
  level_led(NUM_LEDS/2, NUM_LEDS/2);
#endif
  FastLED.show();
  delay(500);
#endif
}

uint32_t count = 0;

void loop()
{
  M5.update();
  if (M5.BtnA.wasPressed()) {
    palette_index++;
    if (palette_index > 5) {
      palette_index = 0;
    }
    avatar.setColorPalette(*cps[palette_index]);
  }
  
//  if ((millis() - last_rotation_msec) > 100) {
    //float angle = 10 * sin(count);
    //avatar.setRotation(angle);
    //last_rotation_msec = millis();
    //count++;
  //}

  // avatar's face updates in another thread
  // so no need to loop-by-loop rendering
  lipsync();
  vTaskDelay(1/portTICK_PERIOD_MS);
}
