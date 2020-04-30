import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    id: tools

    // Изменить счёт(надпись)
    function changeScore(scr) {
        if( scr === -1 )
            return
        else
            score.text = score.text * 1 + scr
    }

    // Сбросить счёт(надпись)
    function resetScore() {
        score.text = 0
    }

    // Изменить уровень(надпись)
    function changeLevel(lvl) {
        level.text = lvl
    }

    // Изменить след фигуру(табло)
    function changeNextFigure() {
        var array = game_engine.getNextFigure()
        for( var i = 0; i < array.length; i++ ) {
            if( array[i] === 0)
                next_figure.itemAt(i).setColor("black");
            else
                next_figure.itemAt(i).setColor("red");
        }
    }

    function getScore() {
        return score.text
    }

    // Вызвать новую игру
    Button {
        text: "Новая игра"
        font.pointSize: 25
        Layout.alignment  : Qt.AlignCenter

        onClicked: gf.newGame()
    }

    Text {
        text: "Score"
        font.pointSize: 15
        Layout.alignment  : Qt.AlignCenter

        Layout.fillWidth   : true
        verticalAlignment  : Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // Текущий счёт
    Text {
        id: score
        Layout.alignment  : Qt.AlignCenter

        text: "0"
        font.pointSize: 15

        Layout.fillWidth   : true
        verticalAlignment  : Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // Нужен лишь для пустого пространства
    Rectangle {
        height: 50
    }

    Text {
        text: "LVL"
        font.pointSize: 15
        Layout.alignment  : Qt.AlignCenter

        Layout.fillWidth   : true
        verticalAlignment  : Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // Текущий уровень
    Text {
        id: level
        Layout.alignment  : Qt.AlignCenter

        text: "1"
        font.pointSize: 15

        Layout.fillWidth   : true
        verticalAlignment  : Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    // След фигура
    GridLayout {
        Layout.alignment  : Qt.AlignHCenter

        columns: 4
        rows   : 4

        rowSpacing   : 1
        columnSpacing: 1

        Repeater {
            id: next_figure
            model: 16
            Tile {}
        }
    }
}
