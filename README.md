# cmemo

![License](https://img.shields.io/badge/license-GPL--2.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)

**タスクトレイに常駐する、軽量なメモ帳アプリです。**

ちょっとしたメモを取りたいときに、デスクトップを圧迫せずサッと開いて書き込める。そんな「もう一枚の付箋」感覚で使えるWindows向けメモツールです。

<!--
ここにスクリーンショットまたはGIFアニメーションを挿入すると効果的です。
例: ![screenshot](docs/screenshot.png)
実際の画面(タスクトレイアイコン、メモ画面など)を撮って追加してください。
-->

## 特徴

- 🗂️ **タスクトレイに常駐** — 邪魔にならず、必要なときだけすぐ呼び出せます
- 🖱️ **ワンクリックで開閉** — タスクトレイアイコンを左クリックでメモ帳を表示、もう一度左クリックで閉じます
- 💾 **自動保存** — プログラム終了時に内容が自動でファイルに保存されるので、次回起動時も同じ内容から編集を再開できます
- 📝 **複数メモの管理** — 用途ごとに複数のメモを作成・管理できます

## 動作環境

- Windows(対応バージョンは要確認・追記をおすすめします)
- C++Builder(Borland Developer Studio)でビルドされたプロジェクトです

## インストール方法

<!--
ビルド済み実行ファイル(.exe)をGitHub Releasesで配布すると、
ソースをビルドできない一般ユーザーにも届きやすくなります。
配布後、以下のような案内に差し替えてください。

1. [Releases](../../releases) から最新版をダウンロード
2. 任意のフォルダに展開
3. `cmemo.exe` を実行
-->

現時点ではソースコードからのビルドが必要です。手順は下記「ビルド方法」をご覧ください。

## 使い方

1. アプリを起動すると、タスクトレイにアイコンが常駐します
2. アイコンを左クリックするとメモ帳が開きます
3. 内容を入力し、もう一度アイコンを左クリックすると閉じます
4. 入力した内容はプログラム終了時に自動で保存され、次回起動時に復元されます
5. 複数のメモを切り替えて管理できます

## ビルド方法

<!--
実際のビルド手順(必要なIDE・SDKのバージョンなど)を追記してください。
例:
1. C++Builder(Borland Developer Studio)をインストール
2. 本リポジトリをクローン
   git clone https://github.com/sky-seeker99/cmemo.git
3. `cmemo.bdsproj` を開いてビルド
-->

```bash
git clone https://github.com/sky-seeker99/cmemo.git
```

C++Builder(Borland Developer Studio)で `cmemo.bdsproj` を開いてビルドしてください。

## ライセンス

このプロジェクトは [GPL-2.0 License](LICENSE) のもとで公開されています。

## Contributing / Issues

バグ報告・機能要望・プルリクエストを歓迎します。[Issues](../../issues) からお気軽にご連絡ください。

---

## English

**cmemo** is a lightweight memo pad that lives in your Windows task tray.

- Sits quietly in the task tray until you need it
- Left-click the tray icon to open the memo pad, left-click again to close it
- Notes are automatically saved to a file when the program exits, so you can pick up right where you left off
- Manage and edit multiple memos at once

### License

Released under the [GPL-2.0 License](LICENSE).
