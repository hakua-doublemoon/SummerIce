import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    //============================================================
    // <HEAD>
    //============================================================

    //============================================================
    // <BODY>
    //============================================================
    Rectangle {
        height: _pongLabel.height + text_h_margin
        width:  _toggles.wid
        id: _alerm_switch
        color: "#f0f8ff"
        Image {
            source: "qrc:/images/botton_plane.png"
            anchors.fill: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("onclicked->ping");
                _pongLabel.text = Ice.setAlerm(_toggles.public_hour, _toggles.public_min);
                Sky.ping2server("hoge:" + _toggles.public_hour + ":" + _toggles.public_min);
                pingStatus = def_Send;
                //_alerm_voice.play();
                //Ice.rangAlerm();
                //Star.playFile(alerm_name)
                _toggles.stop_privent();
            }
        }
        Text {
            id: _pongLabel
            anchors.horizontalCenter: _alerm_switch.horizontalCenter
            anchors.top: _alerm_switch.top
            anchors.topMargin: text_h_margin/2
            font.pixelSize: 20
            text: "    tap here!    "
        }
    } // _alerm_switch
    Rectangle {
        id: _status_sq
        anchors.top: _alerm_switch.bottom
        anchors.left: _alerm_switch.left
        height: 2
        width: _alerm_switch.width
        color: {
            switch (pingStatus) {
            case def_Recv:
                "transparent";
                break;
            case def_Send:
                "red";
                break;
            default:
                "aquamarine";
                break;
            }
        }
    } // _status_sq
}
