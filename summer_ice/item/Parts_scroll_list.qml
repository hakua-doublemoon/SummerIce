import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    //============================================================
    // <HEAD>
    //============================================================
    property var _a_toggles: null
    property var wheelList: []
    property int selecting: 100
    property int listMax: wheelList.length-1
    property int listMin: 0

    //============================================================
    // <BODY>
    //============================================================
    Rectangle {
        height: _voice_title.height + text_h_margin
        width:  _a_toggles.wid + voice_name_box_added_width
        id: _knocker_rect
        color: "#f0f8ff"

        Image {
            source: "qrc:/images/wheel_area.png"
            anchors.fill: parent
        }
        MouseArea {
            anchors.fill: parent
            //scrollGestureEnabled: true
            property int y_start: 0
            property int y_end: 0
            hoverEnabled: false
            onWheel: {
                proc_onWheel(wheel.angleDelta.y);
            }
            onPressed: {
                y_end = y_start = mouseY;
                //console.log("start[1] = " + y_start);
            }
            //onEntered: {
            //    y_end = y_start = mouseY;
            //    console.log("start[2] = " + y_start);
            //}
            onReleased: {
                y_end = mouseY;
                proc_onWheel(y_start-y_end);
                y_start = y_end;
                //console.log("end[1]   = " + y_end);
            }
            //onExited: {
            //    y_end = mouseY;
            //    proc_onWheel(y_start-y_end);
            //    y_start = y_end;
            //    console.log("end[2]   = " + y_end);
            //}
        }
        Text {
            id: _voice_title
            anchors.top: parent.top
            anchors.topMargin: text_h_margin/2
            anchors.left: parent.left
            anchors.leftMargin: font.pixelSize/2
            font.pixelSize: 20
            text: wheelList[selecting].name
        }
    }

    //============================================================
    // <SCRIPT>
    //============================================================
    function proc_onWheel(delta) {
        if (delta > 0) {
            console.log("SummerIce::onWheel/upper < " + delta);
            if (selecting <= listMin) {
                selecting = listMax;
                return;
            }
            selecting = selecting - 1;
        } else {
            console.log("SummerIce::onWheel/down < " + delta);
            var local_slct = selecting + 1;
            if (local_slct > listMax) {
                selecting = listMin;
                return;
            }
            selecting = local_slct;
        }
    }

    onSelectingChanged: {
        console.log("onSelectingChanged");
    }

    function set_max_min(name)
    {
        var i;
        // まず先頭を取得する
        listMin = 0;
        for (i=0; i < wheelList.length-1; i+=1) {
            if (wheelList[i].name.indexOf(name) !== -1) {
                listMin = i;
                break;
            }
        }
        // うしろを取得する
        listMax = wheelList.length-1;
        for (i=listMax; i >= 0; i-=1) {
            if (wheelList[i].name.indexOf(name) !== -1) {
                listMax = i;
                break;
            }
        }
        // 選択範囲外だったら適当な場所にする
        if (selecting < listMin && selecting > listMax) {
            //selecting = listMin;
        }
    }
}
