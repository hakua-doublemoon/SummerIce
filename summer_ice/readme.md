
# SummerIceの説明

---

## 概要を手短に
Qtで作ったAndroid目覚ましもどきです。
詳しいことは[Qiita](https://qiita.com/hakua-doublemoon/items/5d65cb248753000862e6)に書いてたりするかも。

<br>

## 更新情報
2017/12/31 初版
2018/05/06 アラーム時の画面起動の制御を修正。ほか、構成の整理など。

<br>

## とりあえず開発・動作確認環境
#### Qt version  
5.9.1
5.10.1

#### Android version
5.1.1 （Acerの安い端末）

#### Java version
1.8.0_131
1.8.0_172

#### System dependencies
とくにないはず

#### Configuration  
音声と画像、それらを記載したvoice_list.jsonファイルはAndroidなど動作させる
環境の適当な場所に置き、そのパスをsummer_clock.cppのANDROID_VOICE_DIRに
書いてください。

#### Database creation
voice_list.json を適当に書いてください。
詳細な説明が必要でしたらリクエストをください。一人でやってるとそのモチベーションがないので。

#### Database initialization
voice_list.json を適当に書いてください。

#### Services (job queues, cache servers, search engines, etc.)  
めざましの時間のためにポーリングしているサービスがいます。
そんなにバッテリーは消費しないつもり。

#### Deployment instructions
Qt Creatorでコンパイル、デプロイすれば良いと思います。

<br>

# その他
---
* 書いた人間は基本日本語しかわかりません。何かあったら日本語で問い合わせてください。
* このソースコードの利用、ならびにこのソースコードやこれを元に作成したソースコードから作成したプログラムの利用によりいかなる損害や問題がおきても著作者（hakua-doublemoon）は一切の責任を負いません。


