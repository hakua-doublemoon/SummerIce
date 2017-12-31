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
    property string pict_path: "file://" + voice_resource + pict_name
    property real   ring_interval: (_voice_list.wheelList[_voice_list.selecting].length) + 3.0

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

    // 時間設定域
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
        _a_toggles: _toggles
        wheelList: voice_list.character
        selecting: voice_list.selectChara
        listMin: 0
        listMax: voice_list.character.length-1
        onSelectingChanged: {
            _voice_list.set_max_min(wheelList[selecting]);
            Ice.update_voice_selectChara(selecting);
        }
    }
    Parts_scroll_list {
        id: _voice_list
        anchors.left: _chara_list.left
        anchors.top:  _chara_list.bottom
        //x:  _toggles.x - voice_name_box_added_width/2
        //y:  400
        _a_toggles: _toggles
        wheelList: voice_list.list
        selecting: voice_list.selecting
        onSelectingChanged: {
            Ice.update_voice_select(selecting);
        }
    }

    // 設定／おめざまでの時間表示域
    Parts_knocker {
        id: _knocker
        x:  _toggles.x
        y:  550
        _a_toggles: _toggles
    }

    // めざまし中の画像
    Parts_wakeUpGirls {
        id: _gImg
        anchors.fill: parent
        wakeup_img: pict_path
        visible: false //_toggles.ringing_flag
        onWaked: {
            pingStatus = def_NoSnd;
            Sky.ping2server("hoge:99:99");
            _toggles.stopAlerm();
        }
    }
    
    // 再生機能。とりあえずここに
    //Audio {
    //    id: _alerm_voice
    //    volume: 1.0
    //    source: "qrc:/voices/sayaka_01.mp3"
    //}

    // pingしたときのあれ。多分いらないけど残してある
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
        console.log("SummerIce::onComplete");
        Sky.ping2server("init");
        //Ice.set_window_flag();
    }
}
