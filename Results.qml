import QtQuick 2.0
import QtQuick.Layouts 1.12

ColumnLayout {
    id: root
    property int w: 0

    function show_result() {
        if (tm.running === true)
            return

        visible = !active
        if ( state === "inactive" ) {
            state = "active"
            line.color = "black"
            tm.start()
        } else if ( state === "active" ) {
            state = "inactive"
            line.color = "white"
            tm.start()
        }
    }

    state: "inactive"
    visible: false
    Layout.minimumWidth: 0
    Layout.maximumWidth: 0
    function changeResult(res) {
        for( var i = 0; i < res.length; i++ ) {
            str.itemAt(i).text = res[i]
        }
    }

    Text {
        Layout.fillWidth: true

        text: "Best result:"
        font.pointSize: 25
        verticalAlignment  : Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    states: [
        State {
            name: "active"
            PropertyChanges {
                target: root
                w: 300
                visible: true
                Layout.minimumWidth: 300
                Layout.maximumWidth: 300
            }
        },

        State {
            name: "inactive"
            PropertyChanges {
                target: root
                w: 0
                visible: false
                Layout.minimumWidth: 0
                Layout.maximumWidth: 0
            }
        }
    ]

    Repeater {
        id: str
        model: 10

        Text{
            Layout.fillWidth: true
            font.pointSize: 25
            text: "*************"
            verticalAlignment  : Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Timer {
        id: tm
        interval: 1999
        repeat: false
        running: false
    }

    Behavior on w
    {
        NumberAnimation { duration: 2000
            easing.type: "InOutQuad"
        }
    }
}
