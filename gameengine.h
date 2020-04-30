#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QWidget>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class GameEngine: public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(QObject *parent = nullptr);

private:
    QVector<int> Field;  // Два игровых поля, в функции не могут меняться
    QVector<int> Rezerv; // одновременно. Если какое-то условие проваливается, то
                         // одно из полей сохраняет прошлую позицию, что позволяет
                         // легко откатиться назад

    int result;          // Показывает полученное кол-во очков в этом ходу,
                         // если -1, то игра проиграна(обратывается в GameField.qml

    int current_figure;  // Текущая фигура

    int current_status;  // Поворот текущей фигуры(0 - 3)

    int figure_center;   // Позиция центральной клетки фигуры, используется в turned()

    int next_figure;     // Следующая фигура(на поле её нет, создается при фиксации на поле текущей фигуры
                         // функцией figureStop() )

    int next_turned;     // Поворот следующей фигуры

    void figureStop();   // Остановка текущей фигуры(пользователь теряет возможность управлять ей)

    void checkLine();    // Проверка на собранные линии и подсчёт очков

    int current_color = 2;

public slots:
    // Получение данных текущего игрового поля
    QVector<int> getField();      // Получить разметку поля
    QVector<int> getNextFigure(); // Получить следующую фигуру

    // Преобразования игрового поля
    void generateFigure(); // Создаёт новую фигуру
    void newGame();        // Очищает поле и сбрасывает переменные
    QVector<QString> gameOver(int);       // Завершает игру

    // Взаимодействие с пользователем
    int  moveDown();  // Движение фигуры вниз, может вызываться пользователем
                      // а также таймером

    void moveLeft();  // Движение фигуры влево, вызывается только пользователем

    void moveRight(); // Движение фигуры вправо, вызывается только пользователем

    void turned();    // Поворот фигуры, вызывается при создании фигуры и пользователем
};

#endif // GAMEENGINE_H
