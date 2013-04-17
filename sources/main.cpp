#include <QApplication>
#include "PongServer.hpp"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    PongServer server;
    server.start();
    server.showScene();

    return app.exec();
}
