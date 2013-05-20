#include <QString>
#include <QtNetwork>
#include <QtCore/QCoreApplication>


#include "qtilestream.h"

void help() {
    qDebug() << "usage: qtilestream [--port=N] [--not-found-image=file] [--mbtiles=files]";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int port=80;
    QStringList mbtiles;
    QString notFoundImagePath;
    QStringList args = app.arguments();

    if (args.size() == 1) {
        help();
        return -1;
    }

    for (int i = 0; i < args.size(); ++i) {
        QString arg = args.at(i);
        if (arg == "--help") {
            help();
            return 0;
        }
        int pos = arg.indexOf("=") + 1;
        if (pos > 0) {
            if (arg.startsWith("--port=")) {
                port = arg.mid(pos).toInt();
                continue;
            }
            if (arg.startsWith("--mbtiles=")) {
                mbtiles = arg.mid(pos).split(",", QString::SkipEmptyParts);
                continue;
            }
            if (arg.startsWith("--not-found-image=")) {
                notFoundImagePath = arg.mid(pos);
                continue;
            }
        }
    }

    if (mbtiles.isEmpty()) {
        qDebug() << "MBTiles is not set. Please set mbtiles with --mbtiles option";
        return -1;
    }
    else {
        QTileStream *server;
        if (!notFoundImagePath.isEmpty()) {
            QFile notFoundImage(notFoundImagePath);
            if (notFoundImage.open(QIODevice::ReadOnly)) {
                QByteArray notFoundImageData = notFoundImage.readAll();
                notFoundImage.close();
                qDebug() << "Not found image is set to" << notFoundImagePath;
                server = new QTileStream(mbtiles, notFoundImageData);
            }
            else {
                qDebug() << "Can not open" << notFoundImagePath;
                server = new QTileStream(mbtiles);
            }
        }
        else {
            server = new QTileStream(mbtiles);
        }

        if (!server->listen(QHostAddress::Any, port)) {
            qDebug() << "Unable to start the server:" << server->errorString();
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
