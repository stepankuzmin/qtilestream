#include "qtilestream.h"
#include "qtilestreamthread.h"

QTileStream::QTileStream(QStringList mbtiles, QObject *parent) :
    QTcpServer(parent)
{
    for (int i = 0; i < mbtiles.size(); ++i) {
        QString path = mbtiles.at(i);
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", path);
        database.setDatabaseName(path);
        if (!database.open()) {
            qDebug() << database.lastError().text();
        }
        else {
            db.append(database);
            qDebug() << "Using" << path;
        }
    }
}

QTileStream::QTileStream(QStringList mbtiles, QByteArray notFoundImageData, QObject *parent) :
    QTcpServer(parent)
{
    for (int i = 0; i < mbtiles.size(); ++i) {
        QString path = mbtiles.at(i);
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", path);
        database.setDatabaseName(path);
        if (!database.open()) {
            qDebug() << database.lastError().text();
        }
        else {
            db.append(database);
            qDebug() << "Using" << path;
        }
    }
    notFoundImage = notFoundImageData;
}

QTileStream::~QTileStream() {
    for (int i = 0; i < db.size(); ++i) {
        db[i].close();
    }
}

void QTileStream::incomingConnection(int socketDescriptor)
{
    QTileStreamThread *thread;
    if (notFoundImage.isEmpty()) {
        thread = new QTileStreamThread(socketDescriptor, &db, this);
    }
    else {
        thread = new QTileStreamThread(socketDescriptor, &db, &notFoundImage, this);
    }
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
