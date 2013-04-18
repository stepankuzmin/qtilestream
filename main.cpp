#include <QString>
#include <QtNetwork>
#include <QtCore/QCoreApplication>

#include "qtilestream.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString port;
    QStringList mbtiles;
    QStringList args = app.arguments();

    for (int i = 0; i < args.size(); ++i) {
        QString arg = args.at(i);
        int pos = arg.indexOf("=") + 1;
        if (pos > 0) {
            if (arg.startsWith("--port=")) {
                port = arg.mid(pos);
            }
            if (arg.startsWith("--mbtiles=")) {
                mbtiles = arg.mid(pos).split(",", QString::SkipEmptyParts);
            }
        }
    }

    if (port.isEmpty()) {
        port = "8080";
        qDebug() << "Port is not set. Using 8080 as default\n";
    }

    if (mbtiles.isEmpty()) {
        qDebug() << "MBTiles is not set. Please set mbtiles with --mbtiles option.\n"
                    "usage example: qtilestream --port=8080 --mbtiles=sverdlovsk.mbtiles,world.mbtiles";
        return -1;
    }
    else {
        QTileStream *server = new QTileStream(mbtiles);

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
