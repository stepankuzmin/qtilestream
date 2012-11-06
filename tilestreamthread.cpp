#include "tilestreamthread.h"

#include <QDebug>
#include <QtNetwork>

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QStringList>

TileStreamThread::TileStreamThread(int socketDescriptor, const QString &fortune, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), text(fortune)
{
}

//! [1]
void TileStreamThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    const char *data = "HTTP/1.1 200 Ok\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<h1>Nothing to see here</h1>\n";
    tcpSocket.write(data);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
