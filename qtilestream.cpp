#include "qtilestream.h"
#include "qtilestreamthread.h"

#include <stdlib.h>

QTileStream::QTileStream(QString path, QObject *parent) :
    QTcpServer(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (!db.open()) {
        qDebug() << db.lastError().text();
    }
    else {
        qDebug() << "Using" << path;
    }
}

QTileStream::~QTileStream() {
    db.close();
}

void QTileStream::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket(this);
    QTileStreamThread *thread = new QTileStreamThread(socketDescriptor, &db, this);
    connect(socket, SIGNAL(readyRead()), thread, SLOT(readClient()));
    thread->start();
    socket->setSocketDescriptor(socketDescriptor);
}
