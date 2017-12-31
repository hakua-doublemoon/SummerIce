
# SummerIceの説明

---

## 概要を手短に
Qtで作ったAndroid目覚ましもどきです。
詳しいことは後でQiitaに書きます。

<br>

## とりあえず開発・動作確認環境
#### Qt version  
5.9.1 (更新をさぼってる)

#### Android version
5.1.1 （Acerの安い端末）

#### Java version
1.8.0_131 (更新をさぼってる)

#### System dependencies
とくにないはず

#### Configuration  
音声と画像、それらを記載したvoice_list.jsonファイルはAndroidなど動作させる
環境の適当な場所に置き、そのパスをsummer_clock.cppのANDROID_VOICE_DIRに
書いてください。

#### Database creation
voice_list.json を適当に書いてください。
書き方は後日書きます。

#### Database initialization
voice_list.json を適当に書いてください。

#### Services (job queues, cache servers, search engines, etc.)  
めざましの時間のためにポーリングしているサービスがいます。
そんなにバッテリーは消費しないつもり。

#### Deployment instructions
このmyosotis_wikiのファイル群をサーバの適当な場所に配置するだけです。  

<br>

# その他
---
* 書いた人間は基本日本語しかわかりません。何かあったら日本語で問い合わせてください。  

