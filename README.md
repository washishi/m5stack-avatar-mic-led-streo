# m5stack-avatar-mic-led
マイクを使ったM5Stack Avatarの例です。
音に合わせてAvatarが口パクしたり、傾いたりします。
mongonta0716さんの「m5stack-avatar-mic」へ、音に合わせてLEDをピカピカさせる機能を追加しました。<br>
[M5Stack用NeoPixel互換LED搭載 HEXボード](https://www.switch-science.com/products/6058 "Title")に対応しました。<br>
オリジナルはこちらです。<br>
<https://github.com/mongonta0716/m5stack-avatar-mic><br>

# 対応デバイス

- Core2 for AWSIoT
- M5Stack Core2 + M5GO Bottom2(M5Stack Core2用バッテリーボトム)
- ~~M5StickC~~
- ~~M5StickCPlus~~
- ~~ATOMS3 + PDMUnit~~
- [M5Stack用NeoPixel互換LED搭載 HEXボード](https://www.switch-science.com/products/6058 "Title") (必要に応じて)<br>

# M5Stack用NeoPixel互換LED搭載 HEXボードを使用する場合<br>
1, LED HEXボードをPORT Aへ接続してください。<br>
2. main.cppの12行目のコメントを外してください。<br>
3. M5Stack Core2を使用する場合、M5GO Bottom2は不要です。<br>
4. Core2 for AWSIoT 及び M5GO Bottom2の内蔵LEDとの同時使用はできません。<br>
<br>
# 環境

・VSCode + PlatformIO

ArduinoIDEの場合は下記のように名前を変更してください。
- srcフォルダ -> m5stack-avatar-mic-led
- main.cpp -> m5stack-avatar-mic-led.ino

# 動作確認済みボード・ライブラリバージョン

詳細はplatformio.iniを見てください。

# ボード

- arduiono-esp32 v2.0.5(espressif 5.2.0)

# ライブラリ
- M5Stack-Avatar v0.8.2
- M5Unified v0.1.1


