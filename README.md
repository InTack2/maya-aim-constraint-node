# AimConstraintNode
[![Image from Gyazo](https://i.gyazo.com/6d27c0e4e7450175d4a632e462b7a69a.gif)](https://gyazo.com/6d27c0e4e7450175d4a632e462b7a69a)

学習用にMayaのエイムコンストレインノードをC++とPythonで作成。  
ほぼ実用性はないですが、Enumで基準軸変えられたり、必要そうな設定は変更可能です。  

## 対象
PythonとC++で実装してみましたので、  
どちらかの作り方がわからない方にはもしかしたらもしかすると、作り方の参考になるかもしれません。  
逆にこうすると良いよ！などはご教示いただけますと幸いです。  

## 動作環境
- Windows10
- Maya2020

## Python
### 前提
特になし

### 確認方法
- aim_constraint_node.pyをpluginManagerでロードする
- NodeEditorで「AimConstraintNode」と入力し、ノードを生成する


## C++
### 前提
- CMakeが導入済み
- Mayaのdevkitをダウンロードしそのルートを「DEVKIT_LOCATION」として環境変数に追加済み

### 確認方法
#### 構築
``` bash
cd "クローンしたパス"
cmake -H. -Bbuild -G "Visual Studio 16 2019"
```

#### ビルド
``` bash
cmake --build build
```

#### Maya側
- ./build/Debug/aimConstraintNode.mllをpluginManagerでロードする
- NodeEditorで「AimConstraintNode」と入力し、ノードを生成する

### デバック
#### VisualStudio
[![Image from Gyazo](https://i.gyazo.com/3a31d35e29a26289c4def4cd9afabd4c.png)](https://gyazo.com/3a31d35e29a26289c4def4cd9afabd4c)
デバックの設定のアタッチを「はい」に変更する。  
こちらを指定する事で既に起動中のMayaへアタッチ。起動していない場合自動起動でデバックが可能となる。  

CMakeで設定できたら良いですが、上手くいかず。。  
詳しい方ご教示いただけますと幸いです。