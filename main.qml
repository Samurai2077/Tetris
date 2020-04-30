import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    id: win
    visible: true
    title  : qsTr("Tetris")
    color: "white"

    minimumWidth : gf.width + tf.width + rs.w + 15
    maximumWidth : gf.width + tf.width + rs.w + 15
    minimumHeight: gf.height
    maximumHeight: gf.height

    RowLayout {
        GameField
        {
            id: gf
        }

        ToolsField
        {
            id: tf
        }

        Rectangle {
            id: line
            width: 1
            height: gf.height
            color: "white"
        }

        Results
        {
            id: rs
        }
    }
}
