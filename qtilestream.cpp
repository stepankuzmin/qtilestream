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
            qDebug() << path << "opened";
        }
    }
}

QTileStream::~QTileStream() {
    for (int i = 0; i < db.size(); ++i) {
        db[i].close();
    }
}

void QTileStream::incomingConnection(int socketDescriptor)
{
    QTileStreamThread *thread = new QTileStreamThread(socketDescriptor, &db, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
