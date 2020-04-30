import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

GridLayout {
    property int fees: 2             // Сложность игры
    property bool game_active: false // Состояние игры

    id: root

    rows   : 25
    columns: 10

    rowSpacing    : 1
    columnSpacing : 1

    // Вывод поля на экран
    function show() {
        var _field = game_engine.getField() // Запрос поля
        for( var i = 40; i < _field.length; i++ ) {

            if( _field[i] === 0 )
                field.itemAt(i - 40).setColor("black");
            if( _field[i] === 1 )
                field.itemAt(i - 40).setColor("red");
            if( _field[i] === 2 )
                field.itemAt(i - 40).setColor("green");
            if( _field[i] === 3 )
                field.itemAt(i - 40).setColor("blue");
            if( _field[i] === 4 )
                field.itemAt(i - 40).setColor("darkmagenta");
            if( _field[i] === 5 )
                field.itemAt(i - 40).setColor("gold");
            if( _field[i] === 6 )
                field.itemAt(i - 40).setColor("cyan");
            if( _field[i] === 7 )
                field.itemAt(i - 40).setColor("magenta");
            if( _field[i] === 8 )
                field.itemAt(i - 40).setColor("green");
            if( _field[i] === 9 )
                field.itemAt(i - 40).setColor("red");
        }
    }

    // Движение фигуры вниз
    function move_down() {
        if( !game_active ) // Если игра не активна
            return
        var result = game_engine.moveDown(); // Отправляем запрос
        if( result === -1 ) // Игра проиграна
            gameOver();
        else if( result !== 0) { // Если были сбриты линии, то увел сложность
            fees++
            tf.changeNextFigure()
            tf.changeScore(result * 100 + result * fees);
            tf.changeLevel((fees - fees % 2) / 2)
            show();
        } else {
            show();
            tf.changeNextFigure()
            return;
        }
    }

    // Движение фигуры влево
    function move_left() {
        if( !game_active ) // Если игра неактивна
            return;
        game_engine.moveLeft();
        show();
    }

    // Движение фигуры вправо
    function move_right() {
        if( !game_active ) // Если игра неактивна
            return
        game_engine.moveRight();
        show();
    }

    // Поворот фигуры
    function turned() {
        if( !game_active ) // Если игра неактивна
            return
        game_engine.turned();
        show();
    }

    // Новая игра
    function newGame() {
        game_active = true // состояние игры - активно
        tf.resetScore()    // сброс счёта
        game_engine.newGame(); // Запрос новой игры
        mv.running = true  // Запуск таймера
        fees = 2           // сброс уровня
        tf.changeLevel(1)  // сброс табла уровня
        show();
    }

    // Конец игры
    function gameOver() {
        game_active = false // состояние игры - неактивно
        mv.running = false  // остановка таймера
        rs.changeResult(game_engine.gameOver(tf.getScore())) // запрос окончания игры
        show();
        tf.changeNextFigure() // очистка табла след фигуры
    }

    // Игровое поле
    Repeater {
        id: field

        model: 250
        Tile {

        }
    }

    // При срабатывании, фигура движется вниз
    Timer {
        id: mv

        interval: 1000 - (fees / 2 * 10)
        running : false
        repeat  : true

        onTriggered: move_down()
    }

    Timer {
        interval: 1
        repeat: false
        running: true

        onTriggered: rs.changeResult(game_engine.gameOver(-1))
    }

    Shortcut {
        sequence   : "Right"
        onActivated: move_right()
    }

    Shortcut {
        sequence   : "Left"
        onActivated: move_left()
    }

    Shortcut {
        sequence   : "Down"
        onActivated: move_down()
    }

    Shortcut {
        sequence   : "Up"
        onActivated: turned()
    }

    Shortcut {
        sequence   : "R"
        onActivated: newGame()
    }

    Shortcut {
        sequence   : "Escape"
        onActivated: Qt.quit()
    }

    Shortcut {
        sequence   : "T"
        onActivated: rs.show_result()
    }
}
