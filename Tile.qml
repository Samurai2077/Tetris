import QtQuick 2.0

Rectangle {
    function setColor(clr) {
        g.color = clr
    }

    width : 26
    height: 26

    color: "black"

    Rectangle {
        id: g

        width : 23
        height: 23

        color: "black"
    }
}
