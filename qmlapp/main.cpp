#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDirIterator>
#include <QDebug>
#include <QtQuick/QQuickWindow>

#include "sculptview.h"

int main(int argc, char *argv[])
{
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext())
        qDebug() << it.next();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<SculptView>("Dilay", 1, 0, "SculptView");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    QObject *root = engine.rootObjects()[0];

    QQuickWindow *window = qobject_cast<QQuickWindow *>(root);
    if (!window) {
        qFatal("Error: Your root item has to be a window.");
        return -1;
    }
    return app.exec();
}
