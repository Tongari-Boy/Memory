# C++ Memory Management Study Project

C++とSDL2を使ったメモリ管理学習用プロジェクトです。  
コンシューマゲームの最適化エンジニアを目指すための実践的な実装を行っています。

---

## 学習テーマ

- スタックとヒープの違い
- メモリレイアウトとパディング
- オブジェクトプールによるメモリ管理
- フリーリストによる探索コストの削減
- アクティブリストによる不要ループの削減
- テンプレートを使った汎用プール設計

---

## 実装済み機能

- `Pool<T, MAX>` : 汎用オブジェクトプール
  - フリーリストによるO(1)のalloc
  - アクティブリストによる不要スロットのスキップ
  - 二重free検出

---

## 環境構築

### 必要なもの

- Visual Studio 2022（C++によるデスクトップ開発）
- CMake 3.20以上
- SDL2（別途ダウンロードが必要）

### SDL2のセットアップ

1. [SDL2公式](https://github.com/libsdl-org/SDL/releases) から`SDL2-devel-x.x.x-VC.zip`をダウンロード
2. 解凍して`libs/SDL2/`に配置する

```
MyGame/
├── CMakeLists.txt
├── README.md
├── src/
│   └── main.cpp
└── libs/
    └── SDL2/        ← ここに配置
        ├── include/
        └── lib/
```

### ビルド手順

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

`build/Debug/MyGame.exe`が生成されます。

---

## 技術スタック

| 項目 | 内容 |
|---|---|
| 言語 | C++17 |
| グラフィック | SDL2 |
| ビルドシステム | CMake |
| IDE | Visual Studio 2022 |