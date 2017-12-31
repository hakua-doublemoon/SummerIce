import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    //============================================================
    // <HEAD>
    //============================================================
    //内部プロパティ
    visible: true
    width:  _img_num_area.width
    height: _img_botton_up.height + _img_botton_down.height + _img_num_area.height
    readonly property int private_rect_size:   60
    readonly property int private_rect_mergin: 5
    
    //外部プロパティ
    property bool   public_is_arrow: true
    property int    public_num:      0
    property int    max_val: 9
    property int    min_val: 0

    signal sigToggle(int val);
    signal sigTogDbg();

    //============================================================
    // <BODY>
    //============================================================
    //上ボタン
    Image {
        id: _img_botton_up
        anchors.top: parent.top
        source: {
            if (public_is_arrow) {
                "qrc:/images/botton_up.png"
            } else {
                "qrc:/images/botton_plane.png"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (public_num < max_val) {
                    public_num = public_num + 1;
                    //_tx_num.text = public_d_value
                } else {
                    public_num = min_val;
                }
            }
        }
    }
    //数字域
    Image {
        id: _img_num_area
        anchors.top: _img_botton_up.bottom
        source: {
            if (public_is_arrow) {
                "qrc:/images/botton_rect.png"
            } else {
                "qrc:/images/botton_rect_t.png"
            }
        }
        Text {
            id: _tx_num
            //anchors.centerIn: parent.Center
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter :  parent.verticalCenter
            font.pixelSize: private_rect_size - private_rect_mergin
            text: (public_is_arrow ? public_num : ":")
        }
    }
    //下ボタン
    Image {
        id: _img_botton_down
        anchors.top: _img_num_area.bottom
        source: {
            if (public_is_arrow) {
                "qrc:/images/botton_down.png"
            } else {
                "qrc:/images/botton_plane.png"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (public_num > min_val) {
                    public_num = public_num - 1
                } else {
                    public_num = max_val;
                }
                if (!public_is_arrow) {
                    sigTogDbg();
                }
            }
        }
    }

    //============================================================
    // <SCRIPT>
    //============================================================
    onPublic_numChanged: {
        //sigToggle(public_num);
    }

}
