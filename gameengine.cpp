#include "gameengine.h"
#include <random>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QPair>
#include <QString>
#include <algorithm>
#include <QMessageBox>

// Инициализация необходимых данных
GameEngine::GameEngine(QObject *parent) : QObject(parent)
{
    Field.resize(29 * 10);
    Field.fill(0);

    Rezerv.resize(29 * 10);
    Rezerv.fill(0);          // Создаём два игровых поля

    result         = 0;
    current_figure = 0;
    current_status = 0;      // Это можно и не выставлять
    next_figure    = 0;      // на самом деле

    std::srand(std::time(0)); // Для рандома
}

// Создание новой игры
void GameEngine::newGame() {
    Field.fill(0);
    Rezerv.fill(0); // Очистка полей

    next_figure = rand() % 7;
    next_turned = rand() % 4;
    generateFigure();         // Первая фигура
}

// Отправляет конфигурацию поля
QVector<int> GameEngine::getField() {
    QVector<int> f = Field;
    for( int i = 0; i < f.size(); i++ ) {
        if( f[i] == 2 )
            f[i] = current_color;
    }
    return f;
}

// Генерирует фигуру
void GameEngine::generateFigure() {
    // Если фигуры зашли за видимую игроком верхнюю линию, то игра окончена
    for( int i = 40; i < 50; i++ ) {
        if( Field[i] != 0 ) {
            result = -1;
            return;
        }
    }

    switch ( next_figure ) {
        case 0: // палка
            Field[5]  = 2;
            Field[15] = 2;
            Field[25] = 2;
            Field[35] = 2;

            current_figure =  0;
            figure_center  = 25;
            current_status =  0;
        break;

        case 1: // Г влево
            Field[14]  = 2;
            Field[15]  = 2;
            Field[25] = 2;
            Field[35] = 2;

            current_figure =  1;
            figure_center  = 25;
            current_status =  0;
        break;

        case 2: // Г вправо
            Field[15]  = 2;
            Field[16]  = 2;
            Field[25] = 2;
            Field[35] = 2;

            current_figure =  2;
            figure_center  = 25;
            current_status =  0;
        break;

        case 3: // Пирамидка
            Field[15] = 2;
            Field[24] = 2;
            Field[25] = 2;
            Field[26] = 2;

            current_figure =  3;
            figure_center  = 25;
            current_status =  0;
        break;

        case 4: // Молния влево
            Field[14]  = 2;
            Field[15]  = 2;
            Field[25] = 2;
            Field[26] = 2;

            current_figure =  4;
            figure_center  = 25;
            current_status =  0;
        break;

        case 5: // Молния вправо
            Field[15] = 2;
            Field[16] = 2;
            Field[24] = 2;
            Field[25] = 2;

            current_figure =  5;
            figure_center  = 25;
            current_status =  0;
        break;

        case 6: // Квадрат
            Field[25] = 2;
            Field[26] = 2;
            Field[35] = 2;
            Field[36] = 2;

            current_figure =  6;
            figure_center  =  0;
            current_status =  0;
        break;
    }

    // Поворот фигуры
    for( int i = next_turned; i > 0; i-- ) {
        turned();
    }

    // Определение параметров для след фигуры
    next_figure = rand() % 7;
    next_turned = rand() % 4;
    current_color = rand() % 7 + 3;

    Rezerv = Field;
}

// Движение фигуры вниз
int GameEngine::moveDown() {
    result = 0;

    // Т.к известен центр фигуры, можно сузить круг поиска
    int lower_bound = qMax(-1, figure_center - 40);
    for( int i = qMin(Rezerv.size() - 1, figure_center + 40) ; i > lower_bound; i-- ) {
        if( Rezerv[i] == 2 ) {
            Rezerv[i] = 0;
            // Если под фигурой расположено дно или другая фигура, то
            // фиксируем фигуру на этом месте
            if( i + 10 >= Rezerv.size() || Rezerv[i + 10] != 0 ) {
                figureStop();
                return result;
            }
            Rezerv[i + 10] = 2;
        }
    }

    // Если удалось выйти из массива, значит под текущей фигурой есть
    // место для её сдвига вниз
    figure_center += 10; // Центр движется в направлении фигуры
    Field = Rezerv;

    return result;
}

// Движение фигуры вправо
void GameEngine::moveRight() {
    // Т.к известен центр фигуры, можно сузить круг поиска
    int lower_bound = qMax(-1, figure_center - 40);
    for( int i = qMin(Field.size() - 1, figure_center + 40); i > lower_bound; i-- ) {
        if( Field[i] == 2 ) {
            Field[i] = 0;
            // Если справа от фигуры расположена стена или другая фигура, то
            // сбрасываем изменения и ничего не делаем
            if( i % 10 == 9 || Field[i + 1] != 0 ) {
                Field = Rezerv;
                return;
            }
            Field[i + 1] = 2;
        }
    }

    // Если удалось выйти из массива, значит справа от текущей фигурой есть
    // место для её сдвига
    figure_center += 1; // Центр движется в направлении фигуры
    Rezerv = Field;
}

// Движение фигуры влево
void GameEngine::moveLeft() {
    // Т.к известен центр фигуры, можно сузить круг поиска
    int upper_bound = qMin(Field.size(), figure_center + 40);
    for( int i = qMax(0, figure_center - 40); i < upper_bound; i++ ) {
        if( Field[i] == 2 ) {
            Field[i] = 0;
            // Если слева от фигуры расположена стена или другая фигура, то
            // сбрасываем изменения и ничего не делаем
            if( i % 10 == 0 || Field[i - 1] != 0 ) {
                Field = Rezerv;
                return;
            }
            Field[i - 1] = 2;
        }
    }

    // Если удалось выйти из массива, значит слева от текущей фигурой есть
    // место для её сдвига
    figure_center -= 1; // Центр движется в направлении фигуры
    Rezerv = Field;
}

// Фиксация фигуры на месте
void GameEngine::figureStop() {
    // Активными считаются клетки со значением 2, сбрасываем их на 1
    for( int i = 0; i < Field.size(); i++ ) {
        if( Field[i] == 2 )
            Field[i] = current_color;
    }

    Rezerv = Field; // Сохраняем изменения

    checkLine();      // Проверяем, не собрались ли линии
    generateFigure(); // И генерируем следующую фигуру
}

// Проверка линий и подсчёт очков
void GameEngine::checkLine() {
    int count_line = 0; // Кол-во сбритых линий
    int score = 0;      // Кол-во очков в этот ход

    // Первый цикл кол-во десятков в номере ячейки
    for( int i = 4; i < Field.size() / 10; i++ ) {
        bool success = true;
        // Второй цикл кол-во единиц в номере ячефки
        for( int x = i * 10; x < (i + 1) * 10; x++ ) {
            // Если в строке найдена нулевая ячейка, то пропускаем её
            if( Field[x] == 0 ) {
                success = false;
                break;
            }
        }

        // Если нулей нет, обнуляем строку
        if(!success)
            continue;
        else
            count_line++;

        for( int x = (i - 1) * 10 + 9; x > -1; x-- ) {
            Field[x + 10] = Field[x];
        }

        Rezerv = Field; // И сохраняем изменения
    }

    // Выполняем подсчёт очков
    for( int i = 0; i < count_line; i++ ) {
        score += score * 1.5 + 10;
    }

    result += score;
}

// Поворот фигуры
void GameEngine::turned() {
    // В зависимости от текущего поворота и фигуры, делаем проверку
    // на возможность поворота, и если возможно, поворачиваем
    switch (current_figure) {
        case 0: // Палка
            if(current_status % 2 == 0) {
                if( figure_center % 10 > 7 || figure_center % 10 == 0 ||
                        Field[figure_center + 1] != 0 || Field[figure_center + 2] != 0 ||
                            Field[figure_center - 1] != 0 )
                    return;
                Field[figure_center - 20] = 0;
                Field[figure_center - 10] = 0;
                Field[figure_center + 10] = 0;

                Field[figure_center - 1] = 2;
                Field[figure_center + 1] = 2;
                Field[figure_center + 2] = 2;

                current_status++;
            } else {
                if( figure_center + 10 >= Field.size() || Field[figure_center - 10] != 0 ||
                        Field[figure_center - 20] != 0 || Field[figure_center + 10] != 0 )
                    return;
                Field[figure_center - 1] = 0;
                Field[figure_center + 1] = 0;
                Field[figure_center + 2] = 0;

                Field[figure_center - 20] = 2;
                Field[figure_center - 10] = 2;
                Field[figure_center + 10] = 2;

                current_status = 0;
            }
        break;

        case 1: // Г вправо
            if(current_status == 0) {
                if( figure_center % 10 == 9 || Field[figure_center + 1] != 0 ||
                        Field[figure_center - 9] != 0 || Field[figure_center - 1] != 0 )
                    return;

                Field[figure_center - 10] = 0;
                Field[figure_center - 11] = 0;
                Field[figure_center + 10] = 0;

                Field[figure_center + 1] = 2;
                Field[figure_center - 9] = 2;
                Field[figure_center - 1] = 2;

                current_status++;
            } else if(current_status == 1) {
                if( figure_center + 10 >= Field.size() || Field[figure_center - 10] != 0 ||
                        Field[figure_center + 10] != 0 || Field[figure_center + 11] != 0 )
                    return;

                Field[figure_center + 1] = 0;
                Field[figure_center - 9] = 0;
                Field[figure_center - 1] = 0;

                Field[figure_center - 10] = 2;
                Field[figure_center + 10] = 2;
                Field[figure_center + 11] = 2;

                current_status++;
            } else if(current_status == 2) {
                if( figure_center % 10 == 0 || Field[figure_center + 1] != 0 ||
                        Field[figure_center - 1] != 0 || Field[figure_center + 9] != 0 )
                    return;

                Field[figure_center - 10] = 0;
                Field[figure_center + 10] = 0;
                Field[figure_center + 11] = 0;

                Field[figure_center + 1] = 2;
                Field[figure_center - 1] = 2;
                Field[figure_center + 9] = 2;

                current_status++;
            } else {
            if( figure_center + 10 >= Field.size() || Field[figure_center - 10] != 0 ||
                    Field[figure_center - 11] != 0 || Field[figure_center + 10] != 0 )
                return;

            Field[figure_center + 1] = 0;
            Field[figure_center - 1] = 0;
            Field[figure_center + 9] = 0;

            Field[figure_center + 10] = 2;
            Field[figure_center - 10] = 2;
            Field[figure_center - 11] = 2;

            current_status = 0;
        }
        break;

        case 2: // Г влево
            if(current_status == 0) {
                if( figure_center % 10 == 0 || Field[figure_center + 1] != 0 ||
                        Field[figure_center + 11] != 0 || Field[figure_center - 1] != 0 )
                    return;

                Field[figure_center - 10] = 0;
                Field[figure_center - 9 ] = 0;
                Field[figure_center + 10] = 0;

                Field[figure_center - 1 ] = 2;
                Field[figure_center + 1 ] = 2;
                Field[figure_center + 11] = 2;

                current_status++;
            } else if(current_status == 1) {
                if( Field[figure_center + 10] != 0 ||
                        Field[figure_center + 9] != 0 || Field[figure_center - 10] != 0 )
                    return;

                Field[figure_center + 1 ] = 0;
                Field[figure_center + 11] = 0;
                Field[figure_center - 1 ] = 0;

                Field[figure_center - 10] = 2;
                Field[figure_center + 9 ] = 2;
                Field[figure_center + 10] = 2;

                current_status++;
            } else if(current_status == 2) {
                if( figure_center % 10 == 9 || Field[figure_center - 1] != 0 ||
                        Field[figure_center - 11] != 0 || Field[figure_center + 1] != 0 )
                    return;

                Field[figure_center - 10] = 0;
                Field[figure_center + 9 ] = 0;
                Field[figure_center + 10] = 0;

                Field[figure_center - 11] = 2;
                Field[figure_center - 1 ] = 2;
                Field[figure_center + 1 ] = 2;

                current_status++;
            } else {
                if( figure_center + 10 >= Field.size() || Field[figure_center - 10] != 0 ||
                        Field[figure_center - 9] != 0 || Field[figure_center + 10] != 0 )
                    return;

                Field[figure_center - 11] = 0;
                Field[figure_center - 1 ] = 0;
                Field[figure_center + 1 ] = 0;

                Field[figure_center - 10] = 2;
                Field[figure_center - 9 ] = 2;
                Field[figure_center + 10] = 2;

                current_status = 0;
            }
        break;

        case 3: // Пирамидка
            if(current_status == 0) {
                if( figure_center >= Field.size() || Field[figure_center + 10] != 0)
                    return;
                Field[figure_center - 1]  = 0;
                Field[figure_center + 10] = 2;
                current_status++;
            } else if(current_status == 1) {
                if( figure_center % 10 == 0 || Field[figure_center - 1] != 0)
                    return;
                Field[figure_center - 10] = 0;
                Field[figure_center - 1]  = 2;
                current_status++;
            } else if(current_status == 2) {
                if( figure_center - 10 < 0 || Field[figure_center - 10] != 0)
                    return;
                Field[figure_center + 1]  = 0;
                Field[figure_center - 10] = 2;
                current_status++;
            } else {
                if( figure_center % 10 == 9 || Field[figure_center + 1] != 0)
                    return;
                Field[figure_center + 10] = 0;
                Field[figure_center + 1]  = 2;
                current_status = 0;
            }
        break;

        case 4: // Молния влево
            if(current_status % 2 == 0) {
                if( figure_center + 10 >= Field.size() || Field[figure_center - 9] != 0 ||
                        Field[figure_center + 10] )
                    return;
                Field[figure_center - 10] = 0;
                Field[figure_center - 11] = 0;

                Field[figure_center - 9 ] = 2;
                Field[figure_center + 10] = 2;

                current_status++;
            } else {
                if( figure_center % 10 == 0 || Field[figure_center - 1] != 0)
                    return;
                Field[figure_center - 9 ] = 0;
                Field[figure_center + 10] = 0;

                Field[figure_center - 10] = 2;
                Field[figure_center - 11] = 2;

                current_status = 0;
            }
        break;

        case 5: // Молния вправо
            if(current_status % 2 == 0) {
                if( figure_center + 10 >= Field.size() || Field[figure_center + 1] != 0 ||
                        Field[figure_center + 11] )
                    return;
                Field[figure_center - 9] = 0;
                Field[figure_center - 1] = 0;

                Field[figure_center + 1 ] = 2;
                Field[figure_center + 11] = 2;

                current_status++;
            } else {
                if( figure_center % 10 == 0 || Field[figure_center - 1] != 0)
                    return;
                Field[figure_center + 1 ] = 0;
                Field[figure_center + 11] = 0;

                Field[figure_center - 9] = 2;
                Field[figure_center - 1] = 2;

                current_status = 0;
            }
        break;

        case 6: // Квадрат(поворот не меняет фигуру)
        break;
    }

    Rezerv = Field;
}

// Отправляет конфигурацию следующей фигуры
QVector<int> GameEngine::getNextFigure() {
    QVector<int> nextFig(16);
    nextFig.fill(0);

    // В зависимости от текущего поворота и фигуры, отправляет
    // требуемый массив конфигурации. Если игра проиграна,
    // отправляет пустой массив
    switch (next_figure) {
        case 0: // Палка
            if(next_turned % 2 == 0) {
                nextFig[1]  = 1;
                nextFig[5]  = 1;
                nextFig[9]  = 1;
                nextFig[13] = 1;
            } else {
                nextFig[4] = 1;
                nextFig[5] = 1;
                nextFig[6] = 1;
                nextFig[7] = 1;
            }
        break;

        case 1: // Г влево
            if(next_turned == 0) {
                nextFig[1]  = 1;
                nextFig[2]  = 1;
                nextFig[6]  = 1;
                nextFig[10] = 1;
            } else if(next_turned == 1) {
                nextFig[6] = 1;
                nextFig[8] = 1;
                nextFig[9] = 1;
                nextFig[10] = 1;
            } else if(next_turned == 2) {
                nextFig[1] = 1;
                nextFig[5] = 1;
                nextFig[9] = 1;
                nextFig[10] = 1;
            } else {
                nextFig[4] = 1;
                nextFig[5] = 1;
                nextFig[6] = 1;
                nextFig[8] = 1;
            }
        break;

        case 2: // Г вправо
            if(next_turned == 0) {
                nextFig[3]  = 1;
                nextFig[2]  = 1;
                nextFig[6]  = 1;
                nextFig[10] = 1;
            } else if(next_turned == 1) {
                nextFig[10] = 1;
                nextFig[4] = 1;
                nextFig[5] = 1;
                nextFig[6] = 1;
            } else if(next_turned == 2) {
                nextFig[1] = 1;
                nextFig[5] = 1;
                nextFig[9] = 1;
                nextFig[8] = 1;
            } else {
                nextFig[4] = 1;
                nextFig[5] = 1;
                nextFig[6] = 1;
                nextFig[0] = 1;
            }
        break;

        case 3: // Пирамидка
            if(next_turned == 0) {
                nextFig[6]  = 1;
                nextFig[9]  = 1;
                nextFig[10]  = 1;
                nextFig[11] = 1;
            } else if(next_turned == 1) {
                nextFig[1] = 1;
                nextFig[5] = 1;
                nextFig[9] = 1;
                nextFig[6] = 1;
            } else if(next_turned == 2) {
                nextFig[4] = 1;
                nextFig[5] = 1;
                nextFig[6] = 1;
                nextFig[9] = 1;
            } else {
                nextFig[2] = 1;
                nextFig[6] = 1;
                nextFig[10] = 1;
                nextFig[5] = 1;
            }
        break;

        case 4: // Молния влево
            if(next_turned % 2 == 0) {
                nextFig[4]  = 1;
                nextFig[5]  = 1;
                nextFig[9]  = 1;
                nextFig[10] = 1;
            } else {
                nextFig[2] = 1;
                nextFig[6] = 1;
                nextFig[5] = 1;
                nextFig[9] = 1;
            }
        break;

        case 5: // Молния вправо
            if(next_turned % 2 == 0) {
                nextFig[6]  = 1;
                nextFig[7]  = 1;
                nextFig[9]  = 1;
                nextFig[10] = 1;
            } else {
                nextFig[1] = 1;
                nextFig[5] = 1;
                nextFig[6] = 1;
                nextFig[10] = 1;
            }
        break;

        case 6: // Квадрат
            nextFig[5] = 1;
            nextFig[6] = 1;
            nextFig[9] = 1;
            nextFig[10] = 1;
        break;
    }

    return nextFig;
}

// Конец Игры
QVector<QString> GameEngine::gameOver(int scr) {
    QVector<QString> ret;
    QFile file("results");

    if( !file.open(QIODevice::ReadWrite) ) {
        return ret;
    }
    file.close();

    if( scr == -1 ) {
        if( !file.open(QIODevice::ReadOnly ) ) {
            return ret;
        }

        QTextStream in(&file);

        QString date;
        int i = 0;
        while(!in.atEnd()) {
            i++;
            in >> scr >> date;
            ret.push_back(QString::number(i, 10) + ". " + QString::number(scr, 10));
        }

        file.close();
        return ret;
    }

    next_figure = -1;
    next_turned = -1; // Выставляем данные значения для
                      // getNextFigure, для возвращения пустого
                      // массива

    // Создаем изображение поражения
    Field.fill(2);
    Field[111] = 1;
    Field[112] = 1;
    Field[113] = 1;
    Field[116] = 1;
    Field[117] = 1;
    Field[118] = 1;

    Field[121] = 1;
    Field[123] = 1;
    Field[126] = 1;
    Field[128] = 1;

    Field[131] = 1;
    Field[132] = 1;
    Field[133] = 1;
    Field[136] = 1;
    Field[137] = 1;
    Field[138] = 1;
    Field[158] = 1;
    Field[168] = 1;

    Field[173] = 1;
    Field[174] = 1;
    Field[175] = 1;
    Field[176] = 1;
    Field[182] = 1;
    Field[183] = 1;
    Field[187] = 1;
    Field[186] = 1;
    Field[191] = 1;
    Field[192] = 1;
    Field[197] = 1;
    Field[198] = 1;

    if( !file.open(QIODevice::ReadOnly ) ) {
        return ret;
    }

    QTextStream in(&file);

    QVector<QPair<int, QString>> res;
    QDate d = QDate::currentDate();
    res.push_back(qMakePair(scr, d.toString("d-MM-yyyy")));

    QString date = "";
    while(!in.atEnd()) {
        in >> scr >> date;
        res.push_back(qMakePair(scr, date));
    }

    file.close();

    std::sort(res.rbegin(), res.rend());

    if( !file.open(QIODevice::WriteOnly ) ) {
        return ret;
    }

    QTextStream out(&file);

    for( int i = 0; i < 10 && i < res.size(); i++ ) {
        out << res[i].first << " " << res[i].second << endl;
        ret.push_back(QString::number(i + 1, 10) + ". " + QString::number(res[i].first, 10));
    }

    file.close();

    return ret;
}
