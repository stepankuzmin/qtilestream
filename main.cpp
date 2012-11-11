#include <QtCore/QCoreApplication>
#include <QNetworkInterface>
#include "tilestream.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TileStream *server = new TileStream(argv[1]);

    if (!server->listen(QHostAddress::Any, 8080)) {
        qDebug() << "Unable to start the server:" << server->errorString();
        server->close();
        return -1;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug() << "The server is running on " << ipAddress << server->serverPort();
    
    return a.exec();
}
