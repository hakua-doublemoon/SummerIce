import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    //============================================================
    // <HEAD>
    //============================================================
    property var wheelList: []
    property int selecting: 100
    property int listMax: wheelList.length-1
    property int listMin: 0

    // private/ReadOnly
    implicitHeight: _knocker_rect.height

    //============================================================
    // <BODY>
    //============================================================
    Rectangle {
        height: _voice_title.height //+ text_h_margin
        width:  _toggles.wid //+ voice_name_box_added_width
        id: _knocker_rect
        color: "aliceblue"

        Image {
            id: _left_mark
            source: "qrc:/images/arrow.png"
            height: parent.height-2
            width:  10
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.verticalCenter: parent.verticalCenter
        }
        Image {
            id: _right_mark
            source: "qrc:/images/arrow.png"
            height: parent.height-2
            width:  10
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            mirror: true
        }

        MouseArea {
            anchors.fill: parent
            //scrollGestureEnabled: true
            property int x_start: 0
            property int x_end: 0
            hoverEnabled: false
            onWheel: {
                isFirstSetting = true;
                proc_onWheel(wheel.angleDelta.x);
            }
            onPressed: {
                x_end = x_start = mouseX;
                //console.log("start[1] = " + y_start);
            }
            onReleased: {
                x_end = mouseX;
                isFirstSetting = true;
                proc_onWheel(x_start-x_end);
                x_start = x_end;
                //console.log("end[1]   = " + y_end);
            }
        }
        Text {
            id: _voice_title
            anchors.top: parent.top
            //anchors.topMargin: text_h_margin/2
            anchors.left: _left_mark.right
            anchors.leftMargin: font.pixelSize/2
            font.pixelSize: 20
            text: wheelList[selecting]
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
            if ((listMax+1) === local_slct) {
                selecting = listMin;
                return;
            }
            selecting = local_slct;
        }
    }

    onSelectingChanged: {
        console.log("onSelectingChanged");
    }
}
