#ifndef TILESTREAM_H
#define TILESTREAM_H

#include <QStringList>
#include <QTcpServer>

class TileStream : public QTcpServer
{
    Q_OBJECT

public:
    TileStream(QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private slots:
    void readClient();
};

#endif // TILESTREAM_H
