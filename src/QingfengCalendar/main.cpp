
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef _WIN32
    QApplication::setFont(QFont("Microsoft YaHei"));
#endif

    QQmlApplicationEngine engine;
    engine.addImportPath("../imports");
    engine.load(QUrl(QStringLiteral("qrc:/test_main.qml")));

    return app.exec();
}

