#ifndef QTILESTREAM_H
#define QTILESTREAM_H

#include <QtSql>
#include <QString>
#include <QTcpServer>
#include <QByteArray>
#include <QStringList>

class QTileStream : public QTcpServer
{
    Q_OBJECT

public:
    QTileStream(QStringList mbtiles, QObject *parent = 0);
    ~QTileStream();

protected:
    void incomingConnection(int socketDescriptor);

private:
    QVector<QSqlDatabase> db;
    
};

#endif // QTILESTREAM_H
