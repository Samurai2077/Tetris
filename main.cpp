#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <gameengine.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    GameEngine game_engine;
    engine.rootContext()->setContextProperty("game_engine", &game_engine);

    engine.load(url);

    return app.exec();
}
