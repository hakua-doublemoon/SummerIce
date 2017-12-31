import QtQuick 2.0

Item {
    id: _girls_root
    visible: false

    property string wakeup_img: "qrc:/images/sayaka.png"

    signal waked()

    Image {
        id: _girls
        height: parent.height
        fillMode: Image.PreserveAspectCrop
        source: wakeup_img
    } // _bgImg

    MouseArea {
        anchors.fill: parent
        onClicked: {
            waked();
            _girls_root.visible = false;
        }
    }
}
