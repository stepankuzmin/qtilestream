#include "qtilestream.h"
#include "qtilestreamthread.h"

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

void QTileStream::incomingConnection(int socketDescriptor)
{
    QTileStreamThread *thread = new QTileStreamThread(socketDescriptor, &db, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
