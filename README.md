# m5stack-avatar-mic-led-streo
マイクを使ったM5Stack Avatarの例です。<br>
音に合わせてAvatarが口パクしたり、傾いたりします、LEDを光らせたりします。<br>
mongonta0716さんの「m5stack-avatar-mic」に robo8080さんがLEDを光らす機能を追加した「m5stack-avatar-mic-led」にマイクと8LEDステックをそれぞれ2つ接続してステレオに対応しました<br>

オリジナルはこちらです。<br>
m5stack-avatar-mic     <https://github.com/mongonta0716/m5stack-avatar-mic><br>
m5stack-avatar-mic-led <https://github.com/robo8080/m5stack-avatar-mic-led><br>

# 対応デバイス

- Core2 for AWSIoT
- M5Stack Core2 + M5GO Bottom2(M5Stack Core2用バッテリーボトム)
- ~~M5StickC~~
- ~~M5StickCPlus~~
- ~~ATOMS3 + PDMUnit~~
- [M5Stack用NeoPixel互換LED搭載 HEXボード](https://www.switch-science.com/products/6058 "Title") (必要に応じて)<br>
- ステレオ対応追加分  ※現状Core2のみ またM-BUSに直接接続する必要があります<br>
    - 8LEDx2<br>
      秋月 マイコン内蔵RGBLED 8LEDスティック [AE-WS2812B-STICK8] x2<br>
      https://akizukidenshi.com/catalog/g/gM-14307/  (またはフルカラーLED WS2812を8個搭載した相当品)<br>
      8LEDスティック2つの+5V,GNDをそれぞれ接続し1つ目のDINにGPIO25をDOUTに2つ目のDINを接続します<br>
      (2つ目のDOUTは未接続) 1つ目が向かって右、2つ目が向かって左になります<br>
    - マイクx2<br>
      秋月 SPH0641LU4H使用 超広帯域マイクモジュールキット [AE-SPH0641LU4H-1-8] x2<br>
      https://akizukidenshi.com/catalog/g/gK-15577/  (またはPDM出力可能でデータタイミングの変更可能なマイクの相当品)<br>
      モジュール上のジャンパーを1つはJ1をはんだでショート(向かって左マイク)、もう1つはJ2をショートします(向かって右マイク)<br>
      そして2つを同じ様にVDDを3.3V(注:5Vに繋ぐと壊れます),GNDをGND,CLKをGPIO0にDATをGPIO34に接続します<br>
      (なおこれらのデバイスが接続可能なｽﾀｯｸﾁｬﾝ用改変基板のデータをのちに公開する予定です)<br>
<br>
    8LEDx2 を無効にするにはソース16行目「#define USE_STICK8X2」をコメントにしてください<br>
    ステレオ対応を無効にするにはソース19行目「#define STEREO」をコメントにしてください<br>
    (上記2つをコメントするとオリジナルと同等になります)<br>
    なお 8LEDx2 と LED HEXボードとの併用はソフトで対応していないためできません<br>
<br>
# M5Stack用NeoPixel互換LED搭載 HEXボードを使用する場合<br>
1, LED HEXボードをPORT Aへ接続してください。<br>
2. main.cppの12行目のコメントを外してください。<br>
3. M5Stack Core2を使用する場合、M5GO Bottom2は不要です。<br>
4. Core2 for AWSIoT 及び M5GO Bottom2の内蔵LEDとの同時使用はできません。<br>
<br>
# 環境

・VSCode + PlatformIO

ArduinoIDEの場合は下記のように名前を変更してください。
- srcフォルダ -> m5stack-avatar-mic-led-stereo
- main.cpp -> m5stack-avatar-mic-led-stereo.ino

# 動作確認済みボード・ライブラリバージョン

詳細はplatformio.iniを見てください。

# ボード

- arduiono-esp32 v2.0.5(espressif 5.2.0)

# ライブラリ
- M5Stack-Avatar v0.8.2
- M5Unified v0.1.1
- FastLED v3.5.0

