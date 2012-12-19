#ifndef TILESTREAM_H
#define TILESTREAM_H

#include <QtSql>
#include <QRegExp>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QStringList>

class TileStream : public QTcpServer
{
    Q_OBJECT

public:
    TileStream(QString path);
    ~TileStream();

private:
    QRegExp rx;
    QSqlDatabase db;

protected:
    void incomingConnection(int socketDescriptor);

private slots:
    void readClient();
};

#endif // TILESTREAM_H
