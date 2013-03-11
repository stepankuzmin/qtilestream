#include <QString>
#include <QtNetwork>
#include <QtCore/QCoreApplication>

#include "qtilestream.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();
    if (args.count() != 3) {
        qDebug() << "usage: qtilestream [port] [file]";
        return -1;
    }
    else {
        QString port(argv[1]);
        QTileStream *server = new QTileStream(argv[2]);

        if (!server->listen(QHostAddress::Any, port.toInt())) {
            qDebug() << "unable to start the server:" << server->errorString();
            server->close();
            return -1;
        }

        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }

        if (ipAddress.isEmpty())
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

        qDebug() << "qtilestream is running on" << ipAddress << server->serverPort();
    }

    return app.exec();
}
