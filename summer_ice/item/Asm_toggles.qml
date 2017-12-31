import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "."

//ApplicationWindow {
Item {
    //============================================================
    // <HEAD>
    //============================================================
    property int public_hour: _hour_top.public_num*10 + _hour_und.public_num
    property int public_min:  _min_top.public_num *10 + _min_und.public_num
    property int wid: _toggles_body.width
    property alias ringingFlag: _ring_timer.running

    //============================================================
    // <BODY>
    //============================================================
    Row {
        //spacing: 0
        id: _toggles_body

        Parts_toggle {
            id: _hour_top
            public_is_arrow: true
            max_val: 2
            //onSigToggle: {}
        }
        Parts_toggle {
            id: _hour_und
            public_is_arrow: true
            max_val: (_hour_top.public_num===2 ? 4 : 9)
        }
        Parts_toggle {
            id: _time_space
            public_is_arrow: false

            onSigTogDbg: {
                Star.playFile(voice_path);
            }
        }
        Parts_toggle {
            id: _min_top
            public_is_arrow: true
            max_val: 5
        }
        Parts_toggle {
            id: _min_und
            public_is_arrow: true
            max_val: 9
        }
    }

    //============================================================
    // <TIMER>
    //============================================================
    // 時間かどうかチェックするタイマー
    Timer {
        id: _check_time
        interval: 10*1000
        running: true
        repeat: true
        onTriggered: {
            console.log("SummerIce/QML::_alerm / polling");
            var prop = Ice.readfile_alerm();
            if (!prop.isOK) {
                prop["hour"] = 0;
                prop["min"]  = 0;
            }
            if (Ice.chkAlerm(prop.hour, prop.min, 15)) {
                console.log("SummerIce/QML::_alerm / start:" + ring_interval);
                startAlerm(ring_interval);
            }
        }
    }
    // 鳴らすためのタイマー
    Timer {
        id: _ring_timer
        interval: 10*1000 // 鳴らすものによって変わる
        running: false
        repeat:  true
        onTriggered: {
            Star.playFile(voice_path);
        }
    }
    // 一度停めたら30秒は鳴らさないためのタイマー
    Timer {
        id: _prevent_timer
        interval: 30*1000
        running: false
        repeat:  false
        onTriggered: {
            console.debug("_prevent_timer is end.");
        }
    }

    //============================================================
    // <SCRIPT>
    //============================================================
    Component.onCompleted: {
        var prop = Ice.readfile_alerm();
        if (!prop.isOK) {
            prop["hour"] = 0;
            prop["min"]  = 0;
        }
        setAlermDisp(prop);
    }

    function setAlermDisp(_prop)
    {
        console.log("SummerIce::tgl.setAlermDisp <- " + _prop);
        _hour_top.public_num = _prop.hour / 10;
        _hour_und.public_num = _prop.hour % 10;
        _min_top.public_num = _prop.min / 10;
        _min_und.public_num = _prop.min % 10;
    }

    function stopAlerm()
    {
        _ring_timer.running = false;
        _prevent_timer.running = true;
    }
    function startAlerm()
    {
        if (_ring_timer.running) {
            return;
        }
        if (_prevent_timer.running) {
            return;
        }
        Star.playFile(voice_path);
        _ring_timer.interval = ring_interval * 1000;
        _ring_timer.running  = true;
        console.log("SummerIce::tgl.startAlerm <- " + ring_interval*1000);
    }
    function stop_privent()
    {
        _prevent_timer.stop();
    }
}
