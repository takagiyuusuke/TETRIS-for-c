# Overview

stringのみで構成されたでテトリスが遊べます。  
※フォントによっては体裁が崩れるので気を付けてください。

## How to Play

プレイ画面です。VScodeの初期設定されているフォントを用いるとうまくいきます。
![tetris for c 改](https://github.com/takagiyuusuke/TETRIS-for-c/assets/142160956/2679a1a6-ae1f-4ffa-947b-28c1c2c21f59)  
基本のテトリスのルール・動作は実装されています。(高度な回転入れは未実装)  
  
操作：  

- d→左移動
- f→右移動
- j→左回転
- k→右回転
- space→下降
- h→ホールド

## 特徴

このゲームではprintf()関数でstringを出力することだけを用いてディスプレイを仮想的に実装しています。
C言語に関する基本的な知識があれば作成可能です。

## はじめに

始めるにはコンパイルしてください。
```bash
gcc TETRIS.c -o TETRIS

./TETRIS
```
