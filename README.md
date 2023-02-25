# Pedal Controller for PSS-A50

## 概要
Raspberry Pi Picoを使ってYAMAHA PSS-A50にサステインペダルを追加します。
PSS-A50から送信されたMIDI信号をそのまま送り返し、追加でペダル情報を送信します。


## ビルド済みuf2ファイル
TODO


## 使い方
1. PicoのGP15にペダル（またはボタン）を取り付ける ([Picoピンアサイン](https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf))
2. ノーマリクローズのペダル（例：YAMAHA FC5）を使用する場合はPico起動前にGP10を短絡する
3. PicoのVBUSに5V電源を供給する
4. PSS-A50をPicoとUSBで接続する
5. PSS-A50のControl Changeをオン、Local Controlをオフにする
6. 好みの音色に切り替えて演奏する


## 開発環境
Windows 11
Visual Studio 2022
CMake 3.24.3


## ビルド方法 
事前にpico-sdkとコンパイラの導入を行う必要があります。
https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf

ビルド（Windows）
Visual Studio 開発者コマンドプロンプトで実行してください
```
cd {project root}
mkdir build
cd build
cmake -G "NMake Makefiles" .. -D FAMILY="rp2040" -D PICO_SDK_PATH="{your pico-sdk path}"
nmake
```

## 注意
- 使用は自己責任です。Picoのピン配置に注意してください。
- 受信したMIDI情報をそのまま送り返しているため、動作がおかしいところがあります。
  - 起動直後の音色がNo.002になっている
  - 初回の録音データ再生時の音色がおかしい
  etc


## ライブラリ
- rppicomidi/tinyusb (forked from hathach/tinyusb)
https://github.com/rppicomidi/tinyusb


## ライセンス
LICENSEファイルを参照してください
