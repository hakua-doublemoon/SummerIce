import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

//import QtMultimedia 5.8

import "qrc:/item/"

//Rectangle {
ApplicationWindow {
    visible: true
    width: 480
    height: 854

    //適当な定数
    readonly property int text_v_margin: 10
    readonly property int text_h_margin: 10
    readonly property int textbox_offset_v: 100
    readonly property int voice_name_box_added_width: 50

    property string text_str_global: "Hello"
    property string voice_name: _voice_list.wheelList[_voice_list.selecting].name
    property string voice_path: voice_resource + voice_name
    property string pict_name: _voice_list.wheelList[_voice_list.selecting].picture
    //property string pict_path: "file://" + voice_resource + pict_name // ストレージのどこかのファイルを使う時はこっちのfile://をつける方にする
    property string pict_path: "qrc://" + voice_resource + pict_name
    property real   ring_interval: (_voice_list.wheelList[_voice_list.selecting].length) + 3.0
    property bool   isFirstSetting: false // 操作するとtrueになり設定を書き換える

    readonly property int def_Recv:  0
    readonly property int def_Send:  1
    readonly property int def_NoSnd: 2
    property int pingStatus: def_NoSnd

    // 背景
    // メモ：サイズの合わせ方とかは適当にやってください
    Image {
        id: _bgImg
        height: parent.height
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/images/sunrise-3048052_640.jpg"
    }

    // 時間設定域。タイマーもこいつが持ってる
    Asm_toggles {
        id: _toggles
        x: 230
        y: 200
        onRingingFlagChanged: {
            _gImg.visible = _toggles.ringingFlag;
        }
    }

    // ファイル選択
    Parts_hscroll_list {
        id: _chara_list
        x:  _toggles.x - voice_name_box_added_width/2
        y:  400
        wheelList: voice_list.character
        selecting: voice_list.selectChara
        listMin: 0
        listMax: voice_list.character.length-1
        onSelectingChanged: {
            if (isFirstSetting) {
                _voice_list.set_max_min(wheelList[selecting]);
                Ice.update_voice_selectChara(selecting);
            }
        }
    }
    Parts_scroll_list {
        id: _voice_list
        anchors.left: _chara_list.left
        anchors.top:  _chara_list.bottom
        //x:  _toggles.x - voice_name_box_added_width/2
        //y:  400
        wheelList: voice_list.list
        selecting: voice_list.selecting
        onSelectingChanged: {
            if (isFirstSetting) {
               Ice.update_voice_select(selecting);
            }
        }
    }

    // 設定／おめざまでの時間表示域
    Parts_knocker {
        id: _knocker
        x:  _toggles.x
        y:  550
    }

    // めざまし中の画像
    Parts_wakeUpGirls {
        id: _gImg
        anchors.fill: parent
        wakeup_img: pict_path
        visible: false //_toggles.ringing_flag
        onWaked: {
            pingStatus = def_NoSnd;
            Ice.set_alerm_monitor(99, 99);
            _toggles.stopAlerm();
        }
    }

    // pingしたときの応答
    Connections {
        target: ping_pong
        onPong: {
            console.log("SummerIce::main.onPong");
            if (pingStatus == def_Send) {
                pingStatus = def_Recv;
            }
        }
    }

    Component.onCompleted: {
        console.log("SummerIce::onComplete [s]");
        console.log("pict_path: " + pict_path);
        console.log("pict_name: " + pict_name);
        Ice.set_main_ready(true);
        _toggles.time_check(); // 一発チェックしておく
        console.log("SummerIce::onComplete [e]");
    }
    Component.onDestruction: {
        console.log("SummerIce::onDestruction [s]"); // ここは通らないっぽい
    }

    property int visibilityFlag: Qt.WindowMaximized
    onVisibilityFlagChanged: {
        console.log("SummerIce::onGivenVisibilityChanged = " + visibilityFlag);
        _toggles.time_check(); // 一発チェックする
        //visibility = visibilityFlag;
    }
}
